/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2561
        This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2
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
    #define TSL2561_DEFAULT_ADDRESS                         (0x39)
    #define TSL2561_DEFAULT_LOW                             (0x29)
    #define TSL2561_DEFAULT_HIGH                            (0x49)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TSL2561_CONVERSIONDELAY                         (100)

/**************************************************************************
    CONSTANTS
**************************************************************************/
    #define TSL2561_LUX_LUXSCALE                            (14)        // Scale by 2^14
    #define TSL2561_LUX_RATIOSCALE                          (9)         // Scale ratio by 2^9

/**************************************************************************
    INTEGRATION TIME SCALING FACTORS
**************************************************************************/
    #define TSL2561_LUX_CHSCALE                             (10)        // Scale channel values by 2^10
    #define TSL2561_LUX_CHSCALE_TINT0                       (0x7517)    // 322/11 * 2^TSL2561_LUX_CHSCALE
    #define TSL2561_LUX_CHSCALE_TINT1                       (0x0FE7)    // 322/81 * 2^TSL2561_LUX_CHSCALE

/**************************************************************************
    T, FN, AND CL PACKAGE COEFFICIENTS
**************************************************************************/
/*
    For Ch1/Ch0=0.00 to 0.50
    // Lux/Ch0=0.0304−0.062*((Ch1/Ch0)^1.4)
    // piecewise approximation
    // For Ch1/Ch0=0.00 to 0.125:
    // Lux/Ch0=0.0304−0.0272*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.125 to 0.250:
    // Lux/Ch0=0.0325−0.0440*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.250 to 0.375:
    // Lux/Ch0=0.0351−0.0544*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.375 to 0.50:
    // Lux/Ch0=0.0381−0.0624*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.50 to 0.61:
    // Lux/Ch0=0.0224−0.031*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.61 to 0.80:
    // Lux/Ch0=0.0128−0.0153*(Ch1/Ch0)
    //
    // For Ch1/Ch0=0.80 to 1.30:
    // Lux/Ch0=0.00146−0.00112*(Ch1/Ch0)
    //
    // For Ch1/Ch0>1.3:
    // Lux/Ch0=0
*/
    #define TSL2561_LUX_K1T                                 (0x0040)    // 0.125 * 2^RATIO_SCALE
    #define TSL2561_LUX_B1T                                 (0x01F2)    // 0.0304 * 2^LUX_SCALE
    #define TSL2561_LUX_M1T                                 (0x01BE)    // 0.0272 * 2^LUX_SCALE
    #define TSL2561_LUX_K2T                                 (0x0080)    // 0.250 * 2^RATIO_SCALE
    #define TSL2561_LUX_B2T                                 (0x0214)    // 0.0325 * 2^LUX_SCALE
    #define TSL2561_LUX_M2T                                 (0x02D1)    // 0.0440 * 2^LUX_SCALE
    #define TSL2561_LUX_K3T                                 (0x00C0)    // 0.375 * 2^RATIO_SCALE
    #define TSL2561_LUX_B3T                                 (0x023F)    // 0.0351 * 2^LUX_SCALE
    #define TSL2561_LUX_M3T                                 (0x037B)    // 0.0544 * 2^LUX_SCALE
    #define TSL2561_LUX_K4T                                 (0x0100)    // 0.50 * 2^RATIO_SCALE
    #define TSL2561_LUX_B4T                                 (0x0270)    // 0.0381 * 2^LUX_SCALE
    #define TSL2561_LUX_M4T                                 (0x03FE)    // 0.0624 * 2^LUX_SCALE
    #define TSL2561_LUX_K5T                                 (0x0138)    // 0.61 * 2^RATIO_SCALE
    #define TSL2561_LUX_B5T                                 (0x016F)    // 0.0224 * 2^LUX_SCALE
    #define TSL2561_LUX_M5T                                 (0x01FC)    // 0.0310 * 2^LUX_SCALE
    #define TSL2561_LUX_K6T                                 (0x019A)    // 0.80 * 2^RATIO_SCALE
    #define TSL2561_LUX_B6T                                 (0x00D2)    // 0.0128 * 2^LUX_SCALE
    #define TSL2561_LUX_M6T                                 (0x00FB)    // 0.0153 * 2^LUX_SCALE
    #define TSL2561_LUX_K7T                                 (0x029A)    // 1.3 * 2^RATIO_SCALE
    #define TSL2561_LUX_B7T                                 (0x0018)    // 0.00146 * 2^LUX_SCALE
    #define TSL2561_LUX_M7T                                 (0x0012)    // 0.00112 * 2^LUX_SCALE
    #define TSL2561_LUX_K8T                                 (0x029A)    // 1.3 * 2^RATIO_SCALE
    #define TSL2561_LUX_B8T                                 (0x0000)    // 0.000 * 2^LUX_SCALE
    #define TSL2561_LUX_M8T                                 (0x0000)    // 0.000 * 2^LUX_SCALE

