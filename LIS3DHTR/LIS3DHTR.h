/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3DHTR
        This code is designed to work with the LIS3DHTR_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS3DHTR_I2CS#tabs-0-product_tabset-2
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
    #define LIS3DHTR_DEFAULT_ADDRESS                        (0x18)    // 3C >> 1 = 7-bit default
    #define LIS3DHTR_ADDRESS_UPDATED                        (0x19)    // 

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LIS3DHTR_CONVERSIONDELAY                        (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define LIS3DHTR_REG_ACCEL_STATUS                       (0x07)      // Status Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC1_L                   (0x28)      // 1-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC1_H                   (0x29)      // 1-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC2_L                   (0x2A)      // 2-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC2_H                   (0x2B)      // 2-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC3_L                   (0x2C)      // 3-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_ADC3_H                   (0x2D)      // 3-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_WHO_AM_I                     (0x0F)      // Device identification Register
    #define LIS3DHTR_REG_TEMP_CFG                           (0x1F)      // Temperature Sensor Register
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1                    (0x20)      // Accelerometer Control Register 1
    #define LIS3DHTR_REG_ACCEL_CTRL_REG2                    (0x21)      // Accelerometer Control Register 2
    #define LIS3DHTR_REG_ACCEL_CTRL_REG3                    (0x22)      // Accelerometer Control Register 3
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4                    (0x23)      // Accelerometer Control Register 4
    #define LIS3DHTR_REG_ACCEL_CTRL_REG5                    (0x24)      // Accelerometer Control Register 5
    #define LIS3DHTR_REG_ACCEL_CTRL_REG6                    (0x25)      // Accelerometer Control Register 6
    #define LIS3DHTR_REG_ACCEL_REFERENCE                    (0x26)      // Reference/Datacapture Register
    #define LIS3DHTR_REG_ACCEL_STATUS2                      (0x27)      // Status Register 2
    #define LIS3DHTR_REG_ACCEL_OUT_X_L                      (0x28)      // X-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_X_H                      (0x29)      // X-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_OUT_Y_L                      (0x2A)      // Y-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_Y_H                      (0x2B)      // Y-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_OUT_Z_L                      (0x2C)      // Z-Axis Acceleration Data Low Register
    #define LIS3DHTR_REG_ACCEL_OUT_Z_H                      (0x2D)      // Z-Axis Acceleration Data High Register
    #define LIS3DHTR_REG_ACCEL_FIFO_CTRL                    (0x2E)      // FIFO Control Register
    #define LIS3DHTR_REG_ACCEL_FIFO_SRC                     (0x2F)      // FIFO Source Register
    #define LIS3DHTR_REG_ACCEL_INT1_CFG                     (0x30)      // Interrupt Configuration Register
    #define LIS3DHTR_REG_ACCEL_INT1_SRC                     (0x31)      // Interrupt Source Register
    #define LIS3DHTR_REG_ACCEL_INT1_THS                     (0x32)      // Interrupt Threshold Register
    #define LIS3DHTR_REG_ACCEL_INT1_DURATION                (0x33)      // Interrupt Duration Register
    #define LIS3DHTR_REG_ACCEL_CLICK_CFG                    (0x38)      // Interrupt Click Recognition Register
    #define LIS3DHTR_REG_ACCEL_CLICK_SRC                    (0x39)      // Interrupt Click Source Register
    #define LIS3DHTR_REG_ACCEL_CLICK_THS                    (0x3A)      // Interrupt Click Threshold Register
    #define LIS3DHTR_REG_ACCEL_TIME_LIMIT                   (0x3B)      // Click Time Limit Register
    #define LIS3DHTR_REG_ACCEL_TIME_LATENCY                 (0x3C)      // Click Time Latency Register
    #define LIS3DHTR_REG_ACCEL_TIME_WINDOW                  (0x3D)      // Click Time Window Register


/**************************************************************************
    TEMPERATURE REGISTER DESCRIPTION
**************************************************************************/
    #define LIS3DHTR_REG_TEMP_ADC_PD_MASK                   (0x80)      // ADC Power Enable Status
    #define LIS3DHTR_REG_TEMP_ADC_PD_DISABLED               (0x00)      // ADC Disabled
    #define LIS3DHTR_REG_TEMP_ADC_PD_ENABLED                (0x80)      // ADC Enabled

    #define LIS3DHTR_REG_TEMP_TEMP_EN_MASK                  (0x40)      // Temperature Sensor (T) Enable Status
    #define LIS3DHTR_REG_TEMP_TEMP_EN_DISABLED              (0x00)      // Temperature Sensor (T) Disabled
    #define LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED               (0x40)      // Temperature Sensor (T) Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_MASK          (0xF0)      // Acceleration Data Rate Selection
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_PD            (0x00)      // Power-Down Mode
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1             (0x10)      // Normal / Low Power Mode (1 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_10            (0x20)      // Normal / Low Power Mode (10 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_25            (0x30)      // Normal / Low Power Mode (25 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_50            (0x40)      // Normal / Low Power Mode (50 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_100           (0x50)      // Normal / Low Power Mode (100 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_200           (0x60)      // Normal / Low Power Mode (200 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_400           (0x70)      // Normal / Low Power Mode (400 Hz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1_6K          (0x80)      // Low Power Mode (1.6 KHz)
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_5K            (0x90)      // Normal (1.25 KHz) / Low Power Mode (5 KHz)

    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_MASK          (0x08)      // Low Power Mode Enable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL        (0x00)      // Normal Mode
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_LOW           (0x08)      // Low Power Mode

    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_MASK          (0x04)      // Acceleration Z-Axis Enable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE       (0x00)      // Acceleration Z-Axis Disabled
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        (0x04)      // Acceleration Z-Axis Enabled

    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_MASK          (0x02)      // Acceleration Y-Axis Enable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE       (0x00)      // Acceleration Y-Axis Disabled
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        (0x02)      // Acceleration Y-Axis Enabled

    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_MASK          (0x01)      // Acceleration X-Axis Enable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE       (0x00)      // Acceleration X-Axis Disabled
    #define LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE        (0x01)      // Acceleration X-Axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_MASK           (0x80)      // Block Data Update
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     (0x00)      // Continuous Update
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED     (0x80)      // Output Registers Not Updated until MSB and LSB Read

    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_MASK           (0x40)      // Big/Little Endian Data Selection
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB            (0x00)      // Data LSB @ lower address
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_MSB            (0x40)      // Data MSB @ lower address

    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_MASK            (0x30)      // Full-Scale Selection
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G              (0x00)      // +/- 2G
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G              (0x10)      // +/- 4G
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G              (0x20)      // +/- 8G
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G             (0x30)      // +/- 16G

    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_MASK            (0x08)      // High Resolution Output Mode
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE         (0x00)      // High Resolution Disable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_ENABLE          (0x08)      // High Resolution Enable


    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_MASK            (0x06)      // Self-Test Enable
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL          (0x00)      // Normal Mode
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_0               (0x02)      // Self-Test 0
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_1               (0x04)      // Self-Test 1

    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_MASK           (0x01)      // SPI Serial Interface Mode Selection
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE          (0x00)      // 4-Wire Interface
    #define LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_3WIRE          (0x01)      // 3-Wire Interface


typedef enum
{
    ADC_DISABLED                    = LIS3DHTR_REG_TEMP_ADC_PD_DISABLED,
    ADC_ENABLED                     = LIS3DHTR_REG_TEMP_ADC_PD_ENABLED
    
} lisADCEnable_t;

typedef enum
{
    TEMP_DISABLED                   = LIS3DHTR_REG_TEMP_TEMP_EN_DISABLED,
    TEMP_ENABLED                    = LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED
    
} lisTempEnable_t;

typedef enum
{
    ACCEL_DATARATE_POWERDOWN        = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_PD,
    ACCEL_DATARATE_1HZ              = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1,
    ACCEL_DATARATE_10HZ             = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_10,
    ACCEL_DATARATE_25HZ             = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_25,
    ACCEL_DATARATE_50HZ             = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_50,
    ACCEL_DATARATE_100HZ            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_200HZ            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_200,
    ACCEL_DATARATE_400HZ            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_400,
    ACCEL_DATARATE_1_6KH            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_1_6K,
    ACCEL_DATARATE_5KHZ             = LIS3DHTR_REG_ACCEL_CTRL_REG1_AODR_5K
    
} lisAccelDataRate_t;

typedef enum
{
    POWER_MODE_NORMAL               = LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL,
    POWER_MODE_LOW                  = LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_LOW
    
} lisAccelPowerMode_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           =   LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} lisAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} lisAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} lisAccelXen_t;

