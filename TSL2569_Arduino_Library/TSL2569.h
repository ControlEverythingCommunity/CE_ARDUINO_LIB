/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2569
        This code is designed to work with the TSL2569_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2569_I2CS#tabs-0-product_tabset-2
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
    #define TSL2569_DEFAULT_ADDRESS                         (0x39)
    #define TSL2569_DEFAULT_LOW                             (0x29)
    #define TSL2569_DEFAULT_HIGH                            (0x49)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL2569_CONVERSIONDELAY                         (100)

/**************************************************************************
    CONSTANTS
**************************************************************************/
    #define TSL2569_LUX_LUXSCALE                            (16)        // Scale by 2^16
    #define TSL2569_LUX_RATIOSCALE                          (9)         // Scale ratio by 2^9

/**************************************************************************
    INTEGRATION TIME SCALING FACTORS
**************************************************************************/
    #define TSL2569_LUX_CHSCALE                             (10)        // Scale channel values by 2^10
    #define TSL2569_LUX_CHSCALE_TINT0                       (0x7517)    // 322/11 * 2^TSL2569_LUX_CHSCALE
    #define TSL2569_LUX_CHSCALE_TINT1                       (0x0FE7)    // 322/81 * 2^TSL2569_LUX_CHSCALE

/**************************************************************************
    T PACKAGE COEFFICIENTS
**************************************************************************/
/*
    // For Ch1/Ch0=0.00 to 0.35:
    // Lux=0.00763*Ch0−0.01031*Ch1
    //
    // For Ch1/Ch0=0.35 to 0.50:
    // Lux=0.00817*Ch0−0.01188*Ch1
    //
    // For Ch1/Ch0=0.50 to 0.60:
    // Lux=0.00723*Ch0−0.01000*Ch1
    //
    // For Ch1/Ch0=0.60 to 0.72:
    // Lux=0.00573*Ch0−0.00750*Ch1
    //
    // For Ch1/Ch0=0.72 to 0.85:
    // Lux=0.00216*Ch0−0.00254*Ch1
    //
    // For Ch1/Ch0>0.85:
    // Lux/Ch0=0
*/
    #define TSL2569_LUX_K1T                                 (0x00B3)    // 0.35 * 2^RATIO_SCALE
    #define TSL2569_LUX_B1T                                 (0x01F4)    // 0.00763 * 2^LUX_SCALE
    #define TSL2569_LUX_M1T                                 (0x02A4)    // 0.01031 * 2^LUX_SCALE
    #define TSL2569_LUX_K2T                                 (0x0100)    // 0.50 * 2^RATIO_SCALE
    #define TSL2569_LUX_B2T                                 (0x0217)    // 0.00817 * 2^LUX_SCALE
    #define TSL2569_LUX_M2T                                 (0x030A)    // 0.01188 * 2^LUX_SCALE
    #define TSL2569_LUX_K3T                                 (0x0133)    // 0.60 * 2^RATIO_SCALE
    #define TSL2569_LUX_B3T                                 (0x01DA)    // 0.00723 * 2^LUX_SCALE
    #define TSL2569_LUX_M3T                                 (0x028F)    // 0.01000 * 2^LUX_SCALE
    #define TSL2569_LUX_K4T                                 (0x0171)    // 0.72 * 2^RATIO_SCALE
    #define TSL2569_LUX_B4T                                 (0x0177)    // 0.00573 * 2^LUX_SCALE
    #define TSL2569_LUX_M4T                                 (0x01EC)    // 0.00750 * 2^LUX_SCALE
    #define TSL2569_LUX_K5T                                 (0x01B3)    // 0.85 * 2^RATIO_SCALE
    #define TSL2569_LUX_B5T                                 (0x008D)    // 0.00216 * 2^LUX_SCALE
    #define TSL2569_LUX_M5T                                 (0x00A6)    // 0.00254 * 2^LUX_SCALE
    #define TSL2569_LUX_K6T                                 (0x01B3)    // 0.85 * 2^RATIO_SCALE
    #define TSL2569_LUX_B6T                                 (0x0000)    // 0.00000 * 2^LUX_SCALE
    #define TSL2569_LUX_M6T                                 (0x0000)    // 0.00000 * 2^LUX_SCALE

