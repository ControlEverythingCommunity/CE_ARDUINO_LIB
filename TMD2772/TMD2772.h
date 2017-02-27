/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2772
        This code is designed to work with the TMD2772_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TMD2772_I2CS#tabs-0-product_tabset-2
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
    #define TMD2772_DEFAULT_ADDRESS                         (0x39)
    #define TMD2772_ALSPROX_DEV_ID                          (0x30)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMD2772_CONVERSIONDELAY                         (100)

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TMD2772_REG_ALSPROX_COMMAND                                 // Specifies Register Address
    #define TMD2772_REG_ALSPROX_ENABLE                      (0x00)      // Enables States and Interrupts Register
    #define TMD2772_REG_ALSPROX_ATIME                       (0x01)      // ALS Integration Time Register
    #define TMD2772_REG_ALSPROX_PTIME                       (0x02)      // Proximity ADC Time Register
    #define TMD2772_REG_ALSPROX_WTIME                       (0x03)      // Wait Time Register
    #define TMD2772_REG_ALSPROX_AILTL                       (0x04)      // ALS Interrupt Low Threshold Low Byte Register
    #define TMD2772_REG_ALSPROX_AILTH                       (0x05)      // ALS Interrupt Low Threshold High Byte Register
    #define TMD2772_REG_ALSPROX_AIHTL                       (0x06)      // ALS Interrupt High Threshold Low Byte Register
    #define TMD2772_REG_ALSPROX_AIHTH                       (0x07)      // ALS Interrupt High Threshold High Byte Register
    #define TMD2772_REG_ALSPROX_PILTL                       (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TMD2772_REG_ALSPROX_PILTH                       (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TMD2772_REG_ALSPROX_PIHTL                       (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TMD2772_REG_ALSPROX_PIHTH                       (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TMD2772_REG_ALSPROX_PERS                        (0x0C)      // Interrupt Persistence Filters Register
    #define TMD2772_REG_ALSPROX_CONFIG                      (0x0D)      // Configuration Register
    #define TMD2772_REG_ALSPROX_PPULSE                      (0x0E)      // Proximity Pulse Count Register
    #define TMD2772_REG_ALSPROX_CONTROL                     (0x0F)      // Control Register
    #define TMD2772_REG_ALSPROX_REVISION                    (0x11)      // Die Revision Number Register
    #define TMD2772_REG_ALSPROX_ID                          (0x12)      // Device ID Register
    #define TMD2772_REG_ALSPROX_STATUS                      (0x13)      // Device Status Register
    #define TMD2772_REG_ALSPROX_C0DATAL                     (0x14)      // CH0 ADC Low Data Register
    #define TMD2772_REG_ALSPROX_C0DATAH                     (0x15)      // CH0 ADC High Data Register
    #define TMD2772_REG_ALSPROX_C1DATAL                     (0x16)      // CH1 ADC Low Data Register
    #define TMD2772_REG_ALSPROX_C1DATAH                     (0x17)      // CH1 ADC High Data Register
    #define TMD2772_REG_ALSPROX_PDATAL                      (0x18)      // Proximity ADC Low Data Register
    #define TMD2772_REG_ALSPROX_PDATAH                      (0x19)      // Proximity ADC High Data Register
    #define TMD2772_REG_ALSPROX_POFFSET                     (0x1E)      // Proximity Offset Register

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_CMD_MASK                    (0x80)      // Select Command Register
    #define TMD2772_REG_ALSPROX_CMD_SELECT                  (0x80)      // Addresses COMMAND Register
    #define TMD2772_REG_ALSPROX_CMD_TYPE_MASK               (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TMD2772_REG_ALSPROX_CMD_TYPE_REP_BYTE           (0x00)      // Repeated Byte Protocol Transaction
    #define TMD2772_REG_ALSPROX_CMD_TYPE_AUTO_INCR          (0x20)      // Auto-Increment Protocol Transaction
    #define TMD2772_REG_ALSPROX_CMD_TYPE_SPECIAL            (0x60)      // Special Function
    #define TMD2772_REG_ALSPROX_CMD_ADD_MASK                (0x1F)      // Specifies a Special Function Command
    #define TMD2772_REG_ALSPROX_CMD_ADD_NORMAL              (0x00)      // Normal — No Action
    #define TMD2772_REG_ALSPROX_CMD_ADD_PROX_INTR           (0x05)      // Proximity Interrupt Clear
    #define TMD2772_REG_ALSPROX_CMD_ADD_ALS_INTR            (0x06)      // ALS Interrupt Clear
    #define TMD2772_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR       (0x07)      // Proximity and ALS Interrupt Clear

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_ENABLE_SAI_MASK             (0x40)      // Sleep After Interrupt
    #define TMD2772_REG_ALSPROX_ENABLE_SAI_NOT_ASSERT       (0x00)      // Not Asserted, the Device will Not Power Down at the End of a Proximity or ALS Cycle if an Interrupt has been Generated
    #define TMD2772_REG_ALSPROX_ENABLE_SAI_ASSERT           (0x40)      // Asserted, the Device will Power Down at the End of a Proximity or ALS Cycle if an Interrupt has been Generated

    #define TMD2772_REG_ALSPROX_ENABLE_PIEN_MASK            (0x20)      // Proximity Interrupt Mask
    #define TMD2772_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT      (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TMD2772_REG_ALSPROX_ENABLE_PIEN_ASSERT          (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TMD2772_REG_ALSPROX_ENABLE_AIEN_MASK            (0x10)      // ALS Interrupt Mask
    #define TMD2772_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT      (0x00)      // Not Asserted, Did Not Permits ALS Interrupts to be Generated
    #define TMD2772_REG_ALSPROX_ENABLE_AIEN_ASSERT          (0x10)      // Asserted, Permits ALS Interrupts to be Generated

    #define TMD2772_REG_ALSPROX_ENABLE_WEN_MASK             (0x08)      // Wait Enable
    #define TMD2772_REG_ALSPROX_ENABLE_WEN_DISABLE          (0x00)      // Disables the Wait Timer
    #define TMD2772_REG_ALSPROX_ENABLE_WEN_ENABLE           (0x08)      // Enables the Wait Timer

    #define TMD2772_REG_ALSPROX_ENABLE_PEN_MASK             (0x04)      // Proximity Enable
    #define TMD2772_REG_ALSPROX_ENABLE_PEN_DISABLE          (0x00)      // Deactivates the Proximity
    #define TMD2772_REG_ALSPROX_ENABLE_PEN_ENABLE           (0x04)      // Activates the Proximity

    #define TMD2772_REG_ALSPROX_ENABLE_AEN_MASK             (0x02)      // ALS Enable
    #define TMD2772_REG_ALSPROX_ENABLE_AEN_DISABLE          (0x00)      // Disables the ALS
    #define TMD2772_REG_ALSPROX_ENABLE_AEN_ENABLE           (0x02)      // Activates the ALS

    #define TMD2772_REG_ALSPROX_ENABLE_PON_MASK             (0x01)      // Power ON
    #define TMD2772_REG_ALSPROX_ENABLE_PON_DISABLE          (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TMD2772_REG_ALSPROX_ENABLE_PON_ENABLE           (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_ATIME_MASK                  (0xFF)      // Controls the Internal Integration Time of the ALS Channel ADC’s in 2.73 ms Increments
    #define TMD2772_REG_ALSPROX_ATIME_2_73MS                (0xFF)      // Cycles: 1, Time: 2.73 ms Max Count: 1024
    #define TMD2772_REG_ALSPROX_ATIME_27_3MS                (0xF6)      // Cycles: 10, Time: 27.3 ms Max Count: 10240
    #define TMD2772_REG_ALSPROX_ATIME_101MS                 (0xDB)      // Cycles: 37, Time: 101 ms Max Count: 37888
    #define TMD2772_REG_ALSPROX_ATIME_175MS                 (0xC0)      // Cycles: 64, Time: 175 ms Max Count: 65535
    #define TMD2772_REG_ALSPROX_ATIME_699MS                 (0x00)      // Cycles: 256, Time: 699 ms Max Count: 65535

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE PTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_PTIME_MASK                  (0xFF)      // Controls the Internal Integration Time of the Proximity ADC’s in 2.73 ms Increments
    #define TMD2772_REG_ALSPROX_PTIME_2_73MS                (0xFF)      // Cycles: 1, Time: 2.73 ms Max Count: 1023

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_WTIME_MASK                  (0xFF)      // Wait Time is Set in 2.73 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TMD2772_REG_ALSPROX_WTIME_1                     (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.73 ms Time (WLONG = 1):  0.033 sec
    #define TMD2772_REG_ALSPROX_WTIME_74                    (0xB6)      // Wait Time: 74, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.4 sec
    #define TMD2772_REG_ALSPROX_WTIME_256                   (0x00)      // Wait Time: 256, Time (WLONG = 0): 699 ms Time (WLONG = 1):  8.4 sec

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_CONFIG_AGL_MASK             (0x04)      // ALS Gain Level
    #define TMD2772_REG_ALSPROX_CONFIG_AGL_NOT_ASSERT       (0x00)      // Not Asserted, the 1X and 8X ALS Gain (AGAIN) Modes are Scaled by 1
    #define TMD2772_REG_ALSPROX_CONFIG_AGL_ASSERT           (0x04)      // Asserted, the 1X and 8X ALS Gain (AGAIN) Modes are Scaled by 0.16

    #define TMD2772_REG_ALSPROX_CONFIG_WLONG_MASK           (0x02)      // Wait Long
    #define TMD2772_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT     (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TMD2772_REG_ALSPROX_CONFIG_WLONG_ASSERT         (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

    #define TMD2772_REG_ALSPROX_CONFIG_PDL_MASK             (0x01)      // Proximity Drive Level
    #define TMD2772_REG_ALSPROX_CONFIG_PDL_NOT_ASSERT       (0x00)      // Not Asserted, the Proximity LDR Drive Current is Not Reduced by 9
    #define TMD2772_REG_ALSPROX_CONFIG_PDL_ASSERT           (0x01)      // Asserted, the Proximity LDR Drive Current is Reduced by 9

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2772_REG_ALSPROX_CONTROL_PDRIVE_MASK         (0xC0)      // Proximity LED Drive Strength
    #define TMD2772_REG_ALSPROX_CONTROL_PDRIVE_100          (0x00)      // LED Strength – PDL=0: 100%, LED Strength – PDL=1: 11.1%
    #define TMD2772_REG_ALSPROX_CONTROL_PDRIVE_50           (0x40)      // LED Strength – PDL=0: 50%, LED Strength – PDL=1: 5.6%
    #define TMD2772_REG_ALSPROX_CONTROL_PDRIVE_25           (0x80)      // LED Strength – PDL=0: 25%, LED Strength – PDL=1: 2.8%
    #define TMD2772_REG_ALSPROX_CONTROL_PDRIVE_12_5         (0xC0)      // LED Strength – PDL=0: 12.5%, LED Strength – PDL=1: 1.4%

    #define TMD2772_REG_ALSPROX_CONTROL_PDIODE_MASK         (0x30)      // Proximity Diode Selector
    #define TMD2772_REG_ALSPROX_CONTROL_PDIODE_NONE         (0x00)      // Proximity Uses Neither Diode
    #define TMD2772_REG_ALSPROX_CONTROL_PDIODE_CH0          (0x10)      // Proximity Uses the CH0 Diode
    #define TMD2772_REG_ALSPROX_CONTROL_PDIODE_CH1          (0x20)      // Proximity Uses the CH1 Diode

    #define TMD2772_REG_ALSPROX_CONTROL_PGAIN_MASK          (0x0C)      // Proximity Gain
    #define TMD2772_REG_ALSPROX_CONTROL_PGAIN_1X            (0x00)      // 1X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_PGAIN_2X            (0x04)      // 2X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_PGAIN_4X            (0x08)      // 4X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_PGAIN_8X            (0x0C)      // 8X Gain

    #define TMD2772_REG_ALSPROX_CONTROL_AGAIN_MASK          (0x03)      // ALS Gain
    #define TMD2772_REG_ALSPROX_CONTROL_AGAIN_1X            (0x00)      // 1X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_AGAIN_8X            (0x01)      // 8X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_AGAIN_16X           (0x02)      // 16X Gain
    #define TMD2772_REG_ALSPROX_CONTROL_AGAIN_120X          (0x03)      // 120X Gain


