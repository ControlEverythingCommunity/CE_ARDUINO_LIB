/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KXTF9-4100
        This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
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
    #define KXTF9_4100_DEFAULT_ADDRESS                      (0x0F)
    #define KXTF9_4100_DEFAULT_DEV_ID                       (0x2A)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define KXTF9_4100_CONVERSIONDELAY                      (300)

/**************************************************************************
    ACCELNETOMETER REGISTERS
**************************************************************************/
    #define KXTF9_4100_REG_ACCEL_XOUT_HPF_L                 (0x00)      // X-Axis High-Pass Filtered Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_XOUT_HPF_H                 (0x01)      // X-Axis High-Pass Filtered Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_YOUT_HPF_L                 (0x02)      // Y-Axis High-Pass Filtered Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_YOUT_HPF_H                 (0x03)      // Y-Axis High-Pass Filtered Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_ZOUT_HPF_L                 (0x04)      // Z-Axis High-Pass Filtered Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_ZOUT_HPF_H                 (0x05)      // Z-Axis High-Pass Filtered Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_XOUT_L                     (0x06)      // X-Axis Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_XOUT_H                     (0x07)      // X-Axis Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_YOUT_L                     (0x08)      // Y-Axis Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_YOUT_H                     (0x09)      // Y-Axis Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_ZOUT_L                     (0x0A)      // Z-Axis Accelerometer Output Least Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_ZOUT_H                     (0x0B)      // Z-Axis Accelerometer Output Most Significant Byte Register
    #define KXTF9_4100_REG_ACCEL_DCST_RESP                  (0x0C)      // Integrated Circuit Functionality Register
    #define KXTF9_4100_REG_ACCEL_WHO_AM_I                   (0x0F)      // Supplier Recognition Register
    #define KXTF9_4100_REG_ACCEL_TILT_POS_CUR               (0x10)      // Current Tilt Position Register
    #define KXTF9_4100_REG_ACCEL_TILT_POS_PRE               (0x11)      // Previous Tilt Position Register
    #define KXTF9_4100_REG_ACCEL_INT_SRC_REG1               (0x15)      // Latched Interrupt Source Information Register 1
    #define KXTF9_4100_REG_ACCEL_INT_SRC_REG2               (0x16)      // Latched Interrupt Source Information Register 2
    #define KXTF9_4100_REG_ACCEL_STATUS_REG                 (0x18)      // Interrupt Status Register
    #define KXTF9_4100_REG_ACCEL_INT_REL                    (0x1A)      // Interrupt Clearance Register
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1                  (0x1B)      // Main Feature Control Register
    #define KXTF9_4100_REG_ACCEL_CTRL_REG2                  (0x1C)      // Tilt Position State Enabling Control Register
    #define KXTF9_4100_REG_ACCEL_CTRL_REG3                  (0x1D)      // More Feature Set Control Register
    #define KXTF9_4100_REG_ACCEL_INT_CTRL_REG1              (0x1E)      // Physical Interrupt Pin Control Setting Register
    #define KXTF9_4100_REG_ACCEL_INT_CTRL_REG2              (0x1F)      // Motion Detection Axis Enabling Register
    #define KXTF9_4100_REG_ACCEL_INT_CTRL_REG3              (0x20)      // Motion Detection Axis Enabling Register
    #define KXTF9_4100_REG_ACCEL_DATA_CTRL_REG              (0x21)      // Acceleration Outputs Configuration Register
    #define KXTF9_4100_REG_ACCEL_TILT_TIMER                 (0x28)      // Tilt Position State Timer Count Register
    #define KXTF9_4100_REG_ACCEL_WUF_TIMER                  (0x29)      // Motion Detection Timer Count Register
    #define KXTF9_4100_REG_ACCEL_TDT_TIMER                  (0x2B)      // Double Tap Event Detection Register
    #define KXTF9_4100_REG_ACCEL_TDT_H_THRESH               (0x2C)      // 9-bit Jerk High Threshold Tap Detection Register
    #define KXTF9_4100_REG_ACCEL_TDT_L_THRESH               (0x2D)      // 7-bit Jerk Low Threshold Tap Detection Register
    #define KXTF9_4100_REG_ACCEL_TDT_TAP_TIMER              (0x2E)      // Any Tap Detection Counter Information Register
    #define KXTF9_4100_REG_ACCEL_TDT_TOTAL_TIMER            (0x2F)      // Double Tap Detection Counter Information Register
    #define KXTF9_4100_REG_ACCEL_TDT_LATENCY_TIMER          (0x30)      // Tap Detection Counter Information Register
    #define KXTF9_4100_REG_ACCEL_TDT_WINDOW_TIMER           (0x31)      // Single/Double Tap Detection Counter Information Register
    #define KXTF9_4100_REG_ACCEL_SELF_TEST                  (0x3A)      // Self-Test Register
    #define KXTF9_4100_REG_ACCEL_WUF_THRESH                 (0x5A)      // Acceleration Threshold Register
    #define KXTF9_4100_REG_ACCEL_TILT_ANGLE                 (0x5C)      // Tilt Angle Register
    #define KXTF9_4100_REG_ACCEL_HYST_SET                   (0x5F)      // Hysteresis Register

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_MASK         (0x80)      // Operating Mode of the KXTF9
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_STANDBY      (0x00)      // Stand-by Mode
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_OPERATE      (0x80)      // Operating Mode

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_MASK         (0x40)      // Performance Mode of the KXTF9
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_0            (0x00)      // Low Current, 8-bit Valid
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_1            (0x40)      // High Current, 12-bit Valid

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_MASK       (0x20)      // Reporting of the Availability of New Acceleration Data on the Interrupt
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_NOT        (0x00)      // Availability of New Acceleration Data Not Reflected on Interrupt Pin (7)
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_NEW        (0x20)      // Availability of New Acceleration Data Reflected on Interrupt Pin (7)

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_MASK        (0x18)      // Acceleration Range of the Accelerometer Outputs
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_2G          (0x00)      // Range: +/-2g
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_4G          (0x08)      // Range: +/-4g
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_8G          (0x10)      // Range: +/-8g

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_MASK        (0x04)      // Directional TapTM Function
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_DISABLE     (0x00)      // Directional TapTM Function Disable
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_ENABLE      (0x04)      // Directional TapTM Function Enable

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_MASK        (0x02)      // Wake Up (Motion Detect) Function
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_DISABLE     (0x00)      // Wake Up (Motion Detect) Function Disable
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_ENABLE      (0x02)      // Wake Up (Motion Detect) Function Enable

    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_MASK         (0x01)      // Tilt Position Function
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_DISABLE      (0x00)      // Tilt Position Function Disable
    #define KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_ENABLE       (0x01)      // Tilt Position Function Enable