/**************************************************************************
    CS PACKAGE COEFFICIENTS
**************************************************************************/
/*
    // For Ch1/Ch0=0.00 to 0.35:
    // Lux=0.00713*Ch0−0.00975*Ch1
    //
    // For Ch1/Ch0=0.35 to 0.45:
    // Lux=0.00813*Ch0−0.01250*Ch1
    //
    // For Ch1/Ch0=0.45 to 0.52:
    // Lux=0.00935*Ch0−0.01521*Ch1
    //
    // For Ch1/Ch0=0.52 to 0.67:
    // Lux=0.00394*Ch0−0.00482*Ch1
    //
    // For Ch1/Ch0=0.67 to 0.85:
    // Lux=0.00337*Ch0−0.00396*Ch1
    //
    // For Ch1/Ch0>0.85:
    // Lux/Ch0=0
*/
    #define TSL2569_LUX_K1C                                 (0x00B3)    // 0.35 * 2^RATIO_SCALE
    #define TSL2569_LUX_B1C                                 (0x01D3)    // 0.00713 * 2^LUX_SCALE
    #define TSL2569_LUX_M1C                                 (0x027F)    // 0.00975 * 2^LUX_SCALE
    #define TSL2569_LUX_K2C                                 (0x00E6)    // 0.45 * 2^RATIO_SCALE
    #define TSL2569_LUX_B2C                                 (0x0214)    // 0.00813 * 2^LUX_SCALE
    #define TSL2569_LUX_M2C                                 (0x0333)    // 0.01250 * 2^LUX_SCALE
    #define TSL2569_LUX_K3C                                 (0x010A)    // 0.52 * 2^RATIO_SCALE
    #define TSL2569_LUX_B3C                                 (0x0265)    // 0.00935 * 2^LUX_SCALE
    #define TSL2569_LUX_M3C                                 (0x03E5)    // 0.01521 * 2^LUX_SCALE
    #define TSL2569_LUX_K4C                                 (0x0157)    // 0.67 * 2^RATIO_SCALE
    #define TSL2569_LUX_B4C                                 (0x0102)    // 0.00394 * 2^LUX_SCALE
    #define TSL2569_LUX_M4C                                 (0x013C)    // 0.00482 * 2^LUX_SCALE
    #define TSL2569_LUX_K5C                                 (0x01B3)    // 0.85 * 2^RATIO_SCALE
    #define TSL2569_LUX_B5C                                 (0x00DD)    // 0.00337 * 2^LUX_SCALE
    #define TSL2569_LUX_M5C                                 (0x0104)    // 0.00396 * 2^LUX_SCALE
    #define TSL2569_LUX_K6C                                 (0x01B3)    // 0.85 * 2^RATIO_SCALE
    #define TSL2569_LUX_B6C                                 (0x0000)    // 0.00000 * 2^LUX_SCALE
    #define TSL2569_LUX_M6C                                 (0x0000)    // 0.00000 * 2^LUX_SCALE

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER REGISTERS
**************************************************************************/
    #define TSL2569_REG_ALS_COMMAND                                     // Specifies Register Address
    #define TSL2569_REG_ALS_CONTROL                         (0x00)      // Control of Basic Functions Register
    #define TSL2569_REG_ALS_TIMING                          (0x01)      // Integration Time / Gain Control Register
    #define TSL2569_REG_ALS_AILTL                           (0x02)      // Low Interrupt Threshold Low Byte Register
    #define TSL2569_REG_ALS_AILTH                           (0x03)      // Low Interrupt Threshold High Byte Register
    #define TSL2569_REG_ALS_AIHTL                           (0x04)      // High Interrupt Threshold Low Byte Register
    #define TSL2569_REG_ALS_AIHTH                           (0x05)      // High Interrupt Threshold High Byte Register
    #define TSL2569_REG_ALS_INTERRUPT                       (0x06)      // Interrupt Control Register
    #define TSL2569_REG_ALS_ID                              (0x0A)      // Part Number / Rev ID Register
    #define TSL2569_REG_ALS_C0DATAL                         (0x0C)      // ADC Channel 0 Low Data Register
    #define TSL2569_REG_ALS_C0DATAH                         (0x0D)      // ADC Channel 0 High Data Register
    #define TSL2569_REG_ALS_C1DATAL                         (0x0E)      // ADC Channel 1 Low Data Register
    #define TSL2569_REG_ALS_C1DATAH                         (0x0F)      // ADC Channel 1 High Data Register


