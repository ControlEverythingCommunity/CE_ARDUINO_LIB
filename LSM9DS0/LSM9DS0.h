/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM9DS0
        This code is designed to work with the LSM9DS0_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM9DS0_I2CS#tabs-0-product_tabset-2
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
    #define LSM9DS0_DEFAULT_ADDRESS_GYRO                    (0x6A)      // D4 >> 1 = 7-bit default
    #define LSM9DS0_ADDRESS_GYRO_UPDATED                    (0x6B)      // D6 >> 1 = 7-bit default
    #define LSM9DS0_DEFAULT_ADDRESS_ACCELMAG                (0x1E)      // 3C >> 1 = 7-bit default
    #define LSM9DS0_ADDRESS_ACCELMAG_UPDATED                (0x1D)      // 3B >> 1 = 7-bit default
    #define LSM9DS0_ACCELMAG_DEV_ID                         (0x49)      // 0100 1001
    #define LSM9DS0_GYRO_DEV_ID                             (0xD4)      // 1101 0100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LSM9DS0_CONVERSIONDELAY                         (100)

/**************************************************************************
    GYROSCOPE REGISTERS
**************************************************************************/
    #define LSM9DS0_REG_GYRO_WHO_AM_I                       (0x0F)      // Device identification Register
    #define LSM9DS0_REG_GYRO_CTRL_REG1                      (0x20)      // Gyroscope Control Register 1
    #define LSM9DS0_REG_GYRO_CTRL_REG2                      (0x21)      // Gyroscope Control Register 2
    #define LSM9DS0_REG_GYRO_CTRL_REG3                      (0x22)      // Gyroscope Control Register 3
    #define LSM9DS0_REG_GYRO_CTRL_REG4                      (0x23)      // Gyroscope Control Register 4
    #define LSM9DS0_REG_GYRO_CTRL_REG5                      (0x24)      // Gyroscope Control Register 5
    #define LSM9DS0_REG_GYRO_REFERENCE                      (0x25)      // Reference/Datacapture Register
    #define LSM9DS0_REG_GYRO_STATUS                         (0x27)      // Status Register
    #define LSM9DS0_REG_GYRO_OUT_X_L                        (0x28)      // X-Axis Angular Rate Data Low Register
    #define LSM9DS0_REG_GYRO_OUT_X_H                        (0x29)      // X-Axis Angular Rate Data High Register
    #define LSM9DS0_REG_GYRO_OUT_Y_L                        (0x2A)      // Y-Axis Angular Rate Data Low Register
    #define LSM9DS0_REG_GYRO_OUT_Y_H                        (0x2B)      // Y-Axis Angular Rate Data High Register
    #define LSM9DS0_REG_GYRO_OUT_Z_L                        (0x2C)      // Z-Axis Angular Rate Data Low Register
    #define LSM9DS0_REG_GYRO_OUT_Z_H                        (0x2D)      // Z-Axis Angular Rate Data High Register
    #define LSM9DS0_REG_GYRO_FIFO_CTRL                      (0x2E)      // FIFO Control Register
    #define LSM9DS0_REG_GYRO_FIFO_SRC                       (0x2F)      // FIFO Source Register
    #define LSM9DS0_REG_GYRO_INT1_CFG                       (0x30)      // Interrupt Configuration Register
    #define LSM9DS0_REG_GYRO_INT1_SRC                       (0x31)      // Interrupt Source Register
    #define LSM9DS0_REG_GYRO_INT1_THS_XH                    (0x32)      // Interrupt X-Axis High Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_THS_XL                    (0x33)      // Interrupt X-Axis Low Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_THS_YH                    (0x34)      // Interrupt Y-Axis High Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_THS_YL                    (0x35)      // Interrupt Y-Axis Low Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_THS_ZH                    (0x36)      // Interrupt Z-Axis High Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_THS_ZL                    (0x37)      // Interrupt Z-Axis Low Threshold Register
    #define LSM9DS0_REG_GYRO_INT1_DURATION                  (0x38)      // Interrupt Duration Register

/**************************************************************************
    TEMPERATURE REGISTERS
**************************************************************************/
    #define LSM9DS0_REG_TEMP_OUT_L                          (0x05)      // Temperature Sensor Low Register
    #define LSM9DS0_REG_TEMP_OUT_H                          (0x06)      // Temperature Sensor High Register

