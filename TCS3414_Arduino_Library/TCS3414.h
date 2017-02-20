/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS3414
        This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2
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
    #define TCS3414_DEFAULT_ADDRESS                             (0x39)
    #define TCS3414_COLOR_DEV_ID                                (0x30)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TCS3414_CONVERSIONDELAY                             (100)

/**************************************************************************
    DIGITAL COLOR SENSOR REGISTERS
**************************************************************************/
    #define TCS3414_REG_COLOR_COMMAND                                       // Specifies Register Address
    #define TCS3414_REG_COLOR_CONTROL                           (0x00)      // Control of Basic Functions Register
    #define TCS3414_REG_COLOR_TIMING                            (0x01)      // Integration Time / Gain Control Register
    #define TCS3414_REG_COLOR_INTERRUPT                         (0x02)      // Interrupt Control Register
    #define TCS3414_REG_COLOR_INT_SOURCE                        (0x03)      // Interrupt Source Register
    #define TCS3414_REG_COLOR_ID                                (0x04)      // Part Number / Rev ID Register
    #define TCS3414_REG_COLOR_GAIN                              (0x07)      // ADC Gain Control Register
    #define TCS3414_REG_COLOR_AILTL                             (0x08)      // Low Interrupt Threshold Low Byte Register
    #define TCS3414_REG_COLOR_AILTH                             (0x09)      // Low Interrupt Threshold High Byte Register
    #define TCS3414_REG_COLOR_AIHTL                             (0x0A)      // High Interrupt Threshold Low Byte Register
    #define TCS3414_REG_COLOR_AIHTH                             (0x0B)      // High Interrupt Threshold High Byte Register
    #define TCS3414_REG_COLOR_GDATAL                            (0x10)      // Green Channel ADC Low Data Register
    #define TCS3414_REG_COLOR_GDATAH                            (0x11)      // Green Channel ADC High Data Register
    #define TCS3414_REG_COLOR_RDATAL                            (0x12)      // Red Channel ADC Low Data Register
    #define TCS3414_REG_COLOR_RDATAH                            (0x13)      // Red Channel ADC High Data Register
    #define TCS3414_REG_COLOR_BDATAL                            (0x14)      // Blue Channel ADC Low Data Register
    #define TCS3414_REG_COLOR_BDATAH                            (0x15)      // Blue Channel ADC High Data Register
    #define TCS3414_REG_COLOR_CDATAL                            (0x16)      // Clear Channel ADC Low Data Register
    #define TCS3414_REG_COLOR_CDATAH                            (0x17)      // Clear Channel ADC High Data Register

