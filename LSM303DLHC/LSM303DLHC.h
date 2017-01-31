/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM303DLHC
        This code is designed to work with the LSM303DLHC_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define LSM303DLHC_DEFAULT_ADDRESS_ACCEL                    (0x19)      // 0011001b
    #define LSM303DLHC_DEFAULT_ADDRESS_MAG                      (0x1E)      // 0011110xb

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LSM303DLHC_CONVERSIONDELAY                          (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1                      (0x20)      // Accelerometer Control Register 1
    #define LSM303DLHC_REG_ACCEL_CTRL_REG2                      (0x21)      // Accelerometer Control Register 2
    #define LSM303DLHC_REG_ACCEL_CTRL_REG3                      (0x22)      // Accelerometer Control Register 3
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4                      (0x23)      // Accelerometer Control Register 4
    #define LSM303DLHC_REG_ACCEL_CTRL_REG5                      (0x24)      // Accelerometer Control Register 5
    #define LSM303DLHC_REG_ACCEL_CTRL_REG6                      (0x25)      // Accelerometer Control Register 6
    #define LSM303DLHC_REG_ACCEL_REFERENCE                      (0x26)      // Reference/Data Capture Register
    #define LSM303DLHC_REG_ACCEL_STATUS                         (0x27)      // Acceleration Status Register
    #define LSM303DLHC_REG_ACCEL_OUT_X_L                        (0x28)      // X-Axis Acceleration Data Low Register
    #define LSM303DLHC_REG_ACCEL_OUT_X_H                        (0x29)      // X-Axis Acceleration Data High Register
    #define LSM303DLHC_REG_ACCEL_OUT_Y_L                        (0x2A)      // Y-Axis Acceleration Data Low Register
    #define LSM303DLHC_REG_ACCEL_OUT_Y_H                        (0x2B)      // Y-Axis Acceleration Data High Register
    #define LSM303DLHC_REG_ACCEL_OUT_Z_L                        (0x2C)      // Z-Axis Acceleration Data Low Register
    #define LSM303DLHC_REG_ACCEL_OUT_Z_H                        (0x2D)      // Z-Axis Acceleration Data High Register
    #define LSM303DLHC_REG_ACCEL_FIFO_CTRL                      (0x2E)      // FIFO Control Register
    #define LSM303DLHC_REG_ACCEL_FIFO_SRC                       (0x2F)      // FIFO Source Register
    #define LSM303DLHC_REG_ACCEL_INT1_CFG                       (0x30)      // Interrupt 1 Configuration Register
    #define LSM303DLHC_REG_ACCEL_INT1_SRC                       (0x31)      // Interrupt 1 Source Register
    #define LSM303DLHC_REG_ACCEL_INT1_THS                       (0x32)      // Interrupt 1 Threshold Register
    #define LSM303DLHC_REG_ACCEL_INT1_DUR                       (0x33)      // Interrupt 1 Duration Register
    #define LSM303DLHC_REG_ACCEL_INT2_CFG                       (0x34)      // Interrupt 2 Configuration Register
    #define LSM303DLHC_REG_ACCEL_INT2_SRC                       (0x35)      // Interrupt 2 Source Register
    #define LSM303DLHC_REG_ACCEL_INT2_THS                       (0x36)      // Interrupt 2 Threshold Register
    #define LSM303DLHC_REG_ACCEL_INT2_DUR                       (0x37)      // Interrupt 2 Duration Register
    #define LSM303DLHC_REG_ACCEL_CLICK_CFG                      (0x38)      // Interrupt Click Recognition Register
    #define LSM303DLHC_REG_ACCEL_CLICK_SRC                      (0x39)      // Interrupt Click Source Register
    #define LSM303DLHC_REG_ACCEL_CLICK_THS                      (0x3A)      // Interrupt Click Threshold Register
    #define LSM303DLHC_REG_ACCEL_TIME_LIMIT                     (0x3B)      // Click Time Limit Register
    #define LSM303DLHC_REG_ACCEL_TIME_LAT                       (0x3C)      // Click Time Latency Register
    #define LSM303DLHC_REG_ACCEL_TIME_WIN                       (0x3D)      // Click Time Window Register

/**************************************************************************
    MAGNETOMETER REGISTERS
**************************************************************************/
    #define LSM303DLHC_REG_MAG_CRA_REG                          (0x00)      // Magnetic Field Sensing A Register
    #define LSM303DLHC_REG_MAG_CRB_REG                          (0x01)      // Magnetic Field Sensing B Register
    #define LSM303DLHC_REG_MAG_MR_REG                           (0x02)      // Magnetic Mode Select Register
    #define LSM303DLHC_REG_MAG_OUT_X_H                          (0x03)      // X-Axis Magnetic Data High Register
    #define LSM303DLHC_REG_MAG_OUT_X_L                          (0x04)      // X-Axis Magnetic Data Low Register
    #define LSM303DLHC_REG_MAG_OUT_Y_H                          (0x05)      // Y-Axis Magnetic Data High Register
    #define LSM303DLHC_REG_MAG_OUT_Y_L                          (0x06)      // Y-Axis Magnetic Data Low Register
    #define LSM303DLHC_REG_MAG_OUT_Z_H                          (0x07)      // Z-Axis Magnetic Data High Register
    #define LSM303DLHC_REG_MAG_OUT_Z_L                          (0x08)      // Z-Axis Magnetic Data Low Register
    #define LSM303DLHC_REG_MAG_SR_REG                           (0x09)      // Data Status Ready Register
    #define LSM303DLHC_REG_MAG_IRA_REG_M                        (0x0A)      // Interrupt Request/Recognition A Register
    #define LSM303DLHC_REG_MAG_IRB_REG_M                        (0x0B)      // Interrupt Request/Recognition B Register
    #define LSM303DLHC_REG_MAG_IRC_REG_M                        (0x0C)      // Interrupt Request/Recognition C Register
    #define LSM303DLHC_REG_TEMP_OUT_H                           (0x31)      // Temperature Sensor Data High Register
    #define LSM303DLHC_REG_TEMP_OUT_L                           (0x32)      // Temperature Sensor Data Low Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_MASK            (0xF0)      // Acceleration Data Rate Selection
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_PD              (0x00)      // Power-Down Mode
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1               (0x10)      // Normal / Low-Power Mode (1 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_10              (0x20)      // Normal / Low-Power Mode (10 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_25              (0x30)      // Normal / Low-Power Mode (25 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_50              (0x40)      // Normal / Low-Power Mode (50 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_100             (0x50)      // Normal / Low-Power Mode (100 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_200             (0x60)      // Normal / Low-Power Mode (200 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_400             (0x70)      // Normal / Low-Power Mode (400 Hz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1600            (0x80)      // Low-Power Mode (1.620 kHz)
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1344            (0x90)      // Normal (1.344 kHz) / Low-Power Mode (5.376 kHz)

    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_MASK            (0x08)      // Low-Power Mode Enable
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_NORMAL          (0x00)      // Normal Mode
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_LOW             (0x08)      // Low-Power Mode

    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_MASK            (0x04)      // Acceleration Z-axis Enable
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_DISABLE         (0x00)      // Acceleration Z-axis Disabled
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_ENABLE          (0x04)      // Acceleration Z-axis Enabled

    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_MASK            (0x02)      // Acceleration Y-axis Enable
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_DISABLE         (0x00)      // Acceleration Y-axis Disabled
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_ENABLE          (0x02)      // Acceleration Y-axis Enabled

    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_MASK            (0x01)      // Acceleration X-axis Enable
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_DISABLE         (0x00)      // Acceleration X-axis Disabled
    #define LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_ENABLE          (0x01)      // Acceleration X-axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_MASK             (0x80)      // Block Data Update
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS       (0x00)      // Continuous Update
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATE        (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_MASK             (0x40)      // Big/Little Endian Data Selection
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_LSB              (0x00)      // Data LSB @ lower address
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_MSB              (0x40)      // Data MSB @ lower address

    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_MASK              (0x30)      // Full-Scale Selection
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_2G                (0x00)      // +/- 2G
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_4G                (0x10)      // +/- 4G
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_8G                (0x20)      // +/- 8G
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_16G               (0x30)      // +/- 16G

    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_MASK              (0x08)      // High Resolution Output Mode
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_DISABLE           (0x00)      // High Resolution Disable
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_ENABLE            (0x08)      // High Resolution Enable

    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_MASK             (0x01)      // SPI Serial Interface Mode Selection
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_4WIRE            (0x00)      // 4-Wire Interface
    #define LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_3WIRE            (0x01)      // 3-Wire Interface

/**************************************************************************
    MAGNETOMETER MAGNETIC FIELD SENSING A REGISTER DESCRIPTION
**************************************************************************/
    #define LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_MASK               (0x80)      // Temperature Sensor Enable
    #define LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_DISABLED           (0x00)      // Temperature Sensor Disabled
    #define LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_ENABLED            (0x80)      // Temperature Sensor Enabled

    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_MASK               (0x1C)      // Magnetic Data Rate Selection
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_0_75               (0x00)      // Minimum Data Output Rate: 0.75 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_1_5                (0x04)      // Minimum Data Output Rate: 1.5 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_3                  (0x08)      // Minimum Data Output Rate: 3 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_7_5                (0x0C)      // Minimum Data Output Rate: 7.5 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_15                 (0x10)      // Minimum Data Output Rate: 15 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_30                 (0x14)      // Minimum Data Output Rate: 30 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_75                 (0x18)      // Minimum Data Output Rate: 75 Hz
    #define LSM303DLHC_REG_MAG_CTRL_CRA_MODR_220                (0x1C)      // Minimum Data Output Rate: 220 Hz

/**************************************************************************
    MAGNETOMETER MAGNETIC FIELD SENSING B REGISTER DESCRIPTION
**************************************************************************/
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_MASK               (0xE0)      // Magnetic Gain Configuration
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_1_3                (0x20)      // Sensor Input Field Range[Gauss]: ±1.3
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_1_9                (0x40)      // Sensor Input Field Range[Gauss]: ±1.9
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_2_5                (0x60)      // Sensor Input Field Range[Gauss]: ±2.5
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_4_0                (0x80)      // Sensor Input Field Range[Gauss]: ±4.0
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_4_7                (0xA0)      // Sensor Input Field Range[Gauss]: ±4.7
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_5_6                (0xC0)      // Sensor Input Field Range[Gauss]: ±5.7
    #define LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_8_1                (0xE0)      // Sensor Input Field Range[Gauss]: ±8.1

/**************************************************************************
    MAGNETOMETER MODE SELECT REGISTER DESCRIPTION
**************************************************************************/
    #define LSM303DLHC_REG_MAG_CTRL_MODE_MASK                   (0x03)      // Magnetic Sensor Mode Selection
    #define LSM303DLHC_REG_MAG_CTRL_MODE_CONTINUOUS             (0x00)      // Continuous-Conversion Mode
    #define LSM303DLHC_REG_MAG_CTRL_MODE_SINGLE                 (0x01)      // Single-conversion mode
    #define LSM303DLHC_REG_MAG_CTRL_MODE_SLEEP                  (0x03)      // Sleep Mode

typedef enum
{
    ACCEL_DATARATE_POWERDOWN        = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_PD,
    ACCEL_DATARATE_1HZ              = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1,
    ACCEL_DATARATE_10HZ             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_10,
    ACCEL_DATARATE_25HZ             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_25,
    ACCEL_DATARATE_50HZ             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_200HZ            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_200,
    ACCEL_DATARATE_400HZ            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_1600HZ           = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1600,
    ACCEL_DATARATE_1344HZ           = LSM303DLHC_REG_ACCEL_CTRL_REG1_AODR_1344
    
} lsmAccelDataRate_t;

typedef enum
{
    ACCEL_NORMAL_MODE               = LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_NORMAL,
    ACCEL_LOW_POWER_MODE            = LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_LOW
    
} lsmAccelMode_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} lsmAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} lsmAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED            = LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED             = LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} lsmAccelXen_t;

