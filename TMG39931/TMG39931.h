/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMG39931
        This code is designed to work with the TMG39931_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/light-sensor-gesture-color-als-and-proximity-sensor
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
    #define TMG39931_DEFAULT_ADDRESS                            (0x39)
    #define TMG39931_COLORPROX_DEV_ID                           (0xA1)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMG39931_CONVERSIONDELAY                            (100)

/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR REGISTERS
**************************************************************************/
    #define TMG39931_REG_COLORPROX_ENABLE                       (0x80)      // Enables States and Interrupts Register
    #define TMG39931_REG_COLORPROX_ATIME                        (0x81)      // ADC Integration Time Register
    #define TMG39931_REG_COLORPROX_WTIME                        (0x83)      // Wait Time Register
    #define TMG39931_REG_COLORPROX_AILTL                        (0x84)      // Clear Interrupt Low Threshold Low Byte Register
    #define TMG39931_REG_COLORPROX_AILTH                        (0x85)      // Clear Interrupt Low Threshold High Byte Register
    #define TMG39931_REG_COLORPROX_AIHTL                        (0x86)      // Clear Interrupt High Threshold Low Byte Register
    #define TMG39931_REG_COLORPROX_AIHTH                        (0x87)      // Clear Interrupt High Threshold High Byte Register
    #define TMG39931_REG_COLORPROX_PITHL                        (0x89)      // Proximity Interrupt Low Threshold Register
    #define TMG39931_REG_COLORPROX_PITHH                        (0x8B)      // Proximity Interrupt High Threshold Register
    #define TMG39931_REG_COLORPROX_PERS                         (0x8C)      // Interrupt Persistence Filters Register
    #define TMG39931_REG_COLORPROX_CONFIG1                      (0x8D)      // Configuration Register One
    #define TMG39931_REG_COLORPROX_PPULSE                       (0x8E)      // Proximity Pulse Count and Length Register
    #define TMG39931_REG_COLORPROX_CONTROL                      (0x8F)      // Gain Control Register
    #define TMG39931_REG_COLORPROX_CONFIG2                      (0x90)      // Configuration Register Two
    #define TMG39931_REG_COLORPROX_REVISION                     (0x91)      // Revision ID Register
    #define TMG39931_REG_COLORPROX_ID                           (0x92)      // Device ID Register
    #define TMG39931_REG_COLORPROX_STATUS                       (0x93)      // Device Status Register
    #define TMG39931_REG_COLORPROX_CDATAL                       (0x94)      // Clear ADC Low Data Register
    #define TMG39931_REG_COLORPROX_CDATAH                       (0x95)      // Clear ADC High Data Register
    #define TMG39931_REG_COLORPROX_RDATAL                       (0x96)      // Red ADC Low Data Register
    #define TMG39931_REG_COLORPROX_RDATAH                       (0x97)      // Red ADC High Data Register
    #define TMG39931_REG_COLORPROX_GDATAL                       (0x98)      // Green ADC Low Data Register
    #define TMG39931_REG_COLORPROX_GDATAH                       (0x99)      // Green ADC High Data Register
    #define TMG39931_REG_COLORPROX_BDATAL                       (0x9A)      // Blue ADC Low Data Register
    #define TMG39931_REG_COLORPROX_BDATAH                       (0x9B)      // Blue ADC High Data Register
    #define TMG39931_REG_COLORPROX_PDATA                        (0x9C)      // Proximity ADC Data Register
    #define TMG39931_REG_COLORPROX_POFFSET_NE                   (0x9D)      // Proximity Offset for North and East Photodiodes Register
    #define TMG39931_REG_COLORPROX_POFFSET_SW                   (0x9E)      // Proximity Offset for South and West Photodiodes Register
    #define TMG39931_REG_COLORPROX_CONFIG3                      (0x9F)      // Configuration Register Three
    #define TMG39931_REG_COLORPROX_CONFIG_A0                    (0xA0)      // Configuration Register A0
    #define TMG39931_REG_COLORPROX_CONFIG_A1                    (0xA1)      // Configuration Register A1
    #define TMG39931_REG_COLORPROX_CONFIG_A2                    (0xA2)      // Configuration Register A2
    #define TMG39931_REG_COLORPROX_CONFIG_A3                    (0xA3)      // Configuration Register A3
    #define TMG39931_REG_COLORPROX_CONFIG_A4                    (0xA4)      // Configuration Register A4
    #define TMG39931_REG_COLORPROX_CONFIG_A5                    (0xA5)      // Configuration Register A5
    #define TMG39931_REG_COLORPROX_CONFIG_A6                    (0xA6)      // Configuration Register A6
    #define TMG39931_REG_COLORPROX_CONFIG_A7                    (0xA7)      // Configuration Register A7
    #define TMG39931_REG_COLORPROX_CONFIG_A8                    (0xA8)      // Configuration Register A8
    #define TMG39931_REG_COLORPROX_CONFIG_A9                    (0xA9)      // Configuration Register A9
    #define TMG39931_REG_COLORPROX_CONFIG_AA                    (0xAA)      // Configuration Register AA
    #define TMG39931_REG_COLORPROX_CONFIG_AB                    (0xAB)      // Configuration Register AB
    #define TMG39931_REG_COLORPROX_GFLVL                        (0xAE)      // Gesture FIFO Level Register
    #define TMG39931_REG_COLORPROX_GSTATUS                      (0xAF)      // Gesture Status Register
    #define TMG39931_REG_COLORPROX_PBCLEAR                      (0xE3)      // Pattern Burst Interrupt Clear Register
    #define TMG39931_REG_COLORPROX_IFORCE                       (0xE4)      // Force Interrupt Register
    #define TMG39931_REG_COLORPROX_PICLEAR                      (0xE5)      // Proximity Interrupt Register
    #define TMG39931_REG_COLORPROX_CICLEAR                      (0xE6)      // ALS Interrupt Register
    #define TMG39931_REG_COLORPROX_AICLEAR                      (0xE7)      // Clear all Non-Gesture Interrupts Register
    #define TMG39931_REG_COLORPROX_GFIFO_N                      (0xFC)      // Gesture North FIFO Register
    #define TMG39931_REG_COLORPROX_GFIFO_S                      (0xFD)      // Gesture South FIFO Register
    #define TMG39931_REG_COLORPROX_GFIFO_W                      (0xFE)      // Gesture West FIFO Register
    #define TMG39931_REG_COLORPROX_GFIFO_E                      (0xFF)      // Gesture East FIFO Register


