/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA8452Q
        This code is designed to work with the MMA8452Q_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
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
    #define MMA8452Q_DEFAULT_ADDRESS_ACCEL                      (0x1C)
    #define MMA8452Q_ADDRESS_ACCEL_UPDATED                      (0x1D)
    #define MMA8452Q_DEFAULT_DEV_ID                             (0x2A)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MMA8452Q_CONVERSIONDELAY                            (100)

/**************************************************************************
    ACCELNETOMETER REGISTERS
**************************************************************************/
    #define MMA8452Q_REG_ACCEL_STATUS                           (0x00)      // Status Register
    #define MMA8452Q_REG_ACCEL_OUT_X_MSB                        (0x01)      // 8 MSBs of 12-bit Sample X-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_OUT_X_LSB                        (0x02)      // 4 LSBs of 12-bit Sample X-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_OUT_Y_MSB                        (0x03)      // 8 MSBs of 12-bit Sample Y-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_OUT_Y_LSB                        (0x04)      // 4 LSBs of 12-bit Sample Y-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_OUT_Z_MSB                        (0x05)      // 8 MSBs of 12-bit Sample Z-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_OUT_Z_LSB                        (0x06)      // 4 LSBs of 12-bit Sample Z-Axis Acceleration Data Register
    #define MMA8452Q_REG_ACCEL_SYSMOD                           (0x0B)      // Current System Mode Register
    #define MMA8452Q_REG_ACCEL_INT_SOURCE                       (0x0C)      // Interrupt Status Register
    #define MMA8452Q_REG_ACCEL_WHO_AM_I                         (0x0D)      // Device ID Number Register
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG                     (0x0E)      // HPF Data Out and Dynamic Range Settings Register
    #define MMA8452Q_REG_ACCEL_HP_FILTER_CUTOFF                 (0x0F)      // Cutoff Frequency is Set to 16 Hz @ 800 Hz Register
    #define MMA8452Q_REG_ACCEL_PL_STATUS                        (0x10)      // Landscape/Portrait Orientation Status Register
    #define MMA8452Q_REG_ACCEL_PL_CFG                           (0x11)      // Landscape/Portrait Configuration Register
    #define MMA8452Q_REG_ACCEL_PL_COUNT                         (0x12)      // Landscape/Portrait Debounce Counter Register
    #define MMA8452Q_REG_ACCEL_PL_BF_ZCOMP                      (0x13)      // Back/Front, Z-Lock Trip Threshold Register
    #define MMA8452Q_REG_ACCEL_P_L_THS_REG                      (0x14)      // Portrait to Landscape Trip Angle is 29° Register
    #define MMA8452Q_REG_ACCEL_FF_MT_CFG                        (0x15)      // Freefall/Motion Functional Block Configuration Register
    #define MMA8452Q_REG_ACCEL_FF_MT_SRC                        (0x16)      // Freefall/Motion Event Source Register
    #define MMA8452Q_REG_ACCEL_FF_MT_THS                        (0x17)      // Freefall/Motion Threshold Register
    #define MMA8452Q_REG_ACCEL_FF_MT_COUNT                      (0x18)      // Freefall/Motion Debounce Counter Register
    #define MMA8452Q_REG_ACCEL_TRANSIENT_CFG                    (0x1D)      // Transient Functional Block Configuration Register
    #define MMA8452Q_REG_ACCEL_TRANSIENT_SRC                    (0x1E)      // Transient Event Status Register
    #define MMA8452Q_REG_ACCEL_TRANSIENT_THS                    (0x1F)      // Transient Event Threshold Register
    #define MMA8452Q_REG_ACCEL_TRANSIENT_COUNT                  (0x20)      // Transient Debounce Counter Register
    #define MMA8452Q_REG_ACCEL_PULSE_CFG                        (0x21)      // ELE, Double_XYZ or Single_XYZ Register
    #define MMA8452Q_REG_ACCEL_PULSE_SRC                        (0x22)      // EA, Double_XYZ or Single_XYZ Register
    #define MMA8452Q_REG_ACCEL_PULSE_THSX                       (0x23)      // X Pulse Threshold Register
    #define MMA8452Q_REG_ACCEL_PULSE_THSY                       (0x24)      // Y Pulse Threshold Register
    #define MMA8452Q_REG_ACCEL_PULSE_THSZ                       (0x25)      // Z Pulse Threshold Register
    #define MMA8452Q_REG_ACCEL_PULSE_TMLT                       (0x26)      // Time Limit for Pulse Register
    #define MMA8452Q_REG_ACCEL_PULSE_LTCY                       (0x27)      // Latency Time for 2nd Pulse Register
    #define MMA8452Q_REG_ACCEL_PULSE_WIND                       (0x28)      // Window Time for 2nd Pulse Register
    #define MMA8452Q_REG_ACCEL_ASLP_COUNT                       (0x29)      // Counter Setting for Auto-Sleep Register
    #define MMA8452Q_REG_ACCEL_CTRL_REG1                        (0x2A)      // Data Rate, Active Mode Register
    #define MMA8452Q_REG_ACCEL_CTRL_REG2                        (0x2B)      // Sleep Enable, OS Modes, RST, ST Register
    #define MMA8452Q_REG_ACCEL_CTRL_REG3                        (0x2C)      // Wake from Sleep, IPOL, PP_OD Register
    #define MMA8452Q_REG_ACCEL_CTRL_REG4                        (0x2D)      // Interrupt Enable Register
    #define MMA8452Q_REG_ACCEL_CTRL_REG5                        (0x2E)      // Interrupt Pin (INT1/INT2) Map Register
    #define MMA8452Q_REG_ACCEL_OFF_X                            (0x2F)      // X-Axis Offset Adjust Register
    #define MMA8452Q_REG_ACCEL_OFF_Y                            (0x30)      // Y-Axis Offset Adjust Register
    #define MMA8452Q_REG_ACCEL_OFF_Z                            (0x31)      // Z-Axis Offset Adjust Register