/**************************************************************************
    MAGNETOMETER/ACCELEROMETER REGISTERS
**************************************************************************/
    #define LSM9DS0_REG_MAG_STATUS_M                        (0x07)      // Magnetic Data Status Register
    #define LSM9DS0_REG_MAG_OUT_X_L                         (0x08)      // X-Axis Magnetic Data Low Register
    #define LSM9DS0_REG_MAG_OUT_X_H                         (0x09)      // X-Axis Magnetic Data High Register
    #define LSM9DS0_REG_MAG_OUT_Y_L                         (0x0A)      // Y-Axis Magnetic Data Low Register
    #define LSM9DS0_REG_MAG_OUT_Y_H                         (0x0B)      // Y-Axis Magnetic Data High Register
    #define LSM9DS0_REG_MAG_OUT_Z_L                         (0x0C)      // Z-Axis Magnetic Data Low Register
    #define LSM9DS0_REG_MAG_OUT_Z_H                         (0x0D)      // Z-Axis Magnetic Data High Register
    #define LSM9DS0_REG_MAG_WHO_AM_I                        (0x0F)      // Device identification Register
    #define LSM9DS0_REG_MAG_INT_CTRL                        (0x12)      // Interrupt Recognition Register
    #define LSM9DS0_REG_MAG_INT_SRC                         (0x13)      // Interrupt Source Register
    #define LSM9DS0_REG_MAG_INT_THS_L                       (0x14)      // Magnetic Interrupt Threshold Low Register
    #define LSM9DS0_REG_MAG_INT_THS_H                       (0x15)      // Magnetic Interrupt Threshold High Register
    #define LSM9DS0_REG_MAG_OFFSET_X_L                      (0x16)      // Magnetic Offset for X-Axis Low Register
    #define LSM9DS0_REG_MAG_OFFSET_X_H                      (0x17)      // Magnetic Offset for X-Axis High Register
    #define LSM9DS0_REG_MAG_OFFSET_Y_L                      (0x18)      // Magnetic Offset for Y-Axis Low Register
    #define LSM9DS0_REG_MAG_OFFSET_Y_H                      (0x19)      // Magnetic Offset for Y-Axis High Register
    #define LSM9DS0_REG_MAG_OFFSET_Z_L                      (0x1A)      // Magnetic Offset for Z-Axis Low Register
    #define LSM9DS0_REG_MAG_OFFSET_Z_H                      (0x1B)      // Magnetic Offset for Z-Axis High Register
    #define LSM9DS0_REG_ACCEL_REF_X                         (0x1C)      // Reference Value for High-Pass Filter for X-Axis Acceleration Data
    #define LSM9DS0_REG_ACCEL_REF_Y                         (0x1D)      // Reference Value for High-Pass Filter for Y-Axis Acceleration Data
    #define LSM9DS0_REG_ACCEL_REF_Z                         (0x1E)      // Reference Value for High-Pass Filter for Z-Axis Acceleration Data
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG0                  (0x1F)      // Accelerometer/Magnetometer Control Register 0
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1                  (0x20)      // Accelerometer/Magnetometer Control Register 1
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2                  (0x21)      // Accelerometer/Magnetometer Control Register 2
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG3                  (0x22)      // Accelerometer/Magnetometer Control Register 3
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG4                  (0x23)      // Accelerometer/Magnetometer Control Register 4
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5                  (0x24)      // Accelerometer/Magnetometer Control Register 5
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6                  (0x25)      // Accelerometer/Magnetometer Control Register 6
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7                  (0x26)      // Accelerometer/Magnetometer Control Register 7
    #define LSM9DS0_REG_ACCEL_STATUS                        (0x27)      // Acceleration Status Register
    #define LSM9DS0_REG_ACCEL_OUT_X_L                       (0x28)      // X-Axis Acceleration Data Low Register
    #define LSM9DS0_REG_ACCEL_OUT_X_H                       (0x29)      // X-Axis Acceleration Data High Register
    #define LSM9DS0_REG_ACCEL_OUT_Y_L                       (0x2A)      // Y-Axis Acceleration Data Low Register
    #define LSM9DS0_REG_ACCEL_OUT_Y_H                       (0x2B)      // Y-Axis Acceleration Data High Register
    #define LSM9DS0_REG_ACCEL_OUT_Z_L                       (0x2C)      // Z-Axis Acceleration Data Low Register
    #define LSM9DS0_REG_ACCEL_OUT_Z_H                       (0x2D)      // Z-Axis Acceleration Data High Register
    #define LSM9DS0_REG_ACCELMAG_FIFO_CTRL                  (0x2E)      // FIFO Control Register
    #define LSM9DS0_REG_ACCELMAG_FIFO_SRC                   (0x2F)      // FIFO Source Register
    #define LSM9DS0_REG_ACCELMAG_INT1_GEN                   (0x30)      // Interrupt 1 Recognition Register
    #define LSM9DS0_REG_ACCELMAG_INT1_SRC                   (0x31)      // Interrupt 1 Source Register
    #define LSM9DS0_REG_ACCELMAG_INT1_THS                   (0x32)      // Interrupt 1 Threshold Register
    #define LSM9DS0_REG_ACCELMAG_INT1_DUR                   (0x33)      // Interrupt 1 Duration Register
    #define LSM9DS0_REG_ACCELMAG_INT2_GEN                   (0x34)      // Interrupt 2 Recognition Register
    #define LSM9DS0_REG_ACCELMAG_INT2_SRC                   (0x35)      // Interrupt 2 Source Register
    #define LSM9DS0_REG_ACCELMAG_INT2_THS                   (0x36)      // Interrupt 2 Threshold Register
    #define LSM9DS0_REG_ACCELMAG_INT2_DUR                   (0x37)      // Interrupt 2 Duration Register
    #define LSM9DS0_REG_ACCELMAG_CLICK_CFG                  (0x38)      // Interrupt Click Recognition Register
    #define LSM9DS0_REG_ACCELMAG_CLICK_SRC                  (0x39)      // Interrupt Click Source Register
    #define LSM9DS0_REG_ACCELMAG_CLICK_THS                  (0x3A)      // Interrupt Click Threshold Register
    #define LSM9DS0_REG_ACCELMAG_TIME_LIMIT                 (0x3B)      // Click Time Limit Register
    #define LSM9DS0_REG_ACCELMAG_TIME_LAT                   (0x3C)      // Click Time Latency Register
    #define LSM9DS0_REG_ACCELMAG_TIME_WIN                   (0x3D)      // Click Time Window Register
    #define LSM9DS0_REG_ACCELMAG_ACT_THS                    (0x3E)      // Sleep-to-Wake, Return-to-Sleep Activation Threshold Register
    #define LSM9DS0_REG_ACCELMAG_ACT_DUR                    (0x3F)      // Sleep-to-Wake, Return-to-Sleep Duration Register

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_MASK            (0xF0)  // Output Data Rate and Bandwidth Selection
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_95_12_5         (0x00)  // ODR (Hz): 95, Cutoff: 12.5
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_95_25           (0x30)  // ODR (Hz): 95, Cutoff: 25
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_12_5        (0x40)  // ODR (Hz): 190, Cutoff: 12.5
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_25          (0x50)  // ODR (Hz): 190, Cutoff: 25
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_50          (0x60)  // ODR (Hz): 190, Cutoff: 50
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_70          (0x70)  // ODR (Hz): 190, Cutoff: 70
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_20          (0x80)  // ODR (Hz): 380, Cutoff: 20
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_25          (0x90)  // ODR (Hz): 380, Cutoff: 25
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_50          (0xA0)  // ODR (Hz): 380, Cutoff: 50
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_100         (0xB0)  // ODR (Hz): 380, Cutoff: 100
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_30          (0xC0)  // ODR (Hz): 760, Cutoff: 30
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_35          (0xD0)  // ODR (Hz): 760, Cutoff: 35
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_50          (0xE0)  // ODR (Hz): 760, Cutoff: 50
    #define LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_100         (0xF0)  // ODR (Hz): 760, Cutoff: 100

    #define LSM9DS0_REG_GYRO_CTRL_REG1_PD_MASK              (0x08)  // Power-Down Mode Enable
    #define LSM9DS0_REG_GYRO_CTRL_REG1_PD_DOWN              (0x00)  // Power-Down Mode
    #define LSM9DS0_REG_GYRO_CTRL_REG1_PD_NORMAL            (0x08)  // Normal Mode or Sleep Mode

    #define LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_MASK             (0x04)  // Gyroscope Z-axis Enable
    #define LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_DISABLE          (0x00)  // Gyroscope Z-axis Disabled
    #define LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_ENABLE           (0x04)  // Gyroscope Z-axis Enabled

    #define LSM9DS0_REG_GYRO_CTRL_REG1_YEN_MASK             (0x02)  // Gyroscope Y-axis Enable
    #define LSM9DS0_REG_GYRO_CTRL_REG1_YEN_DISABLE          (0x00)  // Gyroscope Y-axis Disabled
    #define LSM9DS0_REG_GYRO_CTRL_REG1_YEN_ENABLE           (0x02)  // Gyroscope Y-axis Enabled

    #define LSM9DS0_REG_GYRO_CTRL_REG1_XEN_MASK             (0x01)  // Gyroscope X-axis Enable
    #define LSM9DS0_REG_GYRO_CTRL_REG1_XEN_DISABLE          (0x00)  // Gyroscope X-axis Disabled
    #define LSM9DS0_REG_GYRO_CTRL_REG1_XEN_ENABLE           (0x01)  // Gyroscope X-axis Enabled

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_GYRO_CTRL_REG4_BDU_MASK             (0x80)  // Block Data Update
    #define LSM9DS0_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS       (0x00)  // Continuous Update
    #define LSM9DS0_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE        (0x80)  // Output Registers Not Updated until MSB and LSB Read

    #define LSM9DS0_REG_GYRO_CTRL_REG4_BLE_MASK             (0x40)  // Big/Little Endian Data Selection
    #define LSM9DS0_REG_GYRO_CTRL_REG4_BLE_LSB              (0x00)  // Data LSB @ lower address
    #define LSM9DS0_REG_GYRO_CTRL_REG4_BLE_MSB              (0x40)  // Data MSB @ lower address

    #define LSM9DS0_REG_GYRO_CTRL_REG4_FS_MASK              (0x30)  // Full-Scale Selection
    #define LSM9DS0_REG_GYRO_CTRL_REG4_FS_245               (0x00)  // 245 dps
    #define LSM9DS0_REG_GYRO_CTRL_REG4_FS_500               (0x10)  // 500 dps
    #define LSM9DS0_REG_GYRO_CTRL_REG4_FS_2000              (0x30)  // 2000 dps

    #define LSM9DS0_REG_GYRO_CTRL_REG4_ST_MASK              (0x06)  // Self-Test Enable
    #define LSM9DS0_REG_GYRO_CTRL_REG4_ST_NORMAL            (0x00)  // Normal Mode
    #define LSM9DS0_REG_GYRO_CTRL_REG4_ST_0                 (0x02)  // Self-Test 0 (X Positive Sign, Y and Z Negative Sign)
    #define LSM9DS0_REG_GYRO_CTRL_REG4_ST_1                 (0x06)  // Self-Test 1 (X Negative Sign, Y and Z Positive Sign)

    #define LSM9DS0_REG_GYRO_CTRL_REG4_SIM_MASK             (0x01)  // SPI Serial Interface Mode Selection
    #define LSM9DS0_REG_GYRO_CTRL_REG4_SIM_4WIRE            (0x00)  // 4-Wire Interface
    #define LSM9DS0_REG_GYRO_CTRL_REG4_SIM_3WIRE            (0x01)  // 3-Wire Interface