typedef enum
{
    COMMAND_SELECT                  = TMD2772_REG_ALSPROX_CMD_SELECT,
    COMMAND_REP_BYTE                = TMD2772_REG_ALSPROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TMD2772_REG_ALSPROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TMD2772_REG_ALSPROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TMD2772_REG_ALSPROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR           = TMD2772_REG_ALSPROX_CMD_ADD_PROX_INTR,
    COMMAND_ADD_ALS_INTR            = TMD2772_REG_ALSPROX_CMD_ADD_ALS_INTR,
    COMMAND_ADD_PROX_ALS_INTR       = TMD2772_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR
    
} tmdCommand_t;

typedef enum
{
    SAI_NOT_ASSERT                  = TMD2772_REG_ALSPROX_ENABLE_SAI_NOT_ASSERT,
    SAI_ASSERT                      = TMD2772_REG_ALSPROX_ENABLE_SAI_ASSERT
    
} tmdSleepInterrupt_t;

typedef enum
{
    PIEN_NOT_ASSERT                 = TMD2772_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                     = TMD2772_REG_ALSPROX_ENABLE_PIEN_ASSERT
    
} tmdProximityInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                 = TMD2772_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                     = TMD2772_REG_ALSPROX_ENABLE_AIEN_ASSERT
    
} tmdALSInterrupt_t;

