/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL45315
        This code is designed to work with the TSL45315_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define TSL45315_DEFAULT_ADDRESS                        (0x29)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL45315_CONVERSIONDELAY                        (100)

/**************************************************************************
    DIGITAL ALS REGISTERS
**************************************************************************/
    #define TSL45315_REG_ALS_COMMAND                                    // Specifies Register Address
    #define TSL45315_REG_ALS_CONTROL                        (0x00)      // Power On/Off and Single Cycle Register
    #define TSL45315_REG_ALS_CONFIG                         (0x01)      // Powersave CONTROL / Integration Time
    #define TSL45315_REG_ALS_DATALOW                        (0x04)      // ALS Low Data Register
    #define TSL45315_REG_ALS_DATAHIGH                       (0x05)      // ALS High Data Register
    #define TSL45315_REG_ALS_ID                             (0x0A)      // Device ID Register

/**************************************************************************
    DIGITAL ALS COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL45315_REG_ALS_CMD_MASK                       (0x80)      // Select Command Register
    #define TSL45315_REG_ALS_CMD_SELECT                     (0x80)      // Addresses COMMAND Register

/**************************************************************************
    DIGITAL ALS CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL45315_REG_ALS_CONTROL_MODE_MASK              (0x03)      // Operating Mode
    #define TSL45315_REG_ALS_CONTROL_MODE_POWER_DOWN        (0x00)      // Power Down
    #define TSL45315_REG_ALS_CONTROL_MODE_RUN_ADC           (0x02)      // Run a single ADC cycle and return to PowerDown
    #define TSL45315_REG_ALS_CONTROL_MODE_NORMAL            (0x03)      // Normal Operation

/**************************************************************************
    DIGITAL ALS CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TSL45315_REG_ALS_CONFIG_PSAVESKIP_MASK          (0x08)      // PowerSave Mode
    #define TSL45315_REG_ALS_CONFIG_PSAVESKIP_NOT_ASSERT    (0x00)      // Not Asserted, the Power Save States are NOT Skipped Following a Light Integration Cycle for Shorter Sampling Rates
    #define TSL45315_REG_ALS_CONFIG_PSAVESKIP_ASSERT        (0x08)          // Asserted, the Power Save States are Skipped Following a Light Integration Cycle for Shorter Sampling Rates

    #define TSL45315_REG_ALS_CONFIG_TCNTRL_MASK             (0x03)      // Timer Control sets the Integration Time
    #define TSL45315_REG_ALS_CONFIG_TCNTRL_400MS            (0x00)      // Tint = 400 ms
    #define TSL45315_REG_ALS_CONFIG_TCNTRL_200MS            (0x01)      // Tint = 200 ms
    #define TSL45315_REG_ALS_CONFIG_TCNTRL_100MS            (0x02)      // Tint = 100 ms

typedef enum
{
    COMMAND_SELECT                  = TSL45315_REG_ALS_CMD_SELECT
    
} tslCommand_t;

typedef enum
{
    MODE_POWER_DOWN                 = TSL45315_REG_ALS_CONTROL_MODE_POWER_DOWN,
    MODE_RUN_ADC                    = TSL45315_REG_ALS_CONTROL_MODE_NORMAL,
    MODE_NORMAL                     = TSL45315_REG_ALS_CONTROL_MODE_NORMAL
    
} tslOperatingMode_t;

typedef enum
{
    PSAVESKIP_NOT_ASSERT            = TSL45315_REG_ALS_CONFIG_PSAVESKIP_NOT_ASSERT,
    PSAVESKIP_ASSERT                = TSL45315_REG_ALS_CONFIG_PSAVESKIP_ASSERT
    
} tslPowerSaveMode_t;

typedef enum
{
    TCNTRL_400MS                    = TSL45315_REG_ALS_CONFIG_TCNTRL_400MS,
    TCNTRL_200MS                    = TSL45315_REG_ALS_CONFIG_TCNTRL_200MS,
    TCNTRL_100MS                    = TSL45315_REG_ALS_CONFIG_TCNTRL_100MS
    
} tslTCntrl_t;


typedef struct
{
    float L;
    
} tslSensorData_t;


class TSL45315
{
    protected:
        // Instance-specific properties
        uint8_t tsl_conversionDelay;
        tslCommand_t tsl_command;
        tslOperatingMode_t tsl_operatingmode;;
        tslPowerSaveMode_t tsl_powersavemode;;
        tslTCntrl_t tsl_tcntrl;;

    public:
        uint8_t tsl_i2cAddress;
        tslSensorData_t tsl_alsData;
        void getAddr_TSL45315(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_ALS(void);
        void setCommand(tslCommand_t command);
        tslCommand_t getCommand(void);
        void setOperatingMode(tslOperatingMode_t operatingmode);
        tslOperatingMode_t getOperatingMode(void);
        void setPowerSaveMode(tslPowerSaveMode_t powersavemode);
        tslPowerSaveMode_t getPowerSaveMode(void);
        void setTCntrl(tslTCntrl_t tcntrl);
        tslTCntrl_t getTCntrl(void);
    
    private:
};