/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2569_REG_ALS_CMD_SELECT                      (0x80)      // Select Command Register
    #define TSL2569_REG_ALS_CMD_CLEAR                       (0x40)      // Interrupt Clear
    #define TSL2569_REG_ALS_CMD_WORD                        (0x20)      // SMB Read/Write Word Protocol
    #define TSL2569_REG_ALS_CMD_BLOCK                       (0x10)      // SMB Read/Write Block Protocol

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2569_REG_ALS_CONTROL_POWER_MASK              (0x03)      // Power ON
    #define TSL2569_REG_ALS_CONTROL_POWER_DISABLE           (0x00)      // Powers Off the Device
    #define TSL2569_REG_ALS_CONTROL_POWER_ENABLE            (0x03)      // Powers On the Device

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER TIMING REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2569_REG_ALS_TIMING_GAIN_MASK                (0x10)      // Switches Gain Between Low Gain and High Gain Modes
    #define TSL2569_REG_ALS_TIMING_GAIN_1X                  (0x00)      // 1X
    #define TSL2569_REG_ALS_TIMING_GAIN_16X                 (0x10)      // 16X

    #define TSL2569_REG_ALS_TIMING_INTEG_MASK               (0x03)      // Integrate Time
    #define TSL2569_REG_ALS_TIMING_INTEG_0_034              (0x00)      // 13.7 ms
    #define TSL2569_REG_ALS_TIMING_INTEG_0_252              (0x01)      // 101 ms
    #define TSL2569_REG_ALS_TIMING_INTEG_1                  (0x02)      // 402 ms

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER INTERRUPT CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2569_REG_ALS_INTERRUPT_INTR_MASK             (0x30)      // INTR Control Select. This Field Determines Mode of Interrupt Logic
    #define TSL2569_REG_ALS_INTERRUPT_INTR_DISABLE          (0x00)      // Interrupt Output Disabled
    #define TSL2569_REG_ALS_INTERRUPT_INTR_LEVEL            (0x10)      // Level Interrupt
    #define TSL2569_REG_ALS_INTERRUPT_INTR_SMB              (0x20)      // SMB-Alert Compliant
    #define TSL2569_REG_ALS_INTERRUPT_INTR_SETINTR          (0x30)      // Sets an Interrupt and Functions as Mode 10

    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_MASK          (0x0F)      // Interrupt Persistence. Controls Rate of Interrupts to the Host Processor
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_EVERY         (0x00)      // Every ADC Cycle Generates Interrupt
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_ANY           (0x01)      // Any Value Outside of Threshold Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_2             (0x02)      // 2 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_3             (0x03)      // 3 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_4             (0x04)      // 4 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_5             (0x05)      // 5 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_6             (0x06)      // 6 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_7             (0x07)      // 7 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_8             (0x08)      // 8 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_9             (0x09)      // 9 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_10            (0x0A)      // 10 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_11            (0x0B)      // 11 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_12            (0x0C)      // 12 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_13            (0x0D)      // 13 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_14            (0x0E)      // 14 Integration Time Periods Out of Range
    #define TSL2569_REG_ALS_INTERRUPT_PERSIST_15            (0x0F)      // 15 Integration Time Periods Out of Range


typedef enum
{
    COMMAND_SELECT                  = TSL2569_REG_ALS_CMD_SELECT,
    COMMAND_CLEAR                   = TSL2569_REG_ALS_CMD_CLEAR,
    COMMAND_WORD                    = TSL2569_REG_ALS_CMD_WORD,
    COMMAND_BLOCK                   = TSL2569_REG_ALS_CMD_BLOCK,
    
} tslCommand_t;

