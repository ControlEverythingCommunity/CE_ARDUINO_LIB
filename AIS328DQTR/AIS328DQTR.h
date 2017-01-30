/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AIS328DQTR
        This code is designed to work with the AIS328DQTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=AIS328DQTR_I2CS#tabs-0-product_tabset-2
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
    #define AIS328DQTR_DEFAULT_ADDRESS                      (0x18)      // 3C >> 1 = 7-bit default
    #define AIS328DQTR_ADDRESS_UPDATED                      (0x19)      // 3D >> 1 = 7-bit

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define AIS328DQTR_CONVERSIONDELAY                      (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define AIS328DQTR_REG_ACCEL_WHO_AM_I                   (0x0F)      // Device identification Register
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1                  (0x20)      // Accelerometer Control Register 1
    #define AIS328DQTR_REG_ACCEL_CTRL_REG2                  (0x21)      // Accelerometer Control Register 2
    #define AIS328DQTR_REG_ACCEL_CTRL_REG3                  (0x22)      // Accelerometer Control Register 3
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4                  (0x23)      // Accelerometer Control Register 4
    #define AIS328DQTR_REG_ACCEL_CTRL_REG5                  (0x24)      // Accelerometer Control Register 5
    #define AIS328DQTR_REG_ACCEL_HP_FILTER_RST              (0x25)      // Dummy Register
    #define AIS328DQTR_REG_ACCEL_REFERENCE                  (0x26)      // Reference/Datacapture Register
    #define AIS328DQTR_REG_ACCEL_STATUS                     (0x27)      // Status Register
    #define AIS328DQTR_REG_ACCEL_OUT_X_L                    (0x28)      // X-Axis Acceleration Data Low Register
    #define AIS328DQTR_REG_ACCEL_OUT_X_H                    (0x29)      // X-Axis Acceleration Data High Register
    #define AIS328DQTR_REG_ACCEL_OUT_Y_L                    (0x2A)      // Y-Axis Acceleration Data Low Register
    #define AIS328DQTR_REG_ACCEL_OUT_Y_H                    (0x2B)      // Y-Axis Acceleration Data High Register
    #define AIS328DQTR_REG_ACCEL_OUT_Z_L                    (0x2C)      // Z-Axis Acceleration Data Low Register
    #define AIS328DQTR_REG_ACCEL_OUT_Z_H                    (0x2D)      // Z-Axis Acceleration Data High Register
    #define AIS328DQTR_REG_ACCEL_INT1_CFG                   (0x30)      // Interrupt 1 Configuration Register
    #define AIS328DQTR_REG_ACCEL_INT1_SOURCE                (0x31)      // Interrupt 1 Source Register
    #define AIS328DQTR_REG_ACCEL_INT1_THS                   (0x32)      // Interrupt 1 Threshold Register
    #define AIS328DQTR_REG_ACCEL_INT1_DURATION              (0x33)      // Interrupt 1 Duration Register
    #define AIS328DQTR_REG_ACCEL_INT2_CFG                   (0x34)      // Interrupt 2 Configuration Register
    #define AIS328DQTR_REG_ACCEL_INT2_SOURCE                (0x35)      // Interrupt 2 Source Register
    #define AIS328DQTR_REG_ACCEL_INT2_THS                   (0x36)      // Interrupt 2 Threshold Register
    #define AIS328DQTR_REG_ACCEL_INT2_DURATION              (0x37)      // Interrupt 2 Duration Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 CONFIGURATION
**************************************************************************/
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_MASK          (0xE0)      // Power mode and Low-Power Output Data Rate Configurations
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_PWRDWN        (0x00)      // Power-Down Mode
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_NORMAL        (0x20)      // Normal Mode, ODRLP = ODR
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_0_5       (0x40)      // Low Power Mode, ODRLP: 0.5 Hz
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_1         (0x60)      // Low Power Mode, ODRLP: 1 Hz
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_2         (0x80)      // Low Power Mode, ODRLP: 2 Hz
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_5         (0xA0)      // Low Power Mode, ODRLP: 5 Hz
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_10        (0xC0)      // Low Power Mode, ODRLP: 10 Hz

    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_MASK        (0x18)      // Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_50          (0x00)      // ODR: 50 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 37
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_100         (0x08)      // ODR: 100 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 74
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_400         (0x10)      // ODR: 400 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 292
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_1000        (0x18)      // ODR: 1000 Hz, Low-pass Filter Cut-Off Frequency [Hz]: 780

    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_MASK        (0x04)      // Acceleration Z-Axis Enable
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE     (0x00)      // Acceleration Z-Axis Disabled
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE      (0x04)      // Acceleration Z-Axis Enabled

    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_MASK        (0x02)      // Acceleration Y-Axis Enable
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE     (0x00)      // Acceleration Y-Axis Disabled
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE      (0x02)      // Acceleration Y-Axis Enabled

    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_MASK        (0x01)      // Acceleration X-Axis Enable
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE     (0x00)      // Acceleration X-Axis Disabled
    #define AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE      (0x01)      // Acceleration X-Axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4
**************************************************************************/
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_MASK         (0x80)      // Block Data Update
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS   (0x00)      // Continuous Update
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED   (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_MASK         (0x40)      // Big/Little Endian Data Selection
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_LSB          (0x00)      // Data LSB @ lower address
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_MSB          (0x40)      // Data MSB @ lower address

    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_MASK          (0x30)      // Full-Scale Selection
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_2G            (0x00)      // +/- 2G
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_4G            (0x10)      // +/- 4G
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_8G            (0x30)      // +/- 8G

    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_MASK     (0x08)      // Self-Test Sign
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS     (0x00)      // Self-Test Plus
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_MINUS    (0x08)      // Self-Test Minus

    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_MASK          (0x02)      // Self-Test Enable
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_DISABLED      (0x00)      // Self Test Disabled
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_ENABLED       (0x02)      // Self-Test Enabled

    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_MASK         (0x01)      // SPI Serial Interface Mode Selection
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE        (0x00)      // 4-Wire Interface
    #define AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_3WIRE        (0x01)      // 3-Wire Interface


typedef enum
{
    POWER_MODE_DOWN                 = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_PWRDWN,
    POWER_MODE_NORMAL               = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_NORMAL,
    POWER_MODE_LOW_0_5              = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_0_5,
    POWER_MODE_LOW_1                = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_1,
    POWER_MODE_LOW_2                = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_2,
    POWER_MODE_LOW_5                = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_5,
    POWER_MODE_LOW_10               = AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_LOW_10
    
} aisAccelPowerMode_t;

typedef enum
{
    ACCEL_DATARATE_50HZ             = AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_400HZ            = AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_1000HZ           = AIS328DQTR_REG_ACCEL_CTRL_REG1_AODR_1000
    
} aisAccelDataRate_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           = AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} aisAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} aisAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} aisAccelXen_t;

