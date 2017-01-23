/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HCPA-5V-U3
        This code is designed to work with the HCPA-5V-U3_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HCPA-5V-U3_I2CS#tabs-0-product_tabset-2
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
    #define HCPA_5V_U3_DEFAULT_ADDRESS          (0x28)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HCPA_5V_U3_CONVERSIONDELAY          (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define HCPA_5V_U3_COMMAND_NORMAL                (0x80)    // Ends Command Mode and transitions to Normal Operation Mode
    #define HCPA_5V_U3_COMMAND_START                 (0xA0)    // Start Command Mode: used to enter the command interpreting mode



class HCPA_5V_U3
{
    protected:
        // Instance-specific properties
        uint8_t hcpa_conversionDelay;
        float humidity, temperature;
    
    public:
        HCPA_5V_U3();
        uint8_t hcpa_i2cAddress;
        void getAddr_HCPA_5V_U3(uint8_t i2cAddress);
        void begin(void);
        float getHumidity(void);
        float getTemperature(void);
        bool Measure_HumidityAndTemperature(void);

    private:
};
