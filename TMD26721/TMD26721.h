/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD26721
        This code is designed to work with the TMD26721_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Proximity?sku=TMD26721_I2CS#tabs-0-product_tabset-2
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
    #define TMD26721_DEFAULT_ADDRESS                        (0x39)
    #define TMD26721_PROX_DEV_ID                            (0x32)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMD26721_CONVERSIONDELAY                        (100)

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TMD26721_REG_PROX_COMMAND                                // Specifies Register Address
    #define TMD26721_REG_PROX_ENABLE                        (0x00)      // Enables States and Interrupts Register
    #define TMD26721_REG_PROX_PTIME                         (0x02)      // Proximity ADC Time Register
    #define TMD26721_REG_PROX_WTIME                         (0x03)      // Wait Time Register
    #define TMD26721_REG_PROX_PILTL                         (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TMD26721_REG_PROX_PILTH                         (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TMD26721_REG_PROX_PIHTL                         (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TMD26721_REG_PROX_PIHTH                         (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TMD26721_REG_PROX_PERS                          (0x0C)      // Interrupt Persistence Filters Register
    #define TMD26721_REG_PROX_CONFIG                        (0x0D)      // Configuration Register
    #define TMD26721_REG_PROX_PPULSE                        (0x0E)      // Proximity Pulse Count Register
    #define TMD26721_REG_PROX_CONTROL                       (0x0F)      // Control Register
    #define TMD26721_REG_PROX_REVISION                      (0x11)      // Die Revision Number Register
    #define TMD26721_REG_PROX_ID                            (0x12)      // Device ID Register
    #define TMD26721_REG_PROX_STATUS                        (0x13)      // Device Status Register
    #define TMD26721_REG_PROX_PDATAL                        (0x18)      // Proximity ADC Low Data Register
    #define TMD26721_REG_PROX_PDATAH                        (0x19)      // Proximity ADC High Data Register
    #define TMD26721_REG_PROX_POFFSET                       (0x1E)      // Proximity Offset Register

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_CMD_MASK                      (0x80)      // Select Command Register
    #define TMD26721_REG_PROX_CMD_SELECT                    (0x80)      // Addresses COMMAND Register
    #define TMD26721_REG_PROX_CMD_TYPE_MASK                 (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TMD26721_REG_PROX_CMD_TYPE_REP_BYTE             (0x00)      // Repeated Byte Protocol Transaction
    #define TMD26721_REG_PROX_CMD_TYPE_AUTO_INCR            (0x20)      // Auto-Increment Protocol Transaction
    #define TMD26721_REG_PROX_CMD_TYPE_SPECIAL              (0x60)      // Special Function
    #define TMD26721_REG_PROX_CMD_ADD_MASK                  (0x1F)      // Specifies a Special Function Command
    #define TMD26721_REG_PROX_CMD_ADD_NORMAL                (0x00)      // Normal — No Action
    #define TMD26721_REG_PROX_CMD_ADD_PROX_INTR             (0x05)      // Proximity Interrupt Clear

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_ENABLE_SAI_MASK               (0x40)      // Sleep After Interrupt
    #define TMD26721_REG_PROX_ENABLE_SAI_NOT_ASSERT         (0x00)      // Not Asserted, the Device will Not Power Down at the End of a Proximity Cycle if an Interrupt has been Generated
    #define TMD26721_REG_PROX_ENABLE_SAI_ASSERT             (0x40)      // Asserted, the Device will Power Down at the End of a Proximity Cycle if an Interrupt has been Generated

    #define TMD26721_REG_PROX_ENABLE_PIEN_MASK              (0x20)      // Proximity Interrupt Mask
    #define TMD26721_REG_PROX_ENABLE_PIEN_NOT_ASSERT        (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TMD26721_REG_PROX_ENABLE_PIEN_ASSERT            (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TMD26721_REG_PROX_ENABLE_WEN_MASK               (0x08)      // Wait Enable
    #define TMD26721_REG_PROX_ENABLE_WEN_DISABLE            (0x00)      // Disables the Wait Timer
    #define TMD26721_REG_PROX_ENABLE_WEN_ENABLE             (0x08)      // Enables the Wait Timer

    #define TMD26721_REG_PROX_ENABLE_PEN_MASK               (0x04)      // Proximity Enable
    #define TMD26721_REG_PROX_ENABLE_PEN_DISABLE            (0x00)      // Deactivates the Proximity
    #define TMD26721_REG_PROX_ENABLE_PEN_ENABLE             (0x04)      // Activates the Proximity

    #define TMD26721_REG_PROX_ENABLE_PON_MASK               (0x01)      // Power ON
    #define TMD26721_REG_PROX_ENABLE_PON_DISABLE            (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TMD26721_REG_PROX_ENABLE_PON_ENABLE             (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE PTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_PTIME_MASK                    (0xFF)      // Controls the Internal Integration Time of the Proximity ADC’s in 2.73 ms Increments
    #define TMD26721_REG_PROX_PTIME_2_73MS                  (0xFF)      // Cycles: 1, Time: 2.73 ms Max Count: 1023

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_WTIME_MASK                    (0xFF)      // Wait Time is Set in 2.73 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TMD26721_REG_PROX_WTIME_1                       (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    #define TMD26721_REG_PROX_WTIME_74                      (0xB6)      // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    #define TMD26721_REG_PROX_WTIME_256                     (0x00)      // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_CONFIG_WLONG_MASK             (0x02)      // Wait Long
    #define TMD26721_REG_PROX_CONFIG_WLONG_NOT_ASSERT       (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TMD26721_REG_PROX_CONFIG_WLONG_ASSERT           (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    INFRARED DIGITAL PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TMD26721_REG_PROX_CONTROL_PDRIVE_MASK           (0xC0)      // Proximity LED Drive Strength
    #define TMD26721_REG_PROX_CONTROL_PDRIVE_100            (0x00)      // LED Strength – PDL=0: 100mA, LED Strength – PDL=1: 11.1mA
    #define TMD26721_REG_PROX_CONTROL_PDRIVE_50             (0x40)      // LED Strength – PDL=0: 50mA, LED Strength – PDL=1: 5.6mA
    #define TMD26721_REG_PROX_CONTROL_PDRIVE_25             (0x80)      // LED Strength – PDL=0: 25mA, LED Strength – PDL=1: 2.8mA
    #define TMD26721_REG_PROX_CONTROL_PDRIVE_12_5           (0xC0)      // LED Strength – PDL=0: 12.5mA, LED Strength – PDL=1: 1.4mA

    #define TMD26721_REG_PROX_CONTROL_PDIODE_MASK           (0x30)      // Proximity Diode Selector
    #define TMD26721_REG_PROX_CONTROL_PDIODE_NONE           (0x00)      // Proximity Uses Neither Diode
    #define TMD26721_REG_PROX_CONTROL_PDIODE_CH0            (0x10)      // Proximity Uses the CH0 Diode
    #define TMD26721_REG_PROX_CONTROL_PDIODE_CH1            (0x20)      // Proximity Uses the CH1 Diode

    #define TMD26721_REG_PROX_CONTROL_PGAIN_MASK            (0x0C)      // Proximity Gain
    #define TMD26721_REG_PROX_CONTROL_PGAIN_1X              (0x00)      // 1X Gain
    #define TMD26721_REG_PROX_CONTROL_PGAIN_2X              (0x04)      // 2X Gain
    #define TMD26721_REG_PROX_CONTROL_PGAIN_4X              (0x08)      // 4X Gain
    #define TMD26721_REG_PROX_CONTROL_PGAIN_8X              (0x0C)      // 8X Gain


