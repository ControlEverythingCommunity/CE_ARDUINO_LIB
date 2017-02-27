/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7660FC
        This code is designed to work with the MMA7660FC_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-orientation-motion-detection-sensor-1-5-g-accelerometer?variant=25687536907#tabs-0-product_tabset-2
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
    #define MMA7660FC_DEFAULT_ADDRESS_ACCEL                 (0x4C)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MMA7660FC_CONVERSIONDELAY                       (300)

/**************************************************************************
    ACCELNETOMETER REGISTERS
**************************************************************************/
    #define MMA7660FC_REG_ACCEL_XOUT                        (0x00)      // 6-Bits X-Axis Acceleration Data Register
    #define MMA7660FC_REG_ACCEL_YOUT                        (0x01)      // 6-Bits Y-Axis Acceleration Data Register
    #define MMA7660FC_REG_ACCEL_ZOUT                        (0x02)      // 6-Bits Z-Axis Acceleration Data Register
    #define MMA7660FC_REG_ACCEL_TILT                        (0x03)      // Tilt Status Registers
    #define MMA7660FC_REG_ACCEL_SRST                        (0x04)      // Sampling Rate Status Registers
    #define MMA7660FC_REG_ACCEL_SPCNT                       (0x05)      // Sleep Count Register
    #define MMA7660FC_REG_ACCEL_INTSU                       (0x06)      // Interrupt Setup Register
    #define MMA7660FC_REG_ACCEL_MODE                        (0x07)      // Mode Register
    #define MMA7660FC_REG_ACCEL_SR                          (0x08)      // Auto-Wake/Sleep and Portrait/Landscape Samples per Seconds and Debounce Filter Register
    #define MMA7660FC_REG_ACCEL_PDET                        (0x09)      // Tap Detection Register
    #define MMA7660FC_REG_ACCEL_PD                          (0x0A)      // Tap Debounce Count Register

/**************************************************************************
    ACCELEROMETER MODE REGISTER DESCRIPTION
**************************************************************************/
    #define MMA7660FC_REG_ACCEL_MODE_IAH_MASK               (0x80)      // Interrupt Output Active Status
    #define MMA7660FC_REG_ACCEL_MODE_IAH_LOW                (0x00)      // Interrupt Output INT is Active Low
    #define MMA7660FC_REG_ACCEL_MODE_IAH_HIGH               (0x80)      // Interrupt Output INT is Active Low

    #define MMA7660FC_REG_ACCEL_MODE_IPP_MASK               (0x40)      // Interrupt Output Open Drain/Push-Pull Status
    #define MMA7660FC_REG_ACCEL_MODE_IPP_OPEN_DRAIN         (0x00)      // Interrupt Output INT is Active Open Drain
    #define MMA7660FC_REG_ACCEL_MODE_IPP_PUSH_PULL          (0x40)      // Interrupt Output INT is Active Push-Pull

    #define MMA7660FC_REG_ACCEL_MODE_SCPS_MASK              (0x20)      // Prescaler
    #define MMA7660FC_REG_ACCEL_MODE_SCPS_1                 (0x00)      // The Prescaler is Divide-by-1
    #define MMA7660FC_REG_ACCEL_MODE_SCPS_16                (0x20)      // The Prescaler is Divide-by-16

    #define MMA7660FC_REG_ACCEL_MODE_ASE_MASK               (0x10)      // Auto-Sleep Functionality
    #define MMA7660FC_REG_ACCEL_MODE_ASE_DISABLED           (0x00)      // Auto-Sleep Disabled
    #define MMA7660FC_REG_ACCEL_MODE_ASE_ENABLED            (0x10)      // Auto-Sleep Enabled

    #define MMA7660FC_REG_ACCEL_MODE_AWE_MASK               (0x08)      // Auto-Wake Functionality
    #define MMA7660FC_REG_ACCEL_MODE_AWE_DISABLED           (0x00)      // Auto-Wake Disabled
    #define MMA7660FC_REG_ACCEL_MODE_AWE_ENABLED            (0x08)      // Auto-Wake Enabled

    #define MMA7660FC_REG_ACCEL_MODE_TON_MASK               (0x04)      // TON
    #define MMA7660FC_REG_ACCEL_MODE_TON_STANDBY            (0x00)      // Standby Mode or Active Mode Depending on State of MODE
    #define MMA7660FC_REG_ACCEL_MODE_TON_TEST               (0x04)      // Test Mode

    #define MMA7660FC_REG_ACCEL_MODE_MODE_MASK              (0x01)      // MODE
    #define MMA7660FC_REG_ACCEL_MODE_MODE_STANDBY           (0x00)      // Standby Mode or Test Mode Depending on State of TON
    #define MMA7660FC_REG_ACCEL_MODE_MODE_ACTIVE            (0x01)      // Active Mode

