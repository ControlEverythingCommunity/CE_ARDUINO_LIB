/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP007
        This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2
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
//                                                                      ADR1    ADR0    SLAVE ADDRESS
    #define TMP007_DEFAULT_ADDRESS                  (0x40)      //      0       0       1000000
    #define TMP007_0_1_ADDRESS                      (0x41)      //      0       1       1000001
    #define TMP007_0_SDA_ADDRESS                    (0x42)      //      0       SDA     1000010
    #define TMP007_0_SCL_ADDRESS                    (0x43)      //      0       SCL     1000100
    #define TMP007_1_0_ADDRESS                      (0x44)      //      1       0       1000101
    #define TMP007_1_1_ADDRESS                      (0x45)      //      1       1       1000110
    #define TMP007_1_SDA_ADDRESS                    (0x46)      //      1       SDA     1000011
    #define TMP007_1_SCL_ADDRESS                    (0x47)      //      1       SCL     1000111

    #define TMP007_DEV_ID                           (0x78)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define TMP007_CONVERSIONDELAY                  (100)

/**************************************************************************
    INTERNAL REGISTERS
**************************************************************************/
    #define TMP007_REG_VSENSOR_VOLTAGE              (0x00)
    #define TMP007_REG_TDIE_LOCAL_TEMP              (0x01)
    #define TMP007_REG_CONFIG                       (0x02)
    #define TMP007_REG_TOBJECT_TEMP                 (0x03)
    #define TMP007_REG_STATUS                       (0x04)
    #define TMP007_REG_STATUS_MASK_ENABLE           (0x05)
    #define TMP007_REG_TOBJECT_TEMP_HLIMIT          (0x06)
    #define TMP007_REG_TOBJECT_TEMP_LLIMIT          (0x07)
    #define TMP007_REG_TDIE_LOCAL_TEMP_HLIMIT       (0x08)
    #define TMP007_REG_TDIE_LOCAL_TEMP_LLIMIT       (0x09)
    #define TMP007_REG_S0_COEFFICIENT               (0x0A)
    #define TMP007_REG_A1_COEFFICIENT               (0x0B)
    #define TMP007_REG_A2_COEFFICIENT               (0x0C)
    #define TMP007_REG_B0_COEFFICIENT               (0x0D)
    #define TMP007_REG_B1_COEFFICIENT               (0x0E)
    #define TMP007_REG_B2_COEFFICIENT               (0x0F)
    #define TMP007_REG_C2_COEFFICIENT               (0x10)
    #define TMP007_REG_TC0_COEFFICIENT              (0x11)
    #define TMP007_REG_TC1_COEFFICIENT              (0x12)
    #define TMP007_REG_MANUF_ID                     (0x1E)
    #define TMP007_REG_DEV_ID                       (0x1F)
    #define TMP007_REG_MEMORY_ACCESS                (0x2A)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define TMP007_REG_CONFIG_RST_MASK              (0x8000)      // Software Reset
    #define TMP007_REG_CONFIG_RST_DISABLED          (0x0000)      // System Reset Disabled
    #define TMP007_REG_CONFIG_RST_ENABLED           (0x8000)      // Power ON Reset

    #define TMP007_REG_CONFIG_MOD_MASK              (0x1000)      // Conversion Mode Selection
    #define TMP007_REG_CONFIG_MOD_DOWN              (0x0000)      // Power Down
    #define TMP007_REG_CONFIG_MOD_ON                (0x1000)      // Conversion ON

    #define TMP007_REG_CONFIG_CR_MASK               (0x0E00)      // Conversion Rate/Averaging Mode
    #define TMP007_REG_CONFIG_CR_1                  (0x0000)      // No. of Avgs. per Conversion: 1, Total Conversion Time (s): 0.26
    #define TMP007_REG_CONFIG_CR_2                  (0x0200)      // No. of Avgs. per Conversion: 2, Total Conversion Time (s): 0.51
    #define TMP007_REG_CONFIG_CR_4                  (0x0400)      // No. of Avgs. per Conversion: 4, Total Conversion Time (s): 1.01
    #define TMP007_REG_CONFIG_CR_8                  (0x0600)      // No. of Avgs. per Conversion: 8, Total Conversion Time (s): 2.01
    #define TMP007_REG_CONFIG_CR_16                 (0x0800)      // No. of Avgs. per Conversion: 16, Total Conversion Time (s): 4.01
    #define TMP007_REG_CONFIG_CR_1_IDLE             (0x0A00)      // No. of Avgs. per Conversion: 1, Total Conversion Time (s): 1.0 (Idle for 0.75)
    #define TMP007_REG_CONFIG_CR_2_IDLE             (0x0C00)      // No. of Avgs. per Conversion: 2, Total Conversion Time (s): 4.0 (Idle for 3.5)
    #define TMP007_REG_CONFIG_CR_4_IDLE             (0x0E00)      // No. of Avgs. per Conversion: 4, Total Conversion Time (s): 4.0 (Idle for 3.0)

    #define TMP007_REG_CONFIG_ALRTEN_MASK           (0x0100)      // Alert Pin Enable
    #define TMP007_REG_CONFIG_ALRTEN_DISABLE        (0x0000)      // Alert Pin Disable
    #define TMP007_REG_CONFIG_ALRTEN_ENABLE         (0x0100)      // Alert Pin Enable

    #define TMP007_REG_CONFIG_ALRTF_MASK            (0x0080)      // Cumulative Alert Flag Enable
    #define TMP007_REG_CONFIG_ALRTF_DISABLE         (0x0000)      // Cumulative Alert Flag Disable
    #define TMP007_REG_CONFIG_ALRTF_ENABLE          (0x0080)      // Cumulative Alert Flag Enable

    #define TMP007_REG_CONFIG_TC_MASK               (0x0040)      // Transient Correction Enable
    #define TMP007_REG_CONFIG_TC_DISABLE            (0x0000)      // Transient Correction Disable
    #define TMP007_REG_CONFIG_TC_ENABLE             (0x0040)      // Transient Correction Enable

    #define TMP007_REG_CONFIG_INT_COMP_MASK         (0x0020)      // INT/COMP Bit Controls
    #define TMP007_REG_CONFIG_INT_COMP_INTERRUPT    (0x0000)      // Interrupt Mode
    #define TMP007_REG_CONFIG_INT_COMP_COMPARATOR   (0x0020)      // Comparator Mode

