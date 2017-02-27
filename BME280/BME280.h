/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BME280
        This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BME280_I2CS_A01#tabs-0-product_tabset-2
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
    #define BME280_DEFAULT_ADDRESS                      (0x76)
    #define BME280_ADDRESS_UPDATED                      (0x77)
    #define BME280_CHIP_ID                              (0x60)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BME280_CONVERSIONDELAY                      (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define BME280_DIG_T1_LSB_REG                       (0x88)      // Calibration Data
    #define BME280_DIG_T1_MSB_REG                       (0x89)      // Calibration Data
    #define BME280_DIG_T2_LSB_REG                       (0x8A)      // Calibration Data
    #define BME280_DIG_T2_MSB_REG                       (0x8B)      // Calibration Data
    #define BME280_DIG_T3_LSB_REG                       (0x8C)      // Calibration Data
    #define BME280_DIG_T3_MSB_REG                       (0x8D)      // Calibration Data
    #define BME280_DIG_P1_LSB_REG                       (0x8E)      // Calibration Data
    #define BME280_DIG_P1_MSB_REG                       (0x8F)      // Calibration Data
    #define BME280_DIG_P2_LSB_REG                       (0x90)      // Calibration Data
    #define BME280_DIG_P2_MSB_REG                       (0x91)      // Calibration Data
    #define BME280_DIG_P3_LSB_REG                       (0x92)      // Calibration Data
    #define BME280_DIG_P3_MSB_REG                       (0x93)      // Calibration Data
    #define BME280_DIG_P4_LSB_REG                       (0x94)      // Calibration Data
    #define BME280_DIG_P4_MSB_REG                       (0x95)      // Calibration Data
    #define BME280_DIG_P5_LSB_REG                       (0x96)      // Calibration Data
    #define BME280_DIG_P5_MSB_REG                       (0x97)      // Calibration Data
    #define BME280_DIG_P6_LSB_REG                       (0x98)      // Calibration Data
    #define BME280_DIG_P6_MSB_REG                       (0x99)      // Calibration Data
    #define BME280_DIG_P7_LSB_REG                       (0x9A)      // Calibration Data
    #define BME280_DIG_P7_MSB_REG                       (0x9B)      // Calibration Data
    #define BME280_DIG_P8_LSB_REG                       (0x9C)      // Calibration Data
    #define BME280_DIG_P8_MSB_REG                       (0x9D)      // Calibration Data
    #define BME280_DIG_P9_LSB_REG                       (0x9E)      // Calibration Data
    #define BME280_DIG_P9_MSB_REG                       (0x9F)      // Calibration Data
    #define BME280_DIG_H1_REG                           (0xA0)      // Calibration Data
    #define BME280_CHIP_ID_REG                          (0xD0)      // Chip ID
    #define BME280_SOFTRESET_REG                        (0xE0)      // Softreset Register
    #define BME280_DIG_H2_LSB_REG                       (0xE1)      // Calibration Data
    #define BME280_DIG_H2_MSB_REG                       (0xE2)      // Calibration Data
    #define BME280_DIG_H3_REG                           (0xE3)      // Calibration Data
    #define BME280_DIG_H4_LSB_REG                       (0xE4)      // Calibration Data
    #define BME280_DIG_H4_MSB_REG                       (0xE5)      // Calibration Data
    #define BME280_DIG_H5_REG                           (0xE6)      // Calibration Data
    #define BME280_DIG_H6_REG                           (0xE7)      // Calibration Data
    #define BME280_CTRL_HUMIDITY_REG                    (0xF2)      // Control Humidity Register
    #define BME280_STAT_REG                             (0xF3)      // Status Register
    #define BME280_CTRL_MEAS_REG                        (0xF4)      // Control Measure Register
    #define BME280_CONFIG_REG                           (0xF5)      // Configuration Register
    #define BME280_PRESSURE_MSB_REG                     (0xF7)      // Pressure MSB Register
    #define BME280_PRESSURE_LSB_REG                     (0xF8)      // Pressure LSB Register
    #define BME280_PRESSURE_xLSB_REG                    (0xF9)      // Pressure xLSB Register
    #define BME280_TEMPERATURE_MSB_REG                  (0xFA)      // Temperature MSB Register
    #define BME280_TEMPERATURE_LSB_REG                  (0xFB)      // Temperature LSB Register
    #define BME280_TEMPERATURE_xLSB_REG                 (0xFC)      // Temperature xLSB Register
    #define BME280_HUMIDITY_MSB_REG                     (0xFD)      // Humidity MSB Register
    #define BME280_HUMIDITY_LSB_REG                     (0xFE)      // Humidity LSB Register

/**************************************************************************
    HUMIDITY CONTROL REGISTER CONFIGURATION
**************************************************************************/
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_MASK      (0x07)      // Humidity Oversampling
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_NONE      (0x00)      // Oversampling Skipped
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X1        (0x01)      // Oversampling X1
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X2        (0x02)      // Oversampling X2
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X4        (0x03)      // Oversampling X4
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X8        (0x04)      // Oversampling X8
    #define BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X16       (0x05)      // Oversampling X16