/**************************************************************************
    CS PACKAGE COEFFICIENTS
**************************************************************************/
/*
    // For 0 <= Ch1/Ch0 <= 0.52
    // Lux/Ch0 = 0.0315−0.0593*((Ch1/Ch0)^1.4)
    // piecewise approximation
    // For 0 <= Ch1/Ch0 <= 0.13
    // Lux/Ch0 = 0.0315−0.0262*(Ch1/Ch0)
    // For 0.13 <= Ch1/Ch0 <= 0.26
    // Lux/Ch0 = 0.0337−0.0430*(Ch1/Ch0)
    // For 0.26 <= Ch1/Ch0 <= 0.39
    // Lux/Ch0 = 0.0363−0.0529*(Ch1/Ch0)
    // For 0.39 <= Ch1/Ch0 <= 0.52
    // Lux/Ch0 = 0.0392−0.0605*(Ch1/Ch0)
    // For 0.52 < Ch1/Ch0 <= 0.65
    // Lux/Ch0 = 0.0229−0.0291*(Ch1/Ch0)
    // For 0.65 < Ch1/Ch0 <= 0.80
    // Lux/Ch0 = 0.00157−0.00180*(Ch1/Ch0)
    // For 0.80 < Ch1/Ch0 <= 1.30
    // Lux/Ch0 = 0.00338−0.00260*(Ch1/Ch0)
    // For Ch1/Ch0 > 1.30
    // Lux = 0
*/
    #define TSL2561_LUX_K1C                                 (0x0043)    // 0.130 * 2^RATIO_SCALE
    #define TSL2561_LUX_B1C                                 (0x0204)    // 0.0315 * 2^LUX_SCALE
    #define TSL2561_LUX_M1C                                 (0x01AD)    // 0.0262 * 2^LUX_SCALE
    #define TSL2561_LUX_K2C                                 (0x0085)    // 0.260 * 2^RATIO_SCALE
    #define TSL2561_LUX_B2C                                 (0x0228)    // 0.0337 * 2^LUX_SCALE
    #define TSL2561_LUX_M2C                                 (0x02C1)    // 0.0430 * 2^LUX_SCALE
    #define TSL2561_LUX_K3C                                 (0x00c8)    // 0.390 * 2^RATIO_SCALE
    #define TSL2561_LUX_B3C                                 (0x0253)    // 0.0363 * 2^LUX_SCALE
    #define TSL2561_LUX_M3C                                 (0x0363)    // 0.0529 * 2^LUX_SCALE
    #define TSL2561_LUX_K4C                                 (0x010A)    // 0.520 * 2^RATIO_SCALE
    #define TSL2561_LUX_B4C                                 (0x0282)    // 0.0392 * 2^LUX_SCALE
    #define TSL2561_LUX_M4C                                 (0x03DF)    // 0.0605 * 2^LUX_SCALE
    #define TSL2561_LUX_K5C                                 (0x014D)    // 0.65 * 2^RATIO_SCALE
    #define TSL2561_LUX_B5C                                 (0x0177)    // 0.0229 * 2^LUX_SCALE
    #define TSL2561_LUX_M5C                                 (0x01DD)    // 0.0291 * 2^LUX_SCALE
    #define TSL2561_LUX_K6C                                 (0x019A)    // 0.80 * 2^RATIO_SCALE
    #define TSL2561_LUX_B6C                                 (0x0101)    // 0.0157 * 2^LUX_SCALE
    #define TSL2561_LUX_M6C                                 (0x0127)    // 0.0180 * 2^LUX_SCALE
    #define TSL2561_LUX_K7C                                 (0x029A)    // 1.3 * 2^RATIO_SCALE
    #define TSL2561_LUX_B7C                                 (0x0037)    // 0.00338 * 2^LUX_SCALE
    #define TSL2561_LUX_M7C                                 (0x002B)    // 0.00260 * 2^LUX_SCALE
    #define TSL2561_LUX_K8C                                 (0x029A)    // 1.3 * 2^RATIO_SCALE
    #define TSL2561_LUX_B8C                                 (0x0000)    // 0.000 * 2^LUX_SCALE
    #define TSL2561_LUX_M8C                                 (0x0000)    // 0.000 * 2^LUX_SCALE
