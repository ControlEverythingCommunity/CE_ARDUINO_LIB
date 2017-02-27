/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMP180
        This code is designed to work with the BMP180_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BMP180_I2CS_A01#tabs-0-product_tabset-2
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
    #define BMP180_DEFAULT_ADDRESS                      (0x77)
    #define BMP180_CHIP_ID                              (0x55)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BMP180_CONVERSIONDELAY                      (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define BMP180_CAL_AC1_LSB_REG                      (0xAA)      // Calibration Data
    #define BMP180_CAL_AC1_MSB_REG                      (0xAB)      // Calibration Data
    #define BMP180_CAL_AC2_LSB_REG                      (0xAC)      // Calibration Data
    #define BMP180_CAL_AC2_MSB_REG                      (0xAD)      // Calibration Data
    #define BMP180_CAL_AC3_LSB_REG                      (0xAE)      // Calibration Data
    #define BMP180_CAL_AC3_MSB_REG                      (0xAF)      // Calibration Data
    #define BMP180_CAL_AC4_LSB_REG                      (0xB0)      // Calibration Data
    #define BMP180_CAL_AC4_MSB_REG                      (0xB1)      // Calibration Data
    #define BMP180_CAL_AC5_LSB_REG                      (0xB2)      // Calibration Data
    #define BMP180_CAL_AC5_MSB_REG                      (0xB3)      // Calibration Data
    #define BMP180_CAL_AC6_LSB_REG                      (0xB4)      // Calibration Data
    #define BMP180_CAL_AC6_MSB_REG                      (0xB5)      // Calibration Data
    #define BMP180_CAL_B1_LSB_REG                       (0xB6)      // Calibration Data
    #define BMP180_CAL_B1_MSB_REG                       (0xB7)      // Calibration Data
    #define BMP180_CAL_B2_LSB_REG                       (0xB8)      // Calibration Data
    #define BMP180_CAL_B2_MSB_REG                       (0xB9)      // Calibration Data
    #define BMP180_CAL_MB_LSB_REG                       (0xBA)      // Calibration Data
    #define BMP180_CAL_MB_MSB_REG                       (0xBB)      // Calibration Data
    #define BMP180_CAL_MC_LSB_REG                       (0xBC)      // Calibration Data
    #define BMP180_CAL_MC_MSB_REG                       (0xBD)      // Calibration Data
    #define BMP180_CAL_MD_LSB_REG                       (0xBE)      // Calibration Data
    #define BMP180_CAL_MD_MSB_REG                       (0xBF)      // Calibration Data
    #define BMP180_CHIP_ID_REG                          (0xD0)      // Chip ID
    #define BMP180_SOFTRESET_REG                        (0xE0)      // Softreset Register
    #define BMP180_CTRL_MEAS_REG                        (0xF4)      // Control Measure Register
    #define BMP180_MEASURE_MSB_REG                      (0xF6)      // Measure Value of MSB Register
    #define BMP180_MEASURE_LSB_REG                      (0xF7)      // Measure Value of LSB Register
    #define BMP180_MEASURE_xLSB_REG                     (0xF8)      // Measure Value of xLSB Register

/**************************************************************************
    MEASUREMENT CONTROL REGISTER CONFIGURATION
 **************************************************************************/
    #define BMP180_CTRL_MEAS_REG_MEASURE_TEMPERATURE                    (0x2E)      // Measurement Temperature
    #define BMP180_CTRL_MEAS_REG_MEASURE_PRESSURE                       (0x34)      // Measurement Pressure

    #define BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_LOW_POWER           (0x00)      // Oversampling_Setting: 0
    #define BMP180_CTRL_MEAS_REG_OVERSAMPLING_STANDARD                  (0x01)      // Oversampling_Setting: 1
    #define BMP180_CTRL_MEAS_REG_OVERSAMPLING_HIGH_RESOLUTION           (0x02)      // Oversampling_Setting: 2
    #define BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_HIGH_RESOLUTION     (0x03)      // Oversampling_Setting: 3


typedef struct
{
    
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    
    int16_t _B1;
    int16_t _B2;
    
    int16_t MB;
    int16_t MC;
    int16_t MD;
    
    long B5;
    
} bmpCalibData_t;

typedef struct
{
    float P;
    float T;
    float A;
    
} bmpSensorData_t;

class BMP180
{
    protected:
        // Instance-specific properties
        uint8_t bmp_conversionDelay;
        int32_t t_fine;
    
    public:
        uint8_t bmp_i2cAddress;
        bmpSensorData_t bmp_sensorData;
        bmpCalibData_t bmp_calibdata;
        void readCoefficients(void);
        void getAddr_BMP180(uint8_t i2cAddress);
        bool begin(void);
        void Reset();
        int32_t getUncompPressure(void);
        int16_t getUncompTemperature(void);
        void Measure_Sensor(void);
        void Measure_Pressure(uint8_t oversampling);
        void Measure_Temperature(void);
        void Measure_Altitude(float seaLevel);
        void Measure_seaLevelForAltitude(float altitude, float pressure);
        float readAltitude(float seaLevel);
        float seaLevelForAltitude(float altitude, float pressure);
  
    private:
    
        int16_t readRegisterS16(uint8_t i2cAddress, uint8_t reg);
};