/**************************************************************************
    ACCELEROMETER/MAGNETOMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_MASK        (0xF0)  // Acceleration Data Rate Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_PD          (0x00)  // Power-Down Mode
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_3_125       (0x10)  // 3.125 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_6_25        (0x20)  // 6.25 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_12_5        (0x30)  // 12.5 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_25          (0x40)  // 25 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_50          (0x50)  // 50 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_100         (0x60)  // 100 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_200         (0x70)  // 200 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_400         (0x80)  // 400 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_800         (0x90)  // 800 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_1600        (0xA0)  // 1600 Hz

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_MASK         (0x08)  // Block Data Update for Acceleration and Magnetic Data
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_CONTINUOUS   (0x00)  // Continuous Update
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_NOTUPDATE    (0x08)  // Output Registers Not Updated until MSB and LSB Read

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_MASK        (0x04)  // Acceleration Z-axis Enable
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_DISABLE     (0x00)  // Acceleration Z-axis Disabled
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_ENABLE      (0x04)  // Acceleration Z-axis Enabled

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_MASK        (0x02)  // Acceleration Y-axis Enable
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_DISABLE     (0x00)  // Acceleration Y-axis Disabled
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_ENABLE      (0x02)  // Acceleration Y-axis Enabled

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AXEN_MASK        (0x01)  // Acceleration X-axis Enable
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AXEN_DISABLE     (0x00)  // Acceleration X-axis Disabled
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG1_AXEN_ENABLE      (0x01)  // Acceleration X-axis Enabled

/**************************************************************************
    ACCELEROMETER/MAGNETOMETER CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_MASK         (0xC0)  // Accelerometer Anti-Alias Filter Bandwidth
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_773          (0x00)  // 773 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_194          (0x40)  // 194 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_362          (0x80)  // 362 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_50           (0xC0)  // 50 Hz

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_MASK         (0x38)  // Acceleration Full-Scale Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_2G           (0x00)  // ±2 g
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_4G           (0x08)  // ±4 g
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_6G           (0x10)  // ±6 g
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_8G           (0x18)  // ±8 g
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_16G          (0x20)  // ±16 g

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_MASK         (0x06)  // Acceleration Self-Test Enable
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NORMAL       (0x00)  // Normal Mode
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_POSITIVE     (0x02)  // Positive Sign Self-Test
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NEGATIVE     (0x04)  // Negative Sign Self-Test
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NOT          (0x06)  // Not Allowed

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_MASK         (0x01)  // SPI Serial Interface Mode Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_4WIRE        (0x00)  // 4-Wire Interface
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_3WIRE        (0x01)  // 3-Wire Interface

/**************************************************************************
    ACCELEROMETER/MAGNETOMETER CONTROL REGISTER 5 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_MASK        (0x80)  // Temperature Sensor Enable
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_DISABLED    (0x00)  // Temperature Sensor Disabled
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_ENABLED     (0x80)  // Temperature Sensor Enabled

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_MASK       (0x60)  // Magnetic Resolution Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_LOW        (0x00)  // Magnetic Resolution Low
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_HIGH       (0x60)  // Magnetic Resolution High

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_MASK        (0x1C)  // Magnetic Data Rate Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_3_125       (0x00)  // 3.125 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_6_25        (0x04)  // 6.25 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_12_5        (0x08)  // 12.5 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_25          (0x0C)  // 25 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_50          (0x10)  // 50 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_100         (0x14)  // 100 Hz
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_RES         (0x1C)  // Reserved

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR2_MASK        (0x02)  // Latch Interrupt Request on INT2_SRC Register, with INT2_SRC Register Cleared by Reading INT2_SRC Itself
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR2_NOLATCH     (0x00)  // Interrupt Request Not Latched
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR2_LATCH       (0x02)  // Interrupt Request Latched

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR1_MASK        (0x01)  // Latch Interrupt Request on INT1_SRC Register, with INT1_SRC Register Cleared by Reading INT1_SRC Itself
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR1_NOLATCH     (0x00)  // Interrupt Request Not Latched
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR1_LATCH       (0x01)  // Interrupt Request Latched

/**************************************************************************
    ACCELEROMETER/MAGNETOMETER CONTROL REGISTER 6 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_MASK         (0x60)  // Magnetic Full-Scale Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_2G           (0x00)  // ±2 gauss
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_4G           (0x20)  // ±4 gauss
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_8G           (0x40)  // ±8 gauss
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_12G          (0x60)  // ±12 gauss

/**************************************************************************
    ACCELEROMETER/MAGNETOMETER CONTROL REGISTER 7 DESCRIPTION
**************************************************************************/
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_MASK        (0xC0)  // High-Pass Filter Mode Selection for Acceleration Data
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_NORMAL      (0x00)  // Normal mode (resets x, y and z-axis reading REFERENCE_X (1Ch), REFERENCE_Y (1Dh) and REFERENCE_Y (1Dh) registers respectively)
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_REF         (0x40)  // Reference signal for filtering
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_NORMAL1     (0x80)  // Normal mode
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_AUTO        (0xC0)  // Autoreset on interrupt event

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AFDS_MASK        (0x20)  // Filtered Acceleration Data Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AFDS_BYPASS      (0x00)  // Internal Filter Bypassed
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_AFDS_INTFILTER   (0x20)  // Data From Internal Filter Sent to Output Register and FIFO

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MLP_MASK         (0x04)  // Magnetic Sensor Mode Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MLP_MODR         (0x00)  // Magnetic Data Rate is Configured by MODR Bits
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MLP_3_125        (0x04)  // MODR is Set to 3.125 Hz Independently From the MODR Settings

    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_MASK          (0x03)  // Magnetic Sensor Mode Selection
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_CONTINUOUS    (0x00)  // Continuous-Conversion Mode
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_SINGLE        (0x01)  // Single-conversion mode
    #define LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_PDOWN         (0x03)  // Power-Down mode