typedef enum
{
    ACCEL_CONTINUOUS                = LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS,
    ACCEL_NOT_UPDATED               = LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_NOTUPDATED
    
} lisAccelBlockData_t;

typedef enum
{
    GACCEL_DATA_LSB                 = LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB,
    ACCEL_DATA_MSB                  = LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_MSB
    
} lisAccelEndianData_t;

typedef enum
{
    ACCEL_RANGE_2G                  = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_2G,
    ACCEL_RANGE_4G                  = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_4G,
    ACCEL_RANGE_8G                  = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_8G,
    ACCEL_RANGE_16G                 = LIS3DHTR_REG_ACCEL_CTRL_REG4_FS_16G
    
} lisAccelRange_t;

typedef enum
{
    ACCEL_RESOLUTION_LOW            = LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE,
    ACCEL_RESOLUTION_HIGH           = LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_ENABLE
    
} lisAccelResolution_t;

typedef enum
{
    ACCEL_SELF_TEST_NORMAL          = LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL,
    ACCEL_SELF_TEST_0               = LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_0,
    ACCEL_SELF_TEST_1               = LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_1
    
} lisAccelSelfTest_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                 = LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                 = LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_3WIRE
    
} lisAccelSPIWire_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} lisAccelData_t;


class LIS3DHTR
{
    protected:
        // Instance-specific properties
        uint8_t lis_conversionDelay;
        lisADCEnable_t lis_adcenable;
        lisTempEnable_t lis_tempenable;
        lisAccelDataRate_t lis_acceldatarate;
        lisAccelPowerMode_t lis_accelpowermode;
        lisAccelZen_t lis_accelzen;
        lisAccelYen_t lis_accelyen;
        lisAccelXen_t lis_accelxen;
        lisAccelBlockData_t lis_accelblockdata;
        lisAccelEndianData_t lis_accelendiandata;
        lisAccelRange_t lis_accelrange;
        lisAccelResolution_t lis_accelresolution;
        lisAccelSelfTest_t lis_accelselftest;
        lisAccelSPIWire_t lis_accelspiwire;
    
    public:
        uint8_t lis_i2cAddress;
        lisAccelData_t lis_accelData;
        void getAddr_LIS3DHTR(uint8_t i2cAddress);
        bool begin(void);
        void setUpADCTemperature(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setADCEnable(lisADCEnable_t adcenable);
        lisADCEnable_t getADCEnable(void);
        void setTempEnable(lisTempEnable_t tempenable);
        lisTempEnable_t getTempEnable(void);
        void setAccelDataRate(lisAccelDataRate_t acceldatarate);
        lisAccelDataRate_t getAccelDataRate(void);
        void setAccelPowerMode(lisAccelPowerMode_t accelpowermode);
        lisAccelPowerMode_t getAccelPowerMode(void);
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
        void setAccelResolution(lisAccelResolution_t accelresolution);
        lisAccelResolution_t getAccelResolution(void);
        void setAccelSelfTest(lisAccelSelfTest_t accelselftest);
        lisAccelSelfTest_t getAccelSelfTest(void);
        void setAccelSPIWire(lisAccelSPIWire_t accelspiwire);
        lisAccelSPIWire_t getAccelSPIWire(void);
    
    private:
};
