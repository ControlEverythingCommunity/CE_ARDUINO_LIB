/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TH02
        This code is designed to work with the TH02_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define TH02_DEFAULT_ADDRESS                  (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TH02_CONVERSIONDELAY                  (100)

/**************************************************************************
    CONTROL REGISTER
**************************************************************************/
    #define TH02_REG_CONTROL_MASK                 (0xFF)
    #define TH02_REG_CONTROL_STATUS               (0x00)
    #define TH02_REG_CONTROL_DATA_HI              (0x01)
    #define TH02_REG_CONTROL_DATA_LO              (0x02)
    #define TH02_REG_CONTROL_CONFIG               (0x03)
    #define TH02_REG_CONTROL_ID                   (0x11)

/**************************************************************************
 CONFIG REGISTER
 **************************************************************************/
    #define TH02_REG_STATUS_MASK                  (0x01)    // Status Settings
    #define TH02_REG_STATUS_READY                 (0x00)    // Conversion Complete
    #define TH02_REG_STATUS_NOT_READY             (0x01)    // Conversion in Progress

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define TH02_REG_CONFIG_FAST_MASK             (0x20)      // Fast Mode Enable
    #define TH02_REG_CONFIG_FAST_ENABLE_35        (0x00)      // 35 ms (typical)
    #define TH02_REG_CONFIG_FAST_ENABLE_18        (0x20)      // 18 ms (typical)

    #define TH02_REG_CONFIG_ENABLE_MASK           (0x10)      // Temperature Enable
    #define TH02_REG_CONFIG_ENABLE_HUMIDITY       (0x00)      // Relative humidity
    #define TH02_REG_CONFIG_ENABLE_TEMPERATURE    (0x10)      // Temperature

    #define TH02_REG_CONFIG_HEATER_MASK           (0x02)      // Heater Enable
    #define TH02_REG_CONFIG_HEATER_DISABLE        (0x00)      // Heater Off
    #define TH02_REG_CONFIG_HEATER_ENABLE         (0x02)      // Heater On

    #define TH02_REG_CONFIG_CONVERSION_MASK       (0x01)      // Conversion Start
    #define TH02_REG_CONFIG_CONVERSION_DONT       (0x00)      // Do not start a conversion
    #define TH02_REG_CONFIG_CONVERSION_START      (0x01)      // Start a conversion

/**************************************************************************
    COEFFICIENTS
**************************************************************************/
    #define TH02_TEMPERATURE_OFFSET               (50)
    #define TH02_TEMPERATURE_SLOPE                (32)
    #define TH02_HUMIDITY_OFFSET                  (24)
    #define TH02_HUMIDITY_SLOPE                   (16)
    #define A0                                    (-4.7844)
    #define A1                                    (0.4008)
    #define A2                                    (-0.00393)
    #define Q0                                    (0.1973)
    #define Q1                                    (0.00237)


typedef enum
{
    FAST_ENABLE_35          = TH02_REG_CONFIG_FAST_ENABLE_35,
    FAST_ENABLE_18          = TH02_REG_CONFIG_FAST_ENABLE_18

} thFastEnable_t;

typedef enum
{
    HEATER_OFF              = TH02_REG_CONFIG_HEATER_DISABLE,
    HEATER_ON               = TH02_REG_CONFIG_HEATER_ENABLE
    
} thHeaterEnable_t;

typedef enum
{
    CONVERSION_DONT         = TH02_REG_CONFIG_CONVERSION_DONT,
    CONVERSION_START        = TH02_REG_CONFIG_CONVERSION_START
    
} thConversion_t;

class TH02
{
    protected:
        // Instance-specific properties
        uint8_t th_conversionDelay;
        thFastEnable_t th_fastenable;
        thHeaterEnable_t th_heaterenable;
        thConversion_t th_conversion;
        float last_temperature;

    public:
        uint8_t th_i2cAddress;
        void getAddr_TH02(uint8_t i2cAddress);
        boolean begin(void);
        uint16_t getMeasurement(uint8_t configValue);
        float Measure_Temperature();
        float Measure_Humidity();
        void setFastEnable(thFastEnable_t fastenable);
        thFastEnable_t getFastEnable(void);
        void setHeaterEnable(thHeaterEnable_t heaterenable);
        thHeaterEnable_t getHeaterEnable(void);
        void setConversion(thConversion_t conversion);
        thConversion_t getConversion(void);
  
    private:
};