typedef enum
{
    GYRO_DATARATE_95_12_5           = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_95_12_5,
    GYRO_DATARATE_95_25             = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_95_25,
    GYRO_DATARATE_190_12_5          = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_12_5,
    GYRO_DATARATE_190_25            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_25,
    GYRO_DATARATE_190_50            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_50,
    GYRO_DATARATE_190_70            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_190_70,
    GYRO_DATARATE_380_20            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_20,
    GYRO_DATARATE_380_25            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_25,
    GYRO_DATARATE_380_50            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_50,
    GYRO_DATARATE_380_100           = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_380_100,
    GYRO_DATARATE_760_30            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_30,
    GYRO_DATARATE_760_35            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_35,
    GYRO_DATARATE_760_50            = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_50,
    GYRO_DATARATE_760_100           = LSM9DS0_REG_GYRO_CTRL_REG1_DRBW_760_100
    
} lsmGyroDataRate_t;

typedef enum
{
    GYRO_POWER_DOWN_MODE            = LSM9DS0_REG_GYRO_CTRL_REG1_PD_DOWN,
    GYRO_NORMAL_MODE                = LSM9DS0_REG_GYRO_CTRL_REG1_PD_NORMAL
    
} lsmGyroMode_t;

typedef enum
{
    GYRO_Z_AXIS_DISABLED            = LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_DISABLE,
    GYRO_Z_AXIS_ENABLED             = LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_ENABLE
    
} lsmGyroZen_t;

