/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2771
        This code is designed to work with the TMD2771_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TMD2771_I2CS#tabs-0-product_tabset-2
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
    #define TMD2771_DEFAULT_ADDRESS                         (0x39)
    #define TMD2771_ALSPROX_DEV_ID                          (0x20)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMD2771_CONVERSIONDELAY                         (100)

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TMD2771_REG_ALSPROX_COMMAND                                 // Specifies Register Address
    #define TMD2771_REG_ALSPROX_ENABLE                      (0x00)      // Enables States and Interrupts Register
    #define TMD2771_REG_ALSPROX_ATIME                       (0x01)      // Integration Time Register
    #define TMD2771_REG_ALSPROX_PTIME                       (0x02)      // Proximity ADC Time Register
    #define TMD2771_REG_ALSPROX_WTIME                       (0x03)      // Wait Time Register
    #define TMD2771_REG_ALSPROX_AILTL                       (0x04)      // ALS Interrupt Low Threshold Low Byte Register
    #define TMD2771_REG_ALSPROX_AILTH                       (0x05)      // ALS Interrupt Low Threshold High Byte Register
    #define TMD2771_REG_ALSPROX_AIHTL                       (0x06)      // ALS Interrupt High Threshold Low Byte Register
    #define TMD2771_REG_ALSPROX_AIHTH                       (0x07)      // ALS Interrupt High Threshold High Byte Register
    #define TMD2771_REG_ALSPROX_PILTL                       (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TMD2771_REG_ALSPROX_PILTH                       (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TMD2771_REG_ALSPROX_PIHTL                       (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TMD2771_REG_ALSPROX_PIHTH                       (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TMD2771_REG_ALSPROX_PERS                        (0x0C)      // Interrupt Persistence Filters Register
    #define TMD2771_REG_ALSPROX_CONFIG                      (0x0D)      // Configuration Register
    #define TMD2771_REG_ALSPROX_PPULSE                      (0x0E)      // Proximity Pulse Count Register
    #define TMD2771_REG_ALSPROX_CONTROL                     (0x0F)      // Control Register
    #define TMD2771_REG_ALSPROX_ID                          (0x12)      // Device ID Register
    #define TMD2771_REG_ALSPROX_STATUS                      (0x13)      // Device Status Register
    #define TMD2771_REG_ALSPROX_C0DATAL                     (0x14)      // CH0 ADC Low Data Register
    #define TMD2771_REG_ALSPROX_C0DATAH                     (0x15)      // CH0 ADC High Data Register
    #define TMD2771_REG_ALSPROX_C1DATAL                     (0x16)      // CH1 ADC Low Data Register
    #define TMD2771_REG_ALSPROX_C1DATAH                     (0x17)      // CH1 ADC High Data Register
    #define TMD2771_REG_ALSPROX_PDATAL                      (0x18)      // Proximity ADC Low Data Register
    #define TMD2771_REG_ALSPROX_PDATAH                      (0x19)      // Proximity ADC High Data Register

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_CMD_MASK                    (0x80)      // Select Command Register
    #define TMD2771_REG_ALSPROX_CMD_SELECT                  (0x80)      // Addresses COMMAND Register
    #define TMD2771_REG_ALSPROX_CMD_TYPE_MASK               (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TMD2771_REG_ALSPROX_CMD_TYPE_REP_BYTE           (0x00)      // Repeated Byte Protocol Transaction
    #define TMD2771_REG_ALSPROX_CMD_TYPE_AUTO_INCR          (0x20)      // Auto-Increment Protocol Transaction
    #define TMD2771_REG_ALSPROX_CMD_TYPE_SPECIAL            (0x60)      // Special Function
    #define TMD2771_REG_ALSPROX_CMD_ADD_MASK                (0x1F)      // Specifies a Special Function Command
    #define TMD2771_REG_ALSPROX_CMD_ADD_NORMAL              (0x00)      // Normal — No Action
    #define TMD2771_REG_ALSPROX_CMD_ADD_PROX_INTR           (0x05)      // Proximity Interrupt Clear
    #define TMD2771_REG_ALSPROX_CMD_ADD_ALS_INTR            (0x06)      // ALS Interrupt Clear
    #define TMD2771_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR       (0x07)      // Proximity and ALS Interrupt Clear

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_ENABLE_PIEN_MASK            (0x20)      // Proximity Interrupt Mask
    #define TMD2771_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT      (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TMD2771_REG_ALSPROX_ENABLE_PIEN_ASSERT          (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TMD2771_REG_ALSPROX_ENABLE_AIEN_MASK            (0x10)      // ALS Interrupt Mask
    #define TMD2771_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT      (0x00)      // Not Asserted, Did Not Permits ALS Interrupts to be Generated
    #define TMD2771_REG_ALSPROX_ENABLE_AIEN_ASSERT          (0x10)      // Asserted, Permits ALS Interrupts to be Generated

    #define TMD2771_REG_ALSPROX_ENABLE_WEN_MASK             (0x08)      // Wait Enable
    #define TMD2771_REG_ALSPROX_ENABLE_WEN_DISABLE          (0x00)      // Disables the Wait Timer
    #define TMD2771_REG_ALSPROX_ENABLE_WEN_ENABLE           (0x08)      // Enables the Wait Timer

    #define TMD2771_REG_ALSPROX_ENABLE_PEN_MASK             (0x04)      // Proximity Enable
    #define TMD2771_REG_ALSPROX_ENABLE_PEN_DISABLE          (0x00)      // Deactivates the Proximity
    #define TMD2771_REG_ALSPROX_ENABLE_PEN_ENABLE           (0x04)      // Activates the Proximity

    #define TMD2771_REG_ALSPROX_ENABLE_AEN_MASK             (0x02)      // ALS Enable
    #define TMD2771_REG_ALSPROX_ENABLE_AEN_DISABLE          (0x00)      // Disables the ALS
    #define TMD2771_REG_ALSPROX_ENABLE_AEN_ENABLE           (0x02)      // Activates the ALS

    #define TMD2771_REG_ALSPROX_ENABLE_PON_MASK             (0x01)      // Power ON
    #define TMD2771_REG_ALSPROX_ENABLE_PON_DISABLE          (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TMD2771_REG_ALSPROX_ENABLE_PON_ENABLE           (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_ATIME_MASK                  (0xFF)      // Controls the Internal Integration Time of the ALS Channel ADC’s in 2.72 ms Increments
    #define TMD2771_REG_ALSPROX_ATIME_2_72MS                (0xFF)      // Integration Cycles: 1, Time: 2.72 ms Max Count: 1024
    #define TMD2771_REG_ALSPROX_ATIME_27_2MS                (0xF6)      // Integration Cycles: 10, Time: 27.2 ms Max Count: 10240
    #define TMD2771_REG_ALSPROX_ATIME_101MS                 (0xDB)      // Integration Cycles: 37, Time: 101 ms Max Count: 37888
    #define TMD2771_REG_ALSPROX_ATIME_174MS                 (0xC0)      // Integration Cycles: 64, Time: 174 ms Max Count: 65535
    #define TMD2771_REG_ALSPROX_ATIME_696MS                 (0x00)      // Integration Cycles: 256, Time: 696 ms Max Count: 65535

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE PTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_PTIME_MASK                  (0xFF)      // Controls the Internal Integration Time of the Proximity ADC’s in 2.72 ms Increments
    #define TMD2771_REG_ALSPROX_PTIME_2_72MS                (0xFF)      // Integration Cycles: 1, Time: 2.72 ms Max Count: 1023

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_WTIME_MASK                  (0xFF)      // Wait Time is Set in 2.72 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TMD2771_REG_ALSPROX_WTIME_1                     (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    #define TMD2771_REG_ALSPROX_WTIME_74                    (0xB6)      // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    #define TMD2771_REG_ALSPROX_WTIME_256                   (0x00)      // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_CONFIG_WLONG_MASK           (0x02)      // Wait Long
    #define TMD2771_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT     (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TMD2771_REG_ALSPROX_CONFIG_WLONG_ASSERT         (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    DIGITAL ALS, PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TMD2771_REG_ALSPROX_CONTROL_PDRIVE_MASK         (0xC0)      // Proximity LED Drive Strength
    #define TMD2771_REG_ALSPROX_CONTROL_PDRIVE_100          (0x00)      // LED Strength: 100%
    #define TMD2771_REG_ALSPROX_CONTROL_PDRIVE_50           (0x40)      // LED Strength: 50%
    #define TMD2771_REG_ALSPROX_CONTROL_PDRIVE_25           (0x80)      // LED Strength: 25%
    #define TMD2771_REG_ALSPROX_CONTROL_PDRIVE_12_5         (0xC0)      // LED Strength: 12.5%

    #define TMD2771_REG_ALSPROX_CONTROL_PDIODE_MASK         (0x30)      // Proximity Diode Selector
    #define TMD2771_REG_ALSPROX_CONTROL_PDIODE_NONE         (0x00)      // Proximity Uses Neither Diode
    #define TMD2771_REG_ALSPROX_CONTROL_PDIODE_CH0          (0x10)      // Proximity Uses the CH0 Diode
    #define TMD2771_REG_ALSPROX_CONTROL_PDIODE_CH1          (0x20)      // Proximity Uses the CH1 Diode

    #define TMD2771_REG_ALSPROX_CONTROL_AGAIN_MASK          (0x03)      // ALS Gain
    #define TMD2771_REG_ALSPROX_CONTROL_AGAIN_1X            (0x00)      // 1X Gain
    #define TMD2771_REG_ALSPROX_CONTROL_AGAIN_8X            (0x01)      // 8X Gain
    #define TMD2771_REG_ALSPROX_CONTROL_AGAIN_16X           (0x02)      // 16X Gain
    #define TMD2771_REG_ALSPROX_CONTROL_AGAIN_120X          (0x03)      // 120X Gain


