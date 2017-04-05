/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2571
        This code is designed to work with the TSL2571_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2571_I2CS#tabs-0-product_tabset-2
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
    #define TSL2571_DEFAULT_ADDRESS                         (0x39)
    #define TSL2571_ALS_DEV_ID                              (0x04)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL2571_CONVERSIONDELAY                         (100)

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER REGISTERS
**************************************************************************/
    #define TSL2571_REG_ALS_COMMAND                                     // Specifies Register Address
    #define TSL2571_REG_ALS_ENABLE                          (0x00)      // Enables States and Interrupts Register
    #define TSL2571_REG_ALS_ATIME                           (0x01)      // Integration Time Register
    #define TSL2571_REG_ALS_WTIME                           (0x03)      // Wait Time Register
    #define TSL2571_REG_ALS_AILTL                           (0x04)      // ALS Interrupt Low Threshold Low Byte Register
    #define TSL2571_REG_ALS_AILTH                           (0x05)      // ALS Interrupt Low Threshold High Byte Register
    #define TSL2571_REG_ALS_AIHTL                           (0x06)      // ALS Interrupt High Threshold Low Byte Register
    #define TSL2571_REG_ALS_AIHTH                           (0x07)      // ALS Interrupt High Threshold High Byte Register
    #define TSL2571_REG_ALS_PERS                            (0x0C)      // Interrupt Persistence Filters Register
    #define TSL2571_REG_ALS_CONFIG                          (0x0D)      // Configuration Register
    #define TSL2571_REG_ALS_CONTROL                         (0x0F)      // Control Register
    #define TSL2571_REG_ALS_ID                              (0x12)      // Device ID Register
    #define TSL2571_REG_ALS_STATUS                          (0x13)      // Device Status Register
    #define TSL2571_REG_ALS_C0DATAL                         (0x14)      // CH0 ADC Low Data Register
    #define TSL2571_REG_ALS_C0DATAH                         (0x15)      // CH0 ADC High Data Register
    #define TSL2571_REG_ALS_C1DATAL                         (0x16)      // CH1 ADC Low Data Register
    #define TSL2571_REG_ALS_C1DATAH                         (0x17)      // CH1 ADC High Data Register

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_CMD_MASK                        (0x80)      // Select Command Register
    #define TSL2571_REG_ALS_CMD_SELECT                      (0x80)      // Addresses COMMAND Register
    #define TSL2571_REG_ALS_CMD_TYPE_MASK                   (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TSL2571_REG_ALS_CMD_TYPE_REP_BYTE               (0x00)      // Repeated Byte Protocol Transaction
    #define TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR              (0x20)      // Auto-Increment Protocol Transaction
    #define TSL2571_REG_ALS_CMD_TYPE_SPECIAL                (0x60)      // Special Function
    #define TSL2571_REG_ALS_CMD_ADD_MASK                    (0x1F)      // Specifies a Special Function Command
    #define TSL2571_REG_ALS_CMD_ADD_NORMAL                  (0x00)      // Normal — No Action
    #define TSL2571_REG_ALS_CMD_ADD_ALS_INTR                (0x06)      // ALS Interrupt Clear

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_ENABLE_AIEN_MASK                (0x10)      // ALS Interrupt Mask
    #define TSL2571_REG_ALS_ENABLE_AIEN_NOT_ASSERT          (0x00)      // Not Asserted, Did Not Permits ALS Interrupts to be Generated
    #define TSL2571_REG_ALS_ENABLE_AIEN_ASSERT              (0x10)      // Asserted, Permits ALS Interrupts to be Generated

    #define TSL2571_REG_ALS_ENABLE_WEN_MASK                 (0x08)      // Wait Enable
    #define TSL2571_REG_ALS_ENABLE_WEN_DISABLE              (0x00)      // Disables the Wait Timer
    #define TSL2571_REG_ALS_ENABLE_WEN_ENABLE               (0x08)      // Enables the Wait Timer

    #define TSL2571_REG_ALS_ENABLE_AEN_MASK                 (0x02)      // ALS Enable
    #define TSL2571_REG_ALS_ENABLE_AEN_DISABLE              (0x00)      // Disables the ALS
    #define TSL2571_REG_ALS_ENABLE_AEN_ENABLE               (0x02)      // Activates the ALS

    #define TSL2571_REG_ALS_ENABLE_PON_MASK                 (0x01)      // Power ON
    #define TSL2571_REG_ALS_ENABLE_PON_DISABLE              (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TSL2571_REG_ALS_ENABLE_PON_ENABLE               (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_ATIME_MASK                      (0xFF)      // Controls the Internal Integration Time of the ALS Channel ADC’s in 2.72 ms Increments
    #define TSL2571_REG_ALS_ATIME_2_72MS                    (0xFF)      // Integration Cycles: 1, Time: 2.72 ms Max Count: 1024
    #define TSL2571_REG_ALS_ATIME_27_2MS                    (0xF6)      // Integration Cycles: 10, Time: 27.2 ms Max Count: 10240
    #define TSL2571_REG_ALS_ATIME_101MS                     (0xDB)      // Integration Cycles: 37, Time: 101 ms Max Count: 37888
    #define TSL2571_REG_ALS_ATIME_174MS                     (0xC0)      // Integration Cycles: 64, Time: 174 ms Max Count: 65535
    #define TSL2571_REG_ALS_ATIME_696MS                     (0x00)      // Integration Cycles: 256, Time: 696 ms Max Count: 65535

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_WTIME_MASK                      (0xFF)      // Wait Time is Set in 2.72 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TSL2571_REG_ALS_WTIME_1                         (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    #define TSL2571_REG_ALS_WTIME_74                        (0xB6)      // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    #define TSL2571_REG_ALS_WTIME_256                       (0x00)      // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_CONFIG_WLONG_MASK               (0x02)      // Wait Long
    #define TSL2571_REG_ALS_CONFIG_WLONG_NOT_ASSERT         (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TSL2571_REG_ALS_CONFIG_WLONG_ASSERT             (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2571_REG_ALS_CONTROL_AGAIN_MASK              (0x03)      // ALS Gain
    #define TSL2571_REG_ALS_CONTROL_AGAIN_1X                (0x00)      // 1X Gain
    #define TSL2571_REG_ALS_CONTROL_AGAIN_8X                (0x01)      // 8X Gain
    #define TSL2571_REG_ALS_CONTROL_AGAIN_16X               (0x02)      // 16X Gain
    #define TSL2571_REG_ALS_CONTROL_AGAIN_120X              (0x03)      // 120X Gain