typedef enum
{
    WEN_DISABLE                     = TMD2772_REG_ALSPROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TMD2772_REG_ALSPROX_ENABLE_WEN_ENABLE
    
} tmdWaitEnable_t;

typedef enum
{
    PEN_DISABLE                     = TMD2772_REG_ALSPROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                      = TMD2772_REG_ALSPROX_ENABLE_PEN_ENABLE
    
} tmdProximityEnable_t;

typedef enum
{
    AEN_DISABLE                     = TMD2772_REG_ALSPROX_ENABLE_AEN_DISABLE,
    AEN_ENABLE                      = TMD2772_REG_ALSPROX_ENABLE_AEN_ENABLE
    
} tmdALSEnable_t;

typedef enum
{
    PON_DISABLE                     = TMD2772_REG_ALSPROX_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TMD2772_REG_ALSPROX_ENABLE_PON_ENABLE
    
} tmdPowerEnable_t;

typedef enum
{
    ATIME_2_73MS                    = TMD2772_REG_ALSPROX_ATIME_2_73MS,
    ATIME_27_3MS                    = TMD2772_REG_ALSPROX_ATIME_27_3MS,
    ATIME_101MS                     = TMD2772_REG_ALSPROX_ATIME_101MS,
    ATIME_175MS                     = TMD2772_REG_ALSPROX_ATIME_175MS,
    ATIME_699MS                     = TMD2772_REG_ALSPROX_ATIME_699MS
    
} tmdATime_t;