/**************************************************************************
    DIGITAL COLOR SENSOR COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_CMD_MASK                          (0x80)      // Select Command Register
    #define TCS3414_REG_COLOR_CMD_WRITE                         (0x80)      // Addresses COMMAND Register
    #define TCS3414_REG_COLOR_CMD_TRANSACTION_MASK              (0x60)      // Selects Type of Transaction to Follow in Subsequent Data Transfers
    #define TCS3414_REG_COLOR_CMD_TRANSACTION_BYTE              (0x00)      // SMB Read/Write Byte Protocol
    #define TCS3414_REG_COLOR_CMD_TRANSACTION_WORD              (0x20)      // SMB Read/Write Word Protocol
    #define TCS3414_REG_COLOR_CMD_TRANSACTION_BLOCK             (0x40)      // SMB Read/Write Block Protocol
    #define TCS3414_REG_COLOR_CMD_TRANSACTION_INTR              (0x60)      // Clear Any Pending Interrupt and is a Writeonce-to-Clear Field
/**************************************************************************
    DIGITAL COLOR SENSOR CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_CONTROL_ADC_EN_MASK               (0x02)      // Four-Channel RGBC ADC Enable to Begin Integration
    #define TCS3414_REG_COLOR_CONTROL_ADC_EN_DISABLE            (0x00)      // Disables the RGBC ADC
    #define TCS3414_REG_COLOR_CONTROL_ADC_EN_ENABLE             (0x02)      // Activates the RGBC ADC

    #define TCS3414_REG_COLOR_CONTROL_POWER_MASK                (0x01)      // Power ON
    #define TCS3414_REG_COLOR_CONTROL_POWER_DISABLE             (0x00)      // Powers Off the Device
    #define TCS3414_REG_COLOR_CONTROL_POWER_ENABLE              (0x01)      // Powers On the Device

/**************************************************************************
    DIGITAL COLOR SENSOR TIMING REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_TIMING_SYNC_EDGE_MASK             (0x40)      // Sync Pin Edge
    #define TCS3414_REG_COLOR_TIMING_SYNC_EDGE_LOW              (0x00)      // If SYNC_EDGE is Low, the Falling Edge of the Sync Pin is Used to Stop an Integration Cycle when INTEG_MODE is 11
    #define TCS3414_REG_COLOR_TIMING_SYNC_EDGE_HIGH             (0x40)      // If SYNC_EDGE is High, the Rising Edge of the Sync Pin is Used to Stop an Integration Cycle when INTEG_MODE is 11

    #define TCS3414_REG_COLOR_TIMING_INTEG_MODE_MASK            (0x30)      // Selects Preset Integration Time, Manual Integration (Via Serial Bus), or External Synchronization (SYNC IN) Modes
    #define TCS3414_REG_COLOR_TIMING_INTEG_MODE_FREE            (0x00)      // In this Mode, the Integrator is Free-Running and One of the Three Internally-Generated Nominal Integration Times is Selected for Each Conversion
    #define TCS3414_REG_COLOR_TIMING_INTEG_MODE_MANUAL          (0x10)      // Manually Start/Stop Integration through Serial Bus using ADC_EN Field in Control Register
    #define TCS3414_REG_COLOR_TIMING_INTEG_MODE_SYN_SINGLE      (0x20)      // Synchronize Exactly One Internally-Timed Integration Cycle as Specified in the NOMINAL INTEGRATION TIME Beginning 2.4 μs After Being Initiated by the SYNC IN Pin
    #define TCS3414_REG_COLOR_TIMING_INTEG_MODE_SYN_MULTI       (0x30)      // Integrate over Specified Number of Pulses on SYNC IN pin. Minimum width of sync pulse is 50 μs. SYNC IN must be low at least 3.6 μs

    #define TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_MASK         (0x02)      // NOMINAL INTEGRATION TIME
    #define TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_12MS         (0x00)      // 12 ms
    #define TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_100MS        (0x01)      // 100 ms
    #define TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_400MS        (0x02)      // 400 ms

    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_MASK     (0x0F)      // SYNC IN PULSE COUNT
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_1        (0x00)      // 1
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_2        (0x01)      // 2
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_4        (0x02)      // 4
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_8        (0x03)      // 8
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_16       (0x04)      // 16
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_32       (0x05)      // 32
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_64       (0x06)      // 64
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_128      (0x07)      // 128
    #define TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_256      (0x08)      // 256

/**************************************************************************
    DIGITAL COLOR SENSOR INTERRUPT CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_MASK          (0x40)      // Stop ADC Integration on Interrupt
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_LOW           (0x00)      // When High, ADC Integration will Stop Once an Interrupt is Asserted
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_HIGH          (0x40)      // To Resume Operation (1) De-Assert ADC_EN using CONTROL Register, (2) Clear Interrupt using COMMAND Register, and (3) Re-Assert ADC_EN using CONTROL Register

    #define TCS3414_REG_COLOR_INTERRUPT_INTR_MASK               (0x30)      // INTR Control Select. This Field Determines Mode of Interrupt Logic
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_DISABLE            (0x00)      // Interrupt Output Disabled
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_LEVEL              (0x10)      // Level Interrupt
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_SMB                (0x20)      // SMB-Alert Compliant
    #define TCS3414_REG_COLOR_INTERRUPT_INTR_SETINTR            (0x30)      // Sets an Interrupt and Functions as Mode 10

    #define TCS3414_REG_COLOR_INTERRUPT_PERSIST_MASK            (0x07)      // Interrupt Persistence. Controls Rate of Interrupts to the Host Processor
    #define TCS3414_REG_COLOR_INTERRUPT_PERSIST_EVERY           (0x00)      // Every ADC Cycle Generates Interrupt
    #define TCS3414_REG_COLOR_INTERRUPT_PERSIST_SINGLE          (0x01)      // Any Value Outside of Threshold Range
    #define TCS3414_REG_COLOR_INTERRUPT_PERSIST_0_1SEC          (0x02)      // Consecutively Out of Range for 0.1 Second
    #define TCS3414_REG_COLOR_INTERRUPT_PERSIST_1SEC            (0x03)      // Consecutively Out of Range for 1 Second

/**************************************************************************
    DIGITAL COLOR SENSOR INTERRUPT SOURCE REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_INT_SOURCE_MASK                   (0x03)      // Interrupt Source. Selects which ADC Channel to Use to Generate an Interrupt
    #define TCS3414_REG_COLOR_INT_SOURCE_GREEN                  (0x00)      // Green Channel
    #define TCS3414_REG_COLOR_INT_SOURCE_RED                    (0x01)      // Red Channel
    #define TCS3414_REG_COLOR_INT_SOURCE_BLUE                   (0x02)      // Blue Channel
    #define TCS3414_REG_COLOR_INT_SOURCE_CLEAR                  (0x03)      // Clear Channel

/**************************************************************************
    DIGITAL COLOR SENSOR GAIN REGISTER DESCRIPTION
**************************************************************************/
    #define TCS3414_REG_COLOR_GAIN_MASK                         (0x30)      // Analog Gain Control. This Field Switches the Common Analog Gain of the Four ADC Channels
    #define TCS3414_REG_COLOR_GAIN_1X                           (0x00)      // 1X
    #define TCS3414_REG_COLOR_GAIN_4X                           (0x10)      // 4X
    #define TCS3414_REG_COLOR_GAIN_16X                          (0x20)      // 16X
    #define TCS3414_REG_COLOR_GAIN_64X                          (0x30)      // 64X

    #define TCS3414_REG_COLOR_PRESCALER_MASK                    (0x07)      // This Field Controls a 6-bit Digital Prescaler and Divider. The Prescaler Reduces the Sensitivity of each ADC Integrator
    #define TCS3414_REG_COLOR_PRESCALER_1                       (0x00)      // Divide by 1
    #define TCS3414_REG_COLOR_PRESCALER_2                       (0x01)      // Divide by 2
    #define TCS3414_REG_COLOR_PRESCALER_4                       (0x02)      // Divide by 4
    #define TCS3414_REG_COLOR_PRESCALER_8                       (0x03)      // Divide by 8
    #define TCS3414_REG_COLOR_PRESCALER_16                      (0x04)      // Divide by 16
    #define TCS3414_REG_COLOR_PRESCALER_32                      (0x05)      // Divide by 32
    #define TCS3414_REG_COLOR_PRESCALER_64                      (0x06)      // Divide by 64