typedef enum
{
    COMMAND_SELECT                  = TMD2771_REG_ALSPROX_CMD_SELECT,
    COMMAND_REP_BYTE                = TMD2771_REG_ALSPROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TMD2771_REG_ALSPROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TMD2771_REG_ALSPROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TMD2771_REG_ALSPROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR           = TMD2771_REG_ALSPROX_CMD_ADD_PROX_INTR,
    COMMAND_ADD_ALS_INTR            = TMD2771_REG_ALSPROX_CMD_ADD_ALS_INTR,
    COMMAND_ADD_PROX_ALS_INTR       = TMD2771_REG_ALSPROX_CMD_ADD_PROX_ALS_INTR
    
} tmdCommand_t;

typedef enum
{
    PIEN_NOT_ASSERT                 = TMD2771_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                     = TMD2771_REG_ALSPROX_ENABLE_PIEN_ASSERT
    
} tmdProximityInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                 = TMD2771_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                     = TMD2771_REG_ALSPROX_ENABLE_AIEN_ASSERT
    
} tmdALSInterrupt_t;

typedef enum
{
    WEN_DISABLE                     = TMD2771_REG_ALSPROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TMD2771_REG_ALSPROX_ENABLE_WEN_ENABLE
    
} tmdWaitEnable_t;