typedef enum
{
    ACCEL_CONTINUOUS                = AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED               = AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED
    
} aisAccelBlockData_t;

typedef enum
{
    ACCEL_DATA_LSB                 = AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_LSB,
    ACCEL_DATA_MSB                  = AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_MSB
    
} aisAccelEndianData_t;

typedef enum
{
    ACCEL_RANGE_2G                  = AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_2G,
    ACCEL_RANGE_4G                  = AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_4G,
    ACCEL_RANGE_8G                  = AIS328DQTR_REG_ACCEL_CTRL_REG4_FS_8G
    
} aisAccelRange_t;

typedef enum
{
    ACCEL_SELF_TEST_PLUS            = AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS,
    ACCEL_SELF_TEST_MINUS           = AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_MINUS
    
} aisAccelSelfTestSign_t;

typedef enum
{
    ACCEL_SELF_TEST_DISABLED        = AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_DISABLED,
    ACCEL_SELF_TEST_ENABLED         = AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_ENABLED
    
} aisAccelSelfTest_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                = AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                = AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_3WIRE
    
} aisAccelSPIWire_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} aisAccelData_t;


class AIS328DQTR
{
    protected:
        // Instance-specific properties
        uint8_t ais_conversionDelay;
        aisAccelPowerMode_t ais_accelpowermode;
        aisAccelDataRate_t ais_acceldatarate;
        aisAccelZen_t ais_accelzen;
        aisAccelYen_t ais_accelyen;
        aisAccelXen_t ais_accelxen;
        aisAccelBlockData_t ais_accelblockdata;
        aisAccelEndianData_t ais_accelendiandata;
        aisAccelRange_t ais_accelrange;
        aisAccelSelfTestSign_t ais_accelselftestsign;
        aisAccelSelfTest_t ais_accelselftest;
        aisAccelSPIWire_t ais_accelspiwire;
    
    public:
        uint8_t ais_i2cAddress;
        aisAccelData_t ais_accelData;
        void getAddr_AIS328DQTR(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelPowerMode(aisAccelPowerMode_t accelpowermode);
        aisAccelPowerMode_t getAccelPowerMode(void);
        void setAccelDataRate(aisAccelDataRate_t acceldatarate);
        aisAccelDataRate_t getAccelDataRate(void);
        void setAccelZen(aisAccelZen_t accelzen);
        aisAccelZen_t getAccelZen(void);
        void setAccelYen(aisAccelYen_t accelyen);
        aisAccelYen_t getAccelYen(void);
        void setAccelXen(aisAccelXen_t accelxen);
        aisAccelXen_t getAccelXen(void);
        void setAccelBlockData(aisAccelBlockData_t accelblockdata);
        aisAccelBlockData_t getAccelBlockData(void);
        void setAccelEndianData(aisAccelEndianData_t accelendiandata);
        aisAccelEndianData_t getAccelEndianData(void);
        void setAccelRange(aisAccelRange_t accelrange);
        aisAccelRange_t getAccelRange(void);
        void setAccelSelfTestSign(aisAccelSelfTestSign_t accelselftestsign);
        aisAccelSelfTestSign_t getAccelSelfTestSign(void);
        void setAccelSelfTest(aisAccelSelfTest_t accelselftest);
        aisAccelSelfTest_t getAccelSelfTest(void);
        void setAccelSPIWire(aisAccelSPIWire_t accelspiwire);
        aisAccelSPIWire_t getAccelSPIWire(void);
    
    private:
};