typedef enum
{
    ACCEL_CONTINUOUS                 = LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED                = LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATE
    
} lsmAccelBlockData_t;

typedef enum
{
    ACCEL_DATA_LSB                   = LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_LSB,
    ACCEL_DATA_MSB                   = LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_MSB
    
} lsmAccelEndianData_t;

typedef enum
{
    ACCEL_RANGE_2G                  = LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_2G,
    ACCEL_RANGE_4G                  = LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_4G,
    ACCEL_RANGE_8G                  = LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_8G,
    ACCEL_RANGE_16G                 = LSM303DLHC_REG_ACCEL_CTRL_REG4_FS_16G
    
} lsmAccelRange_t;

typedef enum
{
    ACCEL_HIGH_RES_DISABLE          = LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_DISABLE,
    ACCEL_HIGH_RES_ENABLE           = LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_ENABLE
    
} lsmAccelResolution_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                = LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                = LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_3WIRE
    
} lsmAccelSPIWire_t;

typedef enum
{
    TEMP_SENSOR_DISABLED            = LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_DISABLED,
    TEMP_SENSOR_ENABLED             = LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_ENABLED
    
} lsmTempSensor_t;

typedef enum
{
    MAG_DATARATE_0_75HZ             = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_0_75,
    MAG_DATARATE_1_5HZ              = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_1_5,
    MAG_DATARATE_3HZ                = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_3,
    MAG_DATARATE_7_5HZ              = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_7_5,
    MAG_DATARATE_15HZ               = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_15,
    MAG_DATARATE_30HZ               = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_30,
    MAG_DATARATE_75HZ               = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_75,
    MAG_DATARATE_220HZ              = LSM303DLHC_REG_MAG_CTRL_CRA_MODR_220
    
} lsmMagDataRate_t;

