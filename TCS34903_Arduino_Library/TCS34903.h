/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34903
        This code is designed to work with the TCS34903FN_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34903FN_I2CS#tabs-0-product_tabset-2
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
    #define TCS34903_DEFAULT_ADDRESS                        (0x39)
    #define TCS34903_COLOR_DEV_ID                           (0x87)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TCS34903_CONVERSIONDELAY                        (100)

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER REGISTERS
**************************************************************************/
    #define TCS34903_REG_COLOR_ENABLE                       (0x80)      // Enables States and Interrupts Register
    #define TCS34903_REG_COLOR_ATIME                        (0x81)      // RGBC Integration Time Register
    #define TCS34903_REG_COLOR_WTIME                        (0x83)      // Wait Time Register
    #define TCS34903_REG_COLOR_AILTL                        (0x84)      // Clear Interrupt Low Threshold Low Byte Register
    #define TCS34903_REG_COLOR_AILTH                        (0x85)      // Clear Interrupt Low Threshold High Byte Register
    #define TCS34903_REG_COLOR_AIHTL                        (0x86)      // Clear Interrupt High Threshold Low Byte Register
    #define TCS34903_REG_COLOR_AIHTH                        (0x87)      // Clear Interrupt High Threshold High Byte Register
    #define TCS34903_REG_COLOR_PERS                         (0x8C)      // Interrupt Persistence Filters Register
    #define TCS34903_REG_COLOR_CONFIG                       (0x8D)      // Configuration Register
    #define TCS34903_REG_COLOR_PPULSE                       (0x8E)      // Proximity Pulse Count Register
    #define TCS34903_REG_COLOR_CONTROL                      (0x8F)      // Control Register
    #define TCS34903_REG_COLOR_AUX                          (0x90)      // Auxiliary Control Register
    #define TCS34903_REG_COLOR_REVID                        (0x91)      // Revision ID Register
    #define TCS34903_REG_COLOR_DEVID                        (0x92)      // Device ID Register
    #define TCS34903_REG_COLOR_STATUS                       (0x93)      // Device Status Register
    #define TCS34903_REG_COLOR_CDATAL                       (0x94)      // Clear/IR ADC Low Data Register
    #define TCS34903_REG_COLOR_CDATAH                       (0x95)      // Clear/IR ADC High Data Register
    #define TCS34903_REG_COLOR_RDATAL                       (0x96)      // Red ADC Low Data Register
    #define TCS34903_REG_COLOR_RDATAH                       (0x97)      // Red ADC High Data Register
    #define TCS34903_REG_COLOR_GDATAL                       (0x98)      // Green ADC Low Data Register
    #define TCS34903_REG_COLOR_GDATAH                       (0x99)      // Green ADC High Data Register
    #define TCS34903_REG_COLOR_BDATAL                       (0x9A)      // Blue ADC Low Data Register
    #define TCS34903_REG_COLOR_BDATAH                       (0x9B)      // Blue ADC High Data Register
    #define TCS34903_REG_COLOR_IR                           (0xC0)      // Access IR Channel Register
    #define TCS34903_REG_COLOR_IFORCE                       (0xE4)      // Force Interrupt Register
    #define TCS34903_REG_COLOR_CICLEAR                      (0xE6)      // Clear Channel Interrupt Clear Register
    #define TCS34903_REG_COLOR_AICLEAR                      (0xE6)      // Clear All Interrupts Register

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_ENABLE_SAI_MASK              (0x40)      // Sleep After Interrupt
    #define TCS34903_REG_COLOR_ENABLE_SAI_NOT_ASSERT        (0x00)      // Not Asserted, the Device will Not Power Down at the End of a RGBC Cycle if an Interrupt has been Generated
    #define TCS34903_REG_COLOR_ENABLE_SAI_ASSERT            (0x40)      // Asserted, the Device will Power Down at the End of a RGBC Cycle if an Interrupt has been Generated

    #define TCS34903_REG_COLOR_ENABLE_AIEN_MASK             (0x10)      // Ambient Light Sensing (ALS) Interrupt Mask
    #define TCS34903_REG_COLOR_ENABLE_AIEN_NOT_ASSERT       (0x00)      // Not Asserted, Did Not Permits Ambient Light Sensing (ALS) Interrupts to be Generated
    #define TCS34903_REG_COLOR_ENABLE_AIEN_ASSERT           (0x10)      // Asserted, Permits Ambient Light Sensing (ALS) Interrupts to be Generated

    #define TCS34903_REG_COLOR_ENABLE_WEN_MASK              (0x08)      // Wait Enable
    #define TCS34903_REG_COLOR_ENABLE_WEN_DISABLE           (0x00)      // Disables the Wait Timer
    #define TCS34903_REG_COLOR_ENABLE_WEN_ENABLE            (0x08)      // Enables the Wait Timer

    #define TCS34903_REG_COLOR_ENABLE_AEN_MASK              (0x02)      // 4-Channel RGBC ADC Enable
    #define TCS34903_REG_COLOR_ENABLE_AEN_DISABLE           (0x00)      // Disables the RGBC ADC
    #define TCS34903_REG_COLOR_ENABLE_AEN_ENABLE            (0x02)      // Activates the RGBC ADC

    #define TCS34903_REG_COLOR_ENABLE_PON_MASK              (0x01)      // Power ON
    #define TCS34903_REG_COLOR_ENABLE_PON_DISABLE           (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TCS34903_REG_COLOR_ENABLE_PON_ENABLE            (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_ATIME_MASK                   (0xFF)      // Controls the Internal Integration Time of the RGBC Channel ADC’s in 2.78 ms Increments
    #define TCS34903_REG_COLOR_ATIME_2_78MS                 (0xFF)      // Cycles: 1, Time: 2.78 ms Max Count: 1024
    #define TCS34903_REG_COLOR_ATIME_27_8MS                 (0xF6)      // Cycles: 10, Time: 27.8 ms Max Count: 10240
    #define TCS34903_REG_COLOR_ATIME_103MS                  (0xDB)      // Cycles: 37, Time: 103 ms Max Count: 37888
    #define TCS34903_REG_COLOR_ATIME_178MS                  (0xC0)      // Cycles: 64, Time: 178 ms Max Count: 65535
    #define TCS34903_REG_COLOR_ATIME_712MS                  (0x00)      // Cycles: 256, Time: 712 ms Max Count: 65535

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_WTIME_MASK                   (0xFF)      // Wait Time is Set in 2.78 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TCS34903_REG_COLOR_WTIME_1                      (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.78 ms Time (WLONG = 1):  0.03 sec
    #define TCS34903_REG_COLOR_WTIME_85                     (0xB6)      // Wait Time: 85, Time (WLONG = 0): 236 ms Time (WLONG = 1):  2.84 sec
    #define TCS34903_REG_COLOR_WTIME_256                    (0x00)      // Wait Time: 256, Time (WLONG = 0): 712 ms Time (WLONG = 1):  8.54 sec

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_CONFIG_WLONG_MASK            (0x02)      // Wait Long
    #define TCS34903_REG_COLOR_CONFIG_WLONG_NOT_ASSERT      (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TCS34903_REG_COLOR_CONFIG_WLONG_ASSERT          (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_CONTROL_AGAIN_MASK           (0x03)      // RGBC Gain Control
    #define TCS34903_REG_COLOR_CONTROL_AGAIN_1X             (0x00)      // 1X Gain
    #define TCS34903_REG_COLOR_CONTROL_AGAIN_4X             (0x01)      // 4X Gain
    #define TCS34903_REG_COLOR_CONTROL_AGAIN_16X            (0x02)      // 16X Gain
    #define TCS34903_REG_COLOR_CONTROL_AGAIN_64X            (0x03)      // 64X Gain