typedef enum
{
    COMMAND_WRITE                       = TCS3414_REG_COLOR_CMD_WRITE,
    COMMAND_TRANC_BYTE                  = TCS3414_REG_COLOR_CMD_TRANSACTION_BYTE,
    COMMAND_TRANC_WORD                  = TCS3414_REG_COLOR_CMD_TRANSACTION_WORD,
    COMMAND_TRANC_BLOCK                 = TCS3414_REG_COLOR_CMD_TRANSACTION_BLOCK,
    COMMAND_TRANC_INTR                  = TCS3414_REG_COLOR_CMD_TRANSACTION_INTR
    
} tcsCommand_t;

typedef enum
{
    ADC_DISABLE                         = TCS3414_REG_COLOR_CONTROL_ADC_EN_DISABLE,
    ADC_ENABLE                          = TCS3414_REG_COLOR_CONTROL_ADC_EN_ENABLE
    
} tcsColorEnable_t;

typedef enum
{
    POWER_DISABLE                       = TCS3414_REG_COLOR_CONTROL_POWER_DISABLE,
    POWER_ENABLE                        = TCS3414_REG_COLOR_CONTROL_POWER_ENABLE
    
} tcsPowerEnable_t;

typedef enum
{
    SYNC_EDGE_LOW                       = TCS3414_REG_COLOR_TIMING_SYNC_EDGE_LOW,
    SYNC_EDGE_HIGH                      = TCS3414_REG_COLOR_TIMING_SYNC_EDGE_HIGH
    
} tcsSyncEdge_t;

