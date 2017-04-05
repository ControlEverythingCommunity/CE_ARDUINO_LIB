/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD37821
        This code is designed to work with the TMD37821_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TMD37821_I2CS#tabs-0-product_tabset-2
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
    #define TMD37821_DEFAULT_ADDRESS                            (0x39)
    #define TMD37821_COLORPROX_DEV_ID                           (0x30)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMD37821_CONVERSIONDELAY                            (100)

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE REGISTERS
**************************************************************************/
    #define TMD37821_REG_COLORPROX_COMMAND                                  // Specifies Register Address
    #define TMD37821_REG_COLORPROX_ENABLE                       (0x00)      // Enables States and Interrupts Register
    #define TMD37821_REG_COLORPROX_ATIME                        (0x01)      // RGBC Integration Time Register
    #define TMD37821_REG_COLORPROX_WTIME                        (0x03)      // Wait Time Register
    #define TMD37821_REG_COLORPROX_AILTL                        (0x04)      // Clear Interrupt Low Threshold Low Byte Register
    #define TMD37821_REG_COLORPROX_AILTH                        (0x05)      // Clear Interrupt Low Threshold High Byte Register
    #define TMD37821_REG_COLORPROX_AIHTL                        (0x06)      // Clear Interrupt High Threshold Low Byte Register
    #define TMD37821_REG_COLORPROX_AIHTH                        (0x07)      // Clear Interrupt High Threshold High Byte Register
    #define TMD37821_REG_COLORPROX_PILTL                        (0x08)      // Proximity Interrupt Low Threshold Low Byte Register
    #define TMD37821_REG_COLORPROX_PILTH                        (0x09)      // Proximity Interrupt Low Threshold High Byte Register
    #define TMD37821_REG_COLORPROX_PIHTL                        (0x0A)      // Proximity Interrupt High Threshold Low Byte Register
    #define TMD37821_REG_COLORPROX_PIHTH                        (0x0B)      // Proximity Interrupt High Threshold High Byte Register
    #define TMD37821_REG_COLORPROX_PERS                         (0x0C)      // Interrupt Persistence Filters Register
    #define TMD37821_REG_COLORPROX_CONFIG                       (0x0D)      // Configuration Register
    #define TMD37821_REG_COLORPROX_PPULSE                       (0x0E)      // Proximity Pulse Count Register
    #define TMD37821_REG_COLORPROX_CONTROL                      (0x0F)      // Control Register
    #define TMD37821_REG_COLORPROX_REVISION                     (0x11)      // Die Revision Number Register
    #define TMD37821_REG_COLORPROX_ID                           (0x12)      // Device ID Register
    #define TMD37821_REG_COLORPROX_STATUS                       (0x13)      // Device Status Register
    #define TMD37821_REG_COLORPROX_CDATAL                       (0x14)      // Clear ADC Low Data Register
    #define TMD37821_REG_COLORPROX_CDATAH                       (0x15)      // Clear ADC High Data Register
    #define TMD37821_REG_COLORPROX_RDATAL                       (0x16)      // Red ADC Low Data Register
    #define TMD37821_REG_COLORPROX_RDATAH                       (0x17)      // Red ADC High Data Register
    #define TMD37821_REG_COLORPROX_GDATAL                       (0x18)      // Green ADC Low Data Register
    #define TMD37821_REG_COLORPROX_GDATAH                       (0x19)      // Green ADC High Data Register
    #define TMD37821_REG_COLORPROX_BDATAL                       (0x1A)      // Blue ADC Low Data Register
    #define TMD37821_REG_COLORPROX_BDATAH                       (0x1B)      // Blue ADC High Data Register
    #define TMD37821_REG_COLORPROX_PDATAL                       (0x1C)      // Proximity ADC Low Data Register
    #define TMD37821_REG_COLORPROX_PDATAH                       (0x1D)      // Proximity ADC High Data Register

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_CMD_MASK                     (0x80)      // Select Command Register
    #define TMD37821_REG_COLORPROX_CMD_SELECT                   (0x80)      // Addresses COMMAND Register
    #define TMD37821_REG_COLORPROX_CMD_TYPE_MASK                (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TMD37821_REG_COLORPROX_CMD_TYPE_REP_BYTE            (0x00)      // Repeated Byte Protocol Transaction
    #define TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR           (0x20)      // Auto-Increment Protocol Transaction
    #define TMD37821_REG_COLORPROX_CMD_TYPE_SPECIAL             (0x60)      // Special Function
    #define TMD37821_REG_COLORPROX_CMD_ADD_MASK                 (0x1F)      // Specifies a Special Function Command
    #define TMD37821_REG_COLORPROX_CMD_ADD_NORMAL               (0x00)      // Normal — No Action
    #define TMD37821_REG_COLORPROX_CMD_ADD_PROX_INTR            (0x05)      // Proximity Interrupt Clear
    #define TMD37821_REG_COLORPROX_CMD_ADD_CLEAR_INTR           (0x06)      // Clear Interrupt Clear
    #define TMD37821_REG_COLORPROX_CMD_ADD_PROX_CLEAR_INTR      (0x07)      // Proximity and Clear Interrupt Clear

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE ENABLE REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_ENABLE_PIEN_MASK             (0x20)      // Proximity Interrupt Mask
    #define TMD37821_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT       (0x00)      // Not Asserted, Did Not Permits Proximity Interrupts to be Generated
    #define TMD37821_REG_COLORPROX_ENABLE_PIEN_ASSERT           (0x20)      // Asserted, Permits Proximity Interrupts to be Generated

    #define TMD37821_REG_COLORPROX_ENABLE_AIEN_MASK             (0x10)      // Ambient Light Sensing (ALS) Interrupt Mask
    #define TMD37821_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT       (0x00)      // Not Asserted, Did Not Permits Ambient Light Sensing (ALS) Interrupts to be Generated
    #define TMD37821_REG_COLORPROX_ENABLE_AIEN_ASSERT           (0x10)      // Asserted, Permits Ambient Light Sensing (ALS) Interrupts to be Generated

    #define TMD37821_REG_COLORPROX_ENABLE_WEN_MASK              (0x08)      // Wait Enable
    #define TMD37821_REG_COLORPROX_ENABLE_WEN_DISABLE           (0x00)      // Disables the Wait Timer
    #define TMD37821_REG_COLORPROX_ENABLE_WEN_ENABLE            (0x08)      // Enables the Wait Timer

    #define TMD37821_REG_COLORPROX_ENABLE_PEN_MASK              (0x04)      // Proximity Enable
    #define TMD37821_REG_COLORPROX_ENABLE_PEN_DISABLE           (0x00)      // Deactivates the Proximity
    #define TMD37821_REG_COLORPROX_ENABLE_PEN_ENABLE            (0x04)      // Activates the Proximity

    #define TMD37821_REG_COLORPROX_ENABLE_AEN_MASK              (0x02)      // 4-Channel RGBC ADC Enable
    #define TMD37821_REG_COLORPROX_ENABLE_AEN_DISABLE           (0x00)      // Disables the RGBC ADC
    #define TMD37821_REG_COLORPROX_ENABLE_AEN_ENABLE            (0x02)      // Activates the RGBC ADC

    #define TMD37821_REG_COLORPROX_ENABLE_PON_MASK              (0x01)      // Power ON
    #define TMD37821_REG_COLORPROX_ENABLE_PON_DISABLE           (0x00)      // Disables the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    #define TMD37821_REG_COLORPROX_ENABLE_PON_ENABLE            (0x01)      // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE ATIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_ATIME_MASK                   (0xFF)      // Controls the Internal Integration Time of the RGBC Channel ADC’s in 2.38 ms Increments
    #define TMD37821_REG_COLORPROX_ATIME_2_38MS                 (0xFF)      // Cycles: 1, Time: 2.38 ms Max Count: 1024
    #define TMD37821_REG_COLORPROX_ATIME_24MS                   (0xF6)      // Cycles: 10, Time: 24 ms Max Count: 10240
    #define TMD37821_REG_COLORPROX_ATIME_100MS                  (0xDB)      // Cycles: 37, Time: 100 ms Max Count: 43008
    #define TMD37821_REG_COLORPROX_ATIME_152MS                  (0xC0)      // Cycles: 64, Time: 152 ms Max Count: 65535
    #define TMD37821_REG_COLORPROX_ATIME_609MS                  (0x00)      // Cycles: 256, Time: 609 ms Max Count: 65535

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE WTIME REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_WTIME_MASK                   (0xFF)      // Wait Time is Set in 2.38 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    #define TMD37821_REG_COLORPROX_WTIME_1                      (0xFF)      // Wait Time: 1, Time (WLONG = 0): 2.38 ms Time (WLONG = 1):  0.03 sec
    #define TMD37821_REG_COLORPROX_WTIME_85                     (0xB6)      // Wait Time: 85, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.43 sec
    #define TMD37821_REG_COLORPROX_WTIME_256                    (0x00)      // Wait Time: 256, Time (WLONG = 0): 609 ms Time (WLONG = 1):  7.31 sec

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE CONFIGURATION REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_CONFIG_WLONG_MASK            (0x02)      // Wait Long
    #define TMD37821_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT      (0x00)      // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    #define TMD37821_REG_COLORPROX_CONFIG_WLONG_ASSERT          (0x02)      // Asserted, the Wait Cycles are Increased by a Factor 12X from that Programmed in the WTIME Register