/**************************************************************************
    ACCELEROMETER CONFIGURATION REGISTER DESCRIPTION
 **************************************************************************/
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_MASK        (0x10)      // Output Data High-Pass Filtered
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_DISABLE     (0x00)      // Disable Output Data High-Pass
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_ENABLE      (0x10)      // Enable Output Data High-Pass

    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_MASK             (0x03)      // Output Buffer Data Format Full Scale
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_2                (0x00)      // Full-Scale Range: 2
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_4                (0x01)      // Full-Scale Range: 4
    #define MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_8                (0x02)      // Full-Scale Range: 8

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_MASK         (0xC0)      // Configures the Auto-Wake Sample Frequency When the Device is in Sleep Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_50HZ         (0x00)      // Frequency (Hz): 50
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_12_5HZ       (0x40)      // Frequency (Hz): 12.5
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_6_25HZ       (0x80)      // Frequency (Hz): 6.25
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_1_56HZ       (0xC0)      // Frequency (Hz): 1.56

    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_MASK                (0x38)      // Data Rate Selection
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_800HZ               (0x00)      // ODR: 800 Hz, Period: 1.25 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_400HZ               (0x08)      // ODR: 400 Hz, Period: 2.5 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_200HZ               (0x10)      // ODR: 200 Hz, Period: 5 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_100HZ               (0x18)      // ODR: 100 Hz, Period: 10 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_50HZ                (0x20)      // ODR: 50 Hz, Period: 20 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_12_5HZ              (0x28)      // ODR: 12.5 Hz, Period: 80 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_6_25HZ              (0x30)      // ODR: 6.25 Hz, Period: 160 ms
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_DR_1_56HZ              (0x38)      // ODR: 1.25 Hz, Period: 640 ms

    #define MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_MASK            (0x04)      // Reduced Noise Reduced Maximum Range Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_NORMAL          (0x00)      // Normal Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_REDNOISE        (0x04)      // Reduced Noise Mode

    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_MASK            (0x02)      // Fast-Read Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_NORMAL          (0x00)      // Normal Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_FAST            (0x02)      // Fast-Read Mode

    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_MASK          (0x01)      // Full-Scale Selection
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_STANDBY       (0x00)      // Fast-Read Mode
    #define MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_ACTIVE        (0x01)      // Full-Scale Selection

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 2 DESCRIPTION
**************************************************************************/
    #define MA8452Q_REG_ACCEL_CTRL_REG2_ST_MASK                 (0x80)      // Self-Test Mode
    #define MA8452Q_REG_ACCEL_CTRL_REG2_ST_DISABLE              (0x00)      // Self-Test Disabled
    #define MA8452Q_REG_ACCEL_CTRL_REG2_ST_ENABLE               (0x80)      // Self-Test Enabled

    #define MA8452Q_REG_ACCEL_CTRL_REG2_RST_MASK                (0x40)      // Software Reset Mode
    #define MA8452Q_REG_ACCEL_CTRL_REG2_RST_DISABLE             (0x00)      // Device Reset Disabled
    #define MA8452Q_REG_ACCEL_CTRL_REG2_RST_ENABLE              (0x40)      // Device Reset Enabled

    #define MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_MASK              (0x18)      // Sleep Mode Power Scheme Selection
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_NORMAL            (0x00)      // Power Mode: Normal
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_LNLP              (0x08)      // Power Mode: Low Noise Low Power
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_HIRES             (0x10)      // Power Mode: High Resolution
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_LOPOWER           (0x18)      // Power Mode: Low Power

    #define MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_MASK               (0x04)      // Auto-Sleep Enable
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_DISABLE            (0x00)      // Auto-Sleep Disabled
    #define MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_ENABLE             (0x04)      // Auto-Sleep Enabled

    #define MA8452Q_REG_ACCEL_CTRL_REG2_MODS_MASK               (0x03)      // Active Mode Power Scheme Selection
    #define MA8452Q_REG_ACCEL_CTRL_REG2_MODS_NORMAL             (0x00)      // Power Mode: Normal
    #define MA8452Q_REG_ACCEL_CTRL_REG2_MODS_LNLP               (0x01)      // Power Mode: Low Noise Low Power
    #define MA8452Q_REG_ACCEL_CTRL_REG2_MODS_HIRES              (0x02)      // Power Mode: High Resolution
    #define MA8452Q_REG_ACCEL_CTRL_REG2_MODS_LOPOWER            (0x03)      // Power Mode: Low Power