typedef enum
{
    COMMAND_SELECT                  = TMD26721_REG_PROX_CMD_SELECT,
    COMMAND_REP_BYTE                = TMD26721_REG_PROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TMD26721_REG_PROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TMD26721_REG_PROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TMD26721_REG_PROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR           = TMD26721_REG_PROX_CMD_ADD_PROX_INTR
    
} tmdCommand_t;

typedef enum
{
    SAI_NOT_ASSERT                  = TMD26721_REG_PROX_ENABLE_SAI_NOT_ASSERT,
    SAI_ASSERT                      = TMD26721_REG_PROX_ENABLE_SAI_ASSERT
    
} tmdSleepInterrupt_t;

typedef enum
{
    PIEN_NOT_ASSERT                 = TMD26721_REG_PROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                     = TMD26721_REG_PROX_ENABLE_PIEN_ASSERT
    
} tmdProximityInterrupt_t;


typedef enum
{
    WEN_DISABLE                     = TMD26721_REG_PROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TMD26721_REG_PROX_ENABLE_WEN_ENABLE
    
} tmdWaitEnable_t;

typedef enum
{
    PEN_DISABLE                     = TMD26721_REG_PROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                      = TMD26721_REG_PROX_ENABLE_PEN_ENABLE
    
} tmdProximityEnable_t;

