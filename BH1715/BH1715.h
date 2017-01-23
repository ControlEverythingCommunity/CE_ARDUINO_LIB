/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BH1715
        This code is designed to work with the BH1715_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=BH1715_I2CS#tabs-0-product_tabset-2
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
    #define BH1715_DEFAULT_ADDRESS          (0x23)    // ADDR = 'L', 0100011
    #define BH1715_ADDR_H_ADDRESS           (0x5C)    // ADDR = 'H', 1011100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BH1715_CONVERSIONDELAY          (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define BH1715_INTR_POWER_DOWN          (0x00)
    #define BH1715_INTR_POWER_ON            (0x01)
    #define BH1715_INTR_RESET               (0x03)
    #define BH1715_INTR_CONT_H_MODE         (0x10)
    #define BH1715_INTR_CONT_L_MODE         (0x13)
    #define BH1715_INTR_ONE_H_MODE          (0x20)
    #define BH1715_INTR_ONE_L_MODE          (0x23)
    #define BH1715_INTR_RES_MIN             (0x1F)
    #define BH1715_INTR_RES_DEFAULT         (0x45)
    #define BH1715_INTR_RES_MAX             (0xFE)

typedef enum
{
    CONTINOUS_HIGH      = BH1715_INTR_CONT_H_MODE,
    CONTINOUS_LOW       = BH1715_INTR_CONT_L_MODE,
    ONE_HIGH            = BH1715_INTR_ONE_H_MODE,
    ONE_LOW             = BH1715_INTR_ONE_L_MODE
} bhresMode_t;

typedef enum
{
    RESOLUTION_MIN      = BH1715_INTR_RES_MIN,
    RESOLUTION_DEFAULT  = BH1715_INTR_RES_DEFAULT,
    RESOLUTION_MAX      = BH1715_INTR_RES_MAX
} bhresolution_t;

class BH1715
{
    protected:
        // Instance-specific properties
        uint8_t bh_conversionDelay;
        bhresMode_t bh_resmode;
        bhresolution_t bh_resolution;
    
    public:
        uint8_t bh_i2cAddress;
        void getAddr_BH1715(uint8_t i2cAddress);
        void begin(void);
        uint16_t Measure_Lux(void);
        void Reset(void);
        void setResMode(bhresMode_t resmode);
        bhresMode_t getResMode(void);
        void setResolution(bhresolution_t resolution);
        bhresolution_t getResolution(void);

    private:
};