typedef enum
{
    PTIME_2_73MS                    = TMD2772_REG_ALSPROX_PTIME_2_73MS
    
} tmdPTime_t;

typedef enum
{
    WTIME_1                         = TMD2772_REG_ALSPROX_WTIME_1,
    WTIME_74                        = TMD2772_REG_ALSPROX_WTIME_74,
    WTIME_256                       = TMD2772_REG_ALSPROX_WTIME_256
    
} tmdWTime_t;

typedef enum
{
    AGL_NOT_ASSERT                  = TMD2772_REG_ALSPROX_CONFIG_AGL_NOT_ASSERT,
    AGL_ASSERT                      = TMD2772_REG_ALSPROX_CONFIG_AGL_ASSERT
    
} tmdALSGainLevel_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TMD2772_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TMD2772_REG_ALSPROX_CONFIG_WLONG_ASSERT
    
} tmdWaitLong_t;

typedef enum
{
    PDL_NOT_ASSERT                  = TMD2772_REG_ALSPROX_CONFIG_PDL_NOT_ASSERT,
    PDL_ASSERT                      = TMD2772_REG_ALSPROX_CONFIG_PDL_ASSERT
    
} tmdProximityDriveLevel_t;

typedef enum
{
    PDRIVE_100                      = TMD2772_REG_ALSPROX_CONTROL_PDRIVE_100,
    PDRIVE_50                       = TMD2772_REG_ALSPROX_CONTROL_PDRIVE_50,
    PDRIVE_25                       = TMD2772_REG_ALSPROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                     = TMD2772_REG_ALSPROX_CONTROL_PDRIVE_12_5
    
} tmdProximityDrive_t;

typedef enum
{
    PDIODE_NONE                     = TMD2772_REG_ALSPROX_CONTROL_PDIODE_NONE,
    PDIODE_CH0                      = TMD2772_REG_ALSPROX_CONTROL_PDIODE_CH0,
    PDIODE_CH1                      = TMD2772_REG_ALSPROX_CONTROL_PDIODE_CH1
    
} tmdProximityDiode_t;