/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER REGISTERS
**************************************************************************/
    #define TSL2561_REG_ALS_COMMAND                                     // Specifies Register Address
    #define TSL2561_REG_ALS_CONTROL                         (0x00)      // Control of Basic Functions Register
    #define TSL2561_REG_ALS_TIMING                          (0x01)      // Integration Time / Gain Control Register
    #define TSL2561_REG_ALS_AILTL                           (0x02)      // Low Interrupt Threshold Low Byte Register
    #define TSL2561_REG_ALS_AILTH                           (0x03)      // Low Interrupt Threshold High Byte Register
    #define TSL2561_REG_ALS_AIHTL                           (0x04)      // High Interrupt Threshold Low Byte Register
    #define TSL2561_REG_ALS_AIHTH                           (0x05)      // High Interrupt Threshold High Byte Register
    #define TSL2561_REG_ALS_INTERRUPT                       (0x06)      // Interrupt Control Register
    #define TSL2561_REG_ALS_ID                              (0x0A)      // Part Number / Rev ID Register
    #define TSL2561_REG_ALS_C0DATAL                         (0x0C)      // ADC Channel 0 Low Data Register
    #define TSL2561_REG_ALS_C0DATAH                         (0x0D)      // ADC Channel 0 High Data Register
    #define TSL2561_REG_ALS_C1DATAL                         (0x0E)      // ADC Channel 1 Low Data Register
    #define TSL2561_REG_ALS_C1DATAH                         (0x0F)      // ADC Channel 1 High Data Register


/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2561_REG_ALS_CMD_SELECT                      (0x80)      // Select Command Register
    #define TSL2561_REG_ALS_CMD_CLEAR                       (0x40)      // Interrupt Clear
    #define TSL2561_REG_ALS_CMD_WORD                        (0x20)      // SMB Read/Write Word Protocol
    #define TSL2561_REG_ALS_CMD_BLOCK                       (0x10)      // SMB Read/Write Block Protocol

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2561_REG_ALS_CONTROL_POWER_MASK              (0x03)      // Power ON
    #define TSL2561_REG_ALS_CONTROL_POWER_DISABLE           (0x00)      // Powers Off the Device
    #define TSL2561_REG_ALS_CONTROL_POWER_ENABLE            (0x03)      // Powers On the Device

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER TIMING REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2561_REG_ALS_TIMING_GAIN_MASK                (0x10)      // Switches Gain Between Low Gain and High Gain Modes
    #define TSL2561_REG_ALS_TIMING_GAIN_1X                  (0x00)      // 1X
    #define TSL2561_REG_ALS_TIMING_GAIN_16X                 (0x10)      // 16X

    #define TSL2561_REG_ALS_TIMING_INTEG_MASK               (0x03)      // Integrate Time
    #define TSL2561_REG_ALS_TIMING_INTEG_0_034              (0x00)      // 13.7 ms
    #define TSL2561_REG_ALS_TIMING_INTEG_0_252              (0x01)      // 101 ms
    #define TSL2561_REG_ALS_TIMING_INTEG_1                  (0x02)      // 402 ms

