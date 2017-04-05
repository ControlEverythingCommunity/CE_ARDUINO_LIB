/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM330
        This code is designed to work with the LSM330_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM330_I2CS#tabs-0-product_tabset-2
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
    #define LSM330_DEFAULT_ADDRESS_GYRO                     (0x6A)      // D4 >> 1 = 7-bit default
    #define LSM330_ADDRESS_GYRO_UPDATED                     (0x6B)      // D6 >> 1 = 7-bit default
    #define LSM330_DEFAULT_ADDRESS_ACCEL                    (0x1D)      // 3B >> 1 = 7-bit default
    #define LSM330_ADDRESS_ACCEL_UPDATED                    (0x1E)      // 3C >> 1 = 7-bit default
    #define LSM330_ACCEL_DEV_ID                             (0x40)      // 0100 0000
    #define LSM330_GYRO_DEV_ID                              (0xD4)      // 1101 0100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LSM330_CONVERSIONDELAY                          (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
 **************************************************************************/
    #define LSM330_REG_ACCEL_WHO_AM_I                       (0x0F)      // Device identification Register
    #define LSM330_REG_ACCEL_CTRL_REG4                      (0x23)      // Accelerometer Control Register 4
    #define LSM330_REG_ACCEL_CTRL_REG5                      (0x20)      // Accelerometer Control Register 5
    #define LSM330_REG_ACCEL_CTRL_REG6                      (0x24)      // Accelerometer Control Register 6
    #define LSM330_REG_ACCEL_CTRL_REG7                      (0x25)      // Accelerometer Control Register 7
    #define LSM330_REG_ACCEL_STATUS                         (0x27)      // Acceleration Status Register
    #define LSM330_REG_ACCEL_OFFSET_X                       (0x10)      // Offset Correction for X-Axis Register
    #define LSM330_REG_ACCEL_OFFSET_Y                       (0x11)      // Offset Correction for Y-Axis Register
    #define LSM330_REG_ACCEL_OFFSET_Z                       (0x12)      // Offset Correction for Z-Axis Register
    #define LSM330_REG_ACCEL_CS_X                           (0x13)      // Constant Shift Signed Value for X-Axis Register
    #define LSM330_REG_ACCEL_CS_Y                           (0x14)      // Constant Shift Signed Value for y-Axis Register
    #define LSM330_REG_ACCEL_CS_Z                           (0x15)      // Constant Shift Signed Value for Z-Axis Register
    #define LSM330_REG_ACCEL_LC_L                           (0x16)      // Low 16-bit Long-Counter Register for Interrupt State Machine Programs Timing
    #define LSM330_REG_ACCEL_LC_H                           (0x17)      // High 16-bit Long-Counter Register for Interrupt State Machine Programs Timing
    #define LSM330_REG_ACCEL_STAT                           (0x18)      // Interrupt Synchronization Register
    #define LSM330_REG_ACCEL_VFC_1                          (0x1B)      // Vector Coefficient Register 1 for DIff Filter
    #define LSM330_REG_ACCEL_VFC_2                          (0x1C)      // Vector Coefficient Register 2 for DIff Filter
    #define LSM330_REG_ACCEL_VFC_3                          (0x1D)      // Vector Coefficient Register 3 for DIff Filter
    #define LSM330_REG_ACCEL_VFC_4                          (0x1E)      // Vector Coefficient Register 4 for DIff Filter
    #define LSM330_REG_ACCEL_THRS                           (0x1F)      // Threshold Value Register
    #define LSM330_REG_ACCEL_OUT_X_L                        (0x28)      // X-Axis Acceleration Data Low Register
    #define LSM330_REG_ACCEL_OUT_X_H                        (0x29)      // X-Axis Acceleration Data High Register
    #define LSM330_REG_ACCEL_OUT_Y_L                        (0x2A)      // Y-Axis Acceleration Data Low Register
    #define LSM330_REG_ACCEL_OUT_Y_H                        (0x2B)      // Y-Axis Acceleration Data High Register
    #define LSM330_REG_ACCEL_OUT_Z_L                        (0x2C)      // Z-Axis Acceleration Data Low Register
    #define LSM330_REG_ACCEL_OUT_Z_H                        (0x2D)      // Z-Axis Acceleration Data High Register
    #define LSM330_REG_ACCEL_FIFO_CTRL_A                    (0x2E)      // Linear Acceleration Sensor FIFO Control Register
    #define LSM330_REG_ACCEL_FIFO_SRC_A                     (0x2F)      // Linear Acceleration Sensor FIFO Source Register
    #define LSM330_REG_ACCEL_CTRL_REG2_A                    (0x21)      // State Machine 1 Control Register
    #define LSM330_REG_ACCEL_STX_1                          [0x40-0x4F] // State Machine 1 Code Registers
    #define LSM330_REG_ACCEL_TIM4_1                         (0x50)      // 8-Bit General Timer (unsigned value) for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM3_1                         (0x51)      // 8-Bit General Timer (unsigned value) for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM2_1                         [0x52-0x53] // 16-Bit General Timer (unsigned value) for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM1_1                         [0x54-0x55] // 16-Bit General Timer (unsigned value) for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_THRS2_1                        (0x56)      // Threshold Value Value for State Machine 1 Operator Register
    #define LSM330_REG_ACCEL_THRS1_1                        (0x57)      // Threshold Value Value for State Machine 1 Operator Register
    #define LSM330_REG_ACCEL_MASKB_1                        (0x59)      // Axis and Sign Mask (Swap) for State Machine 1 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_MASKA_1                        (0x5A)      // Axis and Sign Mask (Swap) for State Machine 1 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_SETT1                          (0x5B)      // Setting of Threshold, Peak Detection and Flags for State Machine 1 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_PR1                            (0x5C)      // Program and Reset Pointer for State Machine 1 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_TC1                            [0x5D-0x5E] // 16-Bit General Timer (unsigned value) for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_OUTS1                          (0x5F)      // Output Flags on Axis for Interrupt State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_PEAK1                          (0x19)      // Peak Detection Value Register for State Machine 1 Operation Timing Register
    #define LSM330_REG_ACCEL_CTRL_REG3_A                    (0x22)      // State Machine 2 Control Register
    #define LSM330_REG_ACCEL_STX_1                          [0x60-0x6F] // State Machine 2 Code Registers
    #define LSM330_REG_ACCEL_TIM4_2                         (0x70)      // 8-Bit General Timer (unsigned value) for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM3_2                         (0x71)      // 8-Bit General Timer (unsigned value) for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM2_2                         [0x72-0x73] // 16-Bit General Timer (unsigned value) for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_TIM1_2                         [0x74-0x75] // 16-Bit General Timer (unsigned value) for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_THRS2_2                        (0x76)      // Threshold Value Value for State Machine 2 Operator Register
    #define LSM330_REG_ACCEL_THRS1_2                        (0x77)      // Threshold Value Value for State Machine 2 Operator Register
    #define LSM330_REG_ACCEL_MASKB_2                        (0x79)      // Axis and Sign Mask (Swap) for State Machine 2 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_MASKA_2                        (0x7A)      // Axis and Sign Mask (Swap) for State Machine 2 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_SETT2                          (0x7B)      // Setting of Threshold, Peak Detection and Flags for State Machine 2 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_PR2                            (0x7C)      // Program and Reset Pointer for State Machine 2 Motion-Detection Operation Register
    #define LSM330_REG_ACCEL_TC2                            [0x7D-0x7E] // 16-Bit General Timer (unsigned value) for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_OUTS2                          (0x7F)      // Output Flags on Axis for Interrupt State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_PEAK2                          (0x1A)      // Peak Detection Value Register for State Machine 2 Operation Timing Register
    #define LSM330_REG_ACCEL_DES2                           (0x78)      // Decimation Counter Value Register for SM2 Operation

