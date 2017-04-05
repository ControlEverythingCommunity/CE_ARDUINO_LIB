/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL26711
        This code is designed to work with the TSL26711_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define TSL26711_DEFAULT_ADDRESS                        (0x39)
    #define TSL26711_PROX_DEV_ID                            (0x00)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL26711_CONVERSIONDELAY                        (100)

/**************************************************************************
    DIGITAL PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TSL26711_REG_PROX_COMMAND                                   // Specifies Register Address
    #define TSL26711_REG_PROX_ENABLE                        (0x00)      // Enables States and Interrupts Register
    #define TSL26711_REG_PROX_PTIME                         (0x02)      // Proximity ADC Time Register
    #define TSL26711_REG_PROX_WTIME                         (0x03)      // Wait Time Register
    #define TSL26711_REG_PROX_PILTL                         (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TSL26711_REG_PROX_PILTH                         (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TSL26711_REG_PROX_PIHTL                         (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TSL26711_REG_PROX_PIHTH                         (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TSL26711_REG_PROX_PERS                          (0x0C)      // Interrupt Persistence Filters Register
    #define TSL26711_REG_PROX_CONFIG                        (0x0D)      // Configuration Register
    #define TSL26711_REG_PROX_PPCOUNT                       (0x0E)      // Proximity Pulse Count Register
    #define TSL26711_REG_PROX_CONTROL                       (0x0F)      // Control Register
    #define TSL26711_REG_PROX_ID                            (0x12)      // Device ID Register
    #define TSL26711_REG_PROX_STATUS                        (0x13)      // Device Status Register
    #define TSL26711_REG_PROX_PDATAL                        (0x18)      // Proximity ADC Low Data Register
    #define TSL26711_REG_PROX_PDATAH                        (0x19)      // Proximity ADC High Data Register

/**************************************************************************
    DIGITAL PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_CMD_MASK                      (0x80)      // Select Command Register
    #define TSL26711_REG_PROX_CMD_SELECT                    (0x80)      // Addresses COMMAND Register
    #define TSL26711_REG_PROX_CMD_TYPE_MASK                 (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TSL26711_REG_PROX_CMD_TYPE_REP_BYTE             (0x00)      // Repeated Byte Protocol Transaction
    #define TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR            (0x20)      // Auto-Increment Protocol Transaction
    #define TSL26711_REG_PROX_CMD_TYPE_SPECIAL              (0x60)      // Special Function
    #define TSL26711_REG_PROX_CMD_ADD_MASK                  (0x1F)      // Specifies a Special Function Command
    #define TSL26711_REG_PROX_CMD_ADD_NORMAL                (0x00)      // Normal — No Action
    #define TSL26711_REG_PROX_CMD_ADD_PROX_INTR             (0x05)      // Proximity Interrupt Clear

/**************************************************************************
    DIGITAL PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_ENABLE_PIEN_MASK              (0x20)      // Proximity Interrupt Mask
    #define TSL26711_REG_PROX_ENABLE_PIEN_NOT_ASSERT        (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TSL26711_REG_PROX_ENABLE_PIEN_ASSERT            (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TSL26711_REG_PROX_ENABLE_WEN_MASK               (0x08)      // Wait Enable
    #define TSL26711_REG_PROX_ENABLE_WEN_DISABLE            (0x00)      // Disables the Wait Timer
    #define TSL26711_REG_PROX_ENABLE_WEN_ENABLE             (0x08)      // Enables the Wait Timer

    #define TSL26711_REG_PROX_ENABLE_PEN_MASK               (0x04)      // Proximity Enable
    #define TSL26711_REG_PROX_ENABLE_PEN_DISABLE            (0x00)      // Deactivates the Proximity
    #define TSL26711_REG_PROX_ENABLE_PEN_ENABLE             (0x04)      // Activates the Proximity

    #define TSL26711_REG_PROX_ENABLE_PON_MASK               (0x01)      // Power ON
    #define TSL26711_REG_PROX_ENABLE_PON_DISABLE            (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TSL26711_REG_PROX_ENABLE_PON_ENABLE             (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL PROXIMITY MODULE PTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_PTIME_MASK                    (0xFF)      // Controls the Internal Integration Time of the Proximity ADC’s in 2.72 ms Increments
    #define TSL26711_REG_PROX_PTIME_2_72MS                  (0xFF)      // Cycles: 1, Time: 2.72 ms Max Count: 1023

/**************************************************************************
    DIGITAL PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_WTIME_MASK                    (0xFF)      // Wait Time is Set in 2.73 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TSL26711_REG_PROX_WTIME_1                       (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    #define TSL26711_REG_PROX_WTIME_74                      (0xB6)      // Wait Time: 74, Time (WLONG = 0): 201 ms Time (WLONG = 1):  2.4 sec
    #define TSL26711_REG_PROX_WTIME_256                     (0x00)      // Wait Time: 256, Time (WLONG = 0): 696 ms Time (WLONG = 1):  8.3 sec

/**************************************************************************
    DIGITAL PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_CONFIG_WLONG_MASK             (0x02)      // Wait Long
    #define TSL26711_REG_PROX_CONFIG_WLONG_NOT_ASSERT       (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TSL26711_REG_PROX_CONFIG_WLONG_ASSERT           (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    DIGITAL PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL26711_REG_PROX_CONTROL_PDRIVE_MASK           (0xC0)      // Proximity LED Drive Strength
    #define TSL26711_REG_PROX_CONTROL_PDRIVE_100            (0x00)      // LED Strength – PDL=0: 100mA
    #define TSL26711_REG_PROX_CONTROL_PDRIVE_50             (0x40)      // LED Strength – PDL=0: 50mA
    #define TSL26711_REG_PROX_CONTROL_PDRIVE_25             (0x80)      // LED Strength – PDL=0: 25mA
    #define TSL26711_REG_PROX_CONTROL_PDRIVE_12_5           (0xC0)      // LED Strength – PDL=0: 12.5mA

    #define TSL26711_REG_PROX_CONTROL_PDIODE_MASK           (0x30)      // Proximity Diode Selector
    #define TSL26711_REG_PROX_CONTROL_PDIODE_NONE           (0x00)      // Proximity Uses Neither Diode
    #define TSL26711_REG_PROX_CONTROL_PDIODE_CH0            (0x10)      // Proximity Uses the CH0 Diode
    #define TSL26711_REG_PROX_CONTROL_PDIODE_CH1            (0x20)      // Proximity Uses the CH1 Diode


typedef enum
{
    COMMAND_SELECT                  = TSL26711_REG_PROX_CMD_SELECT,
    COMMAND_REP_BYTE                = TSL26711_REG_PROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR           = TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL             = TSL26711_REG_PROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL              = TSL26711_REG_PROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR           = TSL26711_REG_PROX_CMD_ADD_PROX_INTR
    
} tslCommand_t;

typedef enum
{
    PIEN_NOT_ASSERT                 = TSL26711_REG_PROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                     = TSL26711_REG_PROX_ENABLE_PIEN_ASSERT
    
} tslProximityInterrupt_t;


typedef enum
{
    WEN_DISABLE                     = TSL26711_REG_PROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                      = TSL26711_REG_PROX_ENABLE_WEN_ENABLE
    
} tslWaitEnable_t;

typedef enum
{
    PEN_DISABLE                     = TSL26711_REG_PROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                      = TSL26711_REG_PROX_ENABLE_PEN_ENABLE
    
} tslProximityEnable_t;

typedef enum
{
    PON_DISABLE                     = TSL26711_REG_PROX_ENABLE_PON_DISABLE,
    PON_ENABLE                      = TSL26711_REG_PROX_ENABLE_PON_ENABLE
    
} tslPowerEnable_t;

typedef enum
{
    PTIME_2_72MS                    = TSL26711_REG_PROX_PTIME_2_72MS
    
} tslPTime_t;

typedef enum
{
    WTIME_1                         = TSL26711_REG_PROX_WTIME_1,
    WTIME_74                        = TSL26711_REG_PROX_WTIME_74,
    WTIME_256                       = TSL26711_REG_PROX_WTIME_256
    
} tslWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                = TSL26711_REG_PROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                    = TSL26711_REG_PROX_CONFIG_WLONG_ASSERT
    
} tslWaitLong_t;

typedef enum
{
    PDRIVE_100                      = TSL26711_REG_PROX_CONTROL_PDRIVE_100,
    PDRIVE_50                       = TSL26711_REG_PROX_CONTROL_PDRIVE_50,
    PDRIVE_25                       = TSL26711_REG_PROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                     = TSL26711_REG_PROX_CONTROL_PDRIVE_12_5
    
} tslProximityDrive_t;

typedef enum
{
    PDIODE_NONE                     = TSL26711_REG_PROX_CONTROL_PDIODE_NONE,
    PDIODE_CH0                      = TSL26711_REG_PROX_CONTROL_PDIODE_CH0,
    PDIODE_CH1                      = TSL26711_REG_PROX_CONTROL_PDIODE_CH1
    
} tslProximityDiode_t;

typedef struct
{
    float P;
    
} tslSensorData_t;


class TSL26711
{
    protected:
        // Instance-specific properties
        uint8_t tsl_conversionDelay;
        tslCommand_t tsl_command;
        tslProximityInterrupt_t tsl_proximityinterrupt;
        tslWaitEnable_t tsl_waitenable;
        tslProximityEnable_t tsl_proximityenable;
        tslPowerEnable_t tsl_powerenable;
        tslPTime_t tsl_ptime;
        tslWTime_t tsl_wtime;
        tslWaitLong_t tsl_waitlong;
        tslProximityDrive_t tsl_proximitydrive;
        tslProximityDiode_t tsl_proximitydiode;

    public:
        uint8_t tsl_i2cAddress;
        tslSensorData_t tsl_proxData;
        void getAddr_TSL26711(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpProximity(void);
        void Measure_Proximity(void);
        void setCommand(tslCommand_t command);
        tslCommand_t getCommand(void);
        void setProximityInterrupt(tslProximityInterrupt_t proximityinterrupt);
        tslProximityInterrupt_t getProximityInterrupt(void);
        void setWaitEnable(tslWaitEnable_t waitenable);
        tslWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tslProximityEnable_t proximityenable);
        tslProximityEnable_t getProximityEnable(void);
        void setPowerEnable(tslPowerEnable_t powerenable);
        tslPowerEnable_t getPowerEnable(void);
        void setPTime(tslPTime_t ptime);
        tslPTime_t getPTime(void);
        void setWTime(tslWTime_t wtime);
        tslWTime_t getWTime(void);
        void setWaitLong(tslWaitLong_t waitlong);
        tslWaitLong_t getWaitLong(void);
        void setProximityDrive(tslProximityDrive_t proximitydrive);
        tslProximityDrive_t getProximityDrive(void);
        void setProximityDiode(tslProximityDiode_t proximitydiode);
        tslProximityDiode_t getProximityDiode(void);
    
    private:
};
