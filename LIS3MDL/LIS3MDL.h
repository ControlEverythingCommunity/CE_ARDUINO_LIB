/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3MDL
        This code is designed to work with the LIS3MDL_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Magnetic-Sensor?sku=LIS3MDL_I2CS#tabs-0-product_tabset-2
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
    #define LIS3MDL_DEFAULT_ADDRESS                         (0x1C)      // 0011100b
    #define LIS3MDL_ADDRESS_UPDATED                         (0x1D)      // 0011101b
    #define LIS3MDL_MAG_DEV_ID                              (0x3B)      // 00111011

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LIS3MDL_CONVERSIONDELAY                         (100)

/**************************************************************************
    MAGNETOMETER REGISTERS
**************************************************************************/
    #define LIS3MDL_REG_MAG_WHO_AM_I                        (0x0F)      // Magnetometer Who Am I
    #define LIS3MDL_REG_MAG_CTRL_REG1                       (0x20)      // Magnetometer Control Register 1
    #define LIS3MDL_REG_MAG_CTRL_REG2                       (0x21)      // Magnetometer Control Register 2
    #define LIS3MDL_REG_MAG_CTRL_REG3                       (0x22)      // Magnetometer Control Register 3
    #define LIS3MDL_REG_MAG_CTRL_REG4                       (0x23)      // Magnetometer Control Register 4
    #define LIS3MDL_REG_MAG_CTRL_REG5                       (0x24)      // Magnetometer Control Register 5
    #define LIS3MDL_REG_MAG_STATUS                          (0x27)      // Status Register
    #define LIS3MDL_REG_MAG_OUT_X_L                         (0x28)      // X-Axis Magnetic Field Low Data Register
    #define LIS3MDL_REG_MAG_OUT_X_H                         (0x29)      // X-Axis Magnetic Field High Data Register
    #define LIS3MDL_REG_MAG_OUT_Y_L                         (0x2A)      // Y-Axis Magnetic Field Low Data Register
    #define LIS3MDL_REG_MAG_OUT_Y_H                         (0x2B)      // Y-Axis Magnetic Field High Data Register
    #define LIS3MDL_REG_MAG_OUT_Z_L                         (0x2C)      // Z-Axis Magnetic Field Low Data Register
    #define LIS3MDL_REG_MAG_OUT_Z_H                         (0x2D)      // Z-Axis Magnetic Field High Data Register
    #define LIS3MDL_REG_TEMP_OUT_L                          (0x2E)      // Temperature Low Data Register
    #define LIS3MDL_REG_TEMP_OUT_H                          (0x2F)      // Temperature High Data Register
    #define LIS3MDL_REG_MAG_INT_CFG                         (0x30)      // Interrupt Configuration Register
    #define LIS3MDL_REG_MAG_INT_SRC                         (0x31)      // Interrupt Source Register
    #define LIS3MDL_REG_MAG_INT_THS_L                       (0x32)      // Interrupt Threshold Low Register 1
    #define LIS3MDL_REG_MAG_INT_THS_H                       (0x33)      // Interrupt Duration High Register 1

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LIS3MDL_REG_MAG_CTRL_TEMP_EN_MASK               (0x80)      // Temperature Sensor Enable
    #define LIS3MDL_REG_MAG_CTRL_TEMP_EN_DISABLED           (0x00)      // Temperature Sensor Disabled
    #define LIS3MDL_REG_MAG_CTRL_TEMP_EN_ENABLED            (0x80)      // Temperature Sensor Enabled

    #define LIS3MDL_REG_MAG_CTRL_REG1_OM_MASK               (0x60)      // X and Y Axes Operative Mode Selection
    #define LIS3MDL_REG_MAG_CTRL_REG1_OM_LOW                (0x00)      // Low-power Mode,  ODR [Hz]: 1000, FAST_ODR: 1
    #define LIS3MDL_REG_MAG_CTRL_REG1_OM_MEDIUM             (0x20)      // Medium-Performance Mode,  ODR [Hz]: 560, FAST_ODR: 1
    #define LIS3MDL_REG_MAG_CTRL_REG1_OM_HIGH               (0x40)      // High-Performance Mode,  ODR [Hz]: 300, FAST_ODR: 1
    #define LIS3MDL_REG_MAG_CTRL_REG1_OM_ULTRA_HIGH         (0x60)      // Ultra-High-Performance Mode,  ODR [Hz]: 155, FAST_ODR: 1

    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_MASK               (0x1C)      // Output Data Rate Selection
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_0_625              (0x00)      // ODR [Hz]: 0.625
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_1_25               (0x04)      // ODR [Hz]: 1.25
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_2_5                (0x08)      // ODR [Hz]: 2.5
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_5                  (0x0C)      // ODR [Hz]: 5
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_10                 (0x10)      // ODR [Hz]: 10
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_20                 (0x14)      // ODR [Hz]: 20
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_40                 (0x18)      // ODR [Hz]: 40
    #define LIS3MDL_REG_MAG_CTRL_REG1_DO_80                 (0x1C)      // ODR [Hz]: 80

    #define LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_MASK         (0x02)      // FAST_ODR Enables Data Rates Higher than 80 Hz
    #define LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_DISABLED     (0x00)      // FAST_ODR Disabled
    #define LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_ENABLED      (0x02)      // FAST_ODR Enabled

    #define LIS3MDL_REG_MAG_CTRL_REG1_ST_MASK               (0x10)      // Self-Test Enable
    #define LIS3MDL_REG_MAG_CTRL_REG1_ST_DISABLED           (0x00)      // Self Test Disabled
    #define LIS3MDL_REG_MAG_CTRL_REG1_ST_ENABLED            (0x10)      // Self-Test Enabled

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define LIS3MDL_REG_MAG_CTRL_REG2_FS_MASK               (0x60)      // Full-Scale Selection
    #define LIS3MDL_REG_MAG_CTRL_REG2_FS_4G                 (0x00)      // ±4 Gauss
    #define LIS3MDL_REG_MAG_CTRL_REG2_FS_8G                 (0x20)      // ±8 Gauss
    #define LIS3MDL_REG_MAG_CTRL_REG2_FS_12G                (0x40)      // ±12 Gauss
    #define LIS3MDL_REG_MAG_CTRL_REG2_FS_16G                (0x60)      // ±16 Gauss

    #define LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_MASK           (0x08)      // Reboot Memory Content
    #define LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_NORMAL         (0x00)      // Normal Mode
    #define LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_REBOOT         (0x08)      // Reboot Memory Content

    #define LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_MASK         (0x04)      // Configuration Registers and User Register Reset Function
    #define LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_DEFAULT      (0x00)      // Default Value
    #define LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_RESET        (0x04)      // Reset Operation

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 3 DESCRIPTION
**************************************************************************/
    #define LIS3MDL_REG_MAG_CTRL_REG3_LP_MASK               (0x20)      // Low-Power Mode Configuration
    #define LIS3MDL_REG_MAG_CTRL_REG3_LP_0                  (0x00)      // The Magnetic Data rate is Configured by the DO Bits
    #define LIS3MDL_REG_MAG_CTRL_REG3_LP_1                  (0x20)      // DO[2:0] is Set to 0.625 Hz and the System Performs, for Each Channel, the Minimum Number of Averages

    #define LIS3MDL_REG_MAG_CTRL_REG3_SIM_MASK              (0x04)      // SPI Serial Interface Mode Selection
    #define LIS3MDL_REG_MAG_CTRL_REG3_SIM_4WIRE             (0x00)      // 4-Wire Interface
    #define LIS3MDL_REG_MAG_CTRL_REG3_SIM_3WIRE             (0x04)      // 3-Wire Interface

    #define LIS3MDL_REG_MAG_CTRL_REG3_MD_MASK               (0x03)      // Operating Mode Selection
    #define LIS3MDL_REG_MAG_CTRL_REG3_MD_CONTINUOUS         (0x00)      // Continuous-Conversion Mode
    #define LIS3MDL_REG_MAG_CTRL_REG3_MD_SINGLE             (0x01)      // Single-Conversion Mode
    #define LIS3MDL_REG_MAG_CTRL_REG3_MD_POWER_DOWN         (0x03)      // Power-Down Mode

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 4 DESCRIPTION
**************************************************************************/
    #define LIS3MDL_REG_MAG_CTRL_REG4_OMZ_MASK              (0x0C)      // Z-Axis Operative Mode Selection
    #define LIS3MDL_REG_MAG_CTRL_REG4_OMZ_LOW               (0x00)      // Low-power Mode
    #define LIS3MDL_REG_MAG_CTRL_REG4_OMZ_MEDIUM            (0x04)      // Medium-Performance Mode
    #define LIS3MDL_REG_MAG_CTRL_REG4_OMZ_HIGH              (0x08)      // High-Performance Mode
    #define LIS3MDL_REG_MAG_CTRL_REG4_OMZ_ULTRA_HIGH        (0x0C)      // Ultra-High-Performance Mode

    #define LIS3MDL_REG_MAG_CTRL_REG4_BLE_MASK              (0x02)      // Big/Little Endian Data Selection
    #define LIS3MDL_REG_MAG_CTRL_REG4_BLE_LSB               (0x00)      // Data LSB at Lower Address
    #define LIS3MDL_REG_MAG_CTRL_REG4_BLE_MSB               (0x02)      // Data MSB at Lower Address

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 5 DESCRIPTION
**************************************************************************/
    #define LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_MASK        (0x80)      // FAST READ Allows Reading the High Part of DATA OUT Only in Order to Increase Reading Efficiency
    #define LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_DISABLED    (0x00)      // FAST_READ Disabled
    #define LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_ENABLED     (0x80)      // FAST_READ Enabled

    #define LIS3MDL_REG_MAG_CTRL_REG5_BDU_MASK              (0x40)      // Block Data Update for Magnetic Data
    #define LIS3MDL_REG_MAG_CTRL_REG5_BDU_CONTINUOUS        (0x00)      // Continuous Update Mode
    #define LIS3MDL_REG_MAG_CTRL_REG5_BDU_NOT_UPDATED       (0x40)      // Output Registers not Updated Until MSB and LSB have been Read


