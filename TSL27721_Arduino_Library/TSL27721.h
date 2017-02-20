/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL27721
        This code is designed to work with the TSL27721_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
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
    #define TSL27721_DEFAULT_ADDRESS                        (0x39)
    #define TSL27721_ALSPROX_DEV_ID                         (0x30)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL27721_CONVERSIONDELAY                        (100)

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TSL27721_REG_ALSPROX_COMMAND                                // Specifies Register Address
    #define TSL27721_REG_ALSPROX_ENABLE                     (0x00)      // Enables States and Interrupts Register
    #define TSL27721_REG_ALSPROX_ATIME                      (0x01)      // ALS Integration Time Register
    #define TSL27721_REG_ALSPROX_PTIME                      (0x02)      // Proximity ADC Time Register
    #define TSL27721_REG_ALSPROX_WTIME                      (0x03)      // Wait Time Register
    #define TSL27721_REG_ALSPROX_AILTL                      (0x04)      // ALS Interrupt Low Threshold Low Byte Register
    #define TSL27721_REG_ALSPROX_AILTH                      (0x05)      // ALS Interrupt Low Threshold High Byte Register
    #define TSL27721_REG_ALSPROX_AIHTL                      (0x06)      // ALS Interrupt High Threshold Low Byte Register
    #define TSL27721_REG_ALSPROX_AIHTH                      (0x07)      // ALS Interrupt High Threshold High Byte Register
    #define TSL27721_REG_ALSPROX_PILTL                      (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TSL27721_REG_ALSPROX_PILTH                      (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TSL27721_REG_ALSPROX_PIHTL                      (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TSL27721_REG_ALSPROX_PIHTH                      (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TSL27721_REG_ALSPROX_PERS                       (0x0C)      // Interrupt Persistence Filters Register
    #define TSL27721_REG_ALSPROX_CONFIG                     (0x0D)      // Configuration Register
    #define TSL27721_REG_ALSPROX_PPULSE                     (0x0E)      // Proximity Pulse Count Register
    #define TSL27721_REG_ALSPROX_CONTROL                    (0x0F)      // Control Register
    #define TSL27721_REG_ALSPROX_REVISION                   (0x11)      // Die Revision Number Register
    #define TSL27721_REG_ALSPROX_ID                         (0x12)      // Device ID Register
    #define TSL27721_REG_ALSPROX_STATUS                     (0x13)      // Device Status Register
    #define TSL27721_REG_ALSPROX_C0DATAL                    (0x14)      // CH0 ADC Low Data Register
    #define TSL27721_REG_ALSPROX_C0DATAH                    (0x15)      // CH0 ADC High Data Register
    #define TSL27721_REG_ALSPROX_C1DATAL                    (0x16)      // CH1 ADC Low Data Register
    #define TSL27721_REG_ALSPROX_C1DATAH                    (0x17)      // CH1 ADC High Data Register
    #define TSL27721_REG_ALSPROX_PDATAL                     (0x18)      // Proximity ADC Low Data Register
    #define TSL27721_REG_ALSPROX_PDATAH                     (0x19)      // Proximity ADC High Data Register
    #define TSL27721_REG_ALSPROX_POFFSET                    (0x1E)      // Proximity Offset Register

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_CMD_MASK                   (0x80)      // Select Command Register
    #define TSL27721_REG_ALSPROX_CMD_SELECT                 (0x80)      // Addresses COMMAND Register
    #define TSL27721_REG_ALSPROX_CMD_TYPE_MASK              (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TSL27721_REG_ALSPROX_CMD_TYPE_REP_BYTE          (0x00)      // Repeated Byte Protocol Transaction
    #define TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR         (0x20)      // Auto-Increment Protocol Transaction
    #define TSL27721_REG_ALSPROX_CMD_TYPE_SPECIAL           (0x60)      // Special Function
    #define TSL27721_REG_ALSPROX_CMD_ADD_MASK               (0x1F)      // Specifies a Special Function Command
    #define TSL27721_REG_ALSPROX_CMD_ADD_NORMAL             (0x00)      // Normal — No Action
    #define TSL27721_REG_ALSPROX_CMD_ADD_PROX_INTR          (0x05)      // Proximity Interrupt Clear
    #define TSL27721_REG_ALSPROX_CMD_ADD_ALS_INTR           (0x06)      // ALS Interrupt Clear
    #define TSL27721_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR      (0x07)      // Proximity and ALS Interrupt Clear

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_ENABLE_SAI_MASK            (0x40)      // Sleep After Interrupt
    #define TSL27721_REG_ALSPROX_ENABLE_SAI_NOT_ASSERT      (0x00)      // Not Asserted, the Device will Not Power Down at the End of a Proximity or ALS Cycle if an Interrupt has been Generated
    #define TSL27721_REG_ALSPROX_ENABLE_SAI_ASSERT          (0x40)      // Asserted, the Device will Power Down at the End of a Proximity or ALS Cycle if an Interrupt has been Generated

    #define TSL27721_REG_ALSPROX_ENABLE_PIEN_MASK           (0x20)      // Proximity Interrupt Mask
    #define TSL27721_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT     (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TSL27721_REG_ALSPROX_ENABLE_PIEN_ASSERT         (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TSL27721_REG_ALSPROX_ENABLE_AIEN_MASK           (0x10)      // ALS Interrupt Mask
    #define TSL27721_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT     (0x00)      // Not Asserted, Did Not Permits ALS Interrupts to be Generated
    #define TSL27721_REG_ALSPROX_ENABLE_AIEN_ASSERT         (0x10)      // Asserted, Permits ALS Interrupts to be Generated

    #define TSL27721_REG_ALSPROX_ENABLE_WEN_MASK            (0x08)      // Wait Enable
    #define TSL27721_REG_ALSPROX_ENABLE_WEN_DISABLE         (0x00)      // Disables the Wait Timer
    #define TSL27721_REG_ALSPROX_ENABLE_WEN_ENABLE          (0x08)      // Enables the Wait Timer

    #define TSL27721_REG_ALSPROX_ENABLE_PEN_MASK            (0x04)      // Proximity Enable
    #define TSL27721_REG_ALSPROX_ENABLE_PEN_DISABLE         (0x00)      // Deactivates the Proximity
    #define TSL27721_REG_ALSPROX_ENABLE_PEN_ENABLE          (0x04)      // Activates the Proximity

    #define TSL27721_REG_ALSPROX_ENABLE_AEN_MASK            (0x02)      // ALS Enable
    #define TSL27721_REG_ALSPROX_ENABLE_AEN_DISABLE         (0x00)      // Disables the ALS
    #define TSL27721_REG_ALSPROX_ENABLE_AEN_ENABLE          (0x02)      // Activates the ALS

    #define TSL27721_REG_ALSPROX_ENABLE_PON_MASK            (0x01)      // Power ON
    #define TSL27721_REG_ALSPROX_ENABLE_PON_DISABLE         (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TSL27721_REG_ALSPROX_ENABLE_PON_ENABLE          (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_ATIME_MASK                 (0xFF)      // Controls the Internal Integration Time of the ALS Channel ADC’s in 2.73 ms Increments
    #define TSL27721_REG_ALSPROX_ATIME_2_73MS               (0xFF)      // Cycles: 1, Time: 2.73 ms Max Count: 1024
    #define TSL27721_REG_ALSPROX_ATIME_27_3MS               (0xF6)      // Cycles: 10, Time: 27.3 ms Max Count: 10240
    #define TSL27721_REG_ALSPROX_ATIME_101MS                (0xDB)      // Cycles: 37, Time: 101 ms Max Count: 37888
    #define TSL27721_REG_ALSPROX_ATIME_175MS                (0xC0)      // Cycles: 64, Time: 175 ms Max Count: 65535
    #define TSL27721_REG_ALSPROX_ATIME_699MS                (0x00)      // Cycles: 256, Time: 699 ms Max Count: 65535

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE PTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_PTIME_MASK                 (0xFF)      // Controls the Internal Integration Time of the Proximity ADC’s in 2.73 ms Increments
    #define TSL27721_REG_ALSPROX_PTIME_2_73MS               (0xFF)      // Cycles: 1, Time: 2.73 ms Max Count: 1023

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_WTIME_MASK                 (0xFF)      // Wait Time is Set in 2.73 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TSL27721_REG_ALSPROX_WTIME_1                    (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.73 ms Time (WLONG = 1):  0.033 sec
    #define TSL27721_REG_ALSPROX_WTIME_74                   (0xB6)      // Wait Time: 74, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.4 sec
    #define TSL27721_REG_ALSPROX_WTIME_256                  (0x00)      // Wait Time: 256, Time (WLONG = 0): 699 ms Time (WLONG = 1):  8.4 sec

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_CONFIG_AGL_MASK            (0x04)      // ALS Gain Level
    #define TSL27721_REG_ALSPROX_CONFIG_AGL_NOT_ASSERT      (0x00)      // Not Asserted, the 1X and 8X ALS Gain (AGAIN) Modes are Scaled by 1
    #define TSL27721_REG_ALSPROX_CONFIG_AGL_ASSERT          (0x04)      // Asserted, the 1X and 8X ALS Gain (AGAIN) Modes are Scaled by 0.16

    #define TSL27721_REG_ALSPROX_CONFIG_WLONG_MASK          (0x02)      // Wait Long
    #define TSL27721_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT    (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TSL27721_REG_ALSPROX_CONFIG_WLONG_ASSERT        (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

    #define TSL27721_REG_ALSPROX_CONFIG_PDL_MASK            (0x01)      // Proximity Drive Level
    #define TSL27721_REG_ALSPROX_CONFIG_PDL_NOT_ASSERT      (0x00)      // Not Asserted, the Proximity LDR Drive Current is Not Reduced by 9
    #define TSL27721_REG_ALSPROX_CONFIG_PDL_ASSERT          (0x01)      // Asserted, the Proximity LDR Drive Current is Reduced by 9

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL27721_REG_ALSPROX_CONTROL_PDRIVE_MASK        (0xC0)      // Proximity LED Drive Strength
    #define TSL27721_REG_ALSPROX_CONTROL_PDRIVE_120         (0x00)      // LED Strength – PDL=0: 120mA, LED Strength – PDL=1: 15mA
    #define TSL27721_REG_ALSPROX_CONTROL_PDRIVE_60          (0x40)      // LED Strength – PDL=0: 60mA, LED Strength – PDL=1: 7.5mA
    #define TSL27721_REG_ALSPROX_CONTROL_PDRIVE_30          (0x80)      // LED Strength – PDL=0: 30mA, LED Strength – PDL=1: 3.8mA
    #define TSL27721_REG_ALSPROX_CONTROL_PDRIVE_15          (0xC0)      // LED Strength – PDL=0: 15mA, LED Strength – PDL=1: 1.9mA

    #define TSL27721_REG_ALSPROX_CONTROL_PDIODE_MASK        (0x30)      // Proximity Diode Selector
    #define TSL27721_REG_ALSPROX_CONTROL_PDIODE_NONE        (0x00)      // Proximity Uses Neither Diode
    #define TSL27721_REG_ALSPROX_CONTROL_PDIODE_CH0         (0x10)      // Proximity Uses the CH0 Diode
    #define TSL27721_REG_ALSPROX_CONTROL_PDIODE_CH1         (0x20)      // Proximity Uses the CH1 Diode

    #define TSL27721_REG_ALSPROX_CONTROL_PGAIN_MASK         (0x0C)      // Proximity Gain
    #define TSL27721_REG_ALSPROX_CONTROL_PGAIN_1X           (0x00)      // 1X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_PGAIN_2X           (0x04)      // 2X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_PGAIN_4X           (0x08)      // 4X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_PGAIN_8X           (0x0C)      // 8X Gain

    #define TSL27721_REG_ALSPROX_CONTROL_AGAIN_MASK         (0x03)      // ALS Gain
    #define TSL27721_REG_ALSPROX_CONTROL_AGAIN_1X           (0x00)      // 1X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_AGAIN_8X           (0x01)      // 8X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_AGAIN_16X          (0x02)      // 16X Gain
    #define TSL27721_REG_ALSPROX_CONTROL_AGAIN_120X         (0x03)      // 120X Gain


