/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS8607-02BA01
        This code is designed to work with the MS8607-02BA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=MS8607-02BA01_I2CS_A01#tabs-0-product_tabset-2
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
    #define MS8607_02BA01_PT_ADDRESS            (0x76)
    #define MS8607_02BA01_RH_ADDRESS            (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MS8607_02BA01_CONVERSIONDELAY       (100)

/**************************************************************************
    PRESSURE AND TEMPERATURE COMMAND REGISTER
**************************************************************************/
    #define MS8607_02BA01_CMD_ADC               (0x00)
    #define MS8607_02BA01_CMD_RESET             (0x1E)

    #define MS8607_02BA01_CMD_C0                (0xA0)  // Constant
    #define MS8607_02BA01_CMD_C1                (0xA2)  // Pressure Sensitivity | SENST1
    #define MS8607_02BA01_CMD_C2                (0xA4)  // Pressure Offset | OFFT1
    #define MS8607_02BA01_CMD_C3                (0xA6)  // Temperature Coefficient of Pressure Sensitivity | TCS
    #define MS8607_02BA01_CMD_C4                (0xA8)  // Temperature Coefficient of Pressure Offset | TCO
    #define MS8607_02BA01_CMD_C5                (0xAA)  // Reference Temperature | TREF
    #define MS8607_02BA01_CMD_C6                (0xAC)  // Temperature coefficient of the temperature | TEMPSENS
    #define MS8607_02BA01_CMD_C7                (0xAE)  // Constant

    #define MS8607_02BA01_CMD_D1_OSR_256        (0x40)  // Convert D1 (OSR=256)
    #define MS8607_02BA01_CMD_D1_OSR_512        (0x42)  // Convert D1 (OSR=512)
    #define MS8607_02BA01_CMD_D1_OSR_1024       (0x44)  // Convert D1 (OSR=1024)
    #define MS8607_02BA01_CMD_D1_OSR_2048       (0x46)  // Convert D1 (OSR=2048)
    #define MS8607_02BA01_CMD_D1_OSR_4096       (0x48)  // Convert D1 (OSR=4096)
    #define MS8607_02BA01_CMD_D1_OSR_8192       (0x4A)  // Convert D1 (OSR=8192)

    #define MS8607_02BA01_CMD_D2_OSR_256        (0x50)  // Convert D2 (OSR=256)
    #define MS8607_02BA01_CMD_D2_OSR_512        (0x52)  // Convert D2 (OSR=512)
    #define MS8607_02BA01_CMD_D2_OSR_1024       (0x54)  // Convert D2 (OSR=1024)
    #define MS8607_02BA01_CMD_D2_OSR_2048       (0x56)  // Convert D2 (OSR=2048)
    #define MS8607_02BA01_CMD_D2_OSR_4096       (0x58)  // Convert D2 (OSR=4096)
    #define MS8607_02BA01_CMD_D2_OSR_8192       (0x5A)  // Convert D2 (OSR=8192)

/**************************************************************************
    RELATIVE HUMIDITY COMMAND REGISTER
 **************************************************************************/
    #define MS8607_02BA01_CMD_RESET             (0xFE)
    #define MS8607_02BA01_CMD_WRITE_USER        (0xE6)
    #define MS8607_02BA01_CMD_READ_USER         (0xE7)
    #define MS8607_02BA01_CMD_RH_HOLD           (0xE5)
    #define MS8607_02BA01_CMD_RH_NO_HOLD        (0xF5)

    #define MS8607_02BA01_USER_RES_OSR_MASK     (0x81)  // Measurement Resolution
    #define MS8607_02BA01_USER_RES_OSR_4096     (0x00)  // Resolution: Highest
    #define MS8607_02BA01_USER_RES_OSR_2048     (0x01)  // Resolution: Medium
    #define MS8607_02BA01_USER_RES_OSR_1024     (0x80)  // Resolution: Meduim
    #define MS8607_02BA01_USER_RES_OSR_256      (0x81)  // Resolution: Lowest

    #define MS8607_02BA01_USER_VDDS_MASK        (0x40)  // Battery State
    #define MS8607_02BA01_USER_VDDS_ABOVE       (0x00)  // VDD > 2.25V
    #define MS8607_02BA01_USER_VDDS_BELOW       (0x40)  // VDD < 2.25V

    #define MS8607_02BA01_USER_HTRE_MASK        (0x04)  // On-Chip Heater Status
    #define MS8607_02BA01_USER_HTRE_DISABLE     (0x00)  // On-chip Heater Disable
    #define MS8607_02BA01_USER_HTRE_ENABLE      (0x04)  // On-chip Heater Enable