/**************************************************************************
    GYROSCOPE REGISTERS
**************************************************************************/
    #define LSM330_REG_GYRO_WHO_AM_I                        (0x0F)      // Device identification Register
    #define LSM330_REG_GYRO_CTRL_REG1                       (0x20)      // Gyroscope Control Register 1
    #define LSM330_REG_GYRO_CTRL_REG2                       (0x21)      // Gyroscope Control Register 2
    #define LSM330_REG_GYRO_CTRL_REG3                       (0x22)      // Gyroscope Control Register 3
    #define LSM330_REG_GYRO_CTRL_REG4                       (0x23)      // Gyroscope Control Register 4
    #define LSM330_REG_GYRO_CTRL_REG5                       (0x24)      // Gyroscope Control Register 5
    #define LSM330_REG_GYRO_REFERENCE                       (0x25)      // Reference/Datacapture Register
    #define LSM330_REG_TEMPERATURE_OUT                      (0x26)      // Temperature Data Output Register
    #define LSM330_REG_GYRO_STATUS                          (0x27)      // Status Register
    #define LSM330_REG_GYRO_OUT_X_L                         (0x28)      // X-Axis Angular Rate Data Low Register
    #define LSM330_REG_GYRO_OUT_X_H                         (0x29)      // X-Axis Angular Rate Data High Register
    #define LSM330_REG_GYRO_OUT_Y_L                         (0x2A)      // Y-Axis Angular Rate Data Low Register
    #define LSM330_REG_GYRO_OUT_Y_H                         (0x2B)      // Y-Axis Angular Rate Data High Register
    #define LSM330_REG_GYRO_OUT_Z_L                         (0x2C)      // Z-Axis Angular Rate Data Low Register
    #define LSM330_REG_GYRO_OUT_Z_H                         (0x2D)      // Z-Axis Angular Rate Data High Register
    #define LSM330_REG_GYRO_FIFO_CTRL                       (0x2E)      // FIFO Control Register
    #define LSM330_REG_GYRO_FIFO_SRC                        (0x2F)      // FIFO Source Register
    #define LSM330_REG_GYRO_INT1_CFG                        (0x30)      // Interrupt Configuration Register
    #define LSM330_REG_GYRO_INT1_SRC                        (0x31)      // Interrupt Source Register
    #define LSM330_REG_GYRO_INT1_THS_XH                     (0x32)      // Interrupt X-Axis High Threshold Register
    #define LSM330_REG_GYRO_INT1_THS_XL                     (0x33)      // Interrupt X-Axis Low Threshold Register
    #define LSM330_REG_GYRO_INT1_THS_YH                     (0x34)      // Interrupt Y-Axis High Threshold Register
    #define LSM330_REG_GYRO_INT1_THS_YL                     (0x35)      // Interrupt Y-Axis Low Threshold Register
    #define LSM330_REG_GYRO_INT1_THS_ZH                     (0x36)      // Interrupt Z-Axis High Threshold Register
    #define LSM330_REG_GYRO_INT1_THS_ZL                     (0x37)      // Interrupt Z-Axis Low Threshold Register
    #define LSM330_REG_GYRO_INT1_DURATION                   (0x38)      // Interrupt Duration Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 5 DESCRIPTION