/**************************************************************************
    LIGHT-TO-DIGITAL CONVERTER INTERRUPT CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TSL2561_REG_ALS_INTERRUPT_INTR_MASK             (0x30)      // INTR Control Select. This Field Determines Mode of Interrupt Logic
    #define TSL2561_REG_ALS_INTERRUPT_INTR_DISABLE          (0x00)      // Interrupt Output Disabled
    #define TSL2561_REG_ALS_INTERRUPT_INTR_LEVEL            (0x10)      // Level Interrupt
    #define TSL2561_REG_ALS_INTERRUPT_INTR_SMB              (0x20)      // SMB-Alert Compliant
    #define TSL2561_REG_ALS_INTERRUPT_INTR_SETINTR          (0x30)      // Sets an Interrupt and Functions as Mode 10

    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_MASK          (0x0F)      // Interrupt Persistence. Controls Rate of Interrupts to the Host Processor
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_EVERY         (0x00)      // Every ADC Cycle Generates Interrupt
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_ANY           (0x01)      // Any Value Outside of Threshold Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_2             (0x02)      // 2 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_3             (0x03)      // 3 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_4             (0x04)      // 4 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_5             (0x05)      // 5 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_6             (0x06)      // 6 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_7             (0x07)      // 7 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_8             (0x08)      // 8 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_9             (0x09)      // 9 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_10            (0x0A)      // 10 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_11            (0x0B)      // 11 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_12            (0x0C)      // 12 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_13            (0x0D)      // 13 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_14            (0x0E)      // 14 Integration Time Periods Out of Range
    #define TSL2561_REG_ALS_INTERRUPT_PERSIST_15            (0x0F)      // 15 Integration Time Periods Out of Range


typedef enum
{
    COMMAND_SELECT                  = TSL2561_REG_ALS_CMD_SELECT,
    COMMAND_CLEAR                   = TSL2561_REG_ALS_CMD_CLEAR,
    COMMAND_WORD                    = TSL2561_REG_ALS_CMD_WORD,
    COMMAND_BLOCK                   = TSL2561_REG_ALS_CMD_BLOCK,
    
} tslCommand_t;

typedef enum
{
    POWER_DISABLE                   = TSL2561_REG_ALS_CONTROL_POWER_DISABLE,
    POWER_ENABLE                    = TSL2561_REG_ALS_CONTROL_POWER_ENABLE
    
} tslPowerEnable_t;

typedef enum
{
    GAIN_1X                         = TSL2561_REG_ALS_TIMING_GAIN_1X,
    GAIN_16X                        = TSL2561_REG_ALS_TIMING_GAIN_16X
    
} tslGain_t;

typedef enum
{
    INTEGRATIONTIME_13_7MS          = TSL2561_REG_ALS_TIMING_INTEG_0_034,
    INTEGRATIONTIME_101MS           = TSL2561_REG_ALS_TIMING_INTEG_0_252,
    INTEGRATIONTIME_402MS           = TSL2561_REG_ALS_TIMING_INTEG_1
    
} tslIntegTime_t;

typedef enum
{
    INTR_CTL_DISABLE                = TSL2561_REG_ALS_INTERRUPT_INTR_DISABLE,
    INTR_CTL_LEVEL                  = TSL2561_REG_ALS_INTERRUPT_INTR_LEVEL,
    INTR_CTL_SMB                    = TSL2561_REG_ALS_INTERRUPT_INTR_SMB,
    INTR_CTL_SETINTR                = TSL2561_REG_ALS_INTERRUPT_INTR_SETINTR
    
} tslInterruptControl_t;

typedef enum
{
    INTR_PERSIST_EVERY              = TSL2561_REG_ALS_INTERRUPT_PERSIST_EVERY,
    INTR_PERSIST_ANY                = TSL2561_REG_ALS_INTERRUPT_PERSIST_ANY,
    INTR_PERSIST_2                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_2,
    INTR_PERSIST_3                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_3,
    INTR_PERSIST_4                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_4,
    INTR_PERSIST_5                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_5,
    INTR_PERSIST_6                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_6,
    INTR_PERSIST_7                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_7,
    INTR_PERSIST_8                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_8,
    INTR_PERSIST_9                  = TSL2561_REG_ALS_INTERRUPT_PERSIST_9,
    INTR_PERSIST_10                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_10,
    INTR_PERSIST_11                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_11,
    INTR_PERSIST_12                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_12,
    INTR_PERSIST_13                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_13,
    INTR_PERSIST_14                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_14,
    INTR_PERSIST_15                 = TSL2561_REG_ALS_INTERRUPT_PERSIST_15
    
} tslInterruptPersistence_t;


typedef struct
{
    uint16_t V;
    uint16_t IR;
    uint16_t F;
    
} tslSensorData_t;


class TSL2561
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
        void getAddr_TSL2561(uint8_t i2cAddress);
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
