/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29125
        This code is designed to work with the ISL29125_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/isl29125-digital-rgb-color-light-sensor-with-ir-blocking-filter
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
    #define ISL29125_DEFAULT_ADDRESS                (0x44)          // 1000100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ISL29125_CONVERSIONDELAY                (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define ISL29125_REG_DEV_ID                     (0x00)
    #define ISL29125_REG_CONFIG_1                   (0x01)
    #define ISL29125_REG_CONFIG_2                   (0x02)
    #define ISL29125_REG_CONFIG_3                   (0x03)
    #define ISL29125_REG_THRESHOLD_LL               (0x04)
    #define ISL29125_REG_THRESHOLD_LH               (0x05)
    #define ISL29125_REG_THRESHOLD_HL               (0x06)
    #define ISL29125_REG_THRESHOLD_HH               (0x07)
    #define ISL29125_REG_STATUS                     (0x08)
    #define ISL29125_REG_GREEN_LOW                  (0x09)
    #define ISL29125_REG_GREEN_HIGH                 (0x0A)
    #define ISL29125_REG_RED_LOW                    (0x0B)
    #define ISL29125_REG_RED_HIGH                   (0x0C)
    #define ISL29125_REG_BLUE_LOW                   (0x0D)
    #define ISL29125_REG_BLUE_HIGH                  (0x0E)

/**************************************************************************
    CONFIGURATION REGISTER 1
 **************************************************************************/
    #define ISL29125_REG_CONFIG1_SYNC_MASK          (0x20)    // SYNC Mode
    #define ISL29125_REG_CONFIG1_SYNC_NORMAL        (0x00)    // INT pin Sets to Normal Mode
    #define ISL29125_REG_CONFIG1_SYNC_TO_INT        (0x20)    // INT pin Sets to Interrupt Mode

    #define ISL29125_REG_CONFIG1_BITS_MASK          (0x10)    // ADC Resolution
    #define ISL29125_REG_CONFIG1_BITS_16            (0x00)    // 16 Bits
    #define ISL29125_REG_CONFIG1_BITS_12            (0x10)    // 12 Bits

    #define ISL29125_REG_CONFIG1_RNG_MASK           (0x08)    // RGB Data Sensing Range
    #define ISL29125_REG_CONFIG1_RNG_375LUX         (0x00)    // 375 lux
    #define ISL29125_REG_CONFIG1_RNG_10KLUX         (0x08)    // 10,000 lux

    #define ISL29125_REG_CONFIG1_MODE_MASK          (0x07)    // RGB Data Sensing Range
    #define ISL29125_REG_CONFIG1_MODE_POWERDOWN     (0x00)    // Power Down (ADC conversion)
    #define ISL29125_REG_CONFIG1_MODE_G             (0x01)    // GREEN Only
    #define ISL29125_REG_CONFIG1_MODE_R             (0x02)    // RED Only
    #define ISL29125_REG_CONFIG1_MODE_B             (0x03)    // BLUE Only
    #define ISL29125_REG_CONFIG1_MODE_STANDBY       (0x04)    // Stand by (No ADC conversion)
    #define ISL29125_REG_CONFIG1_MODE_RGB           (0x05)    // GREEN/RED/BLUE
    #define ISL29125_REG_CONFIG1_MODE_RG            (0x06)    // GREEN/RED
    #define ISL29125_REG_CONFIG1_MODE_GB            (0x07)    // GREEN/BLUE

/**************************************************************************
    CONFIGURATION REGISTER 2
**************************************************************************/
    #define ISL29125_REG_CONFIG2_IR_OFFSET_MASK     (0x80)    // Active Infrared (IR) Compensation Offset
    #define ISL29125_REG_CONFIG2_IR_OFFSET_OFF      (0x00)    // INT pin Sets to Normal Mode
    #define ISL29125_REG_CONFIG2_IR_OFFSET_ON       (0x80)    // INT pin Sets to Interrupt Mode

    #define ISL29125_REG_CONFIG2_IR_ADJUST_MASK     (0x3F)    // Active Infrared (IR) Compensation Adjust
    #define ISL29125_REG_CONFIG2_IR_ADJUST_LOW      (0x00)    // Low Amount of IR Filtering
    #define ISL29125_REG_CONFIG2_IR_ADJUST_MID      (0x20)    // Medium Amount of IR Filtering
    #define ISL29125_REG_CONFIG2_IR_ADJUST_HIGH     (0x3F)    // High Amount of IR Filtering

