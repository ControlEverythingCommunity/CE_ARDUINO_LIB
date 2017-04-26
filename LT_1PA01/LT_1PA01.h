/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LT-1PA01
        This code is designed to work with the LT-1PA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
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
    #define LT_1PA01_DEFAULT_ADDRESS                            (0x44)
    #define LT_1PA01_ALSPROX_DEV_ID                             (0xC8)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LT_1PA01_CONVERSIONDELAY                            (100)

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define LT_1PA01_REG_ALSPROX_DEVID                          (0x00)      // Device Identification Register
    #define LT_1PA01_REG_ALSPROX_CONFIG0                        (0x01)      // Proximity Configuration Register
    #define LT_1PA01_REG_ALSPROX_CONFIG1                        (0x02)      // Proximity/ALS Configuration Register
    #define LT_1PA01_REG_ALSPROX_CONFIG2                        (0x03)      // Ambient Light Sensor IR Compensation Register
    #define LT_1PA01_REG_ALSPROX_INTCONFIG                      (0x04)      // Interrupt Configuration, Status & Control Register
    #define LT_1PA01_REG_ALSPROX_PROX_INT_TL                    (0x05)      // Proximity Interrupt LOW Threshold Byte Register
    #define LT_1PA01_REG_ALSPROX_PROX_INT_TH                    (0x06)      // Proximity Interrupt HIGH Threshold Byte Register
    #define LT_1PA01_REG_ALSPROX_ALS_INT_TL                     (0x07)      // ALS Interrupt LOW Threshold Bit [11:4] Register
    #define LT_1PA01_REG_ALSPROX_ALS_INT_TLH                    (0x08)      // ALS Interrupt LOW/HIGH Threshold Bit Register
    #define LT_1PA01_REG_ALSPROX_ALS_INT_TH                     (0x09)      // ALS Interrupt HIGH Threshold Bit [7:0] Register
    #define LT_1PA01_REG_ALSPROX_PROX_DATA                      (0x0A)      // Proximity Data (Unsigned Binary) Register
    #define LT_1PA01_REG_ALSPROX_ALS_DATA_HB                    (0x0B)      // ALS Data HIGH Byte Register
    #define LT_1PA01_REG_ALSPROX_ALS_DATA_LB                    (0x0C)      // ALS Data LOW Byte Register
    #define LT_1PA01_REG_ALSPROX_PROX_AMBIR                     (0x0D)      // Proximity Mode Ambient IR Measurement Register
    #define LT_1PA01_REG_ALSPROX_CONFIG3                        (0x0E)      // Software Reset Register

/**************************************************************************
    CONGIGURATION 0 REGISTER DESCRIPTION
**************************************************************************/
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_MASK           (0x20)      // Proximity Enable/Disable
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_DISABLE        (0x00)      // Proximity Disable
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_ENABLE         (0x20)      // Proximity Enable

    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_MASK          (0x1C)      // Proximity Sleep Time Select
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_800MS         (0x00)      // 800ms
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_200MS         (0x04)      // 200ms
    #define LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_100MS         (0x08)      // 100ms

    #define LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_MASK          (0x03)      // IRDR (VCSEL) Current
    #define LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_3_6MA         (0x00)      // 3.6mA
    #define LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_7_2MA         (0x01)      // 7.2mA
    #define LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_10_8MA        (0x02)      // 10.8mA
    #define LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_14_4MA        (0x03)      // 14.4mA