typedef enum
{
    GYRO_Y_AXIS_DISABLED            = LSM9DS0_REG_GYRO_CTRL_REG1_YEN_DISABLE,
    GYRO_Y_AXIS_ENABLED             = LSM9DS0_REG_GYRO_CTRL_REG1_YEN_ENABLE
    
} lsmGyroYen_t;

typedef enum
{
    GYRO_X_AXIS_DISABLED            = LSM9DS0_REG_GYRO_CTRL_REG1_XEN_DISABLE,
    GYRO_X_AXIS_ENABLED             = LSM9DS0_REG_GYRO_CTRL_REG1_XEN_ENABLE
    
} lsmGyroXen_t;

typedef enum
{
    GYRO_CONTINUOUS                 = LSM9DS0_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS,
    GYRO_NOT_UPDATED                = LSM9DS0_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE
    
} lsmGyroBlockData_t;

typedef enum
{
    GYRO_DATA_LSB                   = LSM9DS0_REG_GYRO_CTRL_REG4_BLE_LSB,
    GYRO_DATA_MSB                   = LSM9DS0_REG_GYRO_CTRL_REG4_BLE_MSB
    
} lsmGyroEndianData_t;

typedef enum
{
    GYRO_SCALE_245DPS               = LSM9DS0_REG_GYRO_CTRL_REG4_FS_245,
    GYRO_SCALE_500DPS               = LSM9DS0_REG_GYRO_CTRL_REG4_FS_500,
    GYRO_SCALE_2000DPS              = LSM9DS0_REG_GYRO_CTRL_REG4_FS_2000
    
} lsmGyroScale_t;

