/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMP280
        This code is designed to work with the BMP280_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BMP280_I2CS_A01#tabs-0-product_tabset-2
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
    #define BMP280_DEFAULT_ADDRESS                      (0x76)
    #define BMP280_ADDRESS_UPDATED                      (0x77)
    #define BMP280_CHIP_ID                              (0x58)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BMP280_CONVERSIONDELAY                      (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define BMP280_DIG_T1_LSB_REG                       (0x88)      // Calibration Data
    #define BMP280_DIG_T1_MSB_REG                       (0x89)      // Calibration Data
    #define BMP280_DIG_T2_LSB_REG                       (0x8A)      // Calibration Data
    #define BMP280_DIG_T2_MSB_REG                       (0x8B)      // Calibration Data
    #define BMP280_DIG_T3_LSB_REG                       (0x8C)      // Calibration Data
    #define BMP280_DIG_T3_MSB_REG                       (0x8D)      // Calibration Data
    #define BMP280_DIG_P1_LSB_REG                       (0x8E)      // Calibration Data
    #define BMP280_DIG_P1_MSB_REG                       (0x8F)      // Calibration Data
    #define BMP280_DIG_P2_LSB_REG                       (0x90)      // Calibration Data
    #define BMP280_DIG_P2_MSB_REG                       (0x91)      // Calibration Data
    #define BMP280_DIG_P3_LSB_REG                       (0x92)      // Calibration Data
    #define BMP280_DIG_P3_MSB_REG                       (0x93)      // Calibration Data
    #define BMP280_DIG_P4_LSB_REG                       (0x94)      // Calibration Data
    #define BMP280_DIG_P4_MSB_REG                       (0x95)      // Calibration Data
    #define BMP280_DIG_P5_LSB_REG                       (0x96)      // Calibration Data
    #define BMP280_DIG_P5_MSB_REG                       (0x97)      // Calibration Data
    #define BMP280_DIG_P6_LSB_REG                       (0x98)      // Calibration Data
    #define BMP280_DIG_P6_MSB_REG                       (0x99)      // Calibration Data
    #define BMP280_DIG_P7_LSB_REG                       (0x9A)      // Calibration Data
    #define BMP280_DIG_P7_MSB_REG                       (0x9B)      // Calibration Data
    #define BMP280_DIG_P8_LSB_REG                       (0x9C)      // Calibration Data
    #define BMP280_DIG_P8_MSB_REG                       (0x9D)      // Calibration Data
    #define BMP280_DIG_P9_LSB_REG                       (0x9E)      // Calibration Data
    #define BMP280_DIG_P9_MSB_REG                       (0x9F)      // Calibration Data
    #define BMP280_CHIP_ID_REG                          (0xD0)      // Chip ID
    #define BMP280_SOFTRESET_REG                        (0xE0)      // Softreset Register
    #define BMP280_STAT_REG                             (0xF3)      // Status Register
    #define BMP280_CTRL_MEAS_REG                        (0xF4)      // Control Measure Register
    #define BMP280_CONFIG_REG                           (0xF5)      // Configuration Register
    #define BMP280_PRESSURE_MSB_REG                     (0xF7)      // Pressure MSB Register
    #define BMP280_PRESSURE_LSB_REG                     (0xF8)      // Pressure LSB Register
    #define BMP280_PRESSURE_xLSB_REG                    (0xF9)      // Pressure xLSB Register
    #define BMP280_TEMPERATURE_MSB_REG                  (0xFA)      // Temperature MSB Register
    #define BMP280_TEMPERATURE_LSB_REG                  (0xFB)      // Temperature LSB Register
    #define BMP280_TEMPERATURE_xLSB_REG                 (0xFC)      // Temperature xLSB Register