typedef enum
{
    TEMP_SENSOR_DISABLED            = LIS3MDL_REG_MAG_CTRL_TEMP_EN_DISABLED,
    TEMP_SENSOR_ENABLED             = LIS3MDL_REG_MAG_CTRL_TEMP_EN_ENABLED
    
} lisTempSensor_t;

typedef enum
{
    XY_OPERATING_MODE_LOW           = LIS3MDL_REG_MAG_CTRL_REG1_OM_LOW,
    XY_OPERATING_MODE_MEDIUM        = LIS3MDL_REG_MAG_CTRL_REG1_OM_MEDIUM,
    XY_OPERATING_MODE_HIGH          = LIS3MDL_REG_MAG_CTRL_REG1_OM_HIGH,
    XY_OPERATING_MODE_ULTRA_HIGH    = LIS3MDL_REG_MAG_CTRL_REG1_OM_ULTRA_HIGH
    
} lisMagXYOpMode_t;

typedef enum
{
    MAG_DATARATE_0_625HZ            = LIS3MDL_REG_MAG_CTRL_REG1_DO_0_625,
    MAG_DATARATE_1_25HZ             = LIS3MDL_REG_MAG_CTRL_REG1_DO_1_25,
    MAG_DATARATE_2_5HZ              = LIS3MDL_REG_MAG_CTRL_REG1_DO_2_5,
    MAG_DATARATE_5HZ                = LIS3MDL_REG_MAG_CTRL_REG1_DO_5,
    MAG_DATARATE_10HZ               = LIS3MDL_REG_MAG_CTRL_REG1_DO_10,
    MAG_DATARATE_20HZ               = LIS3MDL_REG_MAG_CTRL_REG1_DO_20,
    MAG_DATARATE_40HZ               = LIS3MDL_REG_MAG_CTRL_REG1_DO_40,
    MAG_DATARATE_80HZ               = LIS3MDL_REG_MAG_CTRL_REG1_DO_80
    
} lisMagDataRate_t;