/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TMG39931_REG_COLORPROX_ENABLE_PBEN_MASK             (0x80)      // Pattern Burst Enable
    #define TMG39931_REG_COLORPROX_ENABLE_PBEN_NOT_ASSERT       (0x00)      // Not Asserted, No Control of the LED Driver Pin (LDR) is Managed by the IRBeam State Machine
    #define TMG39931_REG_COLORPROX_ENABLE_PBEN_ASSERT           (0x80)      // Asserted, Control of the LED Driver Pin (LDR) is Managed by the IRBeam State Machine

    #define TMG39931_REG_COLORPROX_ENABLE_GEN_MASK              (0x40)      // Gesture Enable
    #define TMG39931_REG_COLORPROX_ENABLE_GEN_NOT_ASSERT        (0x00)      // Not Asserted, the Gesture State Machine can not be Activated
    #define TMG39931_REG_COLORPROX_ENABLE_GEN_ASSERT            (0x40)      // Asserted, the Gesture State Machine can be Activated

    #define TMG39931_REG_COLORPROX_ENABLE_PIEN_MASK             (0x20)      // Proximity Interrupt Mask
    #define TMG39931_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT       (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TMG39931_REG_COLORPROX_ENABLE_PIEN_ASSERT           (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TMG39931_REG_COLORPROX_ENABLE_AIEN_MASK             (0x10)      // Ambient Light Sensing (ALS) Interrupt Mask
    #define TMG39931_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT       (0x00)      // Not Asserted, Did Not Permits Ambient Light Sensing (ALS) Interrupts to be Generated
    #define TMG39931_REG_COLORPROX_ENABLE_AIEN_ASSERT           (0x10)      // Asserted, Permits Ambient Light Sensing (ALS) Interrupts to be Generated

    #define TMG39931_REG_COLORPROX_ENABLE_WEN_MASK              (0x08)      // Wait Enable
    #define TMG39931_REG_COLORPROX_ENABLE_WEN_DISABLE           (0x00)      // Disables the Wait Timer
    #define TMG39931_REG_COLORPROX_ENABLE_WEN_ENABLE            (0x08)      // Enables the Wait Timer

    #define TMG39931_REG_COLORPROX_ENABLE_PEN_MASK              (0x04)      // Proximity Enable
    #define TMG39931_REG_COLORPROX_ENABLE_PEN_DISABLE           (0x00)      // Deactivates the Proximity
    #define TMG39931_REG_COLORPROX_ENABLE_PEN_ENABLE            (0x04)      // Activates the Proximity

    #define TMG39931_REG_COLORPROX_ENABLE_AEN_MASK              (0x02)      // 4-Channel RGBC ADC Enable
    #define TMG39931_REG_COLORPROX_ENABLE_AEN_DISABLE           (0x00)      // Disables the RGBC ADC
    #define TMG39931_REG_COLORPROX_ENABLE_AEN_ENABLE            (0x02)      // Activates the RGBC ADC

    #define TMG39931_REG_COLORPROX_ENABLE_PON_MASK              (0x01)      // Power ON
    #define TMG39931_REG_COLORPROX_ENABLE_PON_DISABLE           (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TMG39931_REG_COLORPROX_ENABLE_PON_ENABLE            (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMG39931_REG_COLORPROX_ATIME_MASK                   (0xFF)      // Controls the Internal Integration Time of the RGBC Channel ADC’s in 2.78 ms Increments
    #define TMG39931_REG_COLORPROX_ATIME_2_78MS                 (0xFF)      // Cycles: 1, Time: 2.78 ms Max Count: 1025
    #define TMG39931_REG_COLORPROX_ATIME_27_8MS                 (0xF6)      // Cycles: 10, Time: 27.8 ms Max Count: 10241
    #define TMG39931_REG_COLORPROX_ATIME_103MS                  (0xDB)      // Cycles: 37, Time: 103 ms Max Count: 37888
    #define TMG39931_REG_COLORPROX_ATIME_178MS                  (0xC0)      // Cycles: 64, Time: 178 ms Max Count: 65535
    #define TMG39931_REG_COLORPROX_ATIME_712MS                  (0x00)      // Cycles: 256, Time: 712 ms Max Count: 65535