/**************************************************************************
    MEASUREMENT CONTROL REGISTER CONFIGURATION
 **************************************************************************/
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_MASK     (0xE0)      // Pressure Oversampling
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_NONE     (0x00)      // Oversampling Skipped
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1       (0x20)      // Oversampling X1
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X2       (0x40)      // Oversampling X2
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X4       (0x60)      // Oversampling X4
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X8       (0x80)      // Oversampling X8
    #define BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X16      (0xA0)      // Oversampling X16

    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_MASK     (0x1C)      // Temperature Oversampling
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_NONE     (0x00)      // Oversampling Skipped
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1       (0x04)      // Oversampling X1
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X2       (0x08)      // Oversampling X2
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X4       (0x0C)      // Oversampling X4
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X8       (0x10)      // Oversampling X8
    #define BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X16      (0x14)      // Oversampling X16

    #define BMP280_CTRL_MEAS_REG_MODE_MASK                  (0x03)      // Mode Status
    #define BMP280_CTRL_MEAS_REG_MODE_SLEEP                 (0x00)      // Sleep Mode
    #define BMP280_CTRL_MEAS_REG_MODE_FORCED                (0x01)      // Forced Mode
    #define BMP280_CTRL_MEAS_REG_MODE_NORMAL                (0x03)      // Normal Mode

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define BMP280_CONFIG_REG_STANDBY_DURATION_MASK         (0xE0)      // Controls Inactive Duration tstandby in Normal Mode
    #define BMP280_CONFIG_REG_STANDBY_DURATION_0_5          (0x00)      // tstandby [ms]: 0.5
    #define BMP280_CONFIG_REG_STANDBY_DURATION_62_5         (0x20)      // tstandby [ms]: 62.5
    #define BMP280_CONFIG_REG_STANDBY_DURATION_125          (0x40)      // tstandby [ms]: 125
    #define BMP280_CONFIG_REG_STANDBY_DURATION_250          (0x60)      // tstandby [ms]: 250
    #define BMP280_CONFIG_REG_STANDBY_DURATION_500          (0x80)      // tstandby [ms]: 500
    #define BMP280_CONFIG_REG_STANDBY_DURATION_1000         (0xA0)      // tstandby [ms]: 1000
    #define BMP280_CONFIG_REG_STANDBY_DURATION_2000         (0xC0)      // tstandby [ms]: 2000
    #define BMP280_CONFIG_REG_STANDBY_DURATION_4000         (0xE0)      // tstandby [ms]: 4000

    #define BMP280_CONFIG_REG_FILTER_COEFF_MASK             (0x1C)      // Controls the Time Constant of the IIR Filter
    #define BMP280_CONFIG_REG_FILTER_COEFF_FILTER_OFF       (0x00)      // Filter Coefficient: Filter Off
    #define BMP280_CONFIG_REG_FILTER_COEFF_2                (0x04)      // Filter Coefficient: 2
    #define BMP280_CONFIG_REG_FILTER_COEFF_4                (0x08)      // Filter Coefficient: 4
    #define BMP280_CONFIG_REG_FILTER_COEFF_8                (0x0C)      // Filter Coefficient: 8
    #define BMP280_CONFIG_REG_FILTER_COEFF_16               (0x10)      // Filter Coefficient: 16

    #define BMP280_CONFIG_REG_SPI3W_EN_MASK                 (0x01)      // 3-Wire SPI Interface
    #define BMP280_CONFIG_REG_SPI3W_EN_DISABLE              (0x00)      // 4-Wire SPI Interface
    #define BMP280_CONFIG_REG_SPI3W_EN_ENABLE               (0x01)      // 3-Wire SPI Interface


typedef enum
{
    PRESSURE_OVERSAMPLING_NONE              = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_NONE,
    PRESSURE_OVERSAMPLING_X1                = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1,
    PRESSURE_OVERSAMPLING_X2                = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X2,
    PRESSURE_OVERSAMPLING_X4                = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X4,
    PRESSURE_OVERSAMPLING_X8                = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X8,
    PRESSURE_OVERSAMPLING_X16               = BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X16
    
} bmpPressureSampling_t;

typedef enum
{
    TEMPERATURE_OVERSAMPLING_NONE           = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_NONE,
    TEMPERATURE_OVERSAMPLING_X1             = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1,
    TEMPERATURE_OVERSAMPLING_X2             = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X2,
    TEMPERATURE_OVERSAMPLING_X4             = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X4,
    TEMPERATURE_OVERSAMPLING_X8             = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X8,
    TEMPERATURE_OVERSAMPLING_X16            = BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X16
    
} bmpTemperatureSampling_t;

