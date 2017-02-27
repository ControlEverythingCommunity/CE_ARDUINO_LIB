/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A1332
        This code is designed to work with the A1332_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/precision-hall-effect-rotational-angle-0-to-360-sensor
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
    #define A1332_DEFAULT_ADDRESS          (0x0C)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define A1332_CONVERSIONDELAY          (100)

class A1332
{
    protected:
        // Instance-specific properties
        uint8_t a1332_conversionDelay;
        float magAngle;
    
    public:
        uint8_t a1332_i2cAddress;
        void getAddr_A1332(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_HallSensor(void);
  
    private:
};