typedef enum
{
    PEN_DISABLE                     = TMD2771_REG_ALSPROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                      = TMD2771_REG_ALSPROX_ENABLE_PEN_ENABLE
    
} tmdProximityEnable_t;

typedef enum
{
    AEN_DISABLE                     = TMD2771_REG_ALSPROX_ENABLE_AEN_DISABLE,
    AEN_ENABLE                      = TMD2771_REG_ALSPROX_ENABLE_AEN_ENABLE
    
} tmdALSEnable_t;

typedef enum
{
    PON_DISABLE                     = TMD2771_REG_ALSPROX_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TMD2771_REG_ALSPROX_ENABLE_PON_ENABLE
    
} tmdPowerEnable_t;

typedef enum
{
    ATIME_2_72MS                    = TMD2771_REG_ALSPROX_ATIME_2_72MS,
    ATIME_27_2MS                    = TMD2771_REG_ALSPROX_ATIME_27_2MS,
    ATIME_101MS                     = TMD2771_REG_ALSPROX_ATIME_101MS,
    ATIME_174MS                     = TMD2771_REG_ALSPROX_ATIME_174MS,
    ATIME_696MS                     = TMD2771_REG_ALSPROX_ATIME_696MS
    
} tmdATime_t;

typedef enum
{
    PTIME_2_72MS                    = TMD2771_REG_ALSPROX_PTIME_2_72MS
    
} tmdPTime_t;

