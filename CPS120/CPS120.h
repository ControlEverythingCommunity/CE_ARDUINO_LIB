/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CPS120
        This code is designed to work with the CPS120_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=CPS120_I2CS#tabs-0-product_tabset-2
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
    #define CPS120_DEFAULT_ADDRESS          (0x28)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define CPS120_CONVERSIONDELAY          (100)

class CPS120
{
    protected:
        // Instance-specific properties
        uint8_t cps_conversionDelay;
        float pressure, temperature;
    
    public:
        uint8_t cps_i2cAddress;
        void getAddr_CPS120(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_Pressure(void);
        float Measure_Temperature(void);
  
    private:
};
