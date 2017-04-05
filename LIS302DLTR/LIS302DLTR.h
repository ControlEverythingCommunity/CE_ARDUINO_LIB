/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS302DLTR
        This code is designed to work with the LIS302DLTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS302DLTR_I2CS#tabs-0-product_tabset-2
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
    #define LIS302DLTR_DEFAULT_ADDRESS                          (0x1C)      // 0011100b
    #define LIS302DLTR_ADDRESS_UPDATED                          (0x1D)      // 0011101b
    #define LIS302DLTR_ACCEL_DEV_ID                             (0x3B)      // 00111011

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define LIS302DLTR_CONVERSIONDELAY                          (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define LIS302DLTR_REG_ACCEL_WHO_AM_I                       (0x0F)      // Accelerometer Who Am I
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1                      (0x20)      // Accelerometer Control Register 1
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2                      (0x21)      // Accelerometer Control Register 2
    #define LIS302DLTR_REG_ACCEL_CTRL_REG3                      (0x22)      // Accelerometer Control Register 3
    #define LIS302DLTR_REG_ACCEL_HP_FILTER_RESET                (0x23)      // Dummy Register
    #define LIS302DLTR_REG_ACCEL_STATUS                         (0x27)      // Status Register
    #define LIS302DLTR_REG_ACCEL_OUT_X                          (0x29)      // X-Axis Acceleration Data Register
    #define LIS302DLTR_REG_ACCEL_OUT_Y                          (0x2B)      // Y-Axis Acceleration Data Register
    #define LIS302DLTR_REG_ACCEL_OUT_Z                          (0x2D)      // Z-Axis Acceleration Data Register
    #define LIS302DLTR_REG_ACCEL_FF_WU_CFG1                     (0x30)      // Free-Fall/Wake-Up Interrupt Configuration Register 1
    #define LIS302DLTR_REG_ACCEL_FF_WU_SRC1                     (0x31)      // Free-Fall/Wake-Up Interrupt Source Register 1
    #define LIS302DLTR_REG_ACCEL_FF_WU_THS1                     (0x32)      // Free-Fall/Wake-Up Interrupt Threshold Register 1
    #define LIS302DLTR_REG_ACCEL_FF_WU_DURATION1                (0x33)      // Free-Fall/Wake-Up Interrupt Duration Register 1
    #define LIS302DLTR_REG_ACCEL_FF_WU_CFG2                     (0x34)      // Free-Fall/Wake-Up Interrupt Configuration Register 2
    #define LIS302DLTR_REG_ACCEL_FF_WU_SRC2                     (0x35)      // Free-Fall/Wake-Up Interrupt Source Register 2
    #define LIS302DLTR_REG_ACCEL_FF_WU_THS2                     (0x36)      // Free-Fall/Wake-Up Interrupt Threshold Register 2
    #define LIS302DLTR_REG_ACCEL_FF_WU_DURATION2                (0x37)      // Free-Fall/Wake-Up Interrupt Duration Register 2
    #define LIS302DLTR_REG_ACCELMAG_CLICK_CFG                   (0x38)      // Interrupt Click Recognition Register
    #define LIS302DLTR_REG_ACCELMAG_CLICK_SRC                   (0x39)      // Interrupt Click Source Register
    #define LIS302DLTR_REG_ACCELMAG_CLICK_THS                   (0x3A)      // Interrupt Click Threshold Register
    #define LIS302DLTR_REG_ACCELMAG_TIME_LIMIT                  (0x3B)      // Click Time Limit Register
    #define LIS302DLTR_REG_ACCELMAG_TIME_LAT                    (0x3C)      // Click Time Latency Register
    #define LIS302DLTR_REG_ACCELMAG_TIME_WIN                    (0x3D)      // Click Time Window Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AODR_MASK           (0x80)      // Output Data Rate Selection
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AODR_100            (0x00)      // ODR: 100 Hz
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AODR_400            (0x80)      // ODR: 400 Hz

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_MASK             (0x40)      // Power Down Control
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_PWRDWN           (0x00)      // Power-Down Mode
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_ACTIVE           (0x40)      // Active Mode

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_FS_MASK             (0x20)      // Full-Scale Selection
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_FS_2G               (0x00)      // +/- 2G
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_FS_8G               (0x20)      // +/- 8G

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_MASK            (0x10)      // Self-Test Enable
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_NORMAL          (0x00)      // Self Test Normal
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_ENABLED         (0x10)      // Self-Test P Enabled

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_MASK            (0x08)      // Self-Test Enable
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_NORMAL          (0x00)      // Self Test Normal
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_ENABLED         (0x08)      // Self-Test M Enabled

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_MASK           (0x04)      // Acceleration Z-Axis Enable
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE        (0x00)      // Acceleration Z-Axis Disabled
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE         (0x04)      // Acceleration Z-Axis Enabled

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_MASK           (0x02)      // Acceleration Y-Axis Enable
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE        (0x00)      // Acceleration Y-Axis Disabled
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE         (0x02)      // Acceleration Y-Axis Enabled

    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_MASK           (0x01)      // Acceleration X-Axis Enable
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE        (0x00)      // Acceleration X-Axis Disabled
    #define LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE         (0x01)      // Acceleration X-Axis Enabled

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_MASK           (0x80)      // SPI Serial Interface Mode Selection
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_4WIRE          (0x00)      // 4-Wire Interface
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_3WIRE          (0x80)      // 3-Wire Interface

    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_MASK          (0x40)      // Reboot Memory Content
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_NORMAL        (0x00)      // Normal Mode
    #define LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_REBOOT        (0x40)      // Reboot Memory Content


typedef enum
{
    ACCEL_DATARATE_100HZ            = LIS302DLTR_REG_ACCEL_CTRL_REG1_AODR_100,
    ACCEL_DATARATE_400HZ            = LIS302DLTR_REG_ACCEL_CTRL_REG1_AODR_400
    
} lisAccelDataRate_t;

typedef enum
{
    POWER_MODE_DOWN                 = LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_PWRDWN,
    POWER_MODE_ACTIVE               = LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_ACTIVE
    
} lisAccelPowerMode_t;

typedef enum
{
    ACCEL_RANGE_2G                  = LIS302DLTR_REG_ACCEL_CTRL_REG1_FS_2G,
    ACCEL_RANGE_8G                  = LIS302DLTR_REG_ACCEL_CTRL_REG1_FS_8G
    
} lisAccelRange_t;

typedef enum
{
    ACCEL_SELF_TEST_P_NORMAL        = LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_NORMAL,
    ACCEL_SELF_TEST_P_ENABLED       = LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_ENABLED
    
} lisAccelSelfTestP_t;

typedef enum
{
    ACCEL_SELF_TEST_M_NORMAL        = LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_NORMAL,
    ACCEL_SELF_TEST_M_ENABLED       = LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_ENABLED
    
} lisAccelSelfTestM_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED           = LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_DISABLE,
    ACCEL_Z_AXIS_ENABLED            = LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE
    
} lisAccelZen_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED           = LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_DISABLE,
    ACCEL_Y_AXIS_ENABLED            = LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE
    
} lisAccelYen_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED           = LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_DISABLE,
    ACCEL_X_AXIS_ENABLED            = LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE
    
} lisAccelXen_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                = LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_4WIRE,
    ACCEL_SPI_WIRE_3                = LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_3WIRE
    
} lisAccelSPIWire_t;