/**************************************************************************
    MEASUREMENT CONTROL REGISTER CONFIGURATION
 **************************************************************************/
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_MASK     (0xE0)      // Pressure Oversampling
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_NONE     (0x00)      // Oversampling Skipped
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1       (0x20)      // Oversampling X1
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X2       (0x40)      // Oversampling X2
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X4       (0x60)      // Oversampling X4
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X8       (0x80)      // Oversampling X8
    #define BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X16      (0xA0)      // Oversampling X16

    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_MASK     (0x1C)      // Temperature Oversampling
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_NONE     (0x00)      // Oversampling Skipped
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1       (0x04)      // Oversampling X1
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X2       (0x08)      // Oversampling X2
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X4       (0x0C)      // Oversampling X4
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X8       (0x10)      // Oversampling X8
    #define BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X16      (0x14)      // Oversampling X16

    #define BME280_CTRL_MEAS_REG_MODE_MASK                  (0x03)      // Mode Status
    #define BME280_CTRL_MEAS_REG_MODE_SLEEP                 (0x00)      // Sleep Mode
    #define BME280_CTRL_MEAS_REG_MODE_FORCED                (0x01)      // Forced Mode
    #define BME280_CTRL_MEAS_REG_MODE_NORMAL                (0x03)      // Normal Mode

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define BME280_CONFIG_REG_STANDBY_DURATION_MASK         (0xE0)      // Controls Inactive Duration tstandby in Normal Mode
    #define BME280_CONFIG_REG_STANDBY_DURATION_0_5          (0x00)      // tstandby [ms]: 0.5
    #define BME280_CONFIG_REG_STANDBY_DURATION_62_5         (0x20)      // tstandby [ms]: 62.5
    #define BME280_CONFIG_REG_STANDBY_DURATION_125          (0x40)      // tstandby [ms]: 125
    #define BME280_CONFIG_REG_STANDBY_DURATION_250          (0x60)      // tstandby [ms]: 250
    #define BME280_CONFIG_REG_STANDBY_DURATION_500          (0x80)      // tstandby [ms]: 500
    #define BME280_CONFIG_REG_STANDBY_DURATION_1000         (0xA0)      // tstandby [ms]: 1000
    #define BME280_CONFIG_REG_STANDBY_DURATION_10           (0xC0)      // tstandby [ms]: 10
    #define BME280_CONFIG_REG_STANDBY_DURATION_20           (0xE0)      // tstandby [ms]: 20

    #define BME280_CONFIG_REG_FILTER_COEFF_MASK             (0x1C)      // Controls the Time Constant of the IIR Filter
    #define BME280_CONFIG_REG_FILTER_COEFF_FILTER_OFF       (0x00)      // Filter Coefficient: Filter Off
    #define BME280_CONFIG_REG_FILTER_COEFF_2                (0x04)      // Filter Coefficient: 2
    #define BME280_CONFIG_REG_FILTER_COEFF_4                (0x08)      // Filter Coefficient: 4
    #define BME280_CONFIG_REG_FILTER_COEFF_8                (0x0C)      // Filter Coefficient: 8
    #define BME280_CONFIG_REG_FILTER_COEFF_16               (0x10)      // Filter Coefficient: 16

    #define BME280_CONFIG_REG_SPI3W_EN_MASK                 (0x01)      // 3-Wire SPI Interface
    #define BME280_CONFIG_REG_SPI3W_EN_DISABLE              (0x00)      // 4-Wire SPI Interface
    #define BME280_CONFIG_REG_SPI3W_EN_ENABLE               (0x01)      // 3-Wire SPI Interface


typedef enum
{
    HUMIDITY_OVERSAMPLING_NONE              = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_NONE,
    HUMIDITY_OVERSAMPLING_X1                = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X1,
    HUMIDITY_OVERSAMPLING_X2                = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X2,
    HUMIDITY_OVERSAMPLING_X4                = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X4,
    HUMIDITY_OVERSAMPLING_X8                = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X8,
    HUMIDITY_OVERSAMPLING_X16               = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X16
    
} bmeHumiditySampling_t;

typedef enum
{
    PRESSURE_OVERSAMPLING_NONE              = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_NONE,
    PRESSURE_OVERSAMPLING_X1                = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1,
    PRESSURE_OVERSAMPLING_X2                = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X2,
    PRESSURE_OVERSAMPLING_X4                = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X4,
    PRESSURE_OVERSAMPLING_X8                = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X8,
    PRESSURE_OVERSAMPLING_X16               = BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X16
    
} bmePressureSampling_t;

typedef enum
{
    TEMPERATURE_OVERSAMPLING_NONE           = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_NONE,
    TEMPERATURE_OVERSAMPLING_X1             = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1,
    TEMPERATURE_OVERSAMPLING_X2             = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X2,
    TEMPERATURE_OVERSAMPLING_X4             = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X4,
    TEMPERATURE_OVERSAMPLING_X8             = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X8,
    TEMPERATURE_OVERSAMPLING_X16            = BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X16
    
} bmeTemperatureSampling_t;