**************************************************************************/
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_MASK            (0xF0)      // Acceleration Data Rate Selection
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_PD              (0x00)      // Power-Down Mode
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_3_125           (0x10)      // 3.125 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_6_25            (0x20)      // 6.25 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_12_5            (0x30)      // 12.5 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_25              (0x40)      // 25 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_50              (0x50)      // 50 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_100             (0x60)      // 100 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_200             (0x70)      // 200 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_400             (0x80)      // 400 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_800             (0x90)      // 800 Hz
    #define LSM330_REG_ACCEL_CTRL_REG5_AODR_1600            (0xA0)      // 1600 Hz

    #define LSM330_REG_ACCEL_CTRL_REG5_BDU_MASK             (0x08)      // Block Data Update for Acceleration Data
    #define LSM330_REG_ACCEL_CTRL_REG5_BDU_CONTINUOUS       (0x00)      // Continuous Update
    #define LSM330_REG_ACCEL_CTRL_REG5_BDU_NOTUPDATE        (0x08)      // Output Registers Not Updated until MSB and LSB Read

    #define LSM330_REG_ACCEL_CTRL_REG5_AZEN_MASK            (0x04)      // Acceleration Z-axis Enable
    #define LSM330_REG_ACCEL_CTRL_REG5_AZEN_DISABLE         (0x00)      // Acceleration Z-axis Disabled
    #define LSM330_REG_ACCEL_CTRL_REG5_AZEN_ENABLE          (0x04)      // Acceleration Z-axis Enabled

    #define LSM330_REG_ACCEL_CTRL_REG5_AYEN_MASK            (0x02)      // Acceleration Y-axis Enable
    #define LSM330_REG_ACCEL_CTRL_REG5_AYEN_DISABLE         (0x00)      // Acceleration Y-axis Disabled
    #define LSM330_REG_ACCEL_CTRL_REG5_AYEN_ENABLE          (0x02)      // Acceleration Y-axis Enabled

    #define LSM330_REG_ACCEL_CTRL_REG5_AXEN_MASK            (0x01)      // Acceleration X-axis Enable
    #define LSM330_REG_ACCEL_CTRL_REG5_AXEN_DISABLE         (0x00)      // Acceleration X-axis Disabled
    #define LSM330_REG_ACCEL_CTRL_REG5_AXEN_ENABLE          (0x01)      // Acceleration X-axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 6 DESCRIPTION
