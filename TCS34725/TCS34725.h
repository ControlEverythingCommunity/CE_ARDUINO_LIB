/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34725
        This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34725_I2CS#tabs-0-product_tabset-2
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
    #define TCS34725_DEFAULT_ADDRESS                            (0x29)
    #define TCS34725_COLOR_DEV_ID                               (0x44)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TCS34725_CONVERSIONDELAY                            (100)

/**************************************************************************
    DIGITAL COLOR REGISTERS
**************************************************************************/
    #define TCS34725_REG_COLOR_COMMAND                                      // Specifies Register Address
    #define TCS34725_REG_COLOR_ENABLE                           (0x00)      // Enables States and Interrupts Register
    #define TCS34725_REG_COLOR_ATIME                            (0x01)      // RGBC Integration Time Register
    #define TCS34725_REG_COLOR_WTIME                            (0x03)      // Wait Time Register
    #define TCS34725_REG_COLOR_AILTL                            (0x04)      // Clear Interrupt Low Threshold Low Byte Register
    #define TCS34725_REG_COLOR_AILTH                            (0x05)      // Clear Interrupt Low Threshold High Byte Register
    #define TCS34725_REG_COLOR_AIHTL                            (0x06)      // Clear Interrupt High Threshold Low Byte Register
    #define TCS34725_REG_COLOR_AIHTH                            (0x07)      // Clear Interrupt High Threshold High Byte Register
    #define TCS34725_REG_COLOR_PERS                             (0x0C)      // Interrupt Persistence Filter Register
    #define TCS34725_REG_COLOR_CONFIG                           (0x0D)      // Configuration Register
    #define TCS34725_REG_COLOR_CONTROL                          (0x0F)      // Control Register
    #define TCS34725_REG_COLOR_ID                               (0x12)      // Device ID Register
    #define TCS34725_REG_COLOR_STATUS                           (0x13)      // Device Status Register
    #define TCS34725_REG_COLOR_CDATAL                           (0x14)      // Clear ADC Low Data Register
    #define TCS34725_REG_COLOR_CDATAH                           (0x15)      // Clear ADC High Data Register
    #define TCS34725_REG_COLOR_RDATAL                           (0x16)      // Red ADC Low Data Register
    #define TCS34725_REG_COLOR_RDATAH                           (0x17)      // Red ADC High Data Register
    #define TCS34725_REG_COLOR_GDATAL                           (0x18)      // Green ADC Low Data Register
    #define TCS34725_REG_COLOR_GDATAH                           (0x19)      // Green ADC High Data Register
    #define TCS34725_REG_COLOR_BDATAL                           (0x1A)      // Blue ADC Low Data Register
    #define TCS34725_REG_COLOR_BDATAH                           (0x1B)      // Blue ADC High Data Register

