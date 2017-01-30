/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS331HH
        This code is designed to work with the LIS331HH_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS331HH_I2CS#tabs-0-product_tabset-2
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
    #define LIS331HH_DEFAULT_ADDRESS                        (0x18)      // 3C >> 1 = 7-bit default
    #define LIS331HH_ADDRESS_UPDATED                        (0x19)      // 3D >> 1 = 7-bit

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LIS331HH_CONVERSIONDELAY                        (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define LIS331HH_REG_ACCEL_CTRL_REG1                    (0x20)      // Accelerometer Control Register 1
    #define LIS331HH_REG_ACCEL_CTRL_REG2                    (0x21)      // Accelerometer Control Register 2
    #define LIS331HH_REG_ACCEL_CTRL_REG3                    (0x22)      // Accelerometer Control Register 3
    #define LIS331HH_REG_ACCEL_CTRL_REG4                    (0x23)      // Accelerometer Control Register 4
    #define LIS331HH_REG_ACCEL_CTRL_REG5                    (0x24)      // Accelerometer Control Register 5
    #define LIS331HH_REG_ACCEL_HP_FILTER_RST                (0x25)      // Dummy Register
    #define LIS331HH_REG_ACCEL_REFERENCE                    (0x26)      // Reference/Datacapture Register
    #define LIS331HH_REG_ACCEL_STATUS                       (0x27)      // Status Register
    #define LIS331HH_REG_ACCEL_OUT_X_L                      (0x28)      // X-Axis Acceleration Data Low Register
    #define LIS331HH_REG_ACCEL_OUT_X_H                      (0x29)      // X-Axis Acceleration Data High Register
    #define LIS331HH_REG_ACCEL_OUT_Y_L                      (0x2A)      // Y-Axis Acceleration Data Low Register
    #define LIS331HH_REG_ACCEL_OUT_Y_H                      (0x2B)      // Y-Axis Acceleration Data High Register
    #define LIS331HH_REG_ACCEL_OUT_Z_L                      (0x2C)      // Z-Axis Acceleration Data Low Register
    #define LIS331HH_REG_ACCEL_OUT_Z_H                      (0x2D)      // Z-Axis Acceleration Data High Register
    #define LIS331HH_REG_ACCEL_INT1_CFG                     (0x30)      // Interrupt 1 Configuration Register
    #define LIS331HH_REG_ACCEL_INT1_SOURCE                  (0x31)      // Interrupt 1 Source Register
    #define LIS331HH_REG_ACCEL_INT1_THS                     (0x32)      // Interrupt 1 Threshold Register
    #define LIS331HH_REG_ACCEL_INT1_DURATION                (0x33)      // Interrupt 1 Duration Register
    #define LIS331HH_REG_ACCEL_INT2_CFG                     (0x34)      // Interrupt 2 Configuration Register
    #define LIS331HH_REG_ACCEL_INT2_SOURCE                  (0x35)      // Interrupt 2 Source Register
    #define LIS331HH_REG_ACCEL_INT2_THS                     (0x36)      // Interrupt 2 Threshold Register
    #define LIS331HH_REG_ACCEL_INT2_DURATION                (0x37)      // Interrupt 2 Duration Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_MASK            (0xE0)      // Power mode and Low-Power Output Data Rate Configurations
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_PWRDWN          (0x00)      // Power-Down Mode
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_NORMAL          (0x20)      // Normal Mode, ODRLP = ODR
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_0_5         (0x40)      // Low Power Mode, ODRLP: 0.5 Hz
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_1           (0x60)      // Low Power Mode, ODRLP: 1 Hz
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_2           (0x80)      // Low Power Mode, ODRLP: 2 Hz
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_5           (0xA0)      // Low Power Mode, ODRLP: 5 Hz
    #define LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_10          (0xC0)      // Low Power Mode, ODRLP: 10 Hz

    #define LIS331HH_REG_ACCEL_CTRL_REG1_AODR_MASK          (0x18)      // Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AODR_50            (0x00)      // ODR: 50 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 37
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AODR_100           (0x08)      // ODR: 100 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 74
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AODR_400           (0x10)      // ODR: 400 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 292
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AODR_1000          (0x18)      // ODR: 1000 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 780

    #define LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_MASK          (0x04)      // Acceleration Z-Axis Enable
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_DISABLE       (0x00)      // Acceleration Z-Axis Disabled
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        (0x04)      // Acceleration Z-Axis Enabled

    #define LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_MASK          (0x02)      // Acceleration Y-Axis Enable
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_DISABLE       (0x00)      // Acceleration Y-Axis Disabled
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        (0x02)      // Acceleration Y-Axis Enabled

    #define LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_MASK          (0x01)      // Acceleration X-Axis Enable
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_DISABLE       (0x00)      // Acceleration X-Axis Disabled
    #define LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_ENABLE        (0x01)      // Acceleration X-Axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LIS331HH_REG_ACCEL_CTRL_REG4_BDU_MASK           (0x80)      // Block Data Update
    #define LIS331HH_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     (0x00)      // Continuous Update
    #define LIS331HH_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED     (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define LIS331HH_REG_ACCEL_CTRL_REG4_BLE_MASK           (0x40)      // Big/Little Endian Data Selection
    #define LIS331HH_REG_ACCEL_CTRL_REG4_BLE_LSB            (0x00)      // Data LSB @ lower address
    #define LIS331HH_REG_ACCEL_CTRL_REG4_BLE_MSB            (0x40)      // Data MSB @ lower address

    #define LIS331HH_REG_ACCEL_CTRL_REG4_FS_MASK            (0x30)      // Full-Scale Selection
    #define LIS331HH_REG_ACCEL_CTRL_REG4_FS_6G              (0x00)      // +/- 6G
    #define LIS331HH_REG_ACCEL_CTRL_REG4_FS_12G             (0x10)      // +/- 12G
    #define LIS331HH_REG_ACCEL_CTRL_REG4_FS_24G             (0x30)      // +/- 24G

    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_MASK       (0x08)      // Self-Test Sign
    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS       (0x00)      // Self-Test Plus
    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_MINUS      (0x08)      // Self-Test Minus

    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_MASK            (0x02)      // Self-Test Enable
    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_DISABLED        (0x00)      // Self Test Disabled
    #define LIS331HH_REG_ACCEL_CTRL_REG4_ST_ENABLED         (0x02)      // Self-Test Enabled

    #define LIS331HH_REG_ACCEL_CTRL_REG4_SIM_MASK           (0x01)      // SPI Serial Interface Mode Selection
    #define LIS331HH_REG_ACCEL_CTRL_REG4_SIM_4WIRE          (0x00)      // 4-Wire Interface
    #define LIS331HH_REG_ACCEL_CTRL_REG4_SIM_3WIRE          (0x01)      // 3-Wire Interface


typedef enum
{
    POWER_MODE_DOWN                 = LIS331HH_REG_ACCEL_CTRL_REG1_PM_PWRDWN,
    POWER_MODE_NORMAL               = LIS331HH_REG_ACCEL_CTRL_REG1_PM_NORMAL,
    POWER_MODE_LOW_0_5              = LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_0_5,
    POWER_MODE_LOW_1                = LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_1,
    POWER_MODE_LOW_2                = LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_2,
    POWER_MODE_LOW_5                = LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_5,
    POWER_MODE_LOW_10               = LIS331HH_REG_ACCEL_CTRL_REG1_PM_LOW_10
    
} lisAccelPowerMode_t;

typedef enum
{
    ACCEL_DATARATE_50HZ             = LIS331HH_REG_ACCEL_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = LIS331HH_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_400HZ            = LIS331HH_REG_ACCEL_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_1000HZ           = LIS331HH_REG_ACCEL_CTRL_REG1_AODR_1000
    
} lisAccelDataRate_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           =   LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} lisAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} lisAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} lisAccelXen_t;