**************************************************************************/
    #define LSM330_REG_ACCEL_CTRL_REG6_ABW_MASK             (0xC0)      // Accelerometer Anti-Alias Filter Bandwidth
    #define LSM330_REG_ACCEL_CTRL_REG6_ABW_800              (0x00)      // 800 Hz
    #define LSM330_REG_ACCEL_CTRL_REG6_ABW_200              (0x40)      // 200 Hz
    #define LSM330_REG_ACCEL_CTRL_REG6_ABW_400              (0x80)      // 400 Hz
    #define LSM330_REG_ACCEL_CTRL_REG6_ABW_50               (0xC0)      // 50 Hz

    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_MASK             (0x38)      // Acceleration Full-Scale Selection
    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_2G               (0x00)      // ±2 g
    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_4G               (0x08)      // ±4 g
    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_6G               (0x10)      // ±6 g
    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_8G               (0x18)      // ±8 g
    #define LSM330_REG_ACCEL_CTRL_REG6_AFS_16G              (0x20)      // ±16 g

    #define LSM330_REG_ACCEL_CTRL_REG6_SIM_MASK             (0x01)      // SPI Serial Interface Mode Selection
    #define LSM330_REG_ACCEL_CTRL_REG6_SIM_4WIRE            (0x00)      // 4-Wire Interface
    #define LSM330_REG_ACCEL_CTRL_REG6_SIM_3WIRE            (0x01)      // 3-Wire Interface

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_MASK             (0xF0)      // Output Data Rate and Bandwidth Selection
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_95_12_5          (0x00)      // ODR (Hz): 95, Cutoff: 12.5
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_95_25            (0x30)      // ODR (Hz): 95, Cutoff: 25
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_190_12_5         (0x40)      // ODR (Hz): 190, Cutoff: 12.5
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_190_25           (0x50)      // ODR (Hz): 190, Cutoff: 25
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_190_50           (0x60)      // ODR (Hz): 190, Cutoff: 50
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_190_70           (0x70)      // ODR (Hz): 190, Cutoff: 70
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_380_20           (0x80)      // ODR (Hz): 380, Cutoff: 20
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_380_25           (0x90)      // ODR (Hz): 380, Cutoff: 25
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_380_50           (0xA0)      // ODR (Hz): 380, Cutoff: 50
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_380_100          (0xB0)      // ODR (Hz): 380, Cutoff: 100
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_760_30           (0xC0)      // ODR (Hz): 760, Cutoff: 30
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_760_35           (0xD0)      // ODR (Hz): 760, Cutoff: 35
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_760_50           (0xE0)      // ODR (Hz): 760, Cutoff: 50
    #define LSM330_REG_GYRO_CTRL_REG1_DRBW_760_100          (0xF0)      // ODR (Hz): 760, Cutoff: 100

    #define LSM330_REG_GYRO_CTRL_REG1_PD_MASK               (0x08)      // Power-Down Mode Enable
    #define LSM330_REG_GYRO_CTRL_REG1_PD_DOWN               (0x00)      // Power-Down Mode
    #define LSM330_REG_GYRO_CTRL_REG1_PD_NORMAL             (0x08)      // Normal Mode or Sleep Mode

    #define LSM330_REG_GYRO_CTRL_REG1_ZEN_MASK              (0x04)      // Gyroscope Z-axis Enable
    #define LSM330_REG_GYRO_CTRL_REG1_ZEN_DISABLE           (0x00)      // Gyroscope Z-axis Disabled
    #define LSM330_REG_GYRO_CTRL_REG1_ZEN_ENABLE            (0x04)      // Gyroscope Z-axis Enabled

    #define LSM330_REG_GYRO_CTRL_REG1_YEN_MASK              (0x02)      // Gyroscope Y-axis Enable
    #define LSM330_REG_GYRO_CTRL_REG1_YEN_DISABLE           (0x00)      // Gyroscope Y-axis Disabled
    #define LSM330_REG_GYRO_CTRL_REG1_YEN_ENABLE            (0x02)      // Gyroscope Y-axis Enabled

    #define LSM330_REG_GYRO_CTRL_REG1_XEN_MASK              (0x01)      // Gyroscope X-axis Enable
    #define LSM330_REG_GYRO_CTRL_REG1_XEN_DISABLE           (0x00)      // Gyroscope X-axis Disabled
    #define LSM330_REG_GYRO_CTRL_REG1_XEN_ENABLE            (0x01)      // Gyroscope X-axis Enabled

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LSM330_REG_GYRO_CTRL_REG4_BDU_MASK              (0x80)      // Block Data Update
    #define LSM330_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS        (0x00)      // Continuous Update
    #define LSM330_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE         (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define LSM330_REG_GYRO_CTRL_REG4_BLE_MASK              (0x40)      // Big/Little Endian Data Selection
    #define LSM330_REG_GYRO_CTRL_REG4_BLE_LSB               (0x00)      // Data LSB @ Lower Address
    #define LSM330_REG_GYRO_CTRL_REG4_BLE_MSB               (0x40)      // Data MSB @ Lower Address

    #define LSM330_REG_GYRO_CTRL_REG4_FS_MASK               (0x30)      // Full-Scale Selection
    #define LSM330_REG_GYRO_CTRL_REG4_FS_250                (0x00)      // 250 dps
    #define LSM330_REG_GYRO_CTRL_REG4_FS_500                (0x10)      // 500 dps
    #define LSM330_REG_GYRO_CTRL_REG4_FS_2000               (0x30)      // 2000 dps

    #define LSM330_REG_GYRO_CTRL_REG4_SIM_MASK              (0x01)      // SPI Serial Read Mode Selection
    #define LSM330_REG_GYRO_CTRL_REG4_SIM_3WIRE_DISABLE     (0x00)      // 3-Wire Read Mode Disbaled
    #define LSM330_REG_GYRO_CTRL_REG4_SIM_3WIRE_ENABLE      (0x01)      // 3-Wire Read Mode Enabled