typedef enum
{
    GYRO_SELF_TEST_NORMAL           = LSM9DS0_REG_GYRO_CTRL_REG4_ST_NORMAL,
    GYRO_SELF_TEST_0                = LSM9DS0_REG_GYRO_CTRL_REG4_ST_0,
    GYRO_SELF_TEST_1                = LSM9DS0_REG_GYRO_CTRL_REG4_ST_1
    
} lsmGyroSelfTest_t;

typedef enum
{
    GYRO_SPI_WIRE_4                 = LSM9DS0_REG_GYRO_CTRL_REG4_SIM_4WIRE,
    GYRO_SPI_WIRE_3                 = LSM9DS0_REG_GYRO_CTRL_REG4_SIM_3WIRE
    
} lsmGyroSPIWire_t;

typedef enum
{
    ACCEL_DATARATE_POWERDOWN        = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_PD,
    ACCEL_DATARATE_3_125HZ          = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_3_125,
    ACCEL_DATARATE_6_25HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_6_25,
    ACCEL_DATARATE_12_5HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_12_5,
    ACCEL_DATARATE_25HZ             = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_25,
    ACCEL_DATARATE_50HZ             = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_200HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_200,
    ACCEL_DATARATE_400HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_800HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_800,
    ACCEL_DATARATE_1600HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AODR_1600
    
} lsmAccelDataRate_t;

typedef enum
{
    ACCELMAG_CONTINUOUS                = LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_CONTINUOUS,
    ACCELMAG_NOT_UPDATED               = LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_NOTUPDATE
    
} lsmAccelMagBlockData_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           =   LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_ENABLE
    
} lsmAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_ENABLE
    
} lsmAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_ENABLE
    
} lsmAccelXen_t;