typedef enum
{
    COMMAND_SELECT                  = TSL2571_REG_ALS_CMD_SELECT,
    COMMAND_REP_BYTE                = TSL2571_REG_ALS_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TSL2571_REG_ALS_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TSL2571_REG_ALS_CMD_ADD_NORMAL,
    COMMAND_ADD_ALS_INTR            = TSL2571_REG_ALS_CMD_ADD_ALS_INTR,
    
} tslCommand_t;

typedef enum
{
    AIEN_NOT_ASSERT                 = TSL2571_REG_ALS_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                     = TSL2571_REG_ALS_ENABLE_AIEN_ASSERT
    
} tslALSInterrupt_t;

typedef enum
{
    WEN_DISABLE                     = TSL2571_REG_ALS_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TSL2571_REG_ALS_ENABLE_WEN_ENABLE
    
} tslWaitEnable_t;

typedef enum
{
    AEN_DISABLE                     = TSL2571_REG_ALS_ENABLE_AEN_DISABLE,
    AEN_ENABLE                      = TSL2571_REG_ALS_ENABLE_AEN_ENABLE
    
} tslALSEnable_t;

typedef enum
{
    PON_DISABLE                     = TSL2571_REG_ALS_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TSL2571_REG_ALS_ENABLE_PON_ENABLE
    
} tslPowerEnable_t;

typedef enum
{
    ATIME_2_72MS                    = TSL2571_REG_ALS_ATIME_2_72MS,
    ATIME_27_2MS                    = TSL2571_REG_ALS_ATIME_27_2MS,
    ATIME_101MS                     = TSL2571_REG_ALS_ATIME_101MS,
    ATIME_174MS                     = TSL2571_REG_ALS_ATIME_174MS,
    ATIME_696MS                     = TSL2571_REG_ALS_ATIME_696MS
    
} tslATime_t;

typedef enum
{
    WTIME_1                         = TSL2571_REG_ALS_WTIME_1,
    WTIME_74                        = TSL2571_REG_ALS_WTIME_74,
    WTIME_256                       = TSL2571_REG_ALS_WTIME_256
    
} tslWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TSL2571_REG_ALS_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TSL2571_REG_ALS_CONFIG_WLONG_ASSERT
    
} tslWaitLong_t;

typedef enum
{
    AGAIN_1X                        = TSL2571_REG_ALS_CONTROL_AGAIN_1X,
    AGAIN_8X                        = TSL2571_REG_ALS_CONTROL_AGAIN_8X,
    AGAIN_16X                       = TSL2571_REG_ALS_CONTROL_AGAIN_16X,
    AGAIN_120X                      = TSL2571_REG_ALS_CONTROL_AGAIN_120X
    
} tslALSGain_t;

typedef struct
{
    float L;
    
} tslSensorData_t;


class TSL2571
{
    protected:
        // Instance-specific properties
        uint8_t tsl_conversionDelay;
        tslCommand_t tsl_command;
        tslALSInterrupt_t tsl_alsinterrupt;
        tslWaitEnable_t tsl_waitenable;
        tslALSEnable_t tsl_alsenable;
        tslPowerEnable_t tsl_powerenable;
        tslATime_t tsl_atime;
        tslWTime_t tsl_wtime;
        tslWaitLong_t tsl_waitlong;
        tslALSGain_t tsl_alsgain;

    public:
        uint8_t tsl_i2cAddress;
        tslSensorData_t tsl_alsData;
        void getAddr_TSL2571(uint8_t i2cAddress);
        bool begin(void);
        void setUpALS(void);
        void Measure_ALS(void);
        void setCommand(tslCommand_t command);
        tslCommand_t getCommand(void);
        void setALSInterrupt(tslALSInterrupt_t alsinterrupt);
        tslALSInterrupt_t getALSInterrupt(void);
        void setWaitEnable(tslWaitEnable_t waitenable);
        tslWaitEnable_t getWaitEnable(void);
        void setALSEnable(tslALSEnable_t alsenable);
        tslALSEnable_t getALSEnable(void);
        void setPowerEnable(tslPowerEnable_t powerenable);
        tslPowerEnable_t getPowerEnable(void);
        void setATime(tslATime_t atime);
        tslATime_t getATime(void);
        void setWTime(tslWTime_t wtime);
        tslWTime_t getWTime(void);
        void setWaitLong(tslWaitLong_t waitlong);
        tslWaitLong_t getWaitLong(void);
        void setALSGain(tslALSGain_t alsgain);
        tslALSGain_t getALSGain(void);
    
    private:
};
