/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6226XU
        This code is designed to work with the MXC6226XU_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=MXC6226XU_I2CS#tabs-0-product_tabset-2
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
    #define MXC6226XU_DEFAULT_ADDRESS                       (0x16)
    #define MXC6226XU_CHIP_ID                               (0x26)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MXC6226XU_CONVERSIONDELAY                       (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define MXC6226XU_REG_ACCEL_OUT_X                       (0x00)      // X-Axis Acceleration Output Register
    #define MXC6226XU_REG_ACCEL_OUT_Y                       (0x01)      // Y-Axis Acceleration Output Register
    #define MXC6226XU_REG_ACCEL_STATUS                      (0x02)      // Orientation and Shake Status Register
    #define MXC6226XU_REG_ACCEL_DETECTION                   (0x03)      // Power Down, Orientation and Shake Detection Parameters Register
    #define MXC6226XU_REG_ACCEL_CHIP_ID                     (0x04)      // Chip ID of MXC6226XU

/**************************************************************************
    DETECTION REGISTER DESCRIPTION
**************************************************************************/
    #define MXC6226XU_REG_ACCEL_DETECTION_PD_MASK           (0x80)      // Power Down DTOS
    #define MXC6226XU_REG_ACCEL_DETECTION_PD_DISABLED       (0x00)      // Disables the Power Down of the DTOS to a Non-Functional Low Power State with a Maximum Current Drain of 1 uA
    #define MXC6226XU_REG_ACCEL_DETECTION_PD_ENABLED        (0x80)      // Enables the Power Down of the DTOS to a Non-Functional Low Power State with a Maximum Current Drain of 1 uA

    #define MXC6226XU_REG_ACCEL_DETECTION_SHM_MASK          (0x40)      // Shake Mode Bit
    #define MXC6226XU_REG_ACCEL_DETECTION_SHM_0             (0x00)      // For a Shake to be Detected, and Written to SH[1:0] in the STATUS Register, the Second Shake Event Must Break the Threshold Set by SHTH[1:0] with the Opposite Sign of the First Shake Event, Within the Time Window Set by SHC[1:0]
    #define MXC6226XU_REG_ACCEL_DETECTION_SHM_1             (0x40)      // The Second Shake Event must just have the Opposite Sign of the First Shake Event, Within the Time Window Set by SHC[1:0]

    #define MXC6226XU_REG_ACCEL_DETECTION_SHTH_MASK         (0x30)      // Sets the Shake Threshold that the Perpendicular Acceleration must Exceed to Trigger the First Shake Event
    #define MXC6226XU_REG_ACCEL_DETECTION_SHTH_0_5          (0x00)      // 0.5 g
    #define MXC6226XU_REG_ACCEL_DETECTION_SHTH_1            (0x10)      // 1.0 g
    #define MXC6226XU_REG_ACCEL_DETECTION_SHTH_1_5          (0x20)      // 1.5 g
    #define MXC6226XU_REG_ACCEL_DETECTION_SHTH_2            (0x30)      // 2.0 g

    #define MXC6226XU_REG_ACCEL_DETECTION_SHC_MASK          (0x0C)      // Sets the Shake Events Time Window, Which Determines the Time Window Allowed Between the First Shake Event (Perpendicular Acceleration Exceeding the Threshold Set by SHTH[1:0]) and the Second Shake Event (Acceleration Breaking the Threshold with Opposite Sign, SHM = 0, or just Reversing Sign, SHM = 1)
    #define MXC6226XU_REG_ACCEL_DETECTION_SHC_80            (0x00)      // 80 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_SHC_160           (0x04)      // 160 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_SHC_320           (0x08)      // 320 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_SHC_640           (0x0C)      // 640 ms

    #define MXC6226XU_REG_ACCEL_DETECTION_ORC_MASK          (0x03)      // Sets the Orientation Hysteresis Time Period, Which is the Time Period of Consecutive Valid New Instantaneous Orientation Measurements that Must be Made Before a New Orientation Value is Written into Bits OR[1:0] in the STATUS Register
    #define MXC6226XU_REG_ACCEL_DETECTION_ORC_160           (0x00)      // 160 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_ORC_320           (0x01)      // 320 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_ORC_640           (0x02)      // 640 ms
    #define MXC6226XU_REG_ACCEL_DETECTION_ORC_1280          (0x03)      // 1280 ms