typedef enum
{
    MAG_GAIN_1_3GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_1_3,
    MAG_GAIN_1_9GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_1_9,
    MAG_GAIN_2_5GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_2_5,
    MAG_GAIN_4GAUSS                 = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_4_0,
    MAG_GAIN_4_7GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_4_7,
    MAG_GAIN_5_6GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_5_6,
    MAG_GAIN_8_1GAUSS               = LSM303DLHC_REG_MAG_CTRL_CRB_GAIN_8_1
    
} lsmMagGain_t;

typedef enum
{
    MAG_CONTINUOUS                  = LSM303DLHC_REG_MAG_CTRL_MODE_CONTINUOUS,
    MAG_SINGLE                      = LSM303DLHC_REG_MAG_CTRL_MODE_SINGLE,
    MAG_SLEEP                       = LSM303DLHC_REG_MAG_CTRL_MODE_SLEEP
    
} lsmMagMode_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float   T;
    
} lsmSensorData_t;


class LSM303DLHC
{
    protected:
        // Instance-specific properties
        uint8_t lsm_conversionDelay;
        lsmAccelDataRate_t lsm_acceldatarate;
        lsmAccelMode_t lsm_accelmode;
        lsmAccelZen_t lsm_accelzen;
        lsmAccelYen_t lsm_accelyen;
        lsmAccelXen_t lsm_accelxen;
        lsmAccelBlockData_t lsm_accelblockdata;
        lsmAccelEndianData_t lsm_accelendiandata;
        lsmAccelRange_t lsm_accelrange;
        lsmAccelResolution_t lsm_accelresolution;
        lsmAccelSPIWire_t lsm_accelspiwire;
        lsmTempSensor_t lsm_tempsensor;
        lsmMagDataRate_t lsm_magdatarate;
        lsmMagGain_t lsm_maggain;
        lsmMagMode_t lsm_magmode;