typedef enum
{
    POWER_DISABLE                   = TSL2569_REG_ALS_CONTROL_POWER_DISABLE,
    POWER_ENABLE                    = TSL2569_REG_ALS_CONTROL_POWER_ENABLE
    
} tslPowerEnable_t;

typedef enum
{
    GAIN_1X                         = TSL2569_REG_ALS_TIMING_GAIN_1X,
    GAIN_16X                        = TSL2569_REG_ALS_TIMING_GAIN_16X
    
} tslGain_t;

typedef enum
{
    INTEGRATIONTIME_13_7MS          = TSL2569_REG_ALS_TIMING_INTEG_0_034,
    INTEGRATIONTIME_101MS           = TSL2569_REG_ALS_TIMING_INTEG_0_252,
    INTEGRATIONTIME_402MS           = TSL2569_REG_ALS_TIMING_INTEG_1
    
} tslIntegTime_t;

typedef enum
{
    INTR_CTL_DISABLE                = TSL2569_REG_ALS_INTERRUPT_INTR_DISABLE,
    INTR_CTL_LEVEL                  = TSL2569_REG_ALS_INTERRUPT_INTR_LEVEL,
    INTR_CTL_SMB                    = TSL2569_REG_ALS_INTERRUPT_INTR_SMB,
    INTR_CTL_SETINTR                = TSL2569_REG_ALS_INTERRUPT_INTR_SETINTR
    
} tslInterruptControl_t;

typedef enum
{
    INTR_PERSIST_EVERY              = TSL2569_REG_ALS_INTERRUPT_PERSIST_EVERY,
    INTR_PERSIST_ANY                = TSL2569_REG_ALS_INTERRUPT_PERSIST_ANY,
    INTR_PERSIST_2                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_2,
    INTR_PERSIST_3                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_3,
    INTR_PERSIST_4                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_4,
    INTR_PERSIST_5                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_5,
    INTR_PERSIST_6                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_6,
    INTR_PERSIST_7                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_7,
    INTR_PERSIST_8                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_8,
    INTR_PERSIST_9                  = TSL2569_REG_ALS_INTERRUPT_PERSIST_9,
    INTR_PERSIST_10                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_10,
    INTR_PERSIST_11                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_11,
    INTR_PERSIST_12                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_12,
    INTR_PERSIST_13                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_13,
    INTR_PERSIST_14                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_14,
    INTR_PERSIST_15                 = TSL2569_REG_ALS_INTERRUPT_PERSIST_15
    
} tslInterruptPersistence_t;


typedef struct
{
    uint16_t V;
    uint16_t IR;
    uint16_t F;
    
} tslSensorData_t;


class TSL2569
{
    protected:
        // Instance-specific properties
        uint8_t tsl_conversionDelay;
        tslCommand_t tsl_command;
        tslPowerEnable_t tsl_powerenable;
        tslGain_t tsl_gain;
        tslIntegTime_t tsl_integtime;
        tslInterruptControl_t tsl_interruptcontrol;
        tslInterruptPersistence_t tsl_interruptpersistence;

    public:
        uint8_t tsl_i2cAddress;
        tslSensorData_t tsl_alsData;
        void getAddr_TSL2569(uint8_t i2cAddress);
        bool begin(void);
        void setUpALS();
        uint32_t Measure_ALS(uint16_t channel1, uint16_t channel2);
        void getLuminosity(void);
        void setCommand(tslCommand_t command);
        tslCommand_t getCommand(void);
        void setPowerEnable(tslPowerEnable_t powerenable);
        tslPowerEnable_t getPowerEnable(void);
        void setGain(tslGain_t gain);
        tslGain_t getGain(void);
        void setIntegTime(tslIntegTime_t integtime);
        tslIntegTime_t getIntegTime(void);
        void setInterruptControl(tslInterruptControl_t interruptcontrol);
        tslInterruptControl_t getInterruptControl(void);
        void setInterruptPersistence(tslInterruptPersistence_t interruptpersistence);
        tslInterruptPersistence_t getInterruptPersistence(void);
    
    private:
};