/**************************************************************************
    CONFIGURATION REGISTER 3
**************************************************************************/
    #define ISL29125_REG_CONFIG3_CONVEN_MASK        (0x10)    // RGB Conversion Done to INT Control
    #define ISL29125_REG_CONFIG3_CONVEN_DISABLE     (0x00)    // Disable
    #define ISL29125_REG_CONFIG3_CONVEN_ENABLE      (0x10)    // Enable

    #define ISL29125_REG_CONFIG3_PRST_MASK          (0x0C)    // Interrupt Persist Control
    #define ISL29125_REG_CONFIG3_PRST_1             (0x00)    // No. of Integration Cycle: 1
    #define ISL29125_REG_CONFIG3_PRST_2             (0x04)    // No. of Integration Cycles: 2
    #define ISL29125_REG_CONFIG3_PRST_4             (0x08)    // No. of Integration Cycles: 4
    #define ISL29125_REG_CONFIG3_PRST_8             (0x0C)    // No. of Integration Cycles: 8

    #define ISL29125_REG_CONFIG3_INTSEL_MASK        (0x03)    // Interrupt Threshold Assignment
    #define ISL29125_REG_CONFIG3_INTSEL_NO_INT      (0x00)    // No Interrupt
    #define ISL29125_REG_CONFIG3_INTSEL_G_INT       (0x01)    // GREEN Interrupt
    #define ISL29125_REG_CONFIG3_INTSEL_R_INT       (0x02)    // RED Interrupt
    #define ISL29125_REG_CONFIG3_INTSEL_B_INT       (0x03)    // BLUE Interrupt

/**************************************************************************
    STATUS FLAG REGISTER
**************************************************************************/
    #define ISL29125_REG_STAT_RGBCF_MASK            (0x30)    // Conversion Flag
    #define ISL29125_REG_STAT_RGBCF_NO_OP           (0x00)    // No Conversion
    #define ISL29125_REG_STAT_RGBCF_GREEN           (0x10)    // GREEN
    #define ISL29125_REG_STAT_RGBCF_RED             (0x20)    // RED
    #define ISL29125_REG_STAT_RGBCF_BLUE            (0x30)    // BLUE

    #define ISL29125_REG_STAT_BOUTF_MASK            (0x04)    // Brownout Flag
    #define ISL29125_REG_STAT_BOUTF_NO_BROWNOUT     (0x00)    // No Brownout
    #define ISL29125_REG_STAT_BOUTF_BROWNOUT        (0x04)    // Power Down or Brownout Occurred

    #define ISL29125_REG_STAT_CONVENF_MASK          (0x02)    // Conversion Flag
    #define ISL29125_REG_STAT_CONVENF_CONV_STILL    (0x00)    // Still Convert or Cleared
    #define ISL29125_REG_STAT_CONVENF_CONV_DONE     (0x02)    // Conversion Completed

    #define ISL29125_REG_STAT_RGBTHF_MASK           (0x01)    // Interrupt Flag
    #define ISL29125_REG_STAT_RGBTHF_INT_NO         (0x00)    // Interrupt is Cleared or not Triggered yet
    #define ISL29125_REG_STAT_RGBTHF_INT            (0x01)    // Interrupt is Triggered


typedef enum
{
    SYNC_NORMAL                 = ISL29125_REG_CONFIG1_SYNC_NORMAL,
    SYNC_TO_INT                 = ISL29125_REG_CONFIG1_SYNC_TO_INT
    
} islSync_t;

typedef enum
{
    RESOLUTION_16               = ISL29125_REG_CONFIG1_BITS_16,
    RESOLUTION_12               = ISL29125_REG_CONFIG1_BITS_12
    
} islResolution_t;

typedef enum
{
    RANGE_375LUX                = ISL29125_REG_CONFIG1_RNG_375LUX,
    RANGE_10KLUX                = ISL29125_REG_CONFIG1_RNG_10KLUX
    
} islRange_t;