/**************************************************************************
    STATUS REGISTER
**************************************************************************/
    #define TMP007_REG_STATUS_ALRTF_MASK            (0x8000)      // Cumulative Alert Flag Enable
    #define TMP007_REG_STATUS_ALRTF_DISABLE         (0x0000)      // Cumulative Alert Flag Disable
    #define TMP007_REG_STATUS_ALRTF_ENABLE          (0x8000)      // Cumulative Alert Flag Enable

    #define TMP007_REG_STATUS_CRTF_MASK             (0x4000)      // Conversion Ready Flag Enable
    #define TMP007_REG_STATUS_CRTF_DISABLE          (0x0000)      // Conversion Ready Flag Disable
    #define TMP007_REG_STATUS_CRTF_ENABLE           (0x4000)      // Conversion Ready Flag Enable


typedef enum
{
    RESET_DISABLED              = TMP007_REG_CONFIG_RST_DISABLED,
    RESET_ENABLED               = TMP007_REG_CONFIG_RST_ENABLED

} tmpReset_t;

typedef enum
{
    POWER_DOWN                  = TMP007_REG_CONFIG_MOD_DOWN,
    POWER_ON                    = TMP007_REG_CONFIG_MOD_ON
    
} tmpPowerMode_t;

typedef enum
{
    CONVERSION_RATE_1           = TMP007_REG_CONFIG_CR_1,
    CONVERSION_RATE_2           = TMP007_REG_CONFIG_CR_2,
    CONVERSION_RATE_4           = TMP007_REG_CONFIG_CR_4,
    CONVERSION_RATE_8           = TMP007_REG_CONFIG_CR_8,
    CONVERSION_RATE_16          = TMP007_REG_CONFIG_CR_16,
    CONVERSION_RATE_1_IDLE      = TMP007_REG_CONFIG_CR_1_IDLE,
    CONVERSION_RATE_2_IDLE      = TMP007_REG_CONFIG_CR_2_IDLE,
    CONVERSION_RATE_4_IDLE      = TMP007_REG_CONFIG_CR_4_IDLE
    
} tmpConvRate_t;