/**************************************************************************
    ACCELEROMETER SAMPLE RATE REGISTER DESCRIPTION
**************************************************************************/
    #define MMA7660FC_REG_ACCEL_SR_FILT_MASK                (0xE0)      // FILT
    #define MMA7660FC_REG_ACCEL_SR_FILT_DISABLED            (0x00)      // Tilt Debounce Filtering is Disabled
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_2              (0x20)      // 2 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_3              (0x40)      // 3 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_4              (0x60)      // 4 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_5              (0x80)      // 5 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_6              (0xA0)      // 6 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_7              (0xC0)      // 7 Measurement Samples
    #define MMA7660FC_REG_ACCEL_SR_FILT_MEAS_8              (0xE0)      // 8 Measurement Samples

    #define MMA7660FC_REG_ACCEL_SR_AWSR_MASK                (0x18)      // Auto-Wake Mode Sample Rate (AWSR)
    #define MMA7660FC_REG_ACCEL_SR_AWSR_32                  (0x00)      // 32 Samples/Second Auto-Wake Mode
    #define MMA7660FC_REG_ACCEL_SR_AWSR_16                  (0x08)      // 16 Samples/Second Auto-Wake Mode
    #define MMA7660FC_REG_ACCEL_SR_AWSR_8                   (0x10)      // 8 Samples/Second Auto-Wake Mode
    #define MMA7660FC_REG_ACCEL_SR_AWSR_1                   (0x18)      // 1 Samples/Second Auto-Wake Mode

    #define MMA7660FC_REG_ACCEL_SR_AMSR_MASK                (0x07)      // Auto-Sleep Mode Sample Rate (AMSR)
    #define MMA7660FC_REG_ACCEL_SR_AMSR_120                 (0x00)      // Tap Detection Mode and 120 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_64                  (0x01)      // 64 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_32                  (0x02)      // 32 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_16                  (0x03)      // 16 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_8                   (0x04)      // 8 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_4                   (0x05)      // 4 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_2                   (0x06)      // 2 Samples/Second Active and Auto-Sleep Mode
    #define MMA7660FC_REG_ACCEL_SR_AMSR_1                   (0x07)      // 1 Sample/Second Active and Auto-Sleep Mode



typedef enum
{
    ACCEL_INTERRUPT_LOW                 = MMA7660FC_REG_ACCEL_MODE_IAH_LOW,
    ACCEL_INTERRUPT_HIGH                = MMA7660FC_REG_ACCEL_MODE_IAH_HIGH
    
} mmaAccelInterrupt1_t;

typedef enum
{
    ACCEL_INTERRUPT_OPEN                = MMA7660FC_REG_ACCEL_MODE_IPP_OPEN_DRAIN,
    ACCEL_INTERRUPT_PUSH                = MMA7660FC_REG_ACCEL_MODE_IPP_PUSH_PULL
    
} mmaAccelInterrupt2_t;

typedef enum
{
    ACCEL_PRESCALER_1                   = MMA7660FC_REG_ACCEL_MODE_SCPS_1,
    ACCEL_PRESCALER_16                  = MMA7660FC_REG_ACCEL_MODE_SCPS_16
    
} mmaAccelPrescaler_t;

typedef enum
{
    ACCEL_AUTO_SLEEP_DISABLED           = MMA7660FC_REG_ACCEL_MODE_ASE_DISABLED,
    ACCEL_AUTO_SLEEP_ENABLED            = MMA7660FC_REG_ACCEL_MODE_ASE_ENABLED
    
} mmaAccelAutoSleep_t;

typedef enum
{
    ACCEL_AUTO_WAKE_DISABLED           = MMA7660FC_REG_ACCEL_MODE_AWE_DISABLED,
    ACCEL_AUTO_WAKE_ENABLED            = MMA7660FC_REG_ACCEL_MODE_AWE_ENABLED
    
} mmaAccelAutoWake_t;

typedef enum
{
    ACCEL_TON_STANDBY                  = MMA7660FC_REG_ACCEL_MODE_TON_STANDBY,
    ACCEL_TON_TEST                     = MMA7660FC_REG_ACCEL_MODE_TON_TEST
    
} mmaAccelTon_t;

typedef enum
{
    ACCEL_MODE_STANDBY                  = MMA7660FC_REG_ACCEL_MODE_MODE_STANDBY,
    ACCEL_MODE_TEST                     = MMA7660FC_REG_ACCEL_MODE_MODE_ACTIVE
    
} mmaAccelMode_t;