/**************************************************************************
    DIGITAL COLOR COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_CMD_MASK                         (0x80)      // Select Command Register
    #define TCS34725_REG_COLOR_CMD_SELECT                       (0x80)      // Addresses COMMAND Register
    #define TCS34725_REG_COLOR_CMD_TYPE_MASK                    (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TCS34725_REG_COLOR_CMD_TYPE_REP_BYTE                (0x00)      // Repeated Byte Protocol Transaction
    #define TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR               (0x20)      // Auto-Increment Protocol Transaction
    #define TCS34725_REG_COLOR_CMD_TYPE_SPECIAL                 (0x60)      // Special Function
    #define TCS34725_REG_COLOR_CMD_ADD_MASK                     (0x1F)      // Specifies a Special Function Command
    #define TCS34725_REG_COLOR_CMD_ADD_CLEAR_INTR               (0x06)      // Clear Interrupt Clear

/**************************************************************************
    DIGITAL COLOR ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_ENABLE_AIEN_MASK                 (0x10)      // Ambient Light Sensing (ALS) Interrupt Mask
    #define TCS34725_REG_COLOR_ENABLE_AIEN_NOT_ASSERT           (0x00)      // Not Asserted, Did Not Permits Ambient Light Sensing (ALS) Interrupts to be Generated
    #define TCS34725_REG_COLOR_ENABLE_AIEN_ASSERT               (0x10)      // Asserted, Permits Ambient Light Sensing (ALS) Interrupts to be Generated

    #define TCS34725_REG_COLOR_ENABLE_WEN_MASK                  (0x08)      // Wait Enable
    #define TCS34725_REG_COLOR_ENABLE_WEN_DISABLE               (0x00)      // Disables the Wait Timer
    #define TCS34725_REG_COLOR_ENABLE_WEN_ENABLE                (0x08)      // Enables the Wait Timer

    #define TCS34725_REG_COLOR_ENABLE_AEN_MASK                  (0x02)      // 4-Channel RGBC ADC Enable
    #define TCS34725_REG_COLOR_ENABLE_AEN_DISABLE               (0x00)      // Disables the RGBC ADC
    #define TCS34725_REG_COLOR_ENABLE_AEN_ENABLE                (0x02)      // Activates the RGBC ADC

    #define TCS34725_REG_COLOR_ENABLE_PON_MASK                  (0x01)      // Power ON
    #define TCS34725_REG_COLOR_ENABLE_PON_DISABLE               (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TCS34725_REG_COLOR_ENABLE_PON_ENABLE                (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL COLOR ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_ATIME_MASK                       (0xFF)      // Controls the Internal Integration Time of the RGBC Channel ADC’s in 2.4 ms Increments
    #define TCS34725_REG_COLOR_ATIME_2_4MS                      (0xFF)      // Cycles: 1, Time: 2.4 ms Max Count: 1024
    #define TCS34725_REG_COLOR_ATIME_24MS                       (0xF6)      // Cycles: 10, Time: 24 ms Max Count: 10240
    #define TCS34725_REG_COLOR_ATIME_101MS                      (0xDB)      // Cycles: 42, Time: 101 ms Max Count: 43008
    #define TCS34725_REG_COLOR_ATIME_154MS                      (0xC0)      // Cycles: 64, Time: 154 ms Max Count: 65535
    #define TCS34725_REG_COLOR_ATIME_700MS                      (0x00)      // Cycles: 256, Time: 700 ms Max Count: 65535

/**************************************************************************
    DIGITAL COLOR WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_WTIME_MASK                       (0xFF)      // Wait Time is Set in 2.4 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TCS34725_REG_COLOR_WTIME_1                          (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.4 ms Time (WLONG = 1):  0.029 sec
    #define TCS34725_REG_COLOR_WTIME_85                         (0xAB)      // Wait Time: 85, Time (WLONG = 0): 204 ms Time (WLONG = 1):  2.45 sec
    #define TCS34725_REG_COLOR_WTIME_256                        (0x00)      // Wait Time: 256, Time (WLONG = 0): 614 ms Time (WLONG = 1):  7.4 sec

/**************************************************************************
    DIGITAL COLOR PERSISTENCE REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_PERS_APERS_MASK                  (0x0F)      // Interrupt Persistence. Controls Rate of Interrupt to the Host Processor
    #define TCS34725_REG_COLOR_PERS_APERS_EVERY                 (0x00)      // Every RGBC Cycle Generates an Interrupt
    #define TCS34725_REG_COLOR_PERS_APERS_1                     (0x01)      // 1 Clear Channel Value Outside of Threshold Range
    #define TCS34725_REG_COLOR_PERS_APERS_2                     (0x02)      // 2 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_3                     (0x03)      // 3 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_5                     (0x04)      // 5 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_10                    (0x05)      // 10 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_15                    (0x06)      // 15 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_20                    (0x07)      // 20 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_25                    (0x09)      // 25 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_30                    (0x0A)      // 30 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_35                    (0x0A)      // 35 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_40                    (0x0B)      // 40 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_45                    (0x0C)      // 45 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_50                    (0x0D)      // 50 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_55                    (0x0E)      // 55 Clear Channels Consecutive Values out of Range
    #define TCS34725_REG_COLOR_PERS_APERS_60                    (0x0F)      // 60 Clear Channels Consecutive Values out of Range

/**************************************************************************
    DIGITAL COLOR CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_CONFIG_WLONG_MASK                (0x02)      // Wait Long
    #define TCS34725_REG_COLOR_CONFIG_WLONG_NOT_ASSERT          (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TCS34725_REG_COLOR_CONFIG_WLONG_ASSERT              (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    DIGITAL COLOR CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TCS34725_REG_COLOR_CONTROL_AGAIN_MASK               (0x03)      // RGBC Gain Control
    #define TCS34725_REG_COLOR_CONTROL_AGAIN_1X                 (0x00)      // 1X Gain
    #define TCS34725_REG_COLOR_CONTROL_AGAIN_4X                 (0x01)      // 4X Gain
    #define TCS34725_REG_COLOR_CONTROL_AGAIN_16X                (0x02)      // 16X Gain
    #define TCS34725_REG_COLOR_CONTROL_AGAIN_60X                (0x03)      // 60X Gain


typedef enum
{
    COMMAND_SELECT                      = TCS34725_REG_COLOR_CMD_SELECT,
    COMMAND_REP_BYTE                    = TCS34725_REG_COLOR_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR               = TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL                 = TCS34725_REG_COLOR_CMD_TYPE_SPECIAL,
    COMMAND_ADD_CLEAR_INTR              = TCS34725_REG_COLOR_CMD_ADD_CLEAR_INTR
    
} tcsCommand_t;

typedef enum
{
    AIEN_NOT_ASSERT                     = TCS34725_REG_COLOR_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                         = TCS34725_REG_COLOR_ENABLE_AIEN_ASSERT
    
} tcsColorInterrupt_t;

typedef enum
{
    WEN_DISABLE                         = TCS34725_REG_COLOR_ENABLE_WEN_DISABLE,
    WEN_ENABLE                          = TCS34725_REG_COLOR_ENABLE_WEN_ENABLE
    
} tcsWaitEnable_t;

typedef enum
{
    AEN_DISABLE                         = TCS34725_REG_COLOR_ENABLE_AEN_DISABLE,
    AEN_ENABLE                          = TCS34725_REG_COLOR_ENABLE_AEN_ENABLE
    
} tcsColorEnable_t;

typedef enum
{
    PON_DISABLE                         = TCS34725_REG_COLOR_ENABLE_PON_DISABLE,
    PON_ENABLE                          = TCS34725_REG_COLOR_ENABLE_PON_ENABLE
    
} tcsPowerEnable_t;

typedef enum
{
    ATIME_2_4MS                         = TCS34725_REG_COLOR_ATIME_2_4MS,
    ATIME_24MS                          = TCS34725_REG_COLOR_ATIME_24MS,
    ATIME_101MS                         = TCS34725_REG_COLOR_ATIME_101MS,
    ATIME_154MS                         = TCS34725_REG_COLOR_ATIME_154MS,
    ATIME_700MS                         = TCS34725_REG_COLOR_ATIME_700MS
    
} tcsATime_t;

typedef enum
{
    WTIME_1                             = TCS34725_REG_COLOR_WTIME_1,
    WTIME_85                            = TCS34725_REG_COLOR_WTIME_85,
    WTIME_256                           = TCS34725_REG_COLOR_WTIME_256
    
} tcsWTime_t;

typedef enum
{
    APERS_EVERY                         = TCS34725_REG_COLOR_PERS_APERS_EVERY,
    APERS_1                             = TCS34725_REG_COLOR_PERS_APERS_1,
    APERS_2                             = TCS34725_REG_COLOR_PERS_APERS_2,
    APERS_3                             = TCS34725_REG_COLOR_PERS_APERS_3,
    APERS_5                             = TCS34725_REG_COLOR_PERS_APERS_5,
    APERS_10                            = TCS34725_REG_COLOR_PERS_APERS_10,
    APERS_15                            = TCS34725_REG_COLOR_PERS_APERS_15,
    APERS_20                            = TCS34725_REG_COLOR_PERS_APERS_20,
    APERS_25                            = TCS34725_REG_COLOR_PERS_APERS_25,
    APERS_30                            = TCS34725_REG_COLOR_PERS_APERS_30,
    APERS_35                            = TCS34725_REG_COLOR_PERS_APERS_35,
    APERS_40                            = TCS34725_REG_COLOR_PERS_APERS_40,
    APERS_45                            = TCS34725_REG_COLOR_PERS_APERS_45,
    APERS_50                            = TCS34725_REG_COLOR_PERS_APERS_50,
    APERS_55                            = TCS34725_REG_COLOR_PERS_APERS_55,
    APERS_60                            = TCS34725_REG_COLOR_PERS_APERS_60
    
} tcsPersistence_t;

typedef enum
{
    WLONG_NOT_ASSERT                    = TCS34725_REG_COLOR_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                        = TCS34725_REG_COLOR_CONFIG_WLONG_ASSERT
    
} tcsWaitLong_t;

typedef enum
{
    AGAIN_1X                            = TCS34725_REG_COLOR_CONTROL_AGAIN_1X,
    AGAIN_4X                            = TCS34725_REG_COLOR_CONTROL_AGAIN_4X,
    AGAIN_16X                           = TCS34725_REG_COLOR_CONTROL_AGAIN_16X,
    AGAIN_60X                           = TCS34725_REG_COLOR_CONTROL_AGAIN_60X
    
} tcsColorGain_t;

typedef struct
{
    uint16_t C;
    uint16_t R;
    uint16_t G;
    uint16_t B;
    
} tcsSensorData_t;


class TCS34725
{
    protected:
        // Instance-specific properties
        uint8_t tcs_conversionDelay;
        tcsCommand_t tcs_command;
        tcsColorInterrupt_t tcs_colorinterrupt;
        tcsWaitEnable_t tcs_waitenable;
        tcsColorEnable_t tcs_colorenable;
        tcsPowerEnable_t tcs_powerenable;
        tcsATime_t tcs_atime;
        tcsWTime_t tcs_wtime;
        tcsPersistence_t tcs_persistence;
        tcsWaitLong_t tcs_waitlong;
        tcsColorGain_t tcs_colorgain;

    public:
        uint8_t tcs_i2cAddress;
        tcsSensorData_t tcs_colorData;
        void getAddr_TCS34725(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_Color(void);
        void setCommand(tcsCommand_t command);
        tcsCommand_t getCommand(void);
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
        void setPersistence(tcsPersistence_t persistence);
        tcsPersistence_t getPersistence(void);
        void setWaitLong(tcsWaitLong_t waitlong);
        tcsWaitLong_t getWaitLong(void);
        void setColorGain(tcsColorGain_t colorgain);
        tcsColorGain_t getColorGain(void);
    
    private:
};