typedef enum
{
    HPF_OUT_DISABLE                     = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_DISABLE,
    HPF_OUT_ENABLE                      = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_ENABLE
    
} mmaAccelHPF_t;

typedef enum
{
    ACCEL_RANGE_2G                      = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_2,
    ACCEL_RANGE_4G                      = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_4,
    ACCEL_RANGE_8G                      = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_FS_8
    
} mmaAccelRange_t;

typedef enum
{
    ASLP_RATE_50HZ                      = MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_50HZ,
    ASLP_RATE_12_5HZ                    = MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_12_5HZ,
    ASLP_RATE_6_25HZ                    = MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_6_25HZ,
    ASLP_RATE_1_56HZ                    = MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_1_56HZ
    
} mmaAccelFrequency_t;

typedef enum
{
    ACCEL_BANDWIDTH_800HZ               = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_800HZ,
    ACCEL_BANDWIDTH_400HZ               = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_400HZ,
    ACCEL_BANDWIDTH_200HZ               = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_200HZ,
    ACCEL_BANDWIDTH_100HZ               = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_100HZ,
    ACCEL_BANDWIDTH_50HZ                = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_50HZ,
    ACCEL_BANDWIDTH_12_5HZ              = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_12_5HZ,
    ACCEL_BANDWIDTH_6_25HZ              = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_6_25HZ,
    ACCEL_BANDWIDTH_1_56HZ              = MMA8452Q_REG_ACCEL_CTRL_REG1_DR_1_56HZ
    
} mmaAccelBandwidth_t;

typedef enum
{
    ACCEL_LNOISE_NORMAL                 = MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_NORMAL,
    ACCEL_LNOISE_REDNOISE               = MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_REDNOISE
    
} mmaAccelNoise_t;

typedef enum
{
    ACCEL_F_READ_NORMAL                 = MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_NORMAL,
    ACCEL_F_READ_FAST                   = MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_FAST
    
} mmaAccelFastRead_t;

typedef enum
{
    ACCEL_MODE_STANDBY                  = MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_STANDBY,
    ACCEL_MODE_ACTIVE                   = MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_ACTIVE
    
} mmaAccelMode_t;

typedef enum
{
    ACCEL_SELF_TEST_DISABLE             = MA8452Q_REG_ACCEL_CTRL_REG2_ST_DISABLE,
    ACCEL_SELF_TEST_ENABLE              = MA8452Q_REG_ACCEL_CTRL_REG2_ST_ENABLE
    
} mmaAccelSelfTest_t;

