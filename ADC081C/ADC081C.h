/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC081C
        This code is designed to work with the ADC081C_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC081C_I2CADC#tabs-0-product_tabset-2
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
/*                                                                                  ADC081C021
                                                                                    (SOT)           (VSSOP)
                                                                                ALERT       ADR1        ADR0
*/
    #define ADC081C_DEFAULT_ADDRESS                 (0x50)      // 1010000      -----       Floating    Floating
    #define ADC081C_ADDRESS_FLGND                   (0x51)      // 1010001      -----       Floating    GND
    #define ADC081C_ADDRESS_FLVA                    (0x52)      // 1010010      -----       Floating    VA
    #define ADC081C_ADDRESS_GNDFL                   (0x54)      // 1010100  Single Address  GND         Floating
    #define ADC081C_ADDRESS_GNDGND                  (0x55)      // 1010101      -----       GND         GND
    #define ADC081C_ADDRESS_GNDVA                   (0x56)      // 1010110      -----       GND         VA
    #define ADC081C_ADDRESS_VAFL                    (0x58)      // 1011000      -----       VA          Floating
    #define ADC081C_ADDRESS_VAGND                   (0x59)      // 1011001      -----       VA          GND
    #define ADC081C_ADDRESS_VAVA                    (0x5A)      // 1011010      -----       VA          VA

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADC081C_CONVERSIONDELAY                 (100)   // define the time interal(in ms) between each conversion

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define ADC081C_REG_POINTER_MASK                (0xFF)
    #define ADC081C_REG_POINTER_CONVERSION          (0x00)
    #define ADC081C_REG_POINTER_ALERT_STATUS        (0x01)
    #define ADC081C_REG_POINTER_CONFIG              (0x02)
    #define ADC081C_REG_POINTER_LOW_LIMIT           (0x03)
    #define ADC081C_REG_POINTER_HIGH_LIMIT          (0x04)
    #define ADC081C_REG_POINTER_HYSTERESIS          (0x05)
    #define ADC081C_REG_POINTER_LOWCONV             (0x06)
    #define ADC081C_REG_POINTER_HIGHCONV            (0x07)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define ADC081C_REG_CONFIG_CYCLE_TIME_MASK      (0xE0)      // Configures Automatic Conversion Mode
    #define ADC081C_REG_CONFIG_CYCLE_TIME_DIS       (0x00)      // Automatic Mode Disabled, 0 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_32        (0x20)      // Tconvert x 32, 27 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_64        (0x40)      // Tconvert x 64, 13.5 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_128       (0x60)      // Tconvert x 128, 6.7 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_256       (0x80)      // Tconvert x 256, 3.4 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_512       (0xA0)      // Tconvert x 512, 1.7 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_1024      (0xC0)      // Tconvert x 1024, 0.9 ksps
    #define ADC081C_REG_CONFIG_CYCLE_TIME_2048      (0xE0)      // Tconvert x 2048, 0.4 ksps

    #define ADC081C_REG_CONFIG_ALERT_HOLD_MASK      (0x10)      // Alert Hold
    #define ADC081C_REG_CONFIG_ALERT_HOLD_CLEAR     (0x00)      // Alerts will self-clear when the measured voltage moves within the limits by more than the hysteresis register value
    #define ADC081C_REG_CONFIG_ALERT_FLAG_NOCLEAR   (0x10)      // Alerts will not self-clear and are only cleared when a one is written to the alert high flag or the alert low flag in the Alert Status register

    #define ADC081C_REG_CONFIG_ALERT_FLAG_MASK      (0x08)      // Alert Flag Enable
    #define ADC081C_REG_CONFIG_ALERT_FLAG_DIS       (0x00)      // Disables alert status bit in the Conversion Result register
    #define ADC081C_REG_CONFIG_ALERT_FLAG_EN        (0x08)      // Enables alert status bit in the Conversion Result register

    #define ADC081C_REG_CONFIG_ALERT_PIN_MASK       (0x04)      // Alert Pin Enable
    #define ADC081C_REG_CONFIG_ALERT_PIN_DIS        (0x00)      // Disables the ALERT output pin. The ALERT output will TRI-STATE when the pin is disabled
    #define ADC081C_REG_CONFIG_ALERT_PIN_EN         (0x04)      // Enables the ALERT output pin.

    #define ADC081C_REG_CONFIG_POLARITY_MASK        (0x01)      // Polarity
    #define ADC081C_REG_CONFIG_POLARITY_LOW         (0x00)      // Sets the ALERT pin to active low
    #define ADC081C_REG_CONFIG_POLARITY_HIGH        (0x01)      // Sets the ALERT pin to active high