typedef enum
{
    ACCEL_BOOT_NORMAL               = LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_NORMAL,
    ACCEL_BOOT_REBOOT               = LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_REBOOT
    
} lisAccelReboot_t;

typedef struct
{
    int8_t X;
    int8_t Y;
    int8_t Z;
} lisAccelData_t;


class LIS302DLTR
{
    protected:
        // Instance-specific properties
        uint8_t lis_conversionDelay;
        lisAccelDataRate_t lis_acceldatarate;
        lisAccelPowerMode_t lis_accelpowermode;
        lisAccelRange_t lis_accelrange;
        lisAccelSelfTestP_t lis_accelselftestp;
        lisAccelSelfTestM_t lis_accelselftestm;
        lisAccelZen_t lis_accelzen;
        lisAccelYen_t lis_accelyen;
        lisAccelXen_t lis_accelxen;
        lisAccelSPIWire_t lis_accelspiwire;
        lisAccelReboot_t lis_accelreboot;
    
    public:
        uint8_t lis_i2cAddress;
        lisAccelData_t lis_accelData;
        void getAddr_LIS302DLTR(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelDataRate(lisAccelDataRate_t acceldatarate);
        lisAccelDataRate_t getAccelDataRate(void);
        void setAccelPowerMode(lisAccelPowerMode_t accelpowermode);
        lisAccelPowerMode_t getAccelPowerMode(void);
        void setAccelRange(lisAccelRange_t accelrange);
        lisAccelRange_t getAccelRange(void);
        void setAccelSelfTestP(lisAccelSelfTestP_t accelselftestp);
        lisAccelSelfTestP_t getAccelSelfTestP(void);
        void setAccelSelfTestM(lisAccelSelfTestM_t accelselftestm);
        lisAccelSelfTestM_t getAccelSelfTestM(void);
        void setAccelZen(lisAccelZen_t accelzen);
        lisAccelZen_t getAccelZen(void);
        void setAccelYen(lisAccelYen_t accelyen);
        lisAccelYen_t getAccelYen(void);
        void setAccelXen(lisAccelXen_t accelxen);
        lisAccelXen_t getAccelXen(void);
        void setAccelSPIWire(lisAccelSPIWire_t accelspiwire);
        lisAccelSPIWire_t getAccelSPIWire(void);
        void setAccelReboot(lisAccelReboot_t accelreboot);
        lisAccelReboot_t getAccelReboot(void);
    
    private:
};