typedef enum
{
    COMMAND_SELECT                  = TSL27721_REG_ALSPROX_CMD_SELECT,
    COMMAND_REP_BYTE                = TSL27721_REG_ALSPROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TSL27721_REG_ALSPROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TSL27721_REG_ALSPROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR           = TSL27721_REG_ALSPROX_CMD_ADD_PROX_INTR,
    COMMAND_ADD_ALS_INTR            = TSL27721_REG_ALSPROX_CMD_ADD_ALS_INTR,
    COMMAND_ADD_PROX_ALS_INTR       = TSL27721_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR
    
} tslCommand_t;

typedef enum
{
    SAI_NOT_ASSERT                  = TSL27721_REG_ALSPROX_ENABLE_SAI_NOT_ASSERT,
    SAI_ASSERT                      = TSL27721_REG_ALSPROX_ENABLE_SAI_ASSERT
    
} tslSleepInterrupt_t;

typedef enum
{
    PIEN_NOT_ASSERT                 = TSL27721_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                     = TSL27721_REG_ALSPROX_ENABLE_PIEN_ASSERT
    
} tslProximityInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                 = TSL27721_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                     = TSL27721_REG_ALSPROX_ENABLE_AIEN_ASSERT
    
} tslALSInterrupt_t;

