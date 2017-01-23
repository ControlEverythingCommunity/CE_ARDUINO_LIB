/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        STTS751
        This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/

/**************************************************************************
 I2C ADDRESS/BITS
 **************************************************************************/
    #define STTS751_DEFAULT_ADDRESS             (0x39)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define STTS751_CONVERSIONDELAY             (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define STTS751_REG_TEMP_HIGH               (0x00)
    #define STTS751_REG_STATUS                  (0x01)
    #define STTS751_REG_TEMP_LOW                (0x02)
    #define STTS751_REG_CONFIG                  (0x03)
    #define STTS751_REG_CONV_RATE               (0x04)
    #define STTS751_REG_TEMP_HIGH_LT_HIGH       (0x05)
    #define STTS751_REG_TEMP_HIGH_LT_LOW        (0x06)
    #define STTS751_REG_TEMP_LOW_LT_HIGH        (0x07)
    #define STTS751_REG_TEMP_LOW_LT_LOW         (0x08)
    #define STTS751_REG_ONE_SHOT                (0x0F)
    #define STTS751_REG_THERM_LIMIT             (0x20)
    #define STTS751_REG_THERM_HYST              (0x21)
    #define STTS751_REG_SMBUS_TM_ENABLE         (0x22)
    #define STTS751_REG_PRODUCT_ID              (0xFD)
    #define STTS751_REG_MANUF_ID                (0xFE)
    #define STTS751_REG_REVISION_NO             (0xFF)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define STTS751_REG_CONFIG_MASK1_MASK       (0x80)      // MASK1
    #define STTS751_REG_CONFIG_MASK1_ENABLED    (0x00)      // EVENT is Enabled
    #define STTS751_REG_CONFIG_MASK1_DISABLED   (0x80)      // EVENT is Disabled

    #define STTS751_REG_CONFIG_MODE_MASK        (0x40)      // RUN/STOP
    #define STTS751_REG_CONFIG_MODE_CONTIN      (0x00)      // Device is running in continuous conversion mode
    #define STTS751_REG_CONFIG_MODE_STANDBY     (0x40)      // Device is in standby mode drawing minimum power

    #define STTS751_REG_CONFIG_TRES_MASK        (0x0C)      // Conversion Resolution
    #define STTS751_REG_CONFIG_TRES_10BIT       (0x00)      // 10 Bits (Default), LSB Step Size (°C): 0.25
    #define STTS751_REG_CONFIG_TRES_11BIT       (0x04)      // 11 Bits, LSB Step Size (°C): 0.125
    #define STTS751_REG_CONFIG_TRES_12BIT       (0x0C)      // 12 Bits, LSB Step Size (°C): 0.0625
    #define STTS751_REG_CONFIG_TRES_9BIT        (0x08)      // 9 Bits, LSB Step Size (°C): 0.5

/**************************************************************************
    CONVERSION RATE REGISTER
**************************************************************************/
    #define STTS751_REG_CONV_RATE_MASK          (0x0F)      // Conversion Rates
    #define STTS751_REG_CONV_RATE_0_0625        (0x00)      // Conversions Per Second: 0.0625
    #define STTS751_REG_CONV_RATE_0_125         (0x01)      // Conversions Per Second: 0.125
    #define STTS751_REG_CONV_RATE_0_25          (0x02)      // Conversions Per Second: 0.25
    #define STTS751_REG_CONV_RATE_0_5           (0x03)      // Conversions Per Second: 0.5
    #define STTS751_REG_CONV_RATE_1             (0x04)      // Conversions Per Second: 1
    #define STTS751_REG_CONV_RATE_2             (0x05)      // Conversions Per Second: 2
    #define STTS751_REG_CONV_RATE_4             (0x06)      // Conversions Per Second: 4
    #define STTS751_REG_CONV_RATE_8             (0x07)      // Conversions Per Second: 8
    #define STTS751_REG_CONV_RATE_16            (0x08)      // Conversions Per Second: 16
    #define STTS751_REG_CONV_RATE_32            (0x09)      // Conversions Per Second: 32


typedef enum
{
    MASK1_ENABLED        = STTS751_REG_CONFIG_MASK1_ENABLED,
    MASK1_DISABLED       = STTS751_REG_CONFIG_MASK1_DISABLED
    
} sttsMask1_t;

typedef enum
{
    MODE_CONTIN         = STTS751_REG_CONFIG_MODE_CONTIN,
    MODE_STANDBY        = STTS751_REG_CONFIG_MODE_STANDBY

} sttsMode_t;

typedef enum
{
    TRES_9BIT           = STTS751_REG_CONFIG_TRES_9BIT,
    TRES_10BIT          = STTS751_REG_CONFIG_TRES_10BIT,
    TRES_11BIT          = STTS751_REG_CONFIG_TRES_11BIT,
    TRES_12BIT          = STTS751_REG_CONFIG_TRES_12BIT
    
} sttsTres_t;

typedef enum
{
    CONVRATE_0_0625     = STTS751_REG_CONV_RATE_0_0625,
    CONVRATE_0_125      = STTS751_REG_CONV_RATE_0_125,
    CONVRATE_0_25       = STTS751_REG_CONV_RATE_0_25,
    CONVRATE_0_5        = STTS751_REG_CONV_RATE_0_5,
    CONVRATE_1          = STTS751_REG_CONV_RATE_1,
    CONVRATE_2          = STTS751_REG_CONV_RATE_2,
    CONVRATE_4          = STTS751_REG_CONV_RATE_4,
    CONVRATE_8          = STTS751_REG_CONV_RATE_8,
    CONVRATE_16         = STTS751_REG_CONV_RATE_16,
    CONVRATE_32         = STTS751_REG_CONV_RATE_32
    
} sttsConvRate_t;


class STTS751
{
    protected:
        // Instance-specific properties
        uint8_t stts_conversionDelay;
        sttsMask1_t stts_mask1;
        sttsMode_t stts_mode;
        sttsTres_t stts_tres;
        sttsConvRate_t stts_convrate;
    
    public:
        uint8_t stts_i2cAddress;
        void getAddr_STTS751(uint8_t i2cAddress);
        boolean begin(void);
        int16_t Measure_Temp(void);
        void setMask1(sttsMask1_t mask1);
        sttsMask1_t getMask1(void);
        void setMode(sttsMode_t mode);
        sttsMode_t getMode(void);
        void setTres(sttsTres_t tres);
        sttsTres_t getTres(void);
        void setConvRate(sttsConvRate_t convrate);
        sttsConvRate_t getConvRate(void);
  
    private:
};