    public:
        uint8_t lsm_i2cAddressX;
        uint8_t lsm_i2cAddressM;
        lsmSensorData_t lsm_accelData;
        lsmSensorData_t lsm_magData;
        lsmSensorData_t lsm_tempData;
        void getAddrX_LSM303DLHC(uint8_t i2cAddress);
        void getAddrM_LSM303DLHC(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpAccelerometer(void);
        void setUpMagnetometer(void);
        void Measure_Accelerometer(void);
        void Measure_Magnetometer(void);
        void Measure_Temperature(void);
        void setAccelDataRate(lsmAccelDataRate_t acceldatarate);
        lsmAccelDataRate_t getAccelDataRate(void);
        void setAccelMode(lsmAccelMode_t accelmode);
        lsmAccelMode_t getAccelMode(void);
        void setAccelZen(lsmAccelZen_t accelzen);
        lsmAccelZen_t getAccelZen(void);
        void setAccelYen(lsmAccelYen_t accelyen);
        lsmAccelYen_t getAccelYen(void);
        void setAccelXen(lsmAccelXen_t accelxen);
        lsmAccelXen_t getAccelXen(void);
        void setAccelBlockData(lsmAccelBlockData_t accelblockdata);
        lsmAccelBlockData_t getAccelBlockData(void);
        void setAccelEndianData(lsmAccelEndianData_t accelendiandata);
        lsmAccelEndianData_t getAccelEndianData(void);
        void setAccelRange(lsmAccelRange_t accelrange);
        lsmAccelRange_t getAccelRange(void);
        void setAccelResolution(lsmAccelResolution_t accelresolution);
        lsmAccelResolution_t getAccelResolution(void);
        void setAccelSPIWire(lsmAccelSPIWire_t accelspiwire);
        lsmAccelSPIWire_t getAccelSPIWire(void);
        void setTempSensor(lsmTempSensor_t tempsensor);
        lsmTempSensor_t getTempSensor(void);
        void setMagDataRate(lsmMagDataRate_t magdatarate);
        lsmMagDataRate_t getMagDataRate(void);
        void setMagGain(lsmMagGain_t maggain);
        lsmMagGain_t getMagGain(void);
        void setMagMode(lsmMagMode_t magmode);
        lsmMagMode_t getMagMode(void);
    
    private:
};