typedef enum
{
    ACCEL_CONTINUOUS                = LIS331HH_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED               = LIS331HH_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED
    
} lisAccelBlockData_t;

typedef enum
{
    GACCEL_DATA_LSB                 = LIS331HH_REG_ACCEL_CTRL_REG4_BLE_LSB,
    ACCEL_DATA_MSB                  = LIS331HH_REG_ACCEL_CTRL_REG4_BLE_MSB
    
} lisAccelEndianData_t;

typedef enum
{
    ACCEL_RANGE_6G                  = LIS331HH_REG_ACCEL_CTRL_REG4_FS_6G,
    ACCEL_RANGE_12G                 = LIS331HH_REG_ACCEL_CTRL_REG4_FS_12G,
    ACCEL_RANGE_24G                 = LIS331HH_REG_ACCEL_CTRL_REG4_FS_24G
    
} lisAccelRange_t;

typedef enum
{
    ACCEL_SELF_TEST_PLUS            = LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS,
    ACCEL_SELF_TEST_MINUS           = LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_MINUS
    
} lisAccelSelfTestSign_t;

typedef enum
{
    ACCEL_SELF_TEST_DISABLED        = LIS331HH_REG_ACCEL_CTRL_REG4_ST_DISABLED,
    ACCEL_SELF_TEST_ENABLED         = LIS331HH_REG_ACCEL_CTRL_REG4_ST_ENABLED
    
} lisAccelSelfTest_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                = LIS331HH_REG_ACCEL_CTRL_REG4_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                = LIS331HH_REG_ACCEL_CTRL_REG4_SIM_3WIRE
    
} lisAccelSPIWire_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} lisAccelData_t;