typedef enum
{
    STANDBY_MODE                = KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_STANDBY,
    OPERATE_MODE                = KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_OPERATE
    
} kxtfAccelOpMode_t;

typedef enum
{
    RES_LOWCURRENT_8BIT         = KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_0,
    RES_HIGHCURRENT_12BIT       = KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_1
    
} kxtfAccelPerformMode_t;

typedef enum
{
    ACCELE_DATA_NOT             = KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_NOT,
    ACCELE_DATA_NEW             = KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_NEW
    
} kxtfAccelData_t;

typedef enum
{
    ACCEL_RANGE_2G               = KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_2G,
    ACCEL_RANGE_4G               = KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_4G,
    ACCEL_RANGE_8G               = KXTF9_4100_REG_ACCEL_CTRL_REG1_GSEL_8G
    
} kxtfAccelRange_t;

typedef enum
{
    TAPTM_DISABLE               = KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_DISABLE,
    TAPTM_ENABLE                = KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_ENABLE
    
} kxtfAccelTapTM_t;

typedef enum
{
    WAKEUP_DISABLE              = KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_DISABLE,
    WAKEUP_ENABLE               = KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_ENABLE
    
} kxtfAccelWakeUp_t;

typedef enum
{
    TILT_DISABLE                = KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_DISABLE,
    TILT_ENABLE                 = KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_ENABLE
    
} kxtfAccelTilt_t;


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} kxtfSensorData_t;


class KXTF9_4100
{
    protected:
        // Instance-specific properties
        uint8_t kxtf_conversionDelay;
        kxtfAccelOpMode_t kxtf_accelopmode;
        kxtfAccelPerformMode_t kxtf_accelperformmode;
        kxtfAccelData_t kxtf_acceldata;
        kxtfAccelRange_t kxtf_accelrange;
        kxtfAccelTapTM_t kxtf_acceltaptm;
        kxtfAccelWakeUp_t kxtf_accelwakeup;
        kxtfAccelTilt_t kxtf_acceltilt;
    
    public:
        uint8_t kxtf_i2cAddress;
        kxtfSensorData_t kxtf_accelData;
        void getAddr_KXTF9_4100(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelOpMode(kxtfAccelOpMode_t accelopmode);
        kxtfAccelOpMode_t getAccelOpMode(void);
        void setAccelPerformMode(kxtfAccelPerformMode_t accelperformmode);
        kxtfAccelPerformMode_t getAccelPerformMode(void);
        void setAccelData(kxtfAccelData_t acceldata);
        kxtfAccelData_t getAccelData(void);
        void setAccelRange(kxtfAccelRange_t accelrange);
        kxtfAccelRange_t getAccelRange(void);
        void setAccelTapTM(kxtfAccelTapTM_t acceltaptm);
        kxtfAccelTapTM_t getAccelTapTM(void);
        void setAccelWakeUp(kxtfAccelWakeUp_t accelwakeup);
        kxtfAccelWakeUp_t getAccelWakeUp(void);
        void setAccelTilt(kxtfAccelTilt_t acceltilt);
        kxtfAccelTilt_t getAccelTilt(void);

    private:
};
