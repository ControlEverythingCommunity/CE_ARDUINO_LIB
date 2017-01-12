/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAX44009
        This code is designed to work with the MAX44009_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.Controleverything.com/products
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
    #define MAX44009_DEFAULT_ADDRESS            (0x4A)          // GND, 1001 010
    #define MAX44009_VCC_ADDRESS                (0x4B)          // 1001 011

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MAX44009_CONVERSIONDELAY            (100)

/**************************************************************************
    REGISTER MAP
**************************************************************************/
    #define MAX44009_REG_MASK                   (0xFF)
    #define MAX44009_REG_INTR_STATUS            (0x00)
    #define MAX44009_REG_INTR_ENABLE            (0x01)
    #define MAX44009_REG_CONFIG                 (0x02)
    #define MAX44009_REG_LUX_HIGH               (0x03)
    #define MAX44009_REG_LUX_LOW                (0x04)
    #define MAX44009_REG_THRES_UP_HI            (0x05)
    #define MAX44009_REG_THRES_LO_HI            (0x06)
    #define MAX44009_REG_THRES_TIMER            (0x07)

/**************************************************************************
    INTERRUPT ENABLE REGISTER
**************************************************************************/
    #define MAX44009_REG_INTR_ENABLE_MASK           (0x01)      // Interrupt Enable
    #define MAX44009_REG_INTR_ENABLE_NO_ASSRT       (0x00)      // The INT pin and the INTS bit are not asserted even if an interrupt event has occurred
    #define MAX44009_REG_INTR_ENABLE_TRIGGER        (0x01)      // Detection of an interrupt event triggers a hardware interrupt (INT pin is pulled low) and sets the INTS bit (register 0x00, bit 0)

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define MAX44009_REG_CONFIG_CONTMODE_MASK       (0x80)      // Continuous Mode
    #define MAX44009_REG_CONFIG_CONTMODE_DEFAULT    (0x00)      // Default mode. The IC measures lux intensity only once every 800ms regardless of integration time.
    #define MAX44009_REG_CONFIG_CONTMODE_CONTIN     (0x80)      // Continuous mode. The IC continuously measures lux intensity

    #define MAX44009_REG_CONFIG_MANUAL_MASK         (0x40)      // Manual Configuration Mode
    #define MAX44009_REG_CONFIG_MANUAL_DEFAULT      (0x00)      // Default mode of configuration is used for the IC
    #define MAX44009_REG_CONFIG_MANUAL_MODEON       (0x40)      // Manual mode of configuration is used for the IC

    #define MAX44009_REG_CONFIG_CDR_MASK            (0x08)      // Current Division Ratio (CDR)
    #define MAX44009_REG_CONFIG_CDR_NODIVIDED       (0x00)      // Current not divided. All of the photodiode current goes to the ADC
    #define MAX44009_REG_CONFIG_CDR_DIVIDED         (0x08)      // Current divided by 8. Only 1/8 of the photodiode current goes to the ADC. This mode is used in high-brightness situations.

    #define MAX44009_REG_CONFIG_INTRTIMER_MASK      (0x07)      // Integration Timer Bits
    #define MAX44009_REG_CONFIG_INTRTIMER_800       (0x00)      // This is a preferred mode for boosting low-light sensitivity
    #define MAX44009_REG_CONFIG_INTRTIMER_400       (0x01)      // -
    #define MAX44009_REG_CONFIG_INTRTIMER_200       (0x02)      // -
    #define MAX44009_REG_CONFIG_INTRTIMER_100       (0x03)      // This is a preferred mode for high-brightness applications
    #define MAX44009_REG_CONFIG_INTRTIMER_50        (0x04)      // Manual Mode only
    #define MAX44009_REG_CONFIG_INTRTIMER_25        (0x05)      // Manual Mode only
    #define MAX44009_REG_CONFIG_INTRTIMER_12_5      (0x06)      // Manual Mode only
    #define MAX44009_REG_CONFIG_INTRTIMER_6_25      (0x07)      // Manual Mode only


typedef enum
{
    ENABLE_NO_ASSRT             = MAX44009_REG_INTR_ENABLE_NO_ASSRT,
    ENABLE_TRIGGER              = MAX44009_REG_INTR_ENABLE_TRIGGER
} maxIntrEnable_t;

typedef enum
{
    CONTMODE_DEFAULT            = MAX44009_REG_CONFIG_CONTMODE_DEFAULT,
    CONTMODE_CONTIN             = MAX44009_REG_CONFIG_CONTMODE_CONTIN
} maxContMode_t;

typedef enum
{
    MANUAL_DEFAULT              = MAX44009_REG_CONFIG_MANUAL_DEFAULT,
    MANUAL_MODEON               = MAX44009_REG_CONFIG_MANUAL_MODEON
    
} maxManualMode_t;

typedef enum
{
    CDR_NODIVIDED               = MAX44009_REG_CONFIG_CDR_NODIVIDED,
    CDR_DIVIDED                 = MAX44009_REG_CONFIG_CDR_DIVIDED
} maxCDR_t;

typedef enum
{
    INTRTIMER_800               = MAX44009_REG_CONFIG_INTRTIMER_800,
    INTRTIMER_400               = MAX44009_REG_CONFIG_INTRTIMER_400,
    INTRTIMER_200               = MAX44009_REG_CONFIG_INTRTIMER_200,
    INTRTIMER_100               = MAX44009_REG_CONFIG_INTRTIMER_100,
    INTRTIMER_50                = MAX44009_REG_CONFIG_INTRTIMER_50,
    INTRTIMER_25                = MAX44009_REG_CONFIG_INTRTIMER_25,
    INTRTIMER_12_5              = MAX44009_REG_CONFIG_INTRTIMER_12_5,
    INTRTIMER_6_25              = MAX44009_REG_CONFIG_INTRTIMER_6_25
    
} maxIntrTimer_t;

class MAX44009
{
    protected:
        // Instance-specific properties
        uint8_t max_conversionDelay;
        maxIntrEnable_t max_intrenable;
        maxContMode_t max_contmode;
        maxManualMode_t max_manualmode;
        maxCDR_t max_cdr;
        maxIntrTimer_t max_intrtimer;

    public:
        uint8_t max_i2cAddress;
        void getAddr_MAX44009(uint8_t i2cAddress);
        void begin(void);
        float Measure_Light(void);
        int8_t getIntr_Status(void);
        void setIntrEnable(maxIntrEnable_t intrenable);
        maxIntrEnable_t getIntrEnable(void);
        void setContMode(maxContMode_t contmode);
        maxContMode_t getContMode(void);
        void setManualMode(maxManualMode_t manualmode);
        maxManualMode_t getManualMode(void);
        void setCDR(maxCDR_t cdr);
        maxCDR_t getCDR(void);
        void setIntrTimer(maxIntrTimer_t intrtimer);
        maxIntrTimer_t getIntrTimer(void);

    private:
};