typedef enum
{
    D1_OSR_256              = MS8607_02BA01_CMD_D1_OSR_256,
    D1_OSR_512              = MS8607_02BA01_CMD_D1_OSR_512,
    D1_OSR_1024             = MS8607_02BA01_CMD_D1_OSR_1024,
    D1_OSR_2048             = MS8607_02BA01_CMD_D1_OSR_2048,
    D1_OSR_4096             = MS8607_02BA01_CMD_D1_OSR_4096
    
} msD1OSR_t;

typedef enum
{
    D2_OSR_256              = MS8607_02BA01_CMD_D2_OSR_256,
    D2_OSR_512              = MS8607_02BA01_CMD_D2_OSR_512,
    D2_OSR_1024             = MS8607_02BA01_CMD_D2_OSR_1024,
    D2_OSR_2048             = MS8607_02BA01_CMD_D2_OSR_2048,
    D2_OSR_4096             = MS8607_02BA01_CMD_D2_OSR_4096
    
} msD2OSR_t;

typedef enum
{
    HUMIDITY_HOLD           = MS8607_02BA01_CMD_RH_HOLD,
    HUMIDITY_NO_HOLD        = MS8607_02BA01_CMD_RH_NO_HOLD
    
} msHumidityMode_t;

typedef enum
{
    D3_OSR_4096             = MS8607_02BA01_USER_RES_OSR_4096,
    D3_OSR_2048             = MS8607_02BA01_USER_RES_OSR_2048,
    D3_OSR_1024             = MS8607_02BA01_USER_RES_OSR_1024,
    D3_OSR_256              = MS8607_02BA01_USER_RES_OSR_256
    
} msResolution_t;

typedef enum
{
    VOLTAGE_OK              = MS8607_02BA01_USER_VDDS_ABOVE,
    VOLTAGE_LOW             = MS8607_02BA01_USER_VDDS_BELOW
    
} msVoltage_t;

typedef enum
{
    HEATER_DISABLE          = MS8607_02BA01_USER_HTRE_DISABLE,
    HEATER_ENABLE           = MS8607_02BA01_USER_HTRE_ENABLE
    
} msHeaterStatus_t;

typedef struct
{
    float P;
    float T;
    float H;
    
} msSensorData_t;


class MS8607_02BA01
{
    protected:
        // Instance-specific properties
        uint8_t ms_conversionDelay;
        msD1OSR_t ms_d1osr;
        msD2OSR_t ms_d2osr;
        msHumidityMode_t ms_humiditymode;
        msResolution_t ms_resolution;
        msVoltage_t ms_voltage;
        msHeaterStatus_t ms_heaterstatus;

    public:
        uint8_t ms_i2cAddressPT;
        uint8_t ms_i2cAddressRH;
    msSensorData_t ms_sensorData;
        uint16_t C[6];
        int32_t T2;
        int64_t OFF2, SENS2;
        void Reset(void);
        void Initialize(void);
        void getAddrPT_MS8607_02BA01(uint8_t i2cAddress);
        void getAddrRH_MS8607_02BA01(uint8_t i2cAddress);
        bool begin(void);
        uint32_t readTemperature(void);
        uint32_t readPressure(void);
        uint16_t readHumidity(void);
        void Measure_Sensor();
        void Measure_Temperature(void);
        void Measure_Pressure(void);
        void Measure_Humidity(void);
        void setinitiatePressure(msD1OSR_t d1osr);
        msD1OSR_t getinitiatePressure(void);
        void setinitiateTemperature(msD2OSR_t d2osr);
        msD2OSR_t getinitiateTemperature(void);
        void setHumidityMode(msHumidityMode_t tempmode);
        msHumidityMode_t getHumidityMode(void);
        void setResolution(msResolution_t resolution);
        msResolution_t getResolution(void);
        void setVoltage(msVoltage_t voltage);
        msVoltage_t getVoltage(void);
        void setHeaterStatus(msHeaterStatus_t heaterstatus);
        msHeaterStatus_t getHeaterStatus(void);
    
    private:

};