typedef enum
{
    AUTOMATIC_MODE_DISABLED     = ADC081C_REG_CONFIG_CYCLE_TIME_DIS,
    CYCLE_TIME_32               = ADC081C_REG_CONFIG_CYCLE_TIME_32,
    CYCLE_TIME_64               = ADC081C_REG_CONFIG_CYCLE_TIME_64,
    CYCLE_TIME_128              = ADC081C_REG_CONFIG_CYCLE_TIME_128,
    CYCLE_TIME_256              = ADC081C_REG_CONFIG_CYCLE_TIME_256,
    CYCLE_TIME_512              = ADC081C_REG_CONFIG_CYCLE_TIME_512,
    CYCLE_TIME_1024             = ADC081C_REG_CONFIG_CYCLE_TIME_1024,
    CYCLE_TIME_2048             = ADC081C_REG_CONFIG_CYCLE_TIME_2048
    
}adccycleTime_t;

typedef enum
{
    ALERT_HOLD_CLEAR            = ADC081C_REG_CONFIG_ALERT_HOLD_CLEAR,
    ALERT_HOLD_NOCLEAR          = ADC081C_REG_CONFIG_ALERT_FLAG_NOCLEAR
    
} adcAlertHold_t;

typedef enum
{
    ALERT_FLAG_DISABLE          = ADC081C_REG_CONFIG_ALERT_FLAG_DIS,
    ALERT_FLAG_ENABLE           = ADC081C_REG_CONFIG_ALERT_FLAG_EN
    
} adcAlertFlag_t;


typedef enum
{
    ALERT_PIN_DISABLE           = ADC081C_REG_CONFIG_ALERT_PIN_DIS,
    ALERT_PIN_ENABLE            = ADC081C_REG_CONFIG_ALERT_PIN_EN
    
} adcAlertPin_t;

typedef enum
{
    POLARITY_LOW                = ADC081C_REG_CONFIG_POLARITY_LOW,
    POLARITY_HIGH               = ADC081C_REG_CONFIG_POLARITY_HIGH
    
} adcPolarity_t;



class ADC081C
{
    protected:
        // Instance-specific properties
        uint8_t adc_conversionDelay;
        bool adc_alertLow;
        bool adc_alertHigh;
        adccycleTime_t adc_cycletime;
        adcAlertHold_t adc_alerthold;
        adcAlertFlag_t adc_alertflag;
        adcAlertPin_t adc_alertpin;
        adcPolarity_t adc_polarity;
    
    public:
        uint8_t adc_i2cAddress;
        void getAddr_ADC081C(uint8_t i2cAddress);
        void begin(void);
        uint8_t Measure_Voltage(void);
        bool getAlertStatus(void);
        bool alertLowTriggered();
        bool alertHighTriggered();
        void clearAlertStatus(void);
        void setCycleTime(adccycleTime_t cycletime);
        adccycleTime_t getCycleTime(void);
        void setAlertHold(adcAlertHold_t alerthold);
        adcAlertHold_t getAlertHold(void);
        void setAlertFlag(adcAlertFlag_t alertflag);
        adcAlertFlag_t getAlertFlag(void);
        void setAlertPin(adcAlertPin_t alertpin);
        adcAlertPin_t getAlertPin(void);
        void setPolarity(adcPolarity_t polarity);
        adcPolarity_t getPolarity(void);
        uint16_t setAlertLowLimit(uint16_t limit);
        uint16_t setAlertHighLimit(uint16_t limit);
        uint16_t setHysteresis(uint16_t hysteresis);
        uint16_t getLowestConversion(void);
        uint16_t getHighestConversion(void);
        uint16_t clearLowestConversion(void);
        uint16_t clearHighestConversion(void);

    private:
};
