/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29003
        This code is designed to work with the ISL29003_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=ISL29003_I2CS#tabs-0-product_tabset-2
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
    #define ISL29003_DEFAULT_ADDRESS                (0x44)          // 1000100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ISL29003_CONVERSIONDELAY                (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define ISL29003_REG_MASK                       (0xFF)
    #define ISL29003_REG_COMMAND                    (0x00)
    #define ISL29003_REG_CONTROL                    (0x01)
    #define ISL29003_REG_INTR_TH_HI                 (0x02)
    #define ISL29003_REG_INTR_TH_LO                 (0x03)
    #define ISL29003_REG_SENSOR_LSB                 (0x04)
    #define ISL29003_REG_SENSOR_MSB                 (0x05)
    #define ISL29003_REG_TIMER_LSB                  (0x06)
    #define ISL29003_REG_TIMER_MSB                  (0x07)

/**************************************************************************
    COMMAND REGISTER
 **************************************************************************/
    #define ISL29003_REG_CAPABL_ADC_MASK            (0x80)    // ADC-Core Output
    #define ISL29003_REG_CAPABL_ADC_DISABLE         (0x00)    // Disable ADC-Core
    #define ISL29003_REG_CAPABL_ADC_ENABLE          (0x80)    // Enable ADC-Core

    #define ISL29003_REG_CAPABL_ADCPD_MASK          (0x40)    // ADC Power Down Mode
    #define ISL29003_REG_CAPABL_ADCPD_NORMAL        (0x00)    // Normal Operation
    #define ISL29003_REG_CAPABL_ADCPD_PDOWN         (0x40)    // Power-Down Mode

    #define ISL29003_REG_CAPABL_TMODE_MASK          (0x20)    // Timing Mode
    #define ISL29003_REG_CAPABL_TMODE_INT           (0x00)    // Integration is Internally Timed
    #define ISL29003_REG_CAPABL_TMODE_EXT           (0x20)    // Integration is externally sync/controlled by I2C host

    #define ISL29003_REG_CAPABL_ADCMODE_MASK        (0x0C)    // ADC Work Mode
    #define ISL29003_REG_CAPABL_ADCMODE_DIODE1      (0x00)    // Diode1’s current to unsigned 16-bit data
    #define ISL29003_REG_CAPABL_ADCMODE_DIODE2      (0x04)    // Diode2’s current to unsigned 16-bit data
    #define ISL29003_REG_CAPABL_ADCMODE_DIFF        (0x08)    // Difference between diodes (I1 - I2) to signed 15-bit data
    #define ISL29003_REG_CAPABL_ADCMODE_RES         (0x0C)    // Reserved

/**************************************************************************
    CONTROL REGISTER
**************************************************************************/
    #define ISL29003_REG_CONTROL_INTRFLAG_MASK      (0x20)    // Interrupt Flag Status
    #define ISL29003_REG_CONTROL_INTRFLAG_NOTRIG    (0x00)    // Interrupt is cleared or not yet Triggered
    #define ISL29003_REG_CONTROL_INTRFLAG_TRIG      (0x20)    // Interrupt is Triggered

    #define ISL29003_REG_CONTROL_GAIN_MASK          (0x0C)    // ADC Gain Range
    #define ISL29003_REG_CONTROL_GAIN_0_1K          (0x00)    // 0 to 1000 lux
    #define ISL29003_REG_CONTROL_GAIN_0_4K          (0x04)    // 0 to 4000 lux
    #define ISL29003_REG_CONTROL_GAIN_0_16K         (0x08)    // 0 to 16000 lux
    #define ISL29003_REG_CONTROL_GAIN_0_64K         (0x0C)    // 0 to 64000 lux

    #define ISL29003_REG_CONTROL_INTRCYCLE_MASK     (0x03)    // Interrupt Persist Time
    #define ISL29003_REG_CONTROL_INTRCYCLE_1        (0x00)    // 1 Integration Cycle
    #define ISL29003_REG_CONTROL_INTRCYCLE_4        (0x01)    // 4 Integration Cycles
    #define ISL29003_REG_CONTROL_INTRCYCLE_8        (0x02)    // 8 Integration Cycles
    #define ISL29003_REG_CONTROL_INTRCYCLE_16       (0x03)    // 16 Integration Cycles