typedef enum
{
    ACCEL_RESET_DISABLE                 = MA8452Q_REG_ACCEL_CTRL_REG2_RST_DISABLE,
    ACCEL_RESET_ENABLE                  = MA8452Q_REG_ACCEL_CTRL_REG2_RST_ENABLE
    
} mmaAccelReset_t;

typedef enum
{
    ACCEL_SLEEP_POWER_NORMAL            = MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_NORMAL,
    ACCEL_SLEEP_POWER_LNLP              = MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_LNLP,
    ACCEL_SLEEP_POWER_HIRES             = MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_HIRES,
    ACCEL_SLEEP_POWER_LOPOWER           = MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_LOPOWER,
    
} mmaAccelSleepPower_t;

typedef enum
{
    ACCEL_AUTO_SLEEP_DISABLE            = MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_DISABLE,
    ACCEL_AUTO_SLEEP_ENABLE             = MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_ENABLE
    
} mmaAccelAutoSleep_t;

typedef enum
{
    ACCEL_ACTIVE_POWER_NORMAL            = MA8452Q_REG_ACCEL_CTRL_REG2_MODS_NORMAL,
    ACCEL_ACTIVE_POWER_LNLP              = MA8452Q_REG_ACCEL_CTRL_REG2_MODS_LNLP,
    ACCEL_ACTIVE_POWER_HIRES             = MA8452Q_REG_ACCEL_CTRL_REG2_MODS_HIRES,
    ACCEL_ACTIVE_POWER_LOPOWER           = MA8452Q_REG_ACCEL_CTRL_REG2_MODS_LOPOWER,
    
} mmaAccelActivePower_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} mmaSensorData_t;


class MMA8452Q
{
    protected:
        // Instance-specific properties
        uint8_t mma_conversionDelay;
        mmaAccelHPF_t mma_accelhpf;
        mmaAccelRange_t mma_accelrange;
        mmaAccelFrequency_t mma_accelfrequency;
        mmaAccelBandwidth_t mma_accelbandwidth;
        mmaAccelNoise_t mma_accelnoise;
        mmaAccelFastRead_t mma_accelfastread;
        mmaAccelMode_t mma_accelmode;
        mmaAccelSelfTest_t mma_accelselftest;
        mmaAccelReset_t mma_accelreset;
        mmaAccelSleepPower_t mma_accelsleeppower;
        mmaAccelAutoSleep_t mma_accelautosleep;
        mmaAccelActivePower_t mma_accelactivepower;
    
    public:
        uint8_t mma_i2cAddress;
        mmaSensorData_t mma_accelData;
        void getAddr_MMA8452Q(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelHPF(mmaAccelHPF_t accelhpf);
        mmaAccelHPF_t getAccelHPF(void);
        void setAccelRange(mmaAccelRange_t accelrange);
        mmaAccelRange_t getAccelRange(void);
        void setAccelFrequency(mmaAccelFrequency_t accelfrequency);
        mmaAccelFrequency_t getAccelFrequency(void);
        void setAccelBandwidth(mmaAccelBandwidth_t accelbandwidth);
        mmaAccelBandwidth_t getAccelBandwidth(void);
        void setAccelNoise(mmaAccelNoise_t accelnoise);
        mmaAccelNoise_t getAccelNoise(void);
        void setAccelFastRead(mmaAccelFastRead_t accelfastread);
        mmaAccelFastRead_t getAccelFastRead(void);
        void setAccelMode(mmaAccelMode_t accelmode);
        mmaAccelMode_t getAccelMode(void);
        void setAccelSelfTest(mmaAccelSelfTest_t accelselftest);
        mmaAccelSelfTest_t getAccelSelfTest(void);
        void setAccelReset(mmaAccelReset_t accelreset);
        mmaAccelReset_t getAccelReset(void);
        void setAccelSleepPower(mmaAccelSleepPower_t accelsleeppower);
        mmaAccelSleepPower_t getAccelSleepPower(void);
        void setAccelAutoSleep(mmaAccelAutoSleep_t accelautosleep);
        mmaAccelAutoSleep_t getAccelAutoSleep(void);
        void setAccelActivePower(mmaAccelActivePower_t accelactivepower);
        mmaAccelActivePower_t getAccelActivePower(void);
    
    private:
};