typedef enum
{
    WEN_DISABLE                     = TSL27721_REG_ALSPROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TSL27721_REG_ALSPROX_ENABLE_WEN_ENABLE
    
} tslWaitEnable_t;

typedef enum
{
    PEN_DISABLE                     = TSL27721_REG_ALSPROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                      = TSL27721_REG_ALSPROX_ENABLE_PEN_ENABLE
    
} tslProximityEnable_t;

typedef enum
{
    AEN_DISABLE                     = TSL27721_REG_ALSPROX_ENABLE_AEN_DISABLE,
    AEN_ENABLE                      = TSL27721_REG_ALSPROX_ENABLE_AEN_ENABLE
    
} tslALSEnable_t;

typedef enum
{
    PON_DISABLE                     = TSL27721_REG_ALSPROX_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TSL27721_REG_ALSPROX_ENABLE_PON_ENABLE
    
} tslPowerEnable_t;

typedef enum
{
    ATIME_2_73MS                    = TSL27721_REG_ALSPROX_ATIME_2_73MS,
    ATIME_27_3MS                    = TSL27721_REG_ALSPROX_ATIME_27_3MS,
    ATIME_101MS                     = TSL27721_REG_ALSPROX_ATIME_101MS,
    ATIME_175MS                     = TSL27721_REG_ALSPROX_ATIME_175MS,
    ATIME_699MS                     = TSL27721_REG_ALSPROX_ATIME_699MS
    
} tslATime_t;