typedef enum
{
    ACCEL_BANDWIDTH_773HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_773,
    ACCEL_BANDWIDTH_194HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_194,
    ACCEL_BANDWIDTH_362HZ           = LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_362,
    ACCEL_BANDWIDTH_50HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_50

} lsmAccelBandwidth_t;


typedef enum
{
    ACCEL_RANGE_2G                  = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_2G,
    ACCEL_RANGE_4G                  = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_4G,
    ACCEL_RANGE_6G                  = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_6G,
    ACCEL_RANGE_8G                  = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_8G,
    ACCEL_RANGE_16G                 = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AFS_16G
    
} lsmAccelRange_t;

typedef enum
{
    ACCEL_SELF_TEST_NORMAL          = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NORMAL,
    ACCEL_SELF_TEST_POSITIVE        = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_POSITIVE,
    ACCEl_SELF_TEST_NEGATIVE        = LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NEGATIVE
    
} lsmAccelSelfTest_t;

typedef enum
{
    ACCEL_MAG_SPI_WIRE_4                 = LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_4WIRE,
    ACCEL_MAG_SPI_WIRE_3                 = LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_3WIRE
    
} lsmAccelMagSPIWire_t;

typedef enum
{
    TEMP_SENSOR_DISABLED            = LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_DISABLED,
    TEMP_SENSOR_ENABLED             = LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_ENABLED
    
} lsmTempSensor_t;

typedef enum
{
    MAG_RESOLUTION_LOW              = LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_LOW,
    MAG_RESOLUTION_HIGH             = LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_HIGH
    
} lsmMagResolution_t;

typedef enum
{
    MAG_DATARATE_3_125HZ            = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_3_125,
    MAG_DATARATE_6_25HZ             = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_6_25,
    MAG_DATARATE_12_5HZ             = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_12_5,
    MAG_DATARATE_25HZ               = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_25,
    MAG_DATARATE_50HZ               = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_50,
    MAG_DATARATE_100HZ              = LSM9DS0_REG_ACCELMAG_CTRL_REG5_MODR_100
    
} lsmMagDataRate_t;

typedef enum
{
    MAG_INTR2_NOT_LATCHED           = LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR2_NOLATCH,
    MAG_INTR2_LATCHED               = LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR2_LATCH
    
} lsmMagIntr2Latch_t;

typedef enum
{
    MAG_INTR1_NOT_LATCHED           = LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR1_NOLATCH,
    MAG_INTR1_LATCHED               = LSM9DS0_REG_ACCELMAG_CTRL_REG5_LIR1_LATCH
    
} lsmMagIntr1Latch_t;

typedef enum
{
    MAG_GAIN_2GAUSS                 = LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_2G,
    MAG_GAIN_4GAUSS                 = LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_4G,
    MAG_GAIN_8GAUSS                 = LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_8G,
    MAG_GAIN_12GAUSS                = LSM9DS0_REG_ACCELMAG_CTRL_REG6_MFS_12G
    
} lsmMagGain_t;

typedef enum
{
    MAG_CONTINUOUS                  = LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_CONTINUOUS,
    MAG_SINGLE                      = LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_SINGLE,
    MAG_POWER_DOWN                  = LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_PDOWN
    
} lsmMagMode_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float   T;
    
} lsmSensorData_t;


class LSM9DS0
{
    protected:
        // Instance-specific properties
        uint8_t lsm_conversionDelay;
        lsmGyroDataRate_t lsm_gyrodatarate;
        lsmGyroMode_t lsm_gyromode;
        lsmGyroZen_t lsm_gyrozen;
        lsmGyroYen_t lsm_gyroyen;
        lsmGyroXen_t lsm_gyroxen;
        lsmGyroBlockData_t lsm_gyroblockdata;
        lsmGyroEndianData_t lsm_gyroendiandata;
        lsmGyroScale_t lsm_gyroscale;
        lsmGyroSelfTest_t lsm_gyroselftest;
        lsmGyroSPIWire_t lsm_gyrospiwire;
        lsmAccelDataRate_t lsm_acceldatarate;
        lsmAccelMagBlockData_t lsm_accelmagblockdata;
        lsmAccelZen_t lsm_accelzen;
        lsmAccelYen_t lsm_accelyen;
        lsmAccelXen_t lsm_accelxen;
        lsmAccelBandwidth_t lsm_accelbandwidth;
        lsmAccelRange_t lsm_accelrange;
        lsmAccelSelfTest_t lsm_accelselftest;
        lsmAccelMagSPIWire_t lsm_accelmagspiwire;
        lsmTempSensor_t lsm_tempsensor;
        lsmMagResolution_t lsm_magresolution;
        lsmMagDataRate_t lsm_magdatarate;
        lsmMagIntr2Latch_t lsm_magintr2latch;
        lsmMagIntr1Latch_t lsm_magintr1latch;
        lsmMagGain_t lsm_maggain;
        lsmMagMode_t lsm_magmode;

