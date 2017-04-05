/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPU6000
        This code is designed to work with the MPU6000_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-ACCELEROMETER AND GYROSCOPE
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
    #define MPU6000_DEFAULT_ADDRESS_ACCELGYRO                       (0x68)
    #define MPU6000_ADDRESS_ACCELGYRO_UPDATED                       (0x69)
    #define MPU6000_DEFAULT_DEV_ID                                  (0x2A)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MPU6000_CONVERSIONDELAY                                 (100)

/**************************************************************************
    ACCELEROMETER AND GYROSCOPE REGISTERS
**************************************************************************/
    #define MPU6000_REG_ACCELGYRO_SELF_TEST_X                       (0x0D)      // Self Test Registers
    #define MPU6000_REG_ACCELGYRO_SELF_TEST_Y                       (0x0E)      // Self Test Registers
    #define MPU6000_REG_ACCELGYRO_SELF_TEST_Z                       (0x0F)      // Self Test Registers
    #define MPU6000_REG_ACCELGYRO_SELF_TEST_A                       (0x10)      // Self Test Registers
    #define MPU6000_REG_ACCELGYRO_SMPLRT_DIV                        (0x19)      // Sample Rate Divider Register
    #define MPU6000_REG_ACCELGYRO_CONFIG                            (0x1A)      // Configuration Register
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG                       (0x1B)      // Gyroscope Configuration Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG                      (0x1C)      // Accelerometer Configuration Register
    #define MPU6000_REG_ACCELGYRO_FIFO_EN                           (0x23)      // I2C Master Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_MST_CTRL                      (0x24)      // I2C Slave 0 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV0_ADDR                     (0x25)      // I2C Slave 0 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV0_REG                      (0x26)      // I2C Slave 0 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV0_CTRL                     (0x27)      // I2C Slave 0 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV1_ADDR                     (0x28)      // I2C Slave 1 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV1_REG                      (0x29)      // I2C Slave 1 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV1_CTRL                     (0x2A)      // I2C Slave 1 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV2_ADDR                     (0x2B)      // I2C Slave 2 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV2_REG                      (0x2C)      // I2C Slave 2 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV2_CTRL                     (0x2D)      // I2C Slave 2 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV3_ADDR                     (0x2E)      // I2C Slave 3 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV3_REG                      (0x2F)      // I2C Slave 3 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV3_CTRL                     (0x30)      // I2C Slave 3 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV4_ADDR                     (0x31)      // I2C Slave 4 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV4_REG                      (0x32)      // I2C Slave 4 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV4_DO                       (0x33)      // I2C Slave 4 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV4_CTRL                     (0x34)      // I2C Slave 4 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV4_DI                       (0x35)      // I2C Slave 4 Control Register
    #define MPU6000_REG_ACCELGYRO_I2C_MST_STATUS                    (0x36)      // I2C Master Status Register
    #define MPU6000_REG_ACCELGYRO_INT_PIN_CFG                       (0x37)      // INT Pin / Bypass Enable Configuration Register
    #define MPU6000_REG_ACCELGYRO_INT_ENABLE                        (0x38)      // Interrupt Enable Register
    #define MPU6000_REG_ACCELGYRO_INT_STATUS                        (0x3A)      // Interrupt Status Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_XOUT_H                      (0x3B)      // X-Axis Accelerometer High Data Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_XOUT_L                      (0x3C)      // X-Axis Accelerometer Low Data Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_YOUT_H                      (0x3D)      // Y-Axis Accelerometer High Data Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_YOUT_L                      (0x3E)      // Y-Axis Accelerometer Low Data Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_ZOUT_H                      (0x3F)      // Z-Axis Accelerometer High Data Register
    #define MPU6000_REG_ACCELGYRO_ACCEL_ZOUT_L                      (0x40)      // Z-Axis Accelerometer Low Data Register
    #define MPU6000_REG_ACCELGYRO_TEMP_OUT_H                        (0x41)      // Temperature High Data Register
    #define MPU6000_REG_ACCELGYRO_TEMP_OUT_L                        (0x42)      // Temperature Low Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_XOUT_H                       (0x43)      // X-Axis Gyroscope High Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_XOUT_L                       (0x44)      // X-Axis Gyroscope Low Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_YOUT_H                       (0x45)      // Y-Axis Gyroscope High Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_YOUT_L                       (0x46)      // Y-Axis Gyroscope Low Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_ZOUT_H                       (0x47)      // Z-Axis Gyroscope High Data Register
    #define MPU6000_REG_ACCELGYRO_GYRO_ZOUT_L                       (0x48)      // Z-Axis Gyroscope Low Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_00                  (0x49)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_01                  (0x4A)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_02                  (0x4B)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_03                  (0x4C)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_04                  (0x4D)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_05                  (0x4E)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_06                  (0x4F)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_07                  (0x50)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_08                  (0x51)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_09                  (0x52)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_10                  (0x53)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_11                  (0x54)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_12                  (0x55)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_13                  (0x56)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_14                  (0x57)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_15                  (0x58)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_16                  (0x59)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_17                  (0x5A)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_18                  (0x5B)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_19                  (0x5C)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_20                  (0x5D)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_21                  (0x5E)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_22                  (0x5F)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_EXT_SENS_DATA_23                  (0x60)      // External Sensor Data Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV0_DO                       (0x63)      // I2C Slave 0 Data Out Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV1_DO                       (0x64)      // I2C Slave 1 Data Out Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV2_DO                       (0x65)      // I2C Slave 2 Data Out Register
    #define MPU6000_REG_ACCELGYRO_I2C_SLV3_DO                       (0x66)      // I2C Slave 3 Data Out Register
    #define MPU6000_REG_ACCELGYRO_I2C_MST_DELAY_CT                  (0x67)      // I2C Master 0 Delay Control Register
    #define MPU6000_REG_ACCELGYRO_SIGNAL_PATH_RES                   (0x68)      // Signal Path Reset Register
    #define MPU6000_REG_ACCELGYRO_USER_CTRL                         (0x6A)      // User Control Register
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1                        (0x6B)      // Power Management 1 Register
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_2                        (0x6C)      // Power Management 2 Register
    #define MPU6000_REG_ACCELGYRO_FIFO_COUNTH                       (0x72)      // FIFO Count Registers
    #define MPU6000_REG_ACCELGYRO_FIFO_COUNTL                       (0x73)      // FIFO Count Registers
    #define MPU6000_REG_ACCELGYRO_FIFO_R_W                          (0x74)      // FIFO Read Write Register
    #define MPU6000_REG_ACCELGYRO_WHO_AM_I                          (0x75)      // Who Am I Register

