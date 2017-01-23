/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        STS21
        This code is designed to work with the STS21_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=STS21_I2CS#tabs-0-product_tabset-2
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
    #define STS21_DEFAULT_ADDRESS           (0x4A)    // 1001'010'

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define STS21_CONVERSIONDELAY           (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define STS21_CMD_TEMP_HOLD_MASTER      (0xE3)
    #define STS21_CMD_TEMP_NO_HOLD_MASTER   (0xF3)
    #define STS21_CMD_WRITE_USER_1          (0xE6)
    #define STS21_CMD_READ_USER_1           (0xE7)
    #define STS21_CMD_RESET                 (0xFE)

typedef enum
{
    TEMP_HOLD       = STS21_CMD_TEMP_HOLD_MASTER,
    TEMP_NO_HOLD    = STS21_CMD_TEMP_NO_HOLD_MASTER
} ststempMode_t;

class STS21
{
    protected:
        // Instance-specific properties
        uint8_t sts_conversionDelay;
        ststempMode_t sts_tempmode;
    
    public:
        uint8_t sts_i2cAddress;
        void getAddr_STS21(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void Reset(void);
        void setTempMode(ststempMode_t tempmode);
        ststempMode_t getTempMode(void);
  
    private:
};