/**************************************************************************
    COLOR LIGHT-TO-DIGITAL CONVERTER IR REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34903_REG_COLOR_IR_ACCESS_MASK               (0x80)      // IR Sensor Access
    #define TCS34903_REG_COLOR_IR_ACCESS_NOT_SET            (0x00)      // Clear Channel doesn't Reports the Measurement from the IR Sensor
    #define TCS34903_REG_COLOR_IR_ACCESS_SET                (0x80)      // Clear Channel Reports the Measurement from the IR Sensor



typedef enum
{
    SAI_NOT_ASSERT                      = TCS34903_REG_COLOR_ENABLE_SAI_NOT_ASSERT,
    SAI_ASSERT                          = TCS34903_REG_COLOR_ENABLE_SAI_ASSERT
    
} tcsSleepInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                     = TCS34903_REG_COLOR_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                         = TCS34903_REG_COLOR_ENABLE_AIEN_ASSERT
    
} tcsColorInterrupt_t;

typedef enum
{
    WEN_DISABLE                         = TCS34903_REG_COLOR_ENABLE_WEN_DISABLE,
    WEN_ENABLE                          = TCS34903_REG_COLOR_ENABLE_WEN_ENABLE
    
} tcsWaitEnable_t;

typedef enum
{
    AEN_DISABLE                         = TCS34903_REG_COLOR_ENABLE_AEN_DISABLE,
    AEN_ENABLE                          = TCS34903_REG_COLOR_ENABLE_AEN_ENABLE
    
} tcsColorEnable_t;

typedef enum
{
    PON_DISABLE                         = TCS34903_REG_COLOR_ENABLE_PON_DISABLE,
    PON_ENABLE                          = TCS34903_REG_COLOR_ENABLE_PON_ENABLE
    
} tcsPowerEnable_t;

typedef enum
{
    ATIME_2_78MS                        = TCS34903_REG_COLOR_ATIME_2_78MS,
    ATIME_27_8MS                        = TCS34903_REG_COLOR_ATIME_27_8MS,
    ATIME_103MS                         = TCS34903_REG_COLOR_ATIME_103MS,
    ATIME_178MS                         = TCS34903_REG_COLOR_ATIME_178MS,
    ATIME_712MS                         = TCS34903_REG_COLOR_ATIME_712MS
    
} tcsATime_t;

typedef enum
{
    WTIME_1                             = TCS34903_REG_COLOR_WTIME_1,
    WTIME_85                            = TCS34903_REG_COLOR_WTIME_85,
    WTIME_256                           = TCS34903_REG_COLOR_WTIME_256
    
} tcsWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                    = TCS34903_REG_COLOR_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                        = TCS34903_REG_COLOR_CONFIG_WLONG_ASSERT
    
} tcsWaitLong_t;

typedef enum
{
    AGAIN_1X                            = TCS34903_REG_COLOR_CONTROL_AGAIN_1X,
    AGAIN_4X                            = TCS34903_REG_COLOR_CONTROL_AGAIN_4X,
    AGAIN_16X                           = TCS34903_REG_COLOR_CONTROL_AGAIN_16X,
    AGAIN_64X                           = TCS34903_REG_COLOR_CONTROL_AGAIN_64X
    
} tcsColorGain_t;

typedef enum
{
    IR_ACCESS_NOT_SET                   = TCS34903_REG_COLOR_IR_ACCESS_NOT_SET,
    IR_ACCESS_SET                       = TCS34903_REG_COLOR_IR_ACCESS_SET
    
} tcsIRAccess_t;

typedef struct
{
    uint16_t C;
    uint16_t R;
    uint16_t G;
    uint16_t B;
    
} tcsSensorData_t;


class TCS34903
{
    protected:
        // Instance-specific properties
        uint8_t tcs_conversionDelay;
        tcsSleepInterrupt_t tcs_sleepinterrupt;
        tcsColorInterrupt_t tcs_colorinterrupt;
        tcsWaitEnable_t tcs_waitenable;
        tcsColorEnable_t tcs_colorenable;
        tcsPowerEnable_t tcs_powerenable;
        tcsATime_t tcs_atime;
        tcsWTime_t tcs_wtime;
        tcsWaitLong_t tcs_waitlong;
        tcsColorGain_t tcs_colorgain;
        tcsIRAccess_t tcs_iraccess;

    public:
        uint8_t tcs_i2cAddress;
        tcsSensorData_t tcs_colorData;
        void getAddr_TCS34903(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpColor(void);
        void Measure_Color(void);
        void setSleepInterrupt(tcsSleepInterrupt_t sleepinterrupt);
        tcsSleepInterrupt_t getSleepInterrupt(void);
        void setColorInterrupt(tcsColorInterrupt_t colorinterrupt);
        tcsColorInterrupt_t getColorInterrupt(void);
        void setWaitEnable(tcsWaitEnable_t waitenable);
        tcsWaitEnable_t getWaitEnable(void);
        void setColorEnable(tcsColorEnable_t colorenable);
        tcsColorEnable_t getColorEnable(void);
        void setPowerEnable(tcsPowerEnable_t powerenable);
        tcsPowerEnable_t getPowerEnable(void);
        void setATime(tcsATime_t atime);
        tcsATime_t getATime(void);
        void setWTime(tcsWTime_t wtime);
        tcsWTime_t getWTime(void);
        void setWaitLong(tcsWaitLong_t waitlong);
        tcsWaitLong_t getWaitLong(void);
        void setColorGain(tcsColorGain_t colorgain);
        tcsColorGain_t getColorGain(void);
        void setIRAccess(tcsIRAccess_t iraccess);
        tcsIRAccess_t getIRAccess(void);

    
    private:
};
