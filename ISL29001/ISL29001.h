/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29001
        This code is designed to work with the ISL29001_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define ISL29001_DEFAULT_ADDRESS            (0x44)    // 1000100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ISL29001_CONVERSIONDELAY            (100)

/**************************************************************************
    COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define ISL29001_CMD_ADC_POWER_DOWN         (0x8C)  // ADC is Powered-Down
    #define ISL29001_CMD_ADC_RESET              (0x0C)  // ADC is Reset
    #define ISL29001_CMD_ADC_DIODE_1_INT        (0x00)  // ADC converts Diode 1’s current (IDIODE1) into unsigned-magnitude 16-bit data. The integration is internally timed at 100ms per integration
    #define ISL29001_CMD_ADC_DIODE_2_INT        (0x04)  // ADC converts Diode 2’s current (IDIODE1) into unsigned-magnitude 16-bit data. The integration is internally timed at 100ms per integration
    #define ISL29001_CMD_ADC_DIODE_DIFF_INT     (0x08)  //  ADC converts IDIODE1-IDIODE2 into 2’s-complement 16-bit data. The total integration is internally timed at 200ms per integration
    #define ISL29001_CMD_ADC_DIODE_1_EXT        (0x30)  //  ADC converts Diode 1’s current (IDIODE1) into unsigned-magnitude 16-bit data. The integration is externally timed; each 30H command sent to the device ends one integration period and begins another
    #define ISL29001_CMD_ADC_DIODE_2_EXT        (0x34)  //  ADC converts Diode 2’s current (IDIODE1) into unsigned-magnitude 16-bit data. The integration is externally timed; each 30H command sent to the device ends one integration period and begins another
    #define ISL29001_CMD_ADC_DIODE_DIFF_EXT     (0x38)  // ADC converts IDIODE1-IDIODE2 into 2’s-complement 16-bit data. The integration is externally timed; each 38H command sent to the device ends one integration period and begins another.

typedef enum
{
    ADC_POWER_DOWN          = ISL29001_CMD_ADC_POWER_DOWN,
    ADC_RESET               = ISL29001_CMD_ADC_RESET,
    ADC_DIODE_1_INT         = ISL29001_CMD_ADC_DIODE_1_INT,
    ADC_DIODE_2_INT         = ISL29001_CMD_ADC_DIODE_2_INT,
    ADC_DIODE_DIFF_INT      = ISL29001_CMD_ADC_DIODE_DIFF_INT,
    ADC_DIODE_1_EXT         = ISL29001_CMD_ADC_DIODE_1_EXT,
    ADC_DIODE_2_EXT         = ISL29001_CMD_ADC_DIODE_2_EXT,
    ADC_DIODE_DIFF_EXT      = ISL29001_CMD_ADC_DIODE_DIFF_EXT
} isl_command_t;

class ISL29001
{
    protected:
        // Instance-specific properties
        uint8_t isl_conversionDelay;
    
    public:
        uint8_t isl_i2cAddress;
        ISL29001() {};
        void getAddr_ISL29001(uint8_t i2cAddress);
        void begin(isl_command_t command);
        uint16_t Measure_Light(void);
  
    private:
};