typedef enum
{
    ACCEL_DATARATE_POWERDOWN        = LSM330_REG_ACCEL_CTRL_REG5_AODR_PD,
    ACCEL_DATARATE_3_125HZ          = LSM330_REG_ACCEL_CTRL_REG5_AODR_3_125,
    ACCEL_DATARATE_6_25HZ           = LSM330_REG_ACCEL_CTRL_REG5_AODR_6_25,
    ACCEL_DATARATE_12_5HZ           = LSM330_REG_ACCEL_CTRL_REG5_AODR_12_5,
    ACCEL_DATARATE_25HZ             = LSM330_REG_ACCEL_CTRL_REG5_AODR_25,
    ACCEL_DATARATE_50HZ             = LSM330_REG_ACCEL_CTRL_REG5_AODR_50,
    ACCEL_DATARATE_100HZ            = LSM330_REG_ACCEL_CTRL_REG5_AODR_100,
    ACCEL_DATARATE_200HZ            = LSM330_REG_ACCEL_CTRL_REG5_AODR_200,
    ACCEL_DATARATE_400HZ            = LSM330_REG_ACCEL_CTRL_REG5_AODR_400,
    ACCEL_DATARATE_800HZ            = LSM330_REG_ACCEL_CTRL_REG5_AODR_800,
    ACCEL_DATARATE_1600HZ           = LSM330_REG_ACCEL_CTRL_REG5_AODR_1600
    
} lsmAccelDataRate_t;