typedef enum
{
    SENSOR_MODE_SLEEP                       = BME280_CTRL_MEAS_REG_MODE_SLEEP,
    SENSOR_MODE_FORCED                      = BME280_CTRL_MEAS_REG_MODE_FORCED,
    SENSOR_MODE_NORMAL                      = BME280_CTRL_MEAS_REG_MODE_NORMAL
    
} bmeSensorMode_t;

typedef enum
{
    STANDBY_DURATION_0_5MS                  = BME280_CONFIG_REG_STANDBY_DURATION_0_5,
    STANDBY_DURATION_62_5MS                 = BME280_CONFIG_REG_STANDBY_DURATION_62_5,
    STANDBY_DURATION_125MS                  = BME280_CONFIG_REG_STANDBY_DURATION_125,
    STANDBY_DURATION_250MS                  = BME280_CONFIG_REG_STANDBY_DURATION_250,
    STANDBY_DURATION_500MS                  = BME280_CONFIG_REG_STANDBY_DURATION_500,
    STANDBY_DURATION_1000MS                 = BME280_CONFIG_REG_STANDBY_DURATION_1000,
    STANDBY_DURATION_10MS                   = BME280_CONFIG_REG_STANDBY_DURATION_10,
    STANDBY_DURATION_20MS                   = BME280_CONFIG_REG_STANDBY_DURATION_20
    
} bmeStandbyDuration_t;

typedef enum
{
    SENSOR_FILTER_OFF                       = BME280_CONFIG_REG_FILTER_COEFF_FILTER_OFF,
    SENSOR_FILTER_2                         = BME280_CONFIG_REG_FILTER_COEFF_2,
    SENSOR_FILTER_4                         = BME280_CONFIG_REG_FILTER_COEFF_4,
    SENSOR_FILTER_8                         = BME280_CONFIG_REG_FILTER_COEFF_8,
    SENSOR_FILTER_16                        = BME280_CONFIG_REG_FILTER_COEFF_16
    
} bmeSensorFilter_t;

typedef enum
{
    SPI_4_WIRE                       = BME280_CONFIG_REG_SPI3W_EN_DISABLE,
    SPI_3_WIRE                       = BME280_CONFIG_REG_SPI3W_EN_ENABLE,
    
} bmeSPIWire_t;


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
    
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
    
} bmeCalibData_t;

typedef struct
{
    float P;
    float T;
    float H;
    
} bmeSensorData_t;

class BME280
{
    protected:
        // Instance-specific properties
        uint8_t bme_conversionDelay;
        int32_t t_fine;
        bmeHumiditySampling_t bme_humiditysampling;
        bmePressureSampling_t bme_pressuresampling;
        bmeTemperatureSampling_t bme_temperaturesampling;
        bmeSensorMode_t bme_sensormode;
        bmeStandbyDuration_t bme_standbyduration;
        bmeSensorFilter_t bme_sensorfilter;
        bmeSPIWire_t bme_spiwire;
    
    public:
        uint8_t bme_i2cAddress;
        bmeSensorData_t bme_sensorData;
        bmeCalibData_t bme_calibdata;
        void readCoefficients(void);
        void getAddr_BME280(uint8_t i2cAddress);
        bool begin(void);
        void Reset();
        void setUpSampling(void);
        void Measure_Sensor(void);
        void Measure_Pressure(void);
        void Measure_Temperature(void);
        void Measure_Humidity(void);
        void Measure_Altitude(float seaLevel);
        void Measure_seaLevelForAltitude(float altitude, float pressure);
        void setHumiditySampling(bmeHumiditySampling_t humiditysampling);
        bmeHumiditySampling_t getHumiditySampling(void);
        void setPressureSampling(bmePressureSampling_t pressuresampling);
        bmePressureSampling_t getPressureSampling(void);
        void setTemperatureSampling(bmeTemperatureSampling_t temperaturesampling);
        bmeTemperatureSampling_t getTemperatureSampling(void);
        void setSensorMode(bmeSensorMode_t sensormode);
        bmeSensorMode_t getSensorMode(void);
        void setStandbyDuration(bmeStandbyDuration_t standbyduration);
        bmeStandbyDuration_t getStandbyDuration(void);
        void setSensorFilter(bmeSensorFilter_t sensorfilter);
        bmeSensorFilter_t getSensorFilter(void);
        void setSPIWire(bmeSPIWire_t spiwire);
        bmeSPIWire_t getSPIWire(void);
    
  
    private:
    
        int16_t readRegisterS16(uint8_t i2cAddress, uint8_t reg);
        uint16_t readRegister16LE(uint8_t i2cAddress, uint8_t reg);
        int16_t readRegisterS16LE(uint8_t i2cAddress, uint8_t reg);
};