    public:
        uint8_t lsm_i2cAddressG;
        uint8_t lsm_i2cAddressXM;
        lsmSensorData_t lsm_gyroData;
        lsmSensorData_t lsm_accelData;
        lsmSensorData_t lsm_magData;
        lsmSensorData_t lsm_tempData;
        int16_t tempData;
        void getAddrG_LSM9DS0(uint8_t i2cAddress);
        void getAddrXM_LSM9DS0(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpGyroscope(void);
        void setUpAccelerometer(void);
        void setUpMagnetometer(void);
        void Measure_Gyroscope(void);
        void Measure_Accelerometer(void);
        void Measure_Magnetometer(void);
        void Measure_Temperature(void);
        void setGyroDataRate(lsmGyroDataRate_t gyrodatarate);
        lsmGyroDataRate_t getGyroDataRate(void);
        void setGyroMode(lsmGyroMode_t gyromode);
        lsmGyroMode_t getGyroMode(void);
        void setGyroZen(lsmGyroZen_t gyrozen);
        lsmGyroZen_t getGyroZen(void);
        void setGyroYen(lsmGyroYen_t gyroyen);
        lsmGyroYen_t getGyroYen(void);
        void setGyroXen(lsmGyroXen_t gyroxen);
        lsmGyroXen_t getGyroXen(void);
        void setGyroBlockData(lsmGyroBlockData_t gyroblockdata);
        lsmGyroBlockData_t getGyroBlockData(void);
        void setGyroEndianData(lsmGyroEndianData_t gyroendiandata);
        lsmGyroEndianData_t getGyroEndianData(void);
        void setGyroScale(lsmGyroScale_t gyroscale);
        lsmGyroScale_t getGyroScale(void);
        void setGyroSelfTest(lsmGyroSelfTest_t gyroselftest);
        lsmGyroSelfTest_t getGyroSelfTest(void);
        void setGyroSPIWire(lsmGyroSPIWire_t gyrospiwire);
        lsmGyroSPIWire_t getGyroSPIWire(void);
        void setAccelDataRate(lsmAccelDataRate_t acceldatarate);
        lsmAccelDataRate_t getAccelDataRate(void);
        void setAccelMagBlockData(lsmAccelMagBlockData_t accelmagblockdata);
        lsmAccelMagBlockData_t getAccelMagBlockData(void);
        void setAccelZen(lsmAccelZen_t accelzen);
        lsmAccelZen_t getAccelZen(void);
        void setAccelYen(lsmAccelYen_t accelyen);
        lsmAccelYen_t getAccelYen(void);
        void setAccelXen(lsmAccelXen_t accelxen);
        lsmAccelXen_t getAccelXen(void);
        void setAccelBandwidth(lsmAccelBandwidth_t accelbandwidth);
        lsmAccelBandwidth_t getAccelBandwidth(void);
        void setAccelRange(lsmAccelRange_t accelrange);
        lsmAccelRange_t getAccelRange(void);
        void setAccelSelfTest(lsmAccelSelfTest_t accelselftest);
        lsmAccelSelfTest_t getAccelSelfTest(void);
        void setAccelMagSPIWire(lsmAccelMagSPIWire_t accelmagspiwire);
        lsmAccelMagSPIWire_t getAccelMagSPIWire(void);
        void setTempSensor(lsmTempSensor_t tempsensor);
        lsmTempSensor_t getTempSensor(void);
        void setMagResolution(lsmMagResolution_t magresolution);
        lsmMagResolution_t getMagResolution(void);
        void setMagDataRate(lsmMagDataRate_t magdatarate);
        lsmMagDataRate_t getMagDataRate(void);
        void setMagIntr2Latch(lsmMagIntr2Latch_t magintr2latch);
        lsmMagIntr2Latch_t getMagIntr2Latch(void);
        void setMagIntr1Latch(lsmMagIntr1Latch_t magintr1latch);
        lsmMagIntr1Latch_t getMagIntr1Latch(void);
        void setMagGain(lsmMagGain_t maggain);
        lsmMagGain_t getMagGain(void);
        void setMagMode(lsmMagMode_t magmode);
        lsmMagMode_t getMagMode(void);
    
    private:
};