typedef enum
{
    PON_DISABLE                     = TMD26721_REG_PROX_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TMD26721_REG_PROX_ENABLE_PON_ENABLE
    
} tmdPowerEnable_t;

typedef enum
{
    PTIME_2_73MS                    = TMD26721_REG_PROX_PTIME_2_73MS
    
} tmdPTime_t;

typedef enum
{
    WTIME_1                         = TMD26721_REG_PROX_WTIME_1,
    WTIME_74                        = TMD26721_REG_PROX_WTIME_74,
    WTIME_256                       = TMD26721_REG_PROX_WTIME_256
    
} tmdWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TMD26721_REG_PROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TMD26721_REG_PROX_CONFIG_WLONG_ASSERT
    
} tmdWaitLong_t;

typedef enum
{
    PDRIVE_100                      = TMD26721_REG_PROX_CONTROL_PDRIVE_100,
    PDRIVE_50                       = TMD26721_REG_PROX_CONTROL_PDRIVE_50,
    PDRIVE_25                       = TMD26721_REG_PROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                     = TMD26721_REG_PROX_CONTROL_PDRIVE_12_5
    
} tmdProximityDrive_t;

typedef enum
{
    PDIODE_NONE                     = TMD26721_REG_PROX_CONTROL_PDIODE_NONE,
    PDIODE_CH0                      = TMD26721_REG_PROX_CONTROL_PDIODE_CH0,
    PDIODE_CH1                      = TMD26721_REG_PROX_CONTROL_PDIODE_CH1
    
} tmdProximityDiode_t;

typedef enum
{
    PGAIN_1X                        = TMD26721_REG_PROX_CONTROL_PGAIN_1X,
    PGAIN_2X                        = TMD26721_REG_PROX_CONTROL_PGAIN_2X,
    PGAIN_4X                        = TMD26721_REG_PROX_CONTROL_PGAIN_4X,
    PGAIN_8X                        = TMD26721_REG_PROX_CONTROL_PGAIN_8X
    
} tmdProximityGain_t;

typedef struct
{
    float P;
    
} tmdSensorData_t;


class TMD26721
{
    protected:
        // Instance-specific properties
        uint8_t tmd_conversionDelay;
        tmdCommand_t tmd_command;
        tmdSleepInterrupt_t tmd_sleepinterrupt;
        tmdProximityInterrupt_t tmd_proximityinterrupt;
        tmdWaitEnable_t tmd_waitenable;
        tmdProximityEnable_t tmd_proximityenable;
        tmdPowerEnable_t tmd_powerenable;
        tmdPTime_t tmd_ptime;
        tmdWTime_t tmd_wtime;
        tmdWaitLong_t tmd_waitlong;
        tmdProximityDrive_t tmd_proximitydrive;
        tmdProximityDiode_t tmd_proximitydiode;
        tmdProximityGain_t tmd_proximitygain;

    public:
        uint8_t tmd_i2cAddress;
        tmdSensorData_t tmd_proxData;
        void getAddr_TMD26721(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpProximity(void);
        void Measure_Proximity(void);
        void setCommand(tmdCommand_t command);
        tmdCommand_t getCommand(void);
        void setSleepInterrupt(tmdSleepInterrupt_t sleepinterrupt);
        tmdSleepInterrupt_t getSleepInterrupt(void);
        void setProximityInterrupt(tmdProximityInterrupt_t proximityinterrupt);
        tmdProximityInterrupt_t getProximityInterrupt(void);
        void setWaitEnable(tmdWaitEnable_t waitenable);
        tmdWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tmdProximityEnable_t proximityenable);
        tmdProximityEnable_t getProximityEnable(void);
        void setPowerEnable(tmdPowerEnable_t powerenable);
        tmdPowerEnable_t getPowerEnable(void);
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
        void setProximityGain(tmdProximityGain_t proximitygain);
        tmdProximityGain_t getProximityGain(void);
    
    private:
};