typedef enum
{
    INTEG_MODE_FREE                     = TCS3414_REG_COLOR_TIMING_INTEG_MODE_FREE,
    INTEG_MODE_MANUAL                   = TCS3414_REG_COLOR_TIMING_INTEG_MODE_MANUAL,
    INTEG_MODE_SYN_SINGLE               = TCS3414_REG_COLOR_TIMING_INTEG_MODE_SYN_SINGLE,
    INTEG_MODE_SYN_MULTI                = TCS3414_REG_COLOR_TIMING_INTEG_MODE_SYN_MULTI
    
} tcsIntegMode_t;

typedef enum
{
    INTEG_PARAM_INTTIME_12MS            = TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_12MS,
    INTEG_PARAM_INTTIME_100MS           = TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_100MS,
    INTEG_PARAM_INTTIME_400MS           = TCS3414_REG_COLOR_TIMING_PARAM_INTTIME_400MS
    
} tcsIntegParamTime_t;

typedef enum
{
    INTEG_PARAM_PULSE_COUNT1            = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_1,
    INTEG_PARAM_PULSE_COUNT2            = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_2,
    INTEG_PARAM_PULSE_COUNT4            = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_4,
    INTEG_PARAM_PULSE_COUNT8            = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_8,
    INTEG_PARAM_PULSE_COUNT16           = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_16,
    INTEG_PARAM_PULSE_COUNT32           = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_32 ,
    INTEG_PARAM_PULSE_COUNT64           = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_64,
    INTEG_PARAM_PULSE_COUNT128          = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_128,
    INTEG_PARAM_PULSE_COUNT256          = TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_256
    
} tcsIntegParamPulse_t;

typedef enum
{
    INTR_STOP_LOW                       = TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_LOW,
    INTR_STOP_HIGH                      = TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_HIGH
    
} tcsInterruptStop_t;

typedef enum
{
    INTR_CTL_DISABLE                    = TCS3414_REG_COLOR_INTERRUPT_INTR_DISABLE,
    INTR_CTL_LEVEL                      = TCS3414_REG_COLOR_INTERRUPT_INTR_LEVEL,
    INTR_CTL_SMB                        = TCS3414_REG_COLOR_INTERRUPT_INTR_SMB,
    INTR_CTL_SETINTR                    = TCS3414_REG_COLOR_INTERRUPT_INTR_SETINTR
    
} tcsInterruptControl_t;

typedef enum
{
    INTR_PERSIST_EVERY                  = TCS3414_REG_COLOR_INTERRUPT_PERSIST_EVERY,
    INTR_PERSIST_SINGLE                 = TCS3414_REG_COLOR_INTERRUPT_PERSIST_SINGLE,
    INTR_PERSIST_0_1SEC                 = TCS3414_REG_COLOR_INTERRUPT_PERSIST_0_1SEC,
    INTR_PERSIST_1SEC                   = TCS3414_REG_COLOR_INTERRUPT_PERSIST_1SEC
        
} tcsInterruptPersistence_t;
    
typedef enum
{
    INT_SOURCE_GREEN                    = TCS3414_REG_COLOR_INT_SOURCE_GREEN,
    INT_SOURCE_RED                      = TCS3414_REG_COLOR_INT_SOURCE_RED,
    INT_SOURCE_BLUE                     = TCS3414_REG_COLOR_INT_SOURCE_BLUE,
    INT_SOURCE_CLEAR                    = TCS3414_REG_COLOR_INT_SOURCE_CLEAR
        
} tcsInterruptSource_t;