typedef enum
{
    ACCEL_FILT_DISABLED                 = MMA7660FC_REG_ACCEL_SR_FILT_DISABLED,
    ACCEL_FILT_SAMPLES_2                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_2,
    ACCEL_FILT_SAMPLES_3                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_3,
    ACCEL_FILT_SAMPLES_4                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_4,
    ACCEL_FILT_SAMPLES_5                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_5,
    ACCEL_FILT_SAMPLES_6                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_6,
    ACCEL_FILT_SAMPLES_7                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_7,
    ACCEL_FILT_SAMPLES_8                = MMA7660FC_REG_ACCEL_SR_FILT_MEAS_8,
    
} mmaAccelFilt_t;

typedef enum
{
    ACCEL_AUTOWAKE_SAMPLES_32           = MMA7660FC_REG_ACCEL_SR_AWSR_32,
    ACCEL_AUTOWAKE_SAMPLES_16           = MMA7660FC_REG_ACCEL_SR_AWSR_16,
    ACCEL_AUTOWAKE_SAMPLES_8            = MMA7660FC_REG_ACCEL_SR_AWSR_8,
    ACCEL_AUTOWAKE_SAMPLES_1            = MMA7660FC_REG_ACCEL_SR_AWSR_1,
    
} mmaAccelWakeSample_t;

typedef enum
{
    ACCEL_AUTOSLEEP_SAMPLES_64          = MMA7660FC_REG_ACCEL_SR_AMSR_64,
    ACCEL_AUTOSLEEP_SAMPLES_32          = MMA7660FC_REG_ACCEL_SR_AMSR_32,
    ACCEL_AUTOSLEEP_SAMPLES_16          = MMA7660FC_REG_ACCEL_SR_AMSR_16,
    ACCEL_AUTOSLEEP_SAMPLES_8           = MMA7660FC_REG_ACCEL_SR_AMSR_8,
    ACCEL_AUTOSLEEP_SAMPLES_4           = MMA7660FC_REG_ACCEL_SR_AMSR_4,
    ACCEL_AUTOSLEEP_SAMPLES_2           = MMA7660FC_REG_ACCEL_SR_AMSR_2,
    ACCEL_AUTOSLEEP_SAMPLES_1           = MMA7660FC_REG_ACCEL_SR_AMSR_1,
    
} mmaAccelSleepSample_t;


typedef struct
{
    int8_t X;
    int8_t Y;
    int8_t Z;
    
} mmaSensorData_t;


class MMA7660FC
{
    protected:
        // Instance-specific properties
        uint8_t mma_conversionDelay;
        mmaAccelInterrupt1_t mma_accelinterrupt1;
        mmaAccelInterrupt2_t mma_accelinterrupt2;
        mmaAccelPrescaler_t mma_accelprescaler;
        mmaAccelAutoSleep_t mma_accelautosleep;
        mmaAccelAutoWake_t mma_accelautowake;
        mmaAccelTon_t mma_accelton;
        mmaAccelMode_t mma_accelmode;
        mmaAccelFilt_t mma_accelfilt;
        mmaAccelWakeSample_t mma_accelwakesample;
        mmaAccelSleepSample_t mma_accelsleepsample;

    public:
        uint8_t mma_i2cAddress;
        mmaSensorData_t mma_accelData;
        void getAddr_MMA7660FC(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelInterrupt1(mmaAccelInterrupt1_t accelinterrupt1);
        mmaAccelInterrupt1_t getAccelInterrupt1(void);
        void setAccelInterrupt2(mmaAccelInterrupt2_t accelinterrupt2);
        mmaAccelInterrupt2_t getAccelInterrupt2(void);
        void setAccelPrescaler(mmaAccelPrescaler_t accelprescaler);
        mmaAccelPrescaler_t getAccelPrescaler(void);
        void setAccelAutoSleep(mmaAccelAutoSleep_t accelautosleep);
        mmaAccelAutoSleep_t getAccelAutoSleep(void);
        void setAccelAutoWake(mmaAccelAutoWake_t accelautowake);
        mmaAccelAutoWake_t getAccelAutoWake(void);
        void setAccelTon(mmaAccelTon_t accelton);
        mmaAccelTon_t getAccelTon(void);
        void setAccelMode(mmaAccelMode_t accelmode);
        mmaAccelMode_t getAccelMode(void);
        void setAccelFilt(mmaAccelFilt_t accelfilt);
        mmaAccelFilt_t getAccelFilt(void);
        void setAccelWakeSample(mmaAccelWakeSample_t accelwakesample);
        mmaAccelWakeSample_t getAccelWakeSample(void);
        void setAccelSleepSample(mmaAccelSleepSample_t accelsleepsample);
        mmaAccelSleepSample_t getAccelSleepSample(void);
    
    private:
};