typedef enum
{
    SENSOR_MODE_SLEEP                       = BMP280_CTRL_MEAS_REG_MODE_SLEEP,
    SENSOR_MODE_FORCED                      = BMP280_CTRL_MEAS_REG_MODE_FORCED,
    SENSOR_MODE_NORMAL                      = BMP280_CTRL_MEAS_REG_MODE_NORMAL
    
} bmpSensorMode_t;

typedef enum
{
    STANDBY_DURATION_0_5MS                  = BMP280_CONFIG_REG_STANDBY_DURATION_0_5,
    STANDBY_DURATION_62_5MS                 = BMP280_CONFIG_REG_STANDBY_DURATION_62_5,
    STANDBY_DURATION_125MS                  = BMP280_CONFIG_REG_STANDBY_DURATION_125,
    STANDBY_DURATION_250MS                  = BMP280_CONFIG_REG_STANDBY_DURATION_250,
    STANDBY_DURATION_500MS                  = BMP280_CONFIG_REG_STANDBY_DURATION_500,
    STANDBY_DURATION_1000MS                 = BMP280_CONFIG_REG_STANDBY_DURATION_1000,
    STANDBY_DURATION_2000MS                 = BMP280_CONFIG_REG_STANDBY_DURATION_2000,
    STANDBY_DURATION_4000MS                 = BMP280_CONFIG_REG_STANDBY_DURATION_4000
    
} bmpStandbyDuration_t;

typedef enum
{
    SENSOR_FILTER_OFF                       = BMP280_CONFIG_REG_FILTER_COEFF_FILTER_OFF,
    SENSOR_FILTER_2                         = BMP280_CONFIG_REG_FILTER_COEFF_2,
    SENSOR_FILTER_4                         = BMP280_CONFIG_REG_FILTER_COEFF_4,
    SENSOR_FILTER_8                         = BMP280_CONFIG_REG_FILTER_COEFF_8,
    SENSOR_FILTER_16                        = BMP280_CONFIG_REG_FILTER_COEFF_16
    
} bmpSensorFilter_t;

typedef enum
{
    SPI_4_WIRE                       = BMP280_CONFIG_REG_SPI3W_EN_DISABLE,
    SPI_3_WIRE                       = BMP280_CONFIG_REG_SPI3W_EN_ENABLE,
    
} bmpSPIWire_t;


typedef struct
{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    
} bmpCalibData_t;

typedef struct
{
    float P;
    float T;
    
} bmpSensorData_t;

class BMP280
{
    protected:
        // Instance-specific properties
        uint8_t bmp_conversionDelay;
        int32_t t_fine;
        bmpPressureSampling_t bmp_pressuresampling;
        bmpTemperatureSampling_t bmp_temperaturesampling;
        bmpSensorMode_t bmp_sensormode;
        bmpStandbyDuration_t bmp_standbyduration;
        bmpSensorFilter_t bmp_sensorfilter;
        bmpSPIWire_t bmp_spiwire;
    
    public:
        uint8_t bmp_i2cAddress;
        bmpSensorData_t bmp_sensorData;
        bmpCalibData_t bmp_calibdata;
        void readCoefficients(void);
        void getAddr_BMP280(uint8_t i2cAddress);
        bool begin(void);
        void Reset();
        void setUpSampling(void);
        void Measure_Sensor(void);
        void Measure_Pressure(void);
        void Measure_Temperature(void);
        void setPressureSampling(bmpPressureSampling_t pressuresampling);
        bmpPressureSampling_t getPressureSampling(void);
        void setTemperatureSampling(bmpTemperatureSampling_t temperaturesampling);
        bmpTemperatureSampling_t getTemperatureSampling(void);
        void setSensorMode(bmpSensorMode_t sensormode);
        bmpSensorMode_t getSensorMode(void);
        void setStandbyDuration(bmpStandbyDuration_t standbyduration);
        bmpStandbyDuration_t getStandbyDuration(void);
        void setSensorFilter(bmpSensorFilter_t sensorfilter);
        bmpSensorFilter_t getSensorFilter(void);
        void setSPIWire(bmpSPIWire_t spiwire);
        bmpSPIWire_t getSPIWire(void);
    
    private:
    
        int16_t readRegisterS16(uint8_t i2cAddress, uint8_t reg);
        uint16_t readRegister16LE(uint8_t i2cAddress, uint8_t reg);
        int16_t readRegisterS16LE(uint8_t i2cAddress, uint8_t reg);
};