/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMG39931_REG_COLORPROX_WTIME_MASK                   (0xFF)      // Wait Time is Set in 2.78 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TMG39931_REG_COLORPROX_WTIME_1                      (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.78 ms Time (WLONG = 1):  0.03 sec
    #define TMG39931_REG_COLORPROX_WTIME_85                     (0xB6)      // Wait Time: 85, Time (WLONG = 0): 236 ms Time (WLONG = 1):  2.84 sec
    #define TMG39931_REG_COLORPROX_WTIME_256                    (0x00)      // Wait Time: 256, Time (WLONG = 0): 712 ms Time (WLONG = 1):  8.54 sec

/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TMG39931_REG_COLORPROX_CONFIG_WLONG_MASK            (0x02)      // Wait Long
    #define TMG39931_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT      (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TMG39931_REG_COLORPROX_CONFIG_WLONG_ASSERT          (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    LIGHT SENSOR GESTURE, COLOR, ALS, AND PROXIMITY SENSOR CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TMG39931_REG_COLORPROX_CONTROL_PDRIVE_MASK          (0xC0)      // Proximity LED Drive Strength
    #define TMG39931_REG_COLORPROX_CONTROL_PDRIVE_100           (0x00)      // LED Strength – PDL=0: 100%
    #define TMG39931_REG_COLORPROX_CONTROL_PDRIVE_50            (0x40)      // LED Strength – PDL=0: 50%
    #define TMG39931_REG_COLORPROX_CONTROL_PDRIVE_25            (0x80)      // LED Strength – PDL=0: 25%
    #define TMG39931_REG_COLORPROX_CONTROL_PDRIVE_12_5          (0xC0)      // LED Strength – PDL=0: 12.5%

    #define TMG39931_REG_COLORPROX_CONTROL_RPINS_MASK           (0x20)      // LDR signal to INT
    #define TMG39931_REG_COLORPROX_CONTROL_RPINS_DISABLE        (0x00)      // LDR Pin is Deactivated
    #define TMG39931_REG_COLORPROX_CONTROL_RPINS_ENABLE         (0x20)      // Transfers Pattern Burst Output from the LDR Pin to the INT pin

    #define TMG39931_REG_COLORPROX_CONTROL_RINTPOL_MASK         (0x10)      // INT Pin Polarity
    #define TMG39931_REG_COLORPROX_CONTROL_RINTPOL_DISABLE      (0x00)      // Doesn't Invert the Bit Pattern Output of the INT Pin
    #define TMG39931_REG_COLORPROX_CONTROL_RINTPOL_ENABLE       (0x10)      // Inverts the Bit Pattern Output of the INT Pin

    #define TMG39931_REG_COLORPROX_CONTROL_PGAIN_MASK           (0x0C)      // Proximity Gain Value
    #define TMG39931_REG_COLORPROX_CONTROL_PGAIN_1X             (0x00)      // 1X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_PGAIN_2X             (0x04)      // 2X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_PGAIN_4X             (0x08)      // 4X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_PGAIN_8X             (0x0C)      // 8X Gain

    #define TMG39931_REG_COLORPROX_CONTROL_AGAIN_MASK           (0x03)      // RGBC Gain Value
    #define TMG39931_REG_COLORPROX_CONTROL_AGAIN_1X             (0x00)      // 1X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_AGAIN_4X             (0x01)      // 4X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_AGAIN_16X            (0x02)      // 16X Gain
    #define TMG39931_REG_COLORPROX_CONTROL_AGAIN_64X            (0x03)      // 64X Gain