typedef enum
{
    ACCEL_CONTINUOUS                = LSM330_REG_ACCEL_CTRL_REG5_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED               = LSM330_REG_ACCEL_CTRL_REG5_BDU_NOTUPDATE
    
} lsmAccelBlockData_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           = LSM330_REG_ACCEL_CTRL_REG5_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = LSM330_REG_ACCEL_CTRL_REG5_AZEN_ENABLE
    
} lsmAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = LSM330_REG_ACCEL_CTRL_REG5_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = LSM330_REG_ACCEL_CTRL_REG5_AYEN_ENABLE
    
} lsmAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = LSM330_REG_ACCEL_CTRL_REG5_AYEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = LSM330_REG_ACCEL_CTRL_REG5_AYEN_ENABLE
    
} lsmAccelXen_t;

typedef enum
{
    ACCEL_BANDWIDTH_800HZ           = LSM330_REG_ACCEL_CTRL_REG6_ABW_800,
    ACCEL_BANDWIDTH_200HZ           = LSM330_REG_ACCEL_CTRL_REG6_ABW_200,
    ACCEL_BANDWIDTH_400HZ           = LSM330_REG_ACCEL_CTRL_REG6_ABW_400,
    ACCEL_BANDWIDTH_50HZ            = LSM330_REG_ACCEL_CTRL_REG6_ABW_50

} lsmAccelBandwidth_t;


typedef enum
{
    ACCEL_RANGE_2G                  = LSM330_REG_ACCEL_CTRL_REG6_AFS_2G,
    ACCEL_RANGE_4G                  = LSM330_REG_ACCEL_CTRL_REG6_AFS_4G,
    ACCEL_RANGE_6G                  = LSM330_REG_ACCEL_CTRL_REG6_AFS_6G,
    ACCEL_RANGE_8G                  = LSM330_REG_ACCEL_CTRL_REG6_AFS_8G,
    ACCEL_RANGE_16G                 = LSM330_REG_ACCEL_CTRL_REG6_AFS_16G
    
} lsmAccelRange_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                = LSM330_REG_ACCEL_CTRL_REG6_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                = LSM330_REG_ACCEL_CTRL_REG6_SIM_3WIRE
    
} lsmAccelSPIWire_t;


typedef enum
{
    GYRO_DATARATE_95_12_5           = LSM330_REG_GYRO_CTRL_REG1_DRBW_95_12_5,
    GYRO_DATARATE_95_25             = LSM330_REG_GYRO_CTRL_REG1_DRBW_95_25,
    GYRO_DATARATE_190_12_5          = LSM330_REG_GYRO_CTRL_REG1_DRBW_190_12_5,
    GYRO_DATARATE_190_25            = LSM330_REG_GYRO_CTRL_REG1_DRBW_190_25,
    GYRO_DATARATE_190_50            = LSM330_REG_GYRO_CTRL_REG1_DRBW_190_50,
    GYRO_DATARATE_190_70            = LSM330_REG_GYRO_CTRL_REG1_DRBW_190_70,
    GYRO_DATARATE_380_20            = LSM330_REG_GYRO_CTRL_REG1_DRBW_380_20,
    GYRO_DATARATE_380_25            = LSM330_REG_GYRO_CTRL_REG1_DRBW_380_25,
    GYRO_DATARATE_380_50            = LSM330_REG_GYRO_CTRL_REG1_DRBW_380_50,
    GYRO_DATARATE_380_100           = LSM330_REG_GYRO_CTRL_REG1_DRBW_380_100,
    GYRO_DATARATE_760_30            = LSM330_REG_GYRO_CTRL_REG1_DRBW_760_30,
    GYRO_DATARATE_760_35            = LSM330_REG_GYRO_CTRL_REG1_DRBW_760_35,
    GYRO_DATARATE_760_50            = LSM330_REG_GYRO_CTRL_REG1_DRBW_760_50,
    GYRO_DATARATE_760_100           = LSM330_REG_GYRO_CTRL_REG1_DRBW_760_100
    
} lsmGyroDataRate_t;

typedef enum
{
    GYRO_POWER_DOWN_MODE            = LSM330_REG_GYRO_CTRL_REG1_PD_DOWN,
    GYRO_NORMAL_MODE                = LSM330_REG_GYRO_CTRL_REG1_PD_NORMAL
    
} lsmGyroMode_t;

typedef enum
{
    GYRO_Z_AXIS_DISABLED            = LSM330_REG_GYRO_CTRL_REG1_ZEN_DISABLE,
    GYRO_Z_AXIS_ENABLED             = LSM330_REG_GYRO_CTRL_REG1_ZEN_ENABLE
    
} lsmGyroZen_t;