class LIS331HH
{
    protected:
        // Instance-specific properties
        uint8_t lis_conversionDelay;
        lisAccelPowerMode_t lis_accelpowermode;
        lisAccelDataRate_t lis_acceldatarate;
        lisAccelZen_t lis_accelzen;
        lisAccelYen_t lis_accelyen;
        lisAccelXen_t lis_accelxen;
        lisAccelBlockData_t lis_accelblockdata;
        lisAccelEndianData_t lis_accelendiandata;
        lisAccelRange_t lis_accelrange;
        lisAccelSelfTestSign_t lis_accelselftestsign;
        lisAccelSelfTest_t lis_accelselftest;
        lisAccelSPIWire_t lis_accelspiwire;
    
    public:
        uint8_t lis_i2cAddress;
        lisAccelData_t lis_accelData;
        void getAddr_LIS331HH(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelPowerMode(lisAccelPowerMode_t accelpowermode);
        lisAccelPowerMode_t getAccelPowerMode(void);
        void setAccelDataRate(lisAccelDataRate_t acceldatarate);
        lisAccelDataRate_t getAccelDataRate(void);
        void setAccelZen(lisAccelZen_t accelzen);
        lisAccelZen_t getAccelZen(void);
        void setAccelYen(lisAccelYen_t accelyen);
        lisAccelYen_t getAccelYen(void);
        void setAccelXen(lisAccelXen_t accelxen);
        lisAccelXen_t getAccelXen(void);
        void setAccelBlockData(lisAccelBlockData_t accelblockdata);
        lisAccelBlockData_t getAccelBlockData(void);
        void setAccelEndianData(lisAccelEndianData_t accelendiandata);
        lisAccelEndianData_t getAccelEndianData(void);
        void setAccelRange(lisAccelRange_t accelrange);
        lisAccelRange_t getAccelRange(void);
        void setAccelSelfTestSign(lisAccelSelfTestSign_t accelselftestsign);
        lisAccelSelfTestSign_t getAccelSelfTestSign(void);
        void setAccelSelfTest(lisAccelSelfTest_t accelselftest);
        lisAccelSelfTest_t getAccelSelfTest(void);
        void setAccelSPIWire(lisAccelSPIWire_t accelspiwire);
        lisAccelSPIWire_t getAccelSPIWire(void);
    
    private:
};