typedef enum
{
    MODE_POWERDOWN              = ISL29125_REG_CONFIG1_MODE_POWERDOWN,
    MODE_GREEN                  = ISL29125_REG_CONFIG1_MODE_G,
    MODE_RED                    = ISL29125_REG_CONFIG1_MODE_R,
    MODE_BLUE                   = ISL29125_REG_CONFIG1_MODE_B,
    MODE_STANDBY                = ISL29125_REG_CONFIG1_MODE_STANDBY,
    MODE_RGB                    = ISL29125_REG_CONFIG1_MODE_RGB,
    MODE_RG                     = ISL29125_REG_CONFIG1_MODE_RG,
    MODE_GB                     = ISL29125_REG_CONFIG1_MODE_GB
    
} islMode_t;

typedef enum
{
    IR_OFFSET_OFF               = ISL29125_REG_CONFIG2_IR_OFFSET_OFF,
    IR_OFFSET_ON                = ISL29125_REG_CONFIG2_IR_OFFSET_ON
    
} islIROffset_t;

typedef enum
{
    IR_ADJUST_LOW               = ISL29125_REG_CONFIG2_IR_ADJUST_LOW,
    IR_ADJUST_MID               = ISL29125_REG_CONFIG2_IR_ADJUST_MID,
    IR_ADJUST_HIGH              = ISL29125_REG_CONFIG2_IR_ADJUST_HIGH
    
} islIRAdjust_t;


typedef enum
{
    CONVERSION_DISABLE          = ISL29125_REG_CONFIG3_CONVEN_DISABLE,
    CONVERSION_ENABLE           = ISL29125_REG_CONFIG3_CONVEN_ENABLE
    
} islConversion_t;

typedef enum
{
    INTEGRATION_CYCLE_1         = ISL29125_REG_CONFIG3_PRST_1,
    INTEGRATION_CYCLE_2         = ISL29125_REG_CONFIG3_PRST_2,
    INTEGRATION_CYCLE_4         = ISL29125_REG_CONFIG3_PRST_4,
    INTEGRATION_CYCLE_8         = ISL29125_REG_CONFIG3_PRST_8
    
} islIntegrationCycle_t;

typedef enum
{
    NO_INTERRUPT                = ISL29125_REG_CONFIG3_INTSEL_NO_INT,
    GREEN_INTERRUPT             = ISL29125_REG_CONFIG3_INTSEL_G_INT,
    RED_INTERRUPT               = ISL29125_REG_CONFIG3_INTSEL_R_INT,
    BLUE_INTERRUPT              = ISL29125_REG_CONFIG3_INTSEL_B_INT
    
} islInterrupt_t;

typedef struct
{
    uint16_t C;
    uint16_t R;
    uint16_t G;
    uint16_t B;
    
} islSensorData_t;


class ISL29125
{
    protected:
        // Instance-specific properties
        uint8_t isl_conversionDelay;
        islSync_t isl_sync;
        islResolution_t isl_resolution;
        islRange_t isl_range;
        islMode_t isl_mode;
        islIROffset_t isl_iroffset;
        islIRAdjust_t isl_iradjust;
        islConversion_t isl_conversion;
        islIntegrationCycle_t isl_integrationcycle;
        islInterrupt_t isl_interrupt;

    public:
        uint8_t isl_i2cAddress;
        islSensorData_t isl_colorData;
        void getAddr_ISL29125(uint8_t i2cAddress);
        void begin(void);
        void setUpColor(void);
        void Measure_Sensor(void);
        void Measure_Color(void);
        void setSync(islSync_t sync);
        islSync_t getSync(void);
        void setResolution(islResolution_t resolution);
        islResolution_t getResolution(void);
        void setRange(islRange_t range);
        islRange_t getRange(void);
        void setMode(islMode_t mode);
        islMode_t getMode(void);
        void setIROffset(islIROffset_t iroffset);
        islIROffset_t getIROffset(void);
        void setIRAdjust(islIRAdjust_t iradjust);
        islIRAdjust_t getIRAdjust(void);
        void setConversion(islConversion_t conversion);
        islConversion_t getConversion(void);
        void setIntegrationCycle(islIntegrationCycle_t integrationcycle);
        islIntegrationCycle_t getIntegrationCycle(void);
        void setInterrupt(islInterrupt_t interrupt);
        islInterrupt_t getInterrupt(void);

    private:
};
