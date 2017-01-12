/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7005
        This code is designed to work with the SI7005_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7005_I2CS#tabs-0-product_tabset-2
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
    #define SI7005_DEFAULT_ADDRESS                  (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SI7005_CONVERSIONDELAY                  (100)

/**************************************************************************
    CONTROL REGISTER
**************************************************************************/
    #define SI7005_REG_CONTROL_MASK                 (0xFF)
    #define SI7005_REG_CONTROL_STATUS               (0x00)
    #define SI7005_REG_CONTROL_DATA_HI              (0x01)
    #define SI7005_REG_CONTROL_DATA_LO              (0x02)
    #define SI7005_REG_CONTROL_CONFIG               (0x03)
    #define SI7005_REG_CONTROL_ID                   (0x11)

/**************************************************************************
 CONFIG REGISTER
 **************************************************************************/
    #define SI7005_REG_STATUS_MASK                  (0x01)  // Status Settings
    #define SI7005_REG_STATUS_READY                 (0x00)  // Conversion Complete
    #define SI7005_REG_STATUS_NOT_READY             (0x01)  // Conversion in Progress

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define SI7005_REG_CONFIG_FAST_MASK             (0x20)      // Fast Mode Enable
    #define SI7005_REG_CONFIG_FAST_ENABLE_35        (0x00)      // 35 ms (typical)
    #define SI7005_REG_CONFIG_FAST_ENABLE_18        (0x20)      // 18 ms (typical)

    #define SI7005_REG_CONFIG_ENABLE_MASK           (0x10)      // Temperature Enable
    #define SI7005_REG_CONFIG_ENABLE_HUMIDITY       (0x00)      // Relative humidity
    #define SI7005_REG_CONFIG_ENABLE_TEMPERATURE    (0x10)      // Temperature

    #define SI7005_REG_CONFIG_HEATER_MASK           (0x02)      // Heater Enable
    #define SI7005_REG_CONFIG_HEATER_DISABLE        (0x00)      // Heater Off
    #define SI7005_REG_CONFIG_HEATER_ENABLE         (0x02)      // Heater On

    #define SI7005_REG_CONFIG_CONVERSION_MASK       (0x01)      // Conversion Start
    #define SI7005_REG_CONFIG_CONVERSION_DONT       (0x00)      // Do not start a conversion
    #define SI7005_REG_CONFIG_CONVERSION_START      (0x01)      // Start a conversion

/**************************************************************************
    COEFFICIENTS
**************************************************************************/
    #define SI7005_TEMPERATURE_OFFSET               (50)
    #define SI7005_TEMPERATURE_SLOPE                (32)
    #define SI7005_HUMIDITY_OFFSET                  (24)
    #define SI7005_HUMIDITY_SLOPE                   (16)
    #define A0                                      (-4.7844)
    #define A1                                      (0.4008)
    #define A2                                      (-0.00393)
    #define Q0                                      (0.1973)
    #define Q1                                      (0.00237)


typedef enum
{
    FAST_ENABLE_35          = SI7005_REG_CONFIG_FAST_ENABLE_35,
    FAST_ENABLE_18          = SI7005_REG_CONFIG_FAST_ENABLE_18

} siFastEnable_t;

typedef enum
{
    HEATER_OFF              = SI7005_REG_CONFIG_HEATER_DISABLE,
    HEATER_ON               = SI7005_REG_CONFIG_HEATER_ENABLE
    
} siHeaterEnable_t;

typedef enum
{
    CONVERSION_DONT         = SI7005_REG_CONFIG_CONVERSION_DONT,
    CONVERSION_START        = SI7005_REG_CONFIG_CONVERSION_START
    
} siConversion_t;

class SI7005
{
    protected:
        // Instance-specific properties
        uint8_t si_conversionDelay;
        siFastEnable_t si_fastenable;
        siHeaterEnable_t si_heaterenable;
        siConversion_t si_conversion;
        float last_temperature;

    public:
        uint8_t si_i2cAddress;
        void getAddr_SI7005(uint8_t i2cAddress);
        boolean begin(void);
        uint16_t getMeasurement(uint8_t configValue);
        float Measure_Temperature();
        float Measure_Humidity();
        void setFastEnable(siFastEnable_t fastenable);
        siFastEnable_t getFastEnable(void);
        void setHeaterEnable(siHeaterEnable_t heaterenable);
        siHeaterEnable_t getHeaterEnable(void);
        void setConversion(siConversion_t conversion);
        siConversion_t getConversion(void);
  
    private:
};