typedef enum
{
    ALERT_DISABLE               = TMP007_REG_CONFIG_ALRTEN_DISABLE,
    ALERT_ENABLE                = TMP007_REG_CONFIG_ALRTEN_ENABLE
    
} tmpAlert_t;

typedef enum
{
    ALERTFLAG_DISABLE           = TMP007_REG_CONFIG_ALRTF_DISABLE,
    ALERTFLAG_ENABLE            = TMP007_REG_CONFIG_ALRTF_ENABLE

} tmpAlertFlag_t;

typedef enum
{
    TRANSCORR_DISABLE           = TMP007_REG_CONFIG_TC_DISABLE,
    TRANSCORR_ENABLE            = TMP007_REG_CONFIG_TC_ENABLE
    
} tmpTransient_t;

typedef enum
{
    INTERRUPT_MODE              = TMP007_REG_CONFIG_INT_COMP_INTERRUPT,
    COMPARATOR_MODE             = TMP007_REG_CONFIG_INT_COMP_COMPARATOR
    
} tmpIntComp_t;

typedef enum
{
    STATUS_ALERTFLAG_DISABLE    = TMP007_REG_STATUS_ALRTF_DISABLE,
    STATUS_ALERTFLAG_ENABLE     = TMP007_REG_STATUS_ALRTF_ENABLE
    
} tmpStatusAlertFlag_t;

typedef enum
{
    STATUS_CONVERSION_DISABLE   = TMP007_REG_STATUS_CRTF_DISABLE,
    STATUS_CONVERSION_ENABLE    = TMP007_REG_STATUS_CRTF_ENABLE
    
} tmpStatusConversion_t;

class TMP007
{
    protected:
        // Instance-specific properties
        uint8_t tmp_conversionDelay;
        tmpReset_t tmp_reset;
        tmpPowerMode_t tmp_powermode;
        tmpConvRate_t tmp_convrate;
        tmpAlert_t tmp_alert;
        tmpAlertFlag_t tmp_alertflag;
        tmpTransient_t tmp_transient;
        tmpIntComp_t tmp_intcomp;
        tmpStatusAlertFlag_t tmp_statusalertflag;
        tmpStatusConversion_t tmp_statusconversion;

    public:
        uint8_t tmp_i2cAddress;
        void getAddr_TMP007(uint8_t i2cAddress);
        bool begin(void);
        void setConfiguration(void);
        float Measure_Voltage(void);
        float Measure_DieTemp(void);
        float Measure_ObjTemp(void);
        void setReset(tmpReset_t reset);
        tmpReset_t getReset(void);
        void setPowerMode(tmpPowerMode_t powermode);
        tmpPowerMode_t getPowerMode(void);
        void setConvRate(tmpConvRate_t convrate);
        tmpConvRate_t getConvRate(void);
        void setAlert(tmpAlert_t alert);
        tmpAlert_t getAlert(void);
        void setAlertFlag(tmpAlertFlag_t alertflag);
        tmpAlertFlag_t getAlertFlag(void);
        void setTransient(tmpTransient_t transient);
        tmpTransient_t getTransient(void);
        void setStatusAlertFlag(tmpStatusAlertFlag_t statusalertflag);
        tmpStatusAlertFlag_t getStatusAlertFlag(void);
        void setStatusConversion(tmpStatusConversion_t statusconversion);
        tmpStatusConversion_t getStatusConversion(void);
  
    private:
};