typedef enum
{
    PBEN_NOT_ASSERT                     = TMG39931_REG_COLORPROX_ENABLE_PBEN_NOT_ASSERT,
    PBEN_ASSERT                         = TMG39931_REG_COLORPROX_ENABLE_PBEN_ASSERT
    
} tmgPatternBurst_t;

typedef enum
{
    GEN_NOT_ASSERT                      = TMG39931_REG_COLORPROX_ENABLE_GEN_NOT_ASSERT,
    GEN_ASSERT                          = TMG39931_REG_COLORPROX_ENABLE_GEN_ASSERT
    
} tmgGestureEnable_t;

typedef enum
{
    PIEN_NOT_ASSERT                     = TMG39931_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                         = TMG39931_REG_COLORPROX_ENABLE_PIEN_ASSERT
    
} tmgProximityInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                     = TMG39931_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                         = TMG39931_REG_COLORPROX_ENABLE_AIEN_ASSERT
    
} tmgColorInterrupt_t;

typedef enum
{
    WEN_DISABLE                         = TMG39931_REG_COLORPROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                          = TMG39931_REG_COLORPROX_ENABLE_WEN_ENABLE
    
} tmgWaitEnable_t;

typedef enum
{
    PEN_DISABLE                         = TMG39931_REG_COLORPROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                          = TMG39931_REG_COLORPROX_ENABLE_PEN_ENABLE
    
} tmgProximityEnable_t;

typedef enum
{
    AEN_DISABLE                         = TMG39931_REG_COLORPROX_ENABLE_AEN_DISABLE,
    AEN_ENABLE                          = TMG39931_REG_COLORPROX_ENABLE_AEN_ENABLE
    
} tmgColorEnable_t;

typedef enum
{
    PON_DISABLE                         = TMG39931_REG_COLORPROX_ENABLE_PON_DISABLE,
    PON_ENABLE                          = TMG39931_REG_COLORPROX_ENABLE_PON_ENABLE
    
} tmgPowerEnable_t;

typedef enum
{
    ATIME_2_78MS                        = TMG39931_REG_COLORPROX_ATIME_2_78MS,
    ATIME_27_8MS                        = TMG39931_REG_COLORPROX_ATIME_27_8MS,
    ATIME_103MS                         = TMG39931_REG_COLORPROX_ATIME_103MS,
    ATIME_178MS                         = TMG39931_REG_COLORPROX_ATIME_178MS,
    ATIME_712MS                         = TMG39931_REG_COLORPROX_ATIME_712MS
    
} tmgATime_t;

typedef enum
{
    WTIME_1                             = TMG39931_REG_COLORPROX_WTIME_1,
    WTIME_85                            = TMG39931_REG_COLORPROX_WTIME_85,
    WTIME_256                           = TMG39931_REG_COLORPROX_WTIME_256
    
} tmgWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                    = TMG39931_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                        = TMG39931_REG_COLORPROX_CONFIG_WLONG_ASSERT
    
} tmgWaitLong_t;