/**************************************************************************
    GYROSCOPE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_XG_ST_MASK            (0x80)      // X-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_XG_ST_DISABLE         (0x00)      // Disable X-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_XG_ST_ENABLE          (0x80)      // Enable X-Axis Gyroscope to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_YG_ST_MASK            (0x40)      // Y-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_YG_ST_DISABLE         (0x00)      // Disable Y-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_YG_ST_ENABLE          (0x40)      // Enable Y-Axis Gyroscope to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_ZG_ST_MASK            (0x20)      // Z-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_ZG_ST_DISABLE         (0x00)      // Disable Z-Axis Gyroscope to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_ZG_ST_ENABLE          (0x20)      // Enable Z-Axis Gyroscope to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_MASK           (0x18)      // Full Scale Range of the Gyroscope Outputs
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_250            (0x00)      // Full Scale Range: ± 250 °/s
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_500            (0x08)      // Full Scale Range: ± 500 °/s
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_1000           (0x10)      // Full Scale Range: ± 1000 °/s
    #define MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_2000           (0x18)      // Full Scale Range: ± 2000 °/s

/**************************************************************************
    ACCELEROMETER CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_XA_ST_MASK           (0x80)      // X-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_XA_ST_DISABLE        (0x00)      // Disable X-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_XA_ST_ENABLE         (0x80)      // Enable X-Axis Accelerometer to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_YA_ST_MASK           (0x40)      // Y-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_YA_ST_DISABLE        (0x00)      // Disable Y-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_YA_ST_ENABLE         (0x40)      // Enable Y-Axis Accelerometer to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_ZA_ST_MASK           (0x20)      // Z-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_ZA_ST_DISABLE        (0x00)      // Disable Z-Axis Accelerometer to Perform Self Test
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_ZA_ST_ENABLE         (0x20)      // Enable Z-Axis Accelerometer to Perform Self Test

    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_MASK         (0x18)      // Full Scale Range of the Accelerometer Outputs
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_2G           (0x00)      // Full Scale Range: ± 2g
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_4G           (0x08)      // Full Scale Range: ± 4g
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_8G           (0x10)      // Full Scale Range: ± 8g
    #define MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_16G          (0x18)      // Full Scale Range: ± 16g

/**************************************************************************
    ACCELEROMETER AND GYROSCOPE POWER MANAGEMENT REGISTER 1 DESCRIPTION
**************************************************************************/
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_MASK      (0x80)      // Resets All Internal Registers
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_DISABLE   (0x00)      // Disable the Reset of All Internal Registers
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_ENABLE    (0x80)      // Enable the Reset of All Internal Registers

    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_MASK             (0x40)      // Sleep Mode
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_DISABLE          (0x00)      // Disables Sleep Mode
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_ENABLE           (0x40)      // Enables Sleep Mode

    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_MASK             (0x20)      // Cycle Between Sleep Mode and Waking Up to Take Sample Data
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_DISABLE          (0x00)      // Disables Cycle Mode
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_ENABLE           (0x20)      // Enables Cycle Mode

    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_MASK          (0x08)      // Temperature Sensor
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_ENABLE        (0x00)      // Enables Temperature Sensor
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_DISABLE       (0x08)      // Disables Temperature Sensor

    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_MASK            (0x07)      // Specifies the Clock Source of the Device
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_INT_8MHZ_OS     (0x00)      // Internal 8MHz Oscillator
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_X_REF       (0x01)      // PLL with X-Axis Gyroscope Reference
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_Y_REF       (0x02)      // PLL with Y-Axis Gyroscope Reference
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_Z_REF       (0x03)      // PLL with Z-Axis Gyroscope Reference
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_EXT_32_768KHZ   (0x04)      // PLL with External 32.768kHz Reference
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_EXT_19_2MHZ     (0x05)      // PLL with External 19.2MHz Reference
    #define MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_STOP            (0x07)      // Stops the Clock and Keeps the Timing Generator in Reset


