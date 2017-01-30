/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7050
        This code is designed to work with the SI7050_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7050_I2CS#tabs-0-product_tabset-2
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
    #define SI7050_DEFAULT_ADDRESS          (0x40)    // 1000000

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SI7050_CONVERSIONDELAY          (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define SI7050_CMD_TEMP_HOLD            (0xE3)
    #define SI7050_CMD_TEMP_NO_HOLD         (0xF3)
    #define SI7050_CMD_RESET                (0xFE)
    #define SI7050_CMD_WRITE_USER_1         (0xE6)
    #define SI7050_CMD_READ_USER_1          (0xE7)
    #define SI7050_CMD_READ_EID_1           [0xE7, 0x0F]
    #define SI7050_CMD_READ_EID_2           [0xFC, 0xC9]
    #define SI7050_CMD_READ_FIRM_REV        [0x84, 0xB8]

typedef enum
{
    TEMP_HOLD       = SI7050_CMD_TEMP_HOLD,
    TEMP_NO_HOLD    = SI7050_CMD_TEMP_NO_HOLD
} sitempMode_t;

class SI7050
{
    protected:
        // Instance-specific properties
        uint8_t si_conversionDelay;
        sitempMode_t si_tempmode;
    
    public:
        uint8_t si_i2cAddress;
        void getAddr_SI7050(uint8_t i2cAddress);
        void begin(void);
        int16_t Measure_Temp(void);
        void Reset(void);
        void setTempMode(sitempMode_t tempmode);
        sitempMode_t getTempMode(void);
  
    private:
};