typedef enum
{
    PGAIN_1X                        = TMD2772_REG_ALSPROX_CONTROL_PGAIN_1X,
    PGAIN_2X                        = TMD2772_REG_ALSPROX_CONTROL_PGAIN_2X,
    PGAIN_4X                        = TMD2772_REG_ALSPROX_CONTROL_PGAIN_4X,
    PGAIN_8X                        = TMD2772_REG_ALSPROX_CONTROL_PGAIN_8X
    
} tmdProximityGain_t;

typedef enum
{
    AGAIN_1X                        = TMD2772_REG_ALSPROX_CONTROL_AGAIN_1X,
    AGAIN_8X                        = TMD2772_REG_ALSPROX_CONTROL_AGAIN_8X,
    AGAIN_16X                       = TMD2772_REG_ALSPROX_CONTROL_AGAIN_16X,
    AGAIN_120X                      = TMD2772_REG_ALSPROX_CONTROL_AGAIN_120X
    
} tmdALSGain_t;

typedef struct
{
    float L;
    float P;
    
} tmdSensorData_t;


class TMD2772
{
    protected:
        // Instance-specific properties
        uint8_t tmd_conversionDelay;
        tmdCommand_t tmd_command;
        tmdSleepInterrupt_t tmd_sleepinterrupt;
        tmdProximityInterrupt_t tmd_proximityinterrupt;
        tmdALSInterrupt_t tmd_alsinterrupt;
        tmdWaitEnable_t tmd_waitenable;
        tmdProximityEnable_t tmd_proximityenable;
        tmdALSEnable_t tmd_alsenable;
        tmdPowerEnable_t tmd_powerenable;
        tmdATime_t tmd_atime;
        tmdPTime_t tmd_ptime;
        tmdWTime_t tmd_wtime;
        tmdALSGainLevel_t tmd_alsgainlevel;
        tmdWaitLong_t tmd_waitlong;
        tmdProximityDriveLevel_t tmd_proximitydrivelevel;
        tmdProximityDrive_t tmd_proximitydrive;
        tmdProximityDiode_t tmd_proximitydiode;
        tmdProximityGain_t tmd_proximitygain;
        tmdALSGain_t tmd_alsgain;

    public:
        uint8_t tmd_i2cAddress;
        tmdSensorData_t tmd_alsproxData;
        void getAddr_TMD2772(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpALSProximity(void);
        void Measure_ALS(void);
        void Measure_Proximity(void);
        void setCommand(tmdCommand_t command);
        tmdCommand_t getCommand(void);
        void setSleepInterrupt(tmdSleepInterrupt_t sleepinterrupt);
        tmdSleepInterrupt_t getSleepInterrupt(void);
        void setProximityInterrupt(tmdProximityInterrupt_t proximityinterrupt);
        tmdProximityInterrupt_t getProximityInterrupt(void);
        void setALSInterrupt(tmdALSInterrupt_t alsinterrupt);
        tmdALSInterrupt_t getALSInterrupt(void);
        void setWaitEnable(tmdWaitEnable_t waitenable);
        tmdWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tmdProximityEnable_t proximityenable);
        tmdProximityEnable_t getProximityEnable(void);
        void setALSEnable(tmdALSEnable_t alsenable);
        tmdALSEnable_t getALSEnable(void);
        void setPowerEnable(tmdPowerEnable_t powerenable);
        tmdPowerEnable_t getPowerEnable(void);
        void setATime(tmdATime_t atime);
        tmdATime_t getATime(void);
        void setPTime(tmdPTime_t ptime);
        tmdPTime_t getPTime(void);
        void setWTime(tmdWTime_t wtime);
        tmdWTime_t getWTime(void);
        void setALSGainLevel(tmdALSGainLevel_t alsgainlevel);
        tmdALSGainLevel_t getALSGainLevel(void);
        void setWaitLong(tmdWaitLong_t waitlong);
        tmdWaitLong_t getWaitLong(void);
        void setProximityDriveLevel(tmdProximityDriveLevel_t proximitydrivelevel);
        tmdProximityDriveLevel_t getProximityDriveLevel(void);
        void setProximityDrive(tmdProximityDrive_t proximitydrive);
        tmdProximityDrive_t getProximityDrive(void);
        void setProximityDiode(tmdProximityDiode_t proximitydiode);
        tmdProximityDiode_t getProximityDiode(void);
        void setProximityGain(tmdProximityGain_t proximitygain);
        tmdProximityGain_t getProximityGain(void);
        void setALSGain(tmdALSGain_t alsgain);
        tmdALSGain_t getALSGain(void);
    
    private:
};