typedef enum
{
    ADC_DISABLE             = ISL29003_REG_CAPABL_ADC_DISABLE,
    ADC_ENABLE              = ISL29003_REG_CAPABL_ADC_ENABLE
} islADCCore_t;

typedef enum
{
    ADCPD_NORMAL            = ISL29003_REG_CAPABL_ADCPD_NORMAL,
    ADCPD_PDOWN             = ISL29003_REG_CAPABL_ADCPD_PDOWN
} islADCPD_t;

typedef enum
{
    TIME_INTERNAL           = ISL29003_REG_CAPABL_TMODE_INT,
    TIME_EXTERNAL           = ISL29003_REG_CAPABL_TMODE_EXT
} islTMode_t;

typedef enum
{
    ADCMODE_DIODE1          = ISL29003_REG_CAPABL_ADCMODE_DIODE1,
    ADCMODE_DIODE2          = ISL29003_REG_CAPABL_ADCMODE_DIODE2,
    ADCMODE_DIFF            = ISL29003_REG_CAPABL_ADCMODE_DIFF,
    ADCMODE_RES             = ISL29003_REG_CAPABL_ADCMODE_RES
    
} islADCMode_t;

typedef enum
{
    FLAG_NOTRIGGRED         = ISL29003_REG_CONTROL_INTRFLAG_NOTRIG,
    FLAG_TRIGGRED           = ISL29003_REG_CONTROL_INTRFLAG_TRIG
} islIntrFlag_t;

typedef enum
{
    GAIN_1K                 = ISL29003_REG_CONTROL_GAIN_0_1K,
    GAIN_4K                 = ISL29003_REG_CONTROL_GAIN_0_4K,
    GAIN_16K                = ISL29003_REG_CONTROL_GAIN_0_16K,
    GAIN_64K                = ISL29003_REG_CONTROL_GAIN_0_64K
} islADCGain_t;


typedef enum
{
    INTRCYCLE_1             = ISL29003_REG_CONTROL_INTRCYCLE_1,
    INTRCYCLE_4             = ISL29003_REG_CONTROL_INTRCYCLE_4,
    INTRCYCLE_8             = ISL29003_REG_CONTROL_INTRCYCLE_8,
    INTRCYCLE_16            = ISL29003_REG_CONTROL_INTRCYCLE_16
} islIntrCycle_t;

class ISL29003
{
    protected:
        // Instance-specific properties
        uint8_t isl_conversionDelay;
        islADCCore_t isl_adccore;
        islADCPD_t isl_adcpd;
        islTMode_t isl_tmode;
        islADCMode_t isl_adcmode;
        islIntrFlag_t isl_intrflag;
        islADCGain_t isl_adcgain;
        islIntrCycle_t isl_intrcycle;

    public:
        uint8_t isl_i2cAddress;
        void getAddr_ISL29003(uint8_t i2cAddress);
        void begin(void);
        uint16_t Measure_Light(void);
        void setADCCore(islADCCore_t core);
        islADCCore_t getADCCore(void);
        void setADCPD(islADCPD_t power);
        islADCPD_t getADCPD(void);
        void setTMode(islTMode_t tmode);
        islTMode_t getTMode(void);
        void setADCMode(islADCMode_t adcmode);
        islADCMode_t getADCMode(void);
        void setIntrFlag(islIntrFlag_t flag);
        islIntrFlag_t getIntrFlag(void);
        void setADCGain(islADCGain_t gain);
        islADCGain_t getADCGain(void);
        void setIntrCycle(islIntrCycle_t cycle);
        islIntrCycle_t getIntrCycle(void);

    private:
};