typedef enum
{
    FAST_ODR_DISABLED               = LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_DISABLED,
    FAST_ODR_ENABLED                = LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_ENABLED
    
} lisMagFastOddr_t;

typedef enum
{
    MAG_SELF_TEST_DISABLED          = LIS3MDL_REG_MAG_CTRL_REG1_ST_DISABLED,
    MAG_SELF_TEST_ENABLED           = LIS3MDL_REG_MAG_CTRL_REG1_ST_ENABLED
    
} lisMagSelfTest_t;


typedef enum
{
    MAG_SCALE_4G                    = LIS3MDL_REG_MAG_CTRL_REG2_FS_4G,
    MAG_SCALE_8G                    = LIS3MDL_REG_MAG_CTRL_REG2_FS_8G,
    MAG_SCALE_12G                   = LIS3MDL_REG_MAG_CTRL_REG2_FS_12G,
    MAG_SCALE_16G                   = LIS3MDL_REG_MAG_CTRL_REG2_FS_16G
    
} lisMagScale_t;

typedef enum
{
    MAG_REBOOT_NORMAL               = LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_NORMAL,
    MAG_REBOOT_REBOOT               = LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_REBOOT
    
} lisMagReboot_t;

typedef enum
{
    MAG_RESET_DEFAULT               = LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_DEFAULT,
    MAG_RESET_RESET                 = LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_RESET
    
} lisMagReset_t;

typedef enum
{
    MAG_LOW_POWER_0_625             = LIS3MDL_REG_MAG_CTRL_REG3_LP_0,
    MAG_LOW_POWER_DO                = LIS3MDL_REG_MAG_CTRL_REG3_LP_1
    
} lisMagLowPower_t;

typedef enum
{
    MAG_SPI_WIRE_4                  = LIS3MDL_REG_MAG_CTRL_REG3_SIM_4WIRE,
    MAG_SPI_WIRE_3                  = LIS3MDL_REG_MAG_CTRL_REG3_SIM_3WIRE
    
} lisMagSPIWire_t;

