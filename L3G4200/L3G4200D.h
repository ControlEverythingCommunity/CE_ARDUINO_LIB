/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        L3G4200D
        This code is designed to work with the L3G4200D_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=L3G4200D_I2CS#tabs-0-product_tabset-2
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
    #define L3G4200D_DEFAULT_ADDRESS_GYRO                   (0x68)      // 1101000b
    #define L3G4200D_ADDRESS_GYRO_UPDATED                   (0x69)      // 1101001b
    #define L3G4200D_GYRO_DEV_ID                            (0xD3)      // 1101 0011

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define L3G4200D_CONVERSIONDELAY                        (100)

/**************************************************************************
    GYROSCOPE REGISTERS
**************************************************************************/
    #define L3G4200D_REG_GYRO_WHO_AM_I                      (0x0F)      // Device identification Register
    #define L3G4200D_REG_GYRO_CTRL_REG1                     (0x20)      // Gyroscope Control Register 1
    #define L3G4200D_REG_GYRO_CTRL_REG2                     (0x21)      // Gyroscope Control Register 2
    #define L3G4200D_REG_GYRO_CTRL_REG3                     (0x22)      // Gyroscope Control Register 3
    #define L3G4200D_REG_GYRO_CTRL_REG4                     (0x23)      // Gyroscope Control Register 4
    #define L3G4200D_REG_GYRO_CTRL_REG5                     (0x24)      // Gyroscope Control Register 5
    #define L3G4200D_REG_GYRO_REFERENCE                     (0x25)      // Reference/Datacapture Register
    #define L3G4200D_REG_TEMPERATURE_OUT                    (0x26)      // Temperature Data Output Register
    #define L3G4200D_REG_GYRO_STATUS                        (0x27)      // Status Register
    #define L3G4200D_REG_GYRO_OUT_X_L                       (0x28)      // X-Axis Angular Rate Data Low Register
    #define L3G4200D_REG_GYRO_OUT_X_H                       (0x29)      // X-Axis Angular Rate Data High Register
    #define L3G4200D_REG_GYRO_OUT_Y_L                       (0x2A)      // Y-Axis Angular Rate Data Low Register
    #define L3G4200D_REG_GYRO_OUT_Y_H                       (0x2B)      // Y-Axis Angular Rate Data High Register
    #define L3G4200D_REG_GYRO_OUT_Z_L                       (0x2C)      // Z-Axis Angular Rate Data Low Register
    #define L3G4200D_REG_GYRO_OUT_Z_H                       (0x2D)      // Z-Axis Angular Rate Data High Register
    #define L3G4200D_REG_GYRO_FIFO_CTRL                     (0x2E)      // FIFO Control Register
    #define L3G4200D_REG_GYRO_FIFO_SRC                      (0x2F)      // FIFO Source Register
    #define L3G4200D_REG_GYRO_INT1_CFG                      (0x30)      // Interrupt Configuration Register
    #define L3G4200D_REG_GYRO_INT1_SRC                      (0x31)      // Interrupt Source Register
    #define L3G4200D_REG_GYRO_INT1_THS_XH                   (0x32)      // Interrupt X-Axis High Threshold Register
    #define L3G4200D_REG_GYRO_INT1_THS_XL                   (0x33)      // Interrupt X-Axis Low Threshold Register
    #define L3G4200D_REG_GYRO_INT1_THS_YH                   (0x34)      // Interrupt Y-Axis High Threshold Register
    #define L3G4200D_REG_GYRO_INT1_THS_YL                   (0x35)      // Interrupt Y-Axis Low Threshold Register
    #define L3G4200D_REG_GYRO_INT1_THS_ZH                   (0x36)      // Interrupt Z-Axis High Threshold Register
    #define L3G4200D_REG_GYRO_INT1_THS_ZL                   (0x37)      // Interrupt Z-Axis Low Threshold Register
    #define L3G4200D_REG_GYRO_INT1_DURATION                 (0x38)      // Interrupt Duration Register

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_MASK           (0xF0)      // Output Data Rate and Bandwidth Selection
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_100_12_5       (0x00)      // ODR (Hz): 100, Cutoff: 12.5
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_100_25         (0x30)      // ODR (Hz): 100, Cutoff: 25
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_12_5       (0x40)      // ODR (Hz): 200, Cutoff: 12.5
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_25         (0x50)      // ODR (Hz): 200, Cutoff: 25
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_50         (0x60)      // ODR (Hz): 200, Cutoff: 50
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_70         (0x70)      // ODR (Hz): 200, Cutoff: 70
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_20         (0x80)      // ODR (Hz): 400, Cutoff: 20
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_25         (0x90)      // ODR (Hz): 400, Cutoff: 25
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_50         (0xA0)      // ODR (Hz): 400, Cutoff: 50
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_100        (0xB0)      // ODR (Hz): 400, Cutoff: 100
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_30         (0xC0)      // ODR (Hz): 800, Cutoff: 30
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_35         (0xD0)      // ODR (Hz): 800, Cutoff: 35
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_50         (0xE0)      // ODR (Hz): 800, Cutoff: 50
    #define L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_100        (0xF0)      // ODR (Hz): 800, Cutoff: 100

    #define L3G4200D_REG_GYRO_CTRL_REG1_PD_MASK             (0x08)      // Power-Down Mode Enable
    #define L3G4200D_REG_GYRO_CTRL_REG1_PD_DOWN             (0x00)      // Power-Down Mode
    #define L3G4200D_REG_GYRO_CTRL_REG1_PD_NORMAL           (0x08)      // Normal Mode or Sleep Mode

    #define L3G4200D_REG_GYRO_CTRL_REG1_ZEN_MASK            (0x04)      // Gyroscope Z-axis Enable
    #define L3G4200D_REG_GYRO_CTRL_REG1_ZEN_DISABLE         (0x00)      // Gyroscope Z-axis Disabled
    #define L3G4200D_REG_GYRO_CTRL_REG1_ZEN_ENABLE          (0x04)      // Gyroscope Z-axis Enabled

    #define L3G4200D_REG_GYRO_CTRL_REG1_YEN_MASK            (0x02)      // Gyroscope Y-axis Enable
    #define L3G4200D_REG_GYRO_CTRL_REG1_YEN_DISABLE         (0x00)      // Gyroscope Y-axis Disabled
    #define L3G4200D_REG_GYRO_CTRL_REG1_YEN_ENABLE          (0x02)      // Gyroscope Y-axis Enabled

    #define L3G4200D_REG_GYRO_CTRL_REG1_XEN_MASK            (0x01)      // Gyroscope X-axis Enable
    #define L3G4200D_REG_GYRO_CTRL_REG1_XEN_DISABLE         (0x00)      // Gyroscope X-axis Disabled
    #define L3G4200D_REG_GYRO_CTRL_REG1_XEN_ENABLE          (0x01)      // Gyroscope X-axis Enabled