typedef enum
{
    WTIME_1                         = TMD2771_REG_ALSPROX_WTIME_1,
    WTIME_74                        = TMD2771_REG_ALSPROX_WTIME_74,
    WTIME_256                       = TMD2771_REG_ALSPROX_WTIME_256
    
} tmdWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TMD2771_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TMD2771_REG_ALSPROX_CONFIG_WLONG_ASSERT
    
} tmdWaitLong_t;

typedef enum
{
    PDRIVE_100                      = TMD2771_REG_ALSPROX_CONTROL_PDRIVE_100,
    PDRIVE_50                       = TMD2771_REG_ALSPROX_CONTROL_PDRIVE_50,
    PDRIVE_25                       = TMD2771_REG_ALSPROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                     = TMD2771_REG_ALSPROX_CONTROL_PDRIVE_12_5
    
} tmdProximityDrive_t;

typedef enum
{
    PDIODE_NONE                     = TMD2771_REG_ALSPROX_CONTROL_PDIODE_NONE,
    PDIODE_CH0                      = TMD2771_REG_ALSPROX_CONTROL_PDIODE_CH0,
    PDIODE_CH1                      = TMD2771_REG_ALSPROX_CONTROL_PDIODE_CH1
    
} tmdProximityDiode_t;

typedef enum
{
    AGAIN_1X                        = TMD2771_REG_ALSPROX_CONTROL_AGAIN_1X,
    AGAIN_8X                        = TMD2771_REG_ALSPROX_CONTROL_AGAIN_8X,
    AGAIN_16X                       = TMD2771_REG_ALSPROX_CONTROL_AGAIN_16X,
    AGAIN_120X                      = TMD2771_REG_ALSPROX_CONTROL_AGAIN_120X
    
} tmdALSGain_t;

typedef struct
{
    float L;
    float P;
    
} tmdSensorData_t;


class TMD2771
{
    protected:
        // Instance-specific properties
        uint8_t tmd_conversionDelay;
        tmdCommand_t tmd_command;
        tmdProximityInterrupt_t tmd_proximityinterrupt;
        tmdALSInterrupt_t tmd_alsinterrupt;
        tmdWaitEnable_t tmd_waitenable;
        tmdProximityEnable_t tmd_proximityenable;
        tmdALSEnable_t tmd_alsenable;
        tmdPowerEnable_t tmd_powerenable;
        tmdATime_t tmd_atime;
        tmdPTime_t tmd_ptime;
        tmdWTime_t tmd_wtime;
        tmdWaitLong_t tmd_waitlong;
        tmdProximityDrive_t tmd_proximitydrive;
        tmdProximityDiode_t tmd_proximitydiode;
        tmdALSGain_t tmd_alsgain;

    public:
        uint8_t tmd_i2cAddress;
        tmdSensorData_t tmd_alsproxData;
        void getAddr_TMD2771(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpALSProximity(void);
        void Measure_ALS(void);
        void Measure_Proximity(void);
        void setCommand(tmdCommand_t command);
        tmdCommand_t getCommand(void);
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
        void setWaitLong(tmdWaitLong_t waitlong);
        tmdWaitLong_t getWaitLong(void);
        void setProximityDrive(tmdProximityDrive_t proximitydrive);
        tmdProximityDrive_t getProximityDrive(void);
        void setProximityDiode(tmdProximityDiode_t proximitydiode);
        tmdProximityDiode_t getProximityDiode(void);
        void setALSGain(tmdALSGain_t alsgain);
        tmdALSGain_t getALSGain(void);
    
    private:
};