/**************************************************************************
    DIGITAL COLOR, PROXIMITY MODULE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TMD37821_REG_COLORPROX_CONTROL_PDRIVE_MASK          (0xC0)      // Proximity LED Drive Strength
    #define TMD37821_REG_COLORPROX_CONTROL_PDRIVE_100           (0x00)      // LED Strength – PDL=0: 100%
    #define TMD37821_REG_COLORPROX_CONTROL_PDRIVE_50            (0x40)      // LED Strength – PDL=0: 50%
    #define TMD37821_REG_COLORPROX_CONTROL_PDRIVE_25            (0x80)      // LED Strength – PDL=0: 25%
    #define TMD37821_REG_COLORPROX_CONTROL_PDRIVE_12_5          (0xC0)      // LED Strength – PDL=0: 12.5%

    #define TMD37821_REG_COLORPROX_CONTROL_PSAT_MASK            (0x10)      // PDATA Output
    #define TMD37821_REG_COLORPROX_CONTROL_PSAT_0               (0x00)      // PDATA Output Regardless of Ambient Light Level
    #define TMD37821_REG_COLORPROX_CONTROL_PSAT_1               (0x10)      // PDATA Output Equal to Dark Current Value if Saturated

    #define TMD37821_REG_COLORPROX_CONTROL_AGAIN_MASK           (0x03)      // RGBC Gain Control
    #define TMD37821_REG_COLORPROX_CONTROL_AGAIN_1X             (0x00)      // 1X Gain
    #define TMD37821_REG_COLORPROX_CONTROL_AGAIN_4X             (0x01)      // 4X Gain
    #define TMD37821_REG_COLORPROX_CONTROL_AGAIN_16X            (0x02)      // 16X Gain
    #define TMD37821_REG_COLORPROX_CONTROL_AGAIN_60X            (0x03)      // 60X Gain


typedef enum
{
    COMMAND_SELECT                      = TMD37821_REG_COLORPROX_CMD_SELECT,
    COMMAND_REP_BYTE                    = TMD37821_REG_COLORPROX_CMD_TYPE_REP_BYTE,
    COMMAND_REP_AUTO_INCR               = TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR,
    COMMAND_REP_SPECIAL                 = TMD37821_REG_COLORPROX_CMD_TYPE_SPECIAL,
    COMMAND_ADD_NORMAL                  = TMD37821_REG_COLORPROX_CMD_ADD_NORMAL,
    COMMAND_ADD_PROX_INTR               = TMD37821_REG_COLORPROX_CMD_ADD_PROX_INTR,
    COMMAND_ADD_CLEAR_INTR              = TMD37821_REG_COLORPROX_CMD_ADD_CLEAR_INTR,
    COMMAND_ADD_PROX_CLEAR_INTR         = TMD37821_REG_COLORPROX_CMD_ADD_PROX_CLEAR_INTR
    
} tmdCommand_t;

typedef enum
{
    PIEN_NOT_ASSERT                     = TMD37821_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT,
    PIEN_ASSERT                         = TMD37821_REG_COLORPROX_ENABLE_PIEN_ASSERT
    
} tmdProximityInterrupt_t;

typedef enum
{
    AIEN_NOT_ASSERT                     = TMD37821_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT,
    AIEN_ASSERT                         = TMD37821_REG_COLORPROX_ENABLE_AIEN_ASSERT
    
} tmdColorInterrupt_t;

typedef enum
{
    WEN_DISABLE                         = TMD37821_REG_COLORPROX_ENABLE_WEN_DISABLE,
    WEN_ENABLE                          = TMD37821_REG_COLORPROX_ENABLE_WEN_ENABLE
    
} tmdWaitEnable_t;

typedef enum
{
    PEN_DISABLE                         = TMD37821_REG_COLORPROX_ENABLE_PEN_DISABLE,
    PEN_ENABLE                          = TMD37821_REG_COLORPROX_ENABLE_PEN_ENABLE
    
} tmdProximityEnable_t;

typedef enum
{
    AEN_DISABLE                         = TMD37821_REG_COLORPROX_ENABLE_AEN_DISABLE,
    AEN_ENABLE                          = TMD37821_REG_COLORPROX_ENABLE_AEN_ENABLE
    
} tmdColorEnable_t;

typedef enum
{
    PON_DISABLE                         = TMD37821_REG_COLORPROX_ENABLE_PON_DISABLE,
    PON_ENABLE                          = TMD37821_REG_COLORPROX_ENABLE_PON_ENABLE
    
} tmdPowerEnable_t;

typedef enum
{
    ATIME_2_38MS                        = TMD37821_REG_COLORPROX_ATIME_2_38MS,
    ATIME_24MS                          = TMD37821_REG_COLORPROX_ATIME_24MS,
    ATIME_100MS                         = TMD37821_REG_COLORPROX_ATIME_100MS,
    ATIME_152MS                         = TMD37821_REG_COLORPROX_ATIME_152MS,
    ATIME_609MS                         = TMD37821_REG_COLORPROX_ATIME_609MS
    
} tmdATime_t;

typedef enum
{
    WTIME_1                             = TMD37821_REG_COLORPROX_WTIME_1,
    WTIME_85                            = TMD37821_REG_COLORPROX_WTIME_85,
    WTIME_256                           = TMD37821_REG_COLORPROX_WTIME_256
    
} tmdWTime_t;

typedef enum
{
    WLONG_NOT_ASSERT                    = TMD37821_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT,
    WLONG_ASSERT                        = TMD37821_REG_COLORPROX_CONFIG_WLONG_ASSERT
    
} tmdWaitLong_t;

typedef enum
{
    PDRIVE_100                          = TMD37821_REG_COLORPROX_CONTROL_PDRIVE_100,
    PDRIVE_50                           = TMD37821_REG_COLORPROX_CONTROL_PDRIVE_50,
    PDRIVE_25                           = TMD37821_REG_COLORPROX_CONTROL_PDRIVE_25,
    PDRIVE_12_5                         = TMD37821_REG_COLORPROX_CONTROL_PDRIVE_12_5
    
} tmdProximityDrive_t;

typedef enum
{
    PSAT_0                              = TMD37821_REG_COLORPROX_CONTROL_PSAT_0,
    PSAT_1                              = TMD37821_REG_COLORPROX_CONTROL_PSAT_1
    
} tmdProximityOutput_t;

typedef enum
{
    AGAIN_1X                            = TMD37821_REG_COLORPROX_CONTROL_AGAIN_1X,
    AGAIN_4X                            = TMD37821_REG_COLORPROX_CONTROL_AGAIN_4X,
    AGAIN_16X                           = TMD37821_REG_COLORPROX_CONTROL_AGAIN_16X,
    AGAIN_60X                           = TMD37821_REG_COLORPROX_CONTROL_AGAIN_60X
    
} tmdColorGain_t;

typedef struct
{
    uint16_t C;
    uint16_t R;
    uint16_t G;
    uint16_t B;
    uint16_t P;
    
} tmdSensorData_t;


class TMD37821
{
    protected:
        // Instance-specific properties
        uint8_t tmd_conversionDelay;
        tmdCommand_t tmd_command;
        tmdProximityInterrupt_t tmd_proximityinterrupt;
        tmdColorInterrupt_t tmd_colorinterrupt;
        tmdWaitEnable_t tmd_waitenable;
        tmdProximityEnable_t tmd_proximityenable;
        tmdColorEnable_t tmd_colorenable;
        tmdPowerEnable_t tmd_powerenable;
        tmdATime_t tmd_atime;
        tmdWTime_t tmd_wtime;
        tmdWaitLong_t tmd_waitlong;
        tmdProximityDrive_t tmd_proximitydrive;
        tmdProximityOutput_t tmd_proximityoutput;
        tmdColorGain_t tmd_colorgain;

    public:
        uint8_t tmd_i2cAddress;
        tmdSensorData_t tmd_colorproxData;
        void getAddr_TMD37821(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpColorProximity(void);
        void Measure_Color(void);
        void Measure_Proximity(void);
        void setCommand(tmdCommand_t command);
        tmdCommand_t getCommand(void);
        void setProximityInterrupt(tmdProximityInterrupt_t proximityinterrupt);
        tmdProximityInterrupt_t getProximityInterrupt(void);
        void setColorInterrupt(tmdColorInterrupt_t colorinterrupt);
        tmdColorInterrupt_t getColorInterrupt(void);
        void setWaitEnable(tmdWaitEnable_t waitenable);
        tmdWaitEnable_t getWaitEnable(void);
        void setProximityEnable(tmdProximityEnable_t proximityenable);
        tmdProximityEnable_t getProximityEnable(void);
        void setColorEnable(tmdColorEnable_t colorenable);
        tmdColorEnable_t getColorEnable(void);
        void setPowerEnable(tmdPowerEnable_t powerenable);
        tmdPowerEnable_t getPowerEnable(void);
        void setATime(tmdATime_t atime);
        tmdATime_t getATime(void);
        void setWTime(tmdWTime_t wtime);
        tmdWTime_t getWTime(void);
        void setWaitLong(tmdWaitLong_t waitlong);
        tmdWaitLong_t getWaitLong(void);
        void setProximityDrive(tmdProximityDrive_t proximitydrive);
        tmdProximityDrive_t getProximityDrive(void);
        void setProximityOutput(tmdProximityOutput_t proximityoutput);
        tmdProximityOutput_t getProximityOutput(void);
        void setColorGain(tmdColorGain_t colorgain);
        tmdColorGain_t getColorGain(void);
    
    private:
};