/**************************************************************************
    GYROSCOPE CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define L3G4200D_REG_GYRO_CTRL_REG4_BDU_MASK            (0x80)      // Block Data Update
    #define L3G4200D_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS      (0x00)      // Continuous Update
    #define L3G4200D_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE       (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define L3G4200D_REG_GYRO_CTRL_REG4_BLE_MASK            (0x40)      // Big/Little Endian Data Selection
    #define L3G4200D_REG_GYRO_CTRL_REG4_BLE_LSB             (0x00)      // Data LSB @ Lower Address
    #define L3G4200D_REG_GYRO_CTRL_REG4_BLE_MSB             (0x40)      // Data MSB @ Lower Address

    #define L3G4200D_REG_GYRO_CTRL_REG4_FS_MASK             (0x30)      // Full-Scale Selection
    #define L3G4200D_REG_GYRO_CTRL_REG4_FS_250              (0x00)      // 250 dps
    #define L3G4200D_REG_GYRO_CTRL_REG4_FS_500              (0x10)      // 500 dps
    #define L3G4200D_REG_GYRO_CTRL_REG4_FS_2000             (0x30)      // 2000 dps

    #define L3G4200D_REG_GYRO_CTRL_REG4_ST_MASK             (0x06)      // Self-Test Enable
    #define L3G4200D_REG_GYRO_CTRL_REG4_ST_NORMAL           (0x00)      // Normal Mode
    #define L3G4200D_REG_GYRO_CTRL_REG4_ST_0                (0x02)      // Self-Test 0 (+)
    #define L3G4200D_REG_GYRO_CTRL_REG4_ST_1                (0x06)      // Self-Test 1 (-)

    #define L3G4200D_REG_GYRO_CTRL_REG4_SIM_MASK            (0x01)      // SPI Serial Interface Mode Selection
    #define L3G4200D_REG_GYRO_CTRL_REG4_SIM_4WIRE           (0x00)      // 4-Wire Interface
    #define L3G4200D_REG_GYRO_CTRL_REG4_SIM_3WIRE           (0x01)      // 3-Wire Interface


typedef enum
{
    GYRO_DATARATE_100_12_5          = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_100_12_5,
    GYRO_DATARATE_100_25            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_100_25,
    GYRO_DATARATE_200_12_5          = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_12_5,
    GYRO_DATARATE_200_25            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_25,
    GYRO_DATARATE_200_50            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_50,
    GYRO_DATARATE_200_70            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_200_70,
    GYRO_DATARATE_400_20            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_20,
    GYRO_DATARATE_400_25            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_25,
    GYRO_DATARATE_400_50            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_50,
    GYRO_DATARATE_400_100           = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_400_100,
    GYRO_DATARATE_800_30            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_30,
    GYRO_DATARATE_800_35            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_35,
    GYRO_DATARATE_800_50            = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_50,
    GYRO_DATARATE_800_100           = L3G4200D_REG_GYRO_CTRL_REG1_DRBW_800_100
    
} l3gGyroDataRate_t;

typedef enum
{
    GYRO_POWER_DOWN_MODE            = L3G4200D_REG_GYRO_CTRL_REG1_PD_DOWN,
    GYRO_NORMAL_MODE                = L3G4200D_REG_GYRO_CTRL_REG1_PD_NORMAL
    
} l3gGyroMode_t;

typedef enum
{
    GYRO_Z_AXIS_DISABLED            = L3G4200D_REG_GYRO_CTRL_REG1_ZEN_DISABLE,
    GYRO_Z_AXIS_ENABLED             = L3G4200D_REG_GYRO_CTRL_REG1_ZEN_ENABLE
    
} l3gGyroZen_t;

typedef enum
{
    GYRO_Y_AXIS_DISABLED            = L3G4200D_REG_GYRO_CTRL_REG1_YEN_DISABLE,
    GYRO_Y_AXIS_ENABLED             = L3G4200D_REG_GYRO_CTRL_REG1_YEN_ENABLE
    
} l3gGyroYen_t;

typedef enum
{
    GYRO_X_AXIS_DISABLED            = L3G4200D_REG_GYRO_CTRL_REG1_XEN_DISABLE,
    GYRO_X_AXIS_ENABLED             = L3G4200D_REG_GYRO_CTRL_REG1_XEN_ENABLE
    
} l3gGyroXen_t;

typedef enum
{
    GYRO_CONTINUOUS                 = L3G4200D_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS,
    GYRO_NOT_UPDATED                = L3G4200D_REG_GYRO_CTRL_REG4_BDU_NOTUPDATE
    
} l3gGyroBlockData_t;

typedef enum
{
    GYRO_DATA_LSB                   = L3G4200D_REG_GYRO_CTRL_REG4_BLE_LSB,
    GYRO_DATA_MSB                   = L3G4200D_REG_GYRO_CTRL_REG4_BLE_MSB
    
} l3gGyroEndianData_t;

typedef enum
{
    GYRO_SCALE_250DPS               = L3G4200D_REG_GYRO_CTRL_REG4_FS_250,
    GYRO_SCALE_500DPS               = L3G4200D_REG_GYRO_CTRL_REG4_FS_500,
    GYRO_SCALE_2000DPS              = L3G4200D_REG_GYRO_CTRL_REG4_FS_2000
    
} l3gGyroScale_t;

typedef enum
{
    GYRO_SELF_TEST_NORMAL           = L3G4200D_REG_GYRO_CTRL_REG4_ST_NORMAL,
    GYRO_SELF_TEST_0                = L3G4200D_REG_GYRO_CTRL_REG4_ST_0,
    GYRO_SELF_TEST_1                = L3G4200D_REG_GYRO_CTRL_REG4_ST_1
    
} l3gGyroSelfTest_t;

typedef enum
{
    GYRO_SPI_WIRE_4                 = L3G4200D_REG_GYRO_CTRL_REG4_SIM_4WIRE,
    GYRO_SPI_WIRE_3                 = L3G4200D_REG_GYRO_CTRL_REG4_SIM_3WIRE
    
} l3gGyroSPIWire_t;


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} l3gSensorData_t;


class L3G4200D
{
    protected:
        // Instance-specific properties
        uint8_t l3g_conversionDelay;
        l3gGyroDataRate_t l3g_gyrodatarate;
        l3gGyroMode_t l3g_gyromode;
        l3gGyroZen_t l3g_gyrozen;
        l3gGyroYen_t l3g_gyroyen;
        l3gGyroXen_t l3g_gyroxen;
        l3gGyroBlockData_t l3g_gyroblockdata;
        l3gGyroEndianData_t l3g_gyroendiandata;
        l3gGyroScale_t l3g_gyroscale;
        l3gGyroSelfTest_t l3g_gyroselftest;
        l3gGyroSPIWire_t l3g_gyrospiwire;

    public:
        uint8_t l3g_i2cAddress;
        l3gSensorData_t l3g_gyroData;
        void getAddr_L3G4200D(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpGyroscope(void);
        void Measure_Gyroscope(void);
        void setGyroDataRate(l3gGyroDataRate_t gyrodatarate);
        l3gGyroDataRate_t getGyroDataRate(void);
        void setGyroMode(l3gGyroMode_t gyromode);
        l3gGyroMode_t getGyroMode(void);
        void setGyroZen(l3gGyroZen_t gyrozen);
        l3gGyroZen_t getGyroZen(void);
        void setGyroYen(l3gGyroYen_t gyroyen);
        l3gGyroYen_t getGyroYen(void);
        void setGyroXen(l3gGyroXen_t gyroxen);
        l3gGyroXen_t getGyroXen(void);
        void setGyroBlockData(l3gGyroBlockData_t gyroblockdata);
        l3gGyroBlockData_t getGyroBlockData(void);
        void setGyroEndianData(l3gGyroEndianData_t gyroendiandata);
        l3gGyroEndianData_t getGyroEndianData(void);
        void setGyroScale(l3gGyroScale_t gyroscale);
        l3gGyroScale_t getGyroScale(void);
        void setGyroSelfTest(l3gGyroSelfTest_t gyroselftest);
        l3gGyroSelfTest_t getGyroSelfTest(void);
        void setGyroSPIWire(l3gGyroSPIWire_t gyrospiwire);
        l3gGyroSPIWire_t getGyroSPIWire(void);
    
    private:
};