typedef enum
{
    GYRO_RANGE_250                      = MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_250,
    GYRO_RANGE_500                      = MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_500,
    GYRO_RANGE_1000                     = MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_1000,
    GYRO_RANGE_2000                     = MPU6000_REG_ACCELGYRO_GYRO_CONFIG_FS_SEL_2000
    
} mpuGyroRange_t;

typedef enum
{
    ACCEL_RANGE_2G                      = MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_2G,
    ACCEL_RANGE_4G                      = MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_4G,
    ACCEL_RANGE_8G                      = MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_8G,
    ACCEL_RANGE_16G                     = MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_AFS_SEL_16G,
    
} mpuAccelRange_t;

typedef enum
{
    DEVICE_RESET_DISABLE                = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_DISABLE,
    DEVICE_RESET_ENABLE                 = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_ENABLE
    
} mpuDeviceReset_t;

typedef enum
{
    SLEEP_DISABLE                       = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_DISABLE,
    SLEEP_ENABLE                        = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_ENABLE
    
} mpuSleepMode_t;

typedef enum
{
    CYCLE_DISABLE                       = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_DISABLE,
    CYCLE_ENABLE                        = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_ENABLE
    
} mpuCycleMode_t;

typedef enum
{
    TEMPERATURE_ENABLE                  = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_ENABLE,
    TEMPERATURE_DISABLE                 = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_DISABLE
    
} mpuTempMode_t;

typedef enum
{
    CLOCK_SOURCE_INT_8MHZ_OS            = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_INT_8MHZ_OS,
    CLOCK_SOURCE_PLL_X_REF              = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_X_REF,
    CLOCK_SOURCE_PLL_Y_REF              = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_Y_REF,
    CLOCK_SOURCE_PLL_Z_REF              = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_PLL_Z_REF,
    CLOCK_SOURCE_EXT_32_768KHZ          = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_EXT_32_768KHZ,
    CLOCK_SOURCE_EXT_19_2MHZ            = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_EXT_19_2MHZ,
    CLOCK_SOURCE_STOP                   = MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CLKSEL_STOP
    
} mpuClockSource_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float T;
    
} mpuSensorData_t;


class MPU6000
{
    protected:
        // Instance-specific properties
        uint8_t mpu_conversionDelay;
        mpuGyroRange_t mpu_gyrorange;
        mpuAccelRange_t mpu_accelrange;
        mpuDeviceReset_t mpu_devicereset;
        mpuSleepMode_t mpu_sleepmode;
        mpuCycleMode_t mpu_cyclemode;
        mpuTempMode_t mpu_tempmode;
        mpuClockSource_t mpu_clocksource;
    
    public:
        uint8_t mpu_i2cAddress;
        mpuSensorData_t mpu_accelData;
        mpuSensorData_t mpu_gyroData;
        mpuSensorData_t mpu_tempData;
        void getAddr_MPU6000(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_Sensor(void);
        void Measure_Accelerometer(void);
        void Measure_Gyroscope(void);
        void Measure_Temperature(void);
        void setGyroRange(mpuGyroRange_t gyrorange);
        mpuGyroRange_t getGyroRange(void);
        void setAccelRange(mpuAccelRange_t accelrange);
        mpuAccelRange_t getAccelRange(void);
        void setDeviceReset(mpuDeviceReset_t devicereset);
        mpuDeviceReset_t getDeviceReset(void);
        void setSleepMode(mpuSleepMode_t sleepmode);
        mpuSleepMode_t getSleepMode(void);
        void setCycleMode(mpuCycleMode_t cyclemode);
        mpuCycleMode_t getCycleMode(void);
        void setTempMode(mpuTempMode_t tempmode);
        mpuTempMode_t getTempMode(void);
        void setClockSource(mpuClockSource_t clocksource);
        mpuClockSource_t getClockSource(void);
    
    private:
};