typedef enum
{
    DTOS_DISABLED                   = MXC6226XU_REG_ACCEL_DETECTION_PD_DISABLED,
    DTOS_ENABLED                    = MXC6226XU_REG_ACCEL_DETECTION_PD_ENABLED
    
} mxcDTOSEnable_t;

typedef enum
{
    SHAKE_MODE_0                    = MXC6226XU_REG_ACCEL_DETECTION_SHM_0,
    SHAKE_MODE_1                    = MXC6226XU_REG_ACCEL_DETECTION_SHM_1
    
} mxcShakeMode_t;

typedef enum
{
    SHAKE_THRESHOLD_0_5             = MXC6226XU_REG_ACCEL_DETECTION_SHTH_0_5,
    SHAKE_THRESHOLD_1               = MXC6226XU_REG_ACCEL_DETECTION_SHTH_1,
    SHAKE_THRESHOLD_1_5             = MXC6226XU_REG_ACCEL_DETECTION_SHTH_1_5,
    SHAKE_THRESHOLD_2               = MXC6226XU_REG_ACCEL_DETECTION_SHTH_2
    
} mxcShakeThreshold_t;

typedef enum
{
    SHAKE_EVENTS_TIME_80            = MXC6226XU_REG_ACCEL_DETECTION_SHC_80,
    SHAKE_EVENTS_TIME_160           = MXC6226XU_REG_ACCEL_DETECTION_SHC_160,
    SHAKE_EVENTS_TIME_320           = MXC6226XU_REG_ACCEL_DETECTION_SHC_320,
    SHAKE_EVENTS_TIME_640           = MXC6226XU_REG_ACCEL_DETECTION_SHC_640,
    
} mxcShakEvenTime_t;

typedef enum
{
    ORIENTATION_TIME_160            = MXC6226XU_REG_ACCEL_DETECTION_ORC_160,
    ORIENTATION_TIME_320            = MXC6226XU_REG_ACCEL_DETECTION_ORC_320,
    ORIENTATION_TIME_1640           = MXC6226XU_REG_ACCEL_DETECTION_ORC_640,
    ORIENTATION_TIME_1280           = MXC6226XU_REG_ACCEL_DETECTION_ORC_1280,
    
} mxcOrientationTime_t;



typedef struct
{
    int8_t X;
    int8_t Y;
    
} mxcAccelData_t;


class MXC6226XU
{
    protected:
        // Instance-specific properties
        uint8_t mxc_conversionDelay;
        mxcDTOSEnable_t mxc_dtosenable;
        mxcShakeMode_t mxc_shakemode;
        mxcShakeThreshold_t mxc_shakethreshold;
        mxcShakEvenTime_t mxc_shakeventime;
        mxcOrientationTime_t mxc_orientationtime;
    
    public:
        uint8_t mxc_i2cAddress;
        mxcAccelData_t mxc_accelData;
        void getAddr_MXC6226XU(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setDTOSEnable(mxcDTOSEnable_t dtosenable);
        mxcDTOSEnable_t getDTOSEnable(void);
        void setShakeMode(mxcShakeMode_t shakemode);
        mxcShakeMode_t getShakeMode(void);
        void setShakeThreshold(mxcShakeThreshold_t shakethreshold);
        mxcShakeThreshold_t getShakeThreshold(void);
        void setShakEvenTime(mxcShakEvenTime_t shakeventime);
        mxcShakEvenTime_t getShakEvenTime(void);
        void setOrientationTime(mxcOrientationTime_t orientationtime);
        mxcOrientationTime_t getOrientationTime(void);
    
    private:
};