typedef enum
{
    PTIME_2_73MS                    = TSL27721_REG_ALSPROX_PTIME_2_73MS
    
} tslPTime_t;

typedef enum
{
    WTIME_1                         = TSL27721_REG_ALSPROX_WTIME_1,
    WTIME_74                        = TSL27721_REG_ALSPROX_WTIME_74,
    WTIME_256                       = TSL27721_REG_ALSPROX_WTIME_256
    
} tslWTime_t;

typedef enum
{
    AGL_NOT_ASSERT                  = TSL27721_REG_ALSPROX_CONFIG_AGL_NOT_ASSERT,
    AGL_ASSERT                      = TSL27721_REG_ALSPROX_CONFIG_AGL_ASSERT
    
} tslALSGainLevel_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TSL27721_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TSL27721_REG_ALSPROX_CONFIG_WLONG_ASSERT
    
} tslWaitLong_t;

typedef enum
{
    PDL_NOT_ASSERT                  = TSL27721_REG_ALSPROX_CONFIG_PDL_NOT_ASSERT,
    PDL_ASSERT                      = TSL27721_REG_ALSPROX_CONFIG_PDL_ASSERT
    
} tslProximityDriveLevel_t;

typedef enum
{
    PDRIVE_120                      = TSL27721_REG_ALSPROX_CONTROL_PDRIVE_120,
    PDRIVE_60                       = TSL27721_REG_ALSPROX_CONTROL_PDRIVE_60,
    PDRIVE_30                       = TSL27721_REG_ALSPROX_CONTROL_PDRIVE_30,
    PDRIVE_15                       = TSL27721_REG_ALSPROX_CONTROL_PDRIVE_15
    
} tslProximityDrive_t;

typedef enum
{
    PDIODE_NONE                     = TSL27721_REG_ALSPROX_CONTROL_PDIODE_NONE,
    PDIODE_CH0                      = TSL27721_REG_ALSPROX_CONTROL_PDIODE_CH0,
    PDIODE_CH1                      = TSL27721_REG_ALSPROX_CONTROL_PDIODE_CH1
    
} tslProximityDiode_t;