typedef enum
{
    MAG_MODE_CONTINUOUS             = LIS3MDL_REG_MAG_CTRL_REG3_MD_CONTINUOUS,
    MAG_MODE_SINGLE                 = LIS3MDL_REG_MAG_CTRL_REG3_MD_SINGLE,
    MAG_MODE_POWER_DOWN             = LIS3MDL_REG_MAG_CTRL_REG3_MD_POWER_DOWN
    
} lisMagMode_t;

typedef enum
{
    Z_OPERATING_MODE_LOW            = LIS3MDL_REG_MAG_CTRL_REG4_OMZ_LOW,
    Z_OPERATING_MODE_MEDIUM         = LIS3MDL_REG_MAG_CTRL_REG4_OMZ_MEDIUM,
    Z_OPERATING_MODE_HIGH           = LIS3MDL_REG_MAG_CTRL_REG4_OMZ_HIGH,
    Z_OPERATING_MODE_ULTRA_HIGH     = LIS3MDL_REG_MAG_CTRL_REG4_OMZ_ULTRA_HIGH
    
} lisMagZOpMode_t;

typedef enum
{
    ENDIAN_DATA_LSB                 = LIS3MDL_REG_MAG_CTRL_REG4_BLE_LSB,
    ENDIAN_DATA_MSB                 = LIS3MDL_REG_MAG_CTRL_REG4_BLE_MSB
    
} lisMagEndianData_t;

typedef enum
{
    FAST_READ_DISABLED              = LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_DISABLED,
    FAST_READ_ENABLED               = LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_ENABLED
    
} lisMagFastRead_t;

typedef enum
{
    MAG_CONTINUOUS                  = LIS3MDL_REG_MAG_CTRL_REG5_BDU_CONTINUOUS,
    MAG_NOT_UPDATED                 = LIS3MDL_REG_MAG_CTRL_REG5_BDU_NOT_UPDATED
    
} lisMagBlockData_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} lisMagData_t;


class LIS3MDL
{
    protected:
        // Instance-specific properties
        uint8_t lis_conversionDelay;
        lisTempSensor_t lis_tempsensor;
        lisMagXYOpMode_t lis_magxyopmode;
        lisMagDataRate_t lis_magdatarate;
        lisMagFastOddr_t lis_magfastoddr;
        lisMagSelfTest_t lis_magselftest;
        lisMagScale_t lis_magscale;
        lisMagReboot_t lis_magreboot;
        lisMagReset_t lis_magreset;
        lisMagLowPower_t lis_maglowpower;
        lisMagSPIWire_t lis_magspiwire;
        lisMagMode_t lis_magmode;
        lisMagZOpMode_t lis_magzopmode;
        lisMagEndianData_t lis_magendiandata;
        lisMagFastRead_t lis_magfastread;
        lisMagBlockData_t lis_magblockdata;
    
    public:
        uint8_t lis_i2cAddress;
        lisMagData_t lis_magData;
        void getAddr_LIS3MDL(uint8_t i2cAddress);
        bool begin(void);
        void setUpMagnetometer(void);
        void Measure_Magnetometer(void);
        void setTempSensor(lisTempSensor_t tempsensor);
        lisTempSensor_t getTempSensor(void);
        void setMagXYOpMode(lisMagXYOpMode_t magxyopmode);
        lisMagXYOpMode_t getMagXYOpMode(void);
        void setMagDataRate(lisMagDataRate_t magdatarate);
        lisMagDataRate_t getMagDataRate(void);
        void setMagFastOddr(lisMagFastOddr_t magfastoddr);
        lisMagFastOddr_t getMagFastOddr(void);
        void setMagSelfTest(lisMagSelfTest_t magselftest);
        lisMagSelfTest_t getMagSelfTest(void);
        void setMagScale(lisMagScale_t magscale);
        lisMagScale_t getMagScale(void);
        void setMagReboot(lisMagReboot_t magreboot);
        lisMagReboot_t getMagReboot(void);
        void setMagReset(lisMagReset_t magreset);
        lisMagReset_t getMagReset(void);
        void setMagLowPower(lisMagLowPower_t maglowpower);
        lisMagLowPower_t getMagLowPower(void);
        void setMagSPIWire(lisMagSPIWire_t magspiwire);
        lisMagSPIWire_t getMagSPIWire(void);
        void setMagMode(lisMagMode_t magmode);
        lisMagMode_t getMagMode(void);
        void setMagZOpMode(lisMagZOpMode_t magzopmode);
        lisMagZOpMode_t getMagZOpMode(void);
        void setMagEndianData(lisMagEndianData_t magendiandata);
        lisMagEndianData_t getMagEndianData(void);
        void setMagFastRead(lisMagFastRead_t magfastread);
        lisMagFastRead_t getMagFastRead(void);
        void setMagBlockData(lisMagBlockData_t magblockdata);
        lisMagBlockData_t getMagBlockData(void);
    
    private:
};