typedef enum
{
    GAIN_1X                             = TCS3414_REG_COLOR_GAIN_1X,
    GAIN_4X                             = TCS3414_REG_COLOR_GAIN_4X,
    GAIN_16X                            = TCS3414_REG_COLOR_GAIN_16X,
    GAIN_64X                            = TCS3414_REG_COLOR_GAIN_64X
    
} tcsAnalogGain_t;

typedef enum
{
    PRESCALER_1                         = TCS3414_REG_COLOR_PRESCALER_1,
    PRESCALER_2                         = TCS3414_REG_COLOR_PRESCALER_2,
    PRESCALER_4                         = TCS3414_REG_COLOR_PRESCALER_4,
    PRESCALER_8                         = TCS3414_REG_COLOR_PRESCALER_8,
    PRESCALER_16                        = TCS3414_REG_COLOR_PRESCALER_16,
    PRESCALER_32                        = TCS3414_REG_COLOR_PRESCALER_32 ,
    PRESCALER_64                        = TCS3414_REG_COLOR_PRESCALER_64
        
} tcsPrescaler_t;
    
    
typedef struct
{
    uint16_t R;
    uint16_t G;
    uint16_t B;
    uint16_t C;
    
} tcsSensorData_t;


class TCS3414
{
    protected:
        // Instance-specific properties
        uint8_t tcs_conversionDelay;
        tcsCommand_t tcs_command;
        tcsColorEnable_t tcs_colorenable;
        tcsPowerEnable_t tcs_powerenable;
        tcsSyncEdge_t tcs_syncedge;
        tcsIntegMode_t tcs_integmode;
        tcsIntegParamTime_t tcs_integparamtime;
        tcsIntegParamPulse_t tcs_integparampulse;
        tcsInterruptStop_t tcs_interruptstop;
        tcsInterruptControl_t tcs_interruptcontrol;
        tcsInterruptPersistence_t tcs_interruptpersistence;
        tcsInterruptSource_t tcs_interruptsource;
        tcsAnalogGain_t tcs_analoggain;
        tcsPrescaler_t tcs_prescaler;

    public:
        uint8_t tcs_i2cAddress;
        tcsSensorData_t tcs_colorData;
        void getAddr_TCS3414(uint8_t i2cAddress);
        bool begin(void);
        void setUpColor();
        void Measure_Color(void);
        void setCommand(tcsCommand_t command);
        tcsCommand_t getCommand(void);
        void setColorEnable(tcsColorEnable_t colorenable);
        tcsColorEnable_t getColorEnable(void);
        void setPowerEnable(tcsPowerEnable_t powerenable);
        tcsPowerEnable_t getPowerEnable(void);
        void setSyncEdge(tcsSyncEdge_t syncedge);
        tcsSyncEdge_t getSyncEdge(void);
        void setIntegMode(tcsIntegMode_t integmode);
        tcsIntegMode_t getIntegMode(void);
        void setIntegParamTime(tcsIntegParamTime_t integparamtime);
        tcsIntegParamTime_t getIntegParamTime(void);
        void setIntegParamPulse(tcsIntegParamPulse_t integparampulse);
        tcsIntegParamPulse_t getIntegParamPulse(void);
        void setInterruptStop(tcsInterruptStop_t interruptstop);
        tcsInterruptStop_t getInterruptStop(void);
        void setInterruptControl(tcsInterruptControl_t interruptcontrol);
        tcsInterruptControl_t getInterruptControl(void);
        void setInterruptPersistence(tcsInterruptPersistence_t interruptpersistence);
        tcsInterruptPersistence_t getInterruptPersistence(void);
        void setInterruptSource(tcsInterruptSource_t interruptsource);
        tcsInterruptSource_t getInterruptSource(void);
        void setAnalogGain(tcsAnalogGain_t analoggain);
        tcsAnalogGain_t getAnalogGain(void);
        void setPrescaler(tcsPrescaler_t prescaler);
        tcsPrescaler_t getPrescaler(void);
    
    private:
};