typedef enum
{
    PGAIN_1X                        = TSL27721_REG_ALSPROX_CONTROL_PGAIN_1X,
    PGAIN_2X                        = TSL27721_REG_ALSPROX_CONTROL_PGAIN_2X,
    PGAIN_4X                        = TSL27721_REG_ALSPROX_CONTROL_PGAIN_4X,
    PGAIN_8X                        = TSL27721_REG_ALSPROX_CONTROL_PGAIN_8X
    
} tslProximityGain_t;

typedef enum
{
    AGAIN_1X                        = TSL27721_REG_ALSPROX_CONTROL_AGAIN_1X,
    AGAIN_8X                        = TSL27721_REG_ALSPROX_CONTROL_AGAIN_8X,
    AGAIN_16X                       = TSL27721_REG_ALSPROX_CONTROL_AGAIN_16X,
    AGAIN_120X                      = TSL27721_REG_ALSPROX_CONTROL_AGAIN_120X
    
} tslALSGain_t;

typedef struct
{
    float L;
    float P;
    
} tslSensorData_t;


class TSL27721
{
    protected:
        // Instance-specific properties
        uint8_t tsl_conversionDelay;
        tslCommand_t tsl_command;
        tslSleepInterrupt_t tsl_sleepinterrupt;
        tslProximityInterrupt_t tsl_proximityinterrupt;
        tslALSInterrupt_t tsl_alsinterrupt;
        tslWaitEnable_t tsl_waitenable;
        tslProximityEnable_t tsl_proximityenable;
        tslALSEnable_t tsl_alsenable;
        tslPowerEnable_t tsl_powerenable;
        tslATime_t tsl_atime;
        tslPTime_t tsl_ptime;
        tslWTime_t tsl_wtime;
        tslALSGainLevel_t tsl_alsgainlevel;
        tslWaitLong_t tsl_waitlong;
        tslProximityDriveLevel_t tsl_proximitydrivelevel;
        tslProximityDrive_t tsl_proximitydrive;
        tslProximityDiode_t tsl_proximitydiode;
        tslProximityGain_t tsl_proximitygain;
        tslALSGain_t tsl_alsgain;

    public:
        uint8_t tsl_i2cAddress;
        tslSensorData_t tsl_alsproxData;
        void getAddr_TSL27721(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpALSProximity(void);
        void Measure_ALS(void);
        void Measure_Proximity(void);
        void setCommand(tslCommand_t command);
        tslCommand_t getCommand(void);
        void setSleepInterrupt(tslSleepInterrupt_t sleepinterrupt);
        tslSleepInterrupt_t getSleepInterrupt(void);
        void setProximityInterrupt(tslProximityInterrupt_t proximityinterrupt);
        tslProximityInterrupt_t getProximityInterrupt(void);
        void setALSInterrupt(tslALSInterrupt_t alsinterrupt);
        tslALSInterrupt_t getALSInterrupt(void);
        void setWaitEnable(tslWaitEnable_t waitenable);
        tslWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tslProximityEnable_t proximityenable);
        tslProximityEnable_t getProximityEnable(void);
        void setALSEnable(tslALSEnable_t alsenable);
        tslALSEnable_t getALSEnable(void);
        void setPowerEnable(tslPowerEnable_t powerenable);
        tslPowerEnable_t getPowerEnable(void);
        void setATime(tslATime_t atime);
        tslATime_t getATime(void);
        void setPTime(tslPTime_t ptime);
        tslPTime_t getPTime(void);
        void setWTime(tslWTime_t wtime);
        tslWTime_t getWTime(void);
        void setALSGainLevel(tslALSGainLevel_t alsgainlevel);
        tslALSGainLevel_t getALSGainLevel(void);
        void setWaitLong(tslWaitLong_t waitlong);
        tslWaitLong_t getWaitLong(void);
        void setProximityDriveLevel(tslProximityDriveLevel_t proximitydrivelevel);
        tslProximityDriveLevel_t getProximityDriveLevel(void);
        void setProximityDrive(tslProximityDrive_t proximitydrive);
        tslProximityDrive_t getProximityDrive(void);
        void setProximityDiode(tslProximityDiode_t proximitydiode);
        tslProximityDiode_t getProximityDiode(void);
        void setProximityGain(tslProximityGain_t proximitygain);
        tslProximityGain_t getProximityGain(void);
        void setALSGain(tslALSGain_t alsgain);
        tslALSGain_t getALSGain(void);
    
    private:
};