typedef enum
{
    PDRIVE_100                          = TMG39931_REG_COLORPROX_CONTROL_PDRIVE_100,
    PDRIVE_50                           = TMG39931_REG_COLORPROX_CONTROL_PDRIVE_50,
    PDRIVE_25                           = TMG39931_REG_COLORPROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                         = TMG39931_REG_COLORPROX_CONTROL_PDRIVE_12_5
    
} tmgProximityDrive_t;

typedef enum
{
    PGAIN_1X                            = TMG39931_REG_COLORPROX_CONTROL_PGAIN_1X,
    PGAIN_2X                            = TMG39931_REG_COLORPROX_CONTROL_PGAIN_2X,
    PGAIN_4X                            = TMG39931_REG_COLORPROX_CONTROL_PGAIN_4X,
    PGAIN_8X                            = TMG39931_REG_COLORPROX_CONTROL_PGAIN_8X
    
} tmgProximityGain_t;

typedef enum
{
    AGAIN_1X                            = TMG39931_REG_COLORPROX_CONTROL_AGAIN_1X,
    AGAIN_4X                            = TMG39931_REG_COLORPROX_CONTROL_AGAIN_4X,
    AGAIN_16X                           = TMG39931_REG_COLORPROX_CONTROL_AGAIN_16X,
    AGAIN_64X                           = TMG39931_REG_COLORPROX_CONTROL_AGAIN_64X
    
} tmgColorGain_t;

typedef struct
{
    uint16_t C;
    uint16_t R;
    uint16_t G;
    uint16_t B;
    uint8_t P;
    
} tmgSensorData_t;


class TMG39931
{
    protected:
        // Instance-specific properties
        uint8_t tmg_conversionDelay;
        tmgPatternBurst_t tmg_patternburst;
        tmgGestureEnable_t tmg_gestureenable;
        tmgProximityInterrupt_t tmg_proximityinterrupt;
        tmgColorInterrupt_t tmg_colorinterrupt;
        tmgWaitEnable_t tmg_waitenable;
        tmgProximityEnable_t tmg_proximityenable;
        tmgColorEnable_t tmg_colorenable;
        tmgPowerEnable_t tmg_powerenable;
        tmgATime_t tmg_atime;
        tmgWTime_t tmg_wtime;
        tmgWaitLong_t tmg_waitlong;
        tmgProximityDrive_t tmg_proximitydrive;
        tmgProximityGain_t tmg_proximitygain;
        tmgColorGain_t tmg_colorgain;

    public:
        uint8_t tmg_i2cAddress;
        tmgSensorData_t tmg_colorproxData;
        void getAddr_TMG39931(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpColorProximity(void);
        void Measure_Color(void);
        void Measure_Proximity(void);
        void setPatternBurst(tmgPatternBurst_t patternburst);
        tmgPatternBurst_t getPatternBurst(void);
        void setGestureEnable(tmgGestureEnable_t gestureenable);
        tmgGestureEnable_t getGestureEnable(void);
        void setProximityInterrupt(tmgProximityInterrupt_t proximityinterrupt);
        tmgProximityInterrupt_t getProximityInterrupt(void);
        void setColorInterrupt(tmgColorInterrupt_t colorinterrupt);
        tmgColorInterrupt_t getColorInterrupt(void);
        void setWaitEnable(tmgWaitEnable_t waitenable);
        tmgWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tmgProximityEnable_t proximityenable);
        tmgProximityEnable_t getProximityEnable(void);
        void setColorEnable(tmgColorEnable_t colorenable);
        tmgColorEnable_t getColorEnable(void);
        void setPowerEnable(tmgPowerEnable_t powerenable);
        tmgPowerEnable_t getPowerEnable(void);
        void setATime(tmgATime_t atime);
        tmgATime_t getATime(void);
        void setWTime(tmgWTime_t wtime);
        tmgWTime_t getWTime(void);
        void setWaitLong(tmgWaitLong_t waitlong);
        tmgWaitLong_t getWaitLong(void);
        void setProximityDrive(tmgProximityDrive_t proximitydrive);
        tmgProximityDrive_t getProximityDrive(void);
        void setProximityGain(tmgProximityGain_t proximitygain);
        tmgProximityGain_t getProximityGain(void);
        void setColorGain(tmgColorGain_t colorgain);
        tmgColorGain_t getColorGain(void);
    
    private:
};