typedef enum
{
    GYRO_Y_AXIS_DISABLED            = LSM330_REG_GYRO_CTRL_REG1_YEN_DISABLE,
    GYRO_Y_AXIS_ENABLED             = LSM330_REG_GYRO_CTRL_REG1_YEN_ENABLE
    
} lsmGyroYen_t;

typedef enum
{
    GYRO_X_AXIS_DISABLED            = LSM330_REG_GYRO_CTRL_REG1_XEN_DISABLE,
    GYRO_X_AXIS_ENABLED             = LSM330_REG_GYRO_CTRL_REG1_XEN_ENABLE
    
} lsmGyroXen_t;

typedef enum
{
    GYRO_CONTINUOUS                 = LSM330_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS,
    GYRO_NOT_UPDATED                = LSM330_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE
    
} lsmGyroBlockData_t;

typedef enum
{
    GYRO_DATA_LSB                   = LSM330_REG_GYRO_CTRL_REG4_BLE_LSB,
    GYRO_DATA_MSB                   = LSM330_REG_GYRO_CTRL_REG4_BLE_MSB
    
} lsmGyroEndianData_t;

typedef enum
{
    GYRO_SCALE_250DPS               = LSM330_REG_GYRO_CTRL_REG4_FS_250,
    GYRO_SCALE_500DPS               = LSM330_REG_GYRO_CTRL_REG4_FS_500,
    GYRO_SCALE_2000DPS              = LSM330_REG_GYRO_CTRL_REG4_FS_2000
    
} lsmGyroScale_t;

typedef enum
{
    GYRO_SPI_WIRE_3_DISABLED        = LSM330_REG_GYRO_CTRL_REG4_SIM_3WIRE_DISABLE,
    GYRO_SPI_WIRE_3_ENABLED         = LSM330_REG_GYRO_CTRL_REG4_SIM_3WIRE_ENABLE
    
} lsmGyroSPIWire_t;


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    float   T;
    
} lsmSensorData_t;


class LSM330
{
    protected:
        // Instance-specific properties
        uint8_t lsm_conversionDelay;
        lsmAccelDataRate_t lsm_acceldatarate;
        lsmAccelBlockData_t lsm_accelblockdata;
        lsmAccelZen_t lsm_accelzen;
        lsmAccelYen_t lsm_accelyen;
        lsmAccelXen_t lsm_accelxen;
        lsmAccelBandwidth_t lsm_accelbandwidth;
        lsmAccelRange_t lsm_accelrange;
        lsmAccelSPIWire_t lsm_accelspiwire;
        lsmGyroDataRate_t lsm_gyrodatarate;
        lsmGyroMode_t lsm_gyromode;
        lsmGyroZen_t lsm_gyrozen;
        lsmGyroYen_t lsm_gyroyen;
        lsmGyroXen_t lsm_gyroxen;
        lsmGyroBlockData_t lsm_gyroblockdata;
        lsmGyroEndianData_t lsm_gyroendiandata;
        lsmGyroScale_t lsm_gyroscale;
        lsmGyroSPIWire_t lsm_gyrospiwire;

    public:
        uint8_t lsm_i2cAddressX;
        uint8_t lsm_i2cAddressG;
        lsmSensorData_t lsm_accelData;
        lsmSensorData_t lsm_gyroData;
        void getAddrX_LSM330(uint8_t i2cAddress);
        void getAddrG_LSM330(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpAccelerometer(void);
        void setUpGyroscope(void);
        void Measure_Accelerometer(void);
        void Measure_Gyroscope(void);
        void setAccelDataRate(lsmAccelDataRate_t acceldatarate);
        lsmAccelDataRate_t getAccelDataRate(void);
        void setAccelBlockData(lsmAccelBlockData_t Accelblockdata);
        lsmAccelBlockData_t getAccelBlockData(void);
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
        void setAccelSPIWire(lsmAccelSPIWire_t ACCELspiwire);
        lsmAccelSPIWire_t getAccelSPIWire(void);
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
        void setGyroSPIWire(lsmGyroSPIWire_t gyrospiwire);
        lsmGyroSPIWire_t getGyroSPIWire(void);
    
    private:
};