/**************************************************************************
    CONGIGURATION 1 REGISTER DESCRIPTION
**************************************************************************/
    #define LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_MASK           (0x80)      // Interrupt Algorithm
    #define LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_HYST           (0x00)      // Hysteresis Window
    #define LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_WINDOW         (0x80)      // Window Comparator

    #define LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_MASK       (0x78)      // Proximity Offset Compensation
    #define LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_DISABLE    (0x00)      // Proximity Offset Compensation Disable
    #define LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_ENABLE     (0x78)      // Proximity Offset Compensation Enable

    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_MASK            (0x04)      // Ambient Light Sensing Enable/Disable
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_DISABLE         (0x00)      // Ambient Light Sensing Disable
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_ENABLE          (0x04)      // Ambient Light Sensing Enable

    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_MASK         (0x03)      // Ambient Light Sensor Range Select
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_62_5LX       (0x00)      // 62.5 Lux
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_125LX        (0x01)      // 125 Lux
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_1000LX       (0x02)      // 1000 Lux
    #define LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_2000LX       (0x03)      // 2000 Lux


typedef enum
{
    PROXIMITY_DISABLE           = LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_DISABLE,
    PROXIMITY_ENABLE            = LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_ENABLE
    
} ltProximityEnable_t;

typedef enum
{
    PROXIMITY_SLEEP_800MS       = LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_800MS,
    PROXIMITY_SLEEP_200MS       = LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_200MS,
    PROXIMITY_SLEEP_100MS       = LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_100MS
    
} ltProximitySleep_t;

typedef enum
{
    IRDR_CURRENT_3_6MA          = LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_3_6MA,
    IRDR_CURRENT_7_2MA          = LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_7_2MA,
    IRDR_CURRENT_10_8MA         = LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_10_8MA,
    IRDR_CURRENT_14_4MA         = LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_14_4MA
    
} ltIRDRCurrent_t;

typedef enum
{
    HYSTERSIS_WINDOW            = LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_HYST,
    WINDOW_COMPARATOR           = LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_WINDOW
    
} ltInterrupt_t;

typedef enum
{
    PROXIMITY_OFFSET_DISABLE    = LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_DISABLE,
    PROXIMITY_OFFSET_ENABLE     = LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_ENABLE
    
} ltProximityOffset_t;

typedef enum
{
    ALS_DISABLE                 = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_DISABLE,
    ALS_ENABLE                  = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_ENABLE
    
} ltALSEnable_t;

typedef enum
{
    ALS_RANGE_62_5LX            = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_62_5LX,
    ALS_RANGE_125LX             = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_125LX,
    ALS_RANGE_1000LX            = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_1000LX,
    ALS_RANGE_2000LX            = LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_2000LX
    
} ltALSRange_t;

typedef struct
{
    uint16_t L;
    uint8_t P;
    
} ltSensorData_t;


class LT_1PA01
{
    protected:
        // Instance-specific properties
        uint8_t lt_conversionDelay;
        ltProximityEnable_t lt_proximityenable;
        ltProximitySleep_t lt_proximitysleep;
        ltIRDRCurrent_t lt_irdrcurrent;
        ltInterrupt_t lt_interrupt;
        ltProximityOffset_t lt_proximityoffset;
        ltALSEnable_t lt_alsenable;
        ltALSRange_t lt_alsrange;

    public:
        uint8_t lt_i2cAddress;
        ltSensorData_t lt_alsproxData;
        void getAddr_LT_1PA01(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpALSProximity(void);
        void Measure_ALS(void);
        void Measure_Proximity(void);
        void setProximityEnable(ltProximityEnable_t proximityenable);
        ltProximityEnable_t getProximityEnable(void);
        void setProximitySleep(ltProximitySleep_t proximitysleep);
        ltProximitySleep_t getProximitySleep(void);
        void setIRDRCurrent(ltIRDRCurrent_t irdrcurrent);
        ltIRDRCurrent_t getIRDRCurrent(void);
        void setInterrupt(ltInterrupt_t interrupt);
        ltInterrupt_t getInterrupt(void);
        void setProximityOffset(ltProximityOffset_t proximityoffset);
        ltProximityOffset_t getProximityOffset(void);
        void setALSEnable(ltALSEnable_t alsenable);
        ltALSEnable_t getALSEnable(void);
        void setALSRange(ltALSRange_t alsrange);
        ltALSRange_t getALSRange(void);
    
    private:
};
