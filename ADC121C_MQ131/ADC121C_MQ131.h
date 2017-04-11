/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_MQ131
        This code is designed to work with the ADC121C_I2CGAS_MQ131 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/ozone-gas-sensor-12-bit
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
/*                                                                                      ADC121C_MQ131
                                                                                    (SOT)           (VSSOP)
                                                                                    ALERT       ADR1        ADR0
*/
    #define ADC121C_MQ131_DEFAULT_ADDRESS                   (0x50)      // 1010000      -----       Floating    Floating
    #define ADC121C_MQ131_ADDRESS_FLGND                     (0x51)      // 1010001      -----       Floating    GND
    #define ADC121C_MQ131_ADDRESS_FLVA                      (0x52)      // 1010010      -----       Floating    VA
    #define ADC121C_MQ131_ADDRESS_GNDFL                     (0x54)      // 1010100  Single Address  GND         Floating
    #define ADC121C_MQ131_ADDRESS_GNDGND                    (0x55)      // 1010101      -----       GND         GND
    #define ADC121C_MQ131_ADDRESS_GNDVA                     (0x56)      // 1010110      -----       GND         VA
    #define ADC121C_MQ131_ADDRESS_VAFL                      (0x58)      // 1011000      -----       VA          Floating
    #define ADC121C_MQ131_ADDRESS_VAGND                     (0x59)      // 1011001      -----       VA          GND
    #define ADC121C_MQ131_ADDRESS_VAVA                      (0x5A)      // 1011010      -----       VA          VA

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADC121C_MQ131_CONVERSIONDELAY                   (100)       // Define the Time Interal(in ms) Between each Conversion
    #define ADC121C_MQ9_SAMPLE_TIMES                        (5)         // Define the Number of Readings of Values in a Cycle
    #define ADC121C_MQ9_SAMPLE_INTERVAL                     (20)        // Define the Time Interal(in ms) Between each Sensor Reading
    #define VREF                                            (5.0)       // Define the Voltage Reference
    #define ADC_RESOLUTION                                  (4096.0)    // Define the 12-Bit ADC121 Resolution

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define ADC121C_MQ131_REG_POINTER_MASK                  (0xFF)
    #define ADC121C_MQ131_REG_POINTER_CONVERSION            (0x00)
    #define ADC121C_MQ131_REG_POINTER_ALERT_STATUS          (0x01)
    #define ADC121C_MQ131_REG_POINTER_CONFIG                (0x02)
    #define ADC121C_MQ131_REG_POINTER_LOW_LIMIT             (0x03)
    #define ADC121C_MQ131_REG_POINTER_HIGH_LIMIT            (0x04)
    #define ADC121C_MQ131_REG_POINTER_HYSTERESIS            (0x05)
    #define ADC121C_MQ131_REG_POINTER_LOWCONV               (0x06)
    #define ADC121C_MQ131_REG_POINTER_HIGHCONV              (0x07)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_MASK        (0xE0)      // Configures Automatic Conversion Mode
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_DIS         (0x00)      // Automatic Mode Disabled, 0 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_32          (0x20)      // Tconvert x 32, 27 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_64          (0x40)      // Tconvert x 64, 13.5 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_128         (0x60)      // Tconvert x 128, 6.7 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_256         (0x80)      // Tconvert x 256, 3.4 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_512         (0xA0)      // Tconvert x 512, 1.7 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_1024        (0xC0)      // Tconvert x 1024, 0.9 ksps
    #define ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_2048        (0xE0)      // Tconvert x 2048, 0.4 ksps

    #define ADC121C_MQ131_REG_CONFIG_ALERT_HOLD_MASK        (0x10)      // Alert Hold
    #define ADC121C_MQ131_REG_CONFIG_ALERT_HOLD_CLEAR       (0x00)      // Alerts will self-clear when the measured voltage moves within the limits by more than the hysteresis register value
    #define ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_NOCLEAR     (0x10)      // Alerts will not self-clear and are only cleared when a one is written to the alert high flag or the alert low flag in the Alert Status register

    #define ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_MASK        (0x08)      // Alert Flag Enable
    #define ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_DIS         (0x00)      // Disables alert status bit in the Conversion Result register
    #define ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_EN          (0x08)      // Enables alert status bit in the Conversion Result register

    #define ADC121C_MQ131_REG_CONFIG_ALERT_PIN_MASK         (0x04)      // Alert Pin Enable
    #define ADC121C_MQ131_REG_CONFIG_ALERT_PIN_DIS          (0x00)      // Disables the ALERT output pin. The ALERT output will TRI-STATE when the pin is disabled
    #define ADC121C_MQ131_REG_CONFIG_ALERT_PIN_EN           (0x04)      // Enables the ALERT output pin.

    #define ADC121C_MQ131_REG_CONFIG_POLARITY_MASK          (0x01)      // Polarity
    #define ADC121C_MQ131_REG_CONFIG_POLARITY_LOW           (0x00)      // Sets the ALERT pin to active low
    #define ADC121C_MQ131_REG_CONFIG_POLARITY_HIGH          (0x01)      // Sets the ALERT pin to active high


typedef enum
{
    AUTOMATIC_MODE_DISABLED     = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_DIS,
    CYCLE_TIME_32               = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_32,
    CYCLE_TIME_64               = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_64,
    CYCLE_TIME_128              = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_128,
    CYCLE_TIME_256              = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_256,
    CYCLE_TIME_512              = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_512,
    CYCLE_TIME_1024             = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_1024,
    CYCLE_TIME_2048             = ADC121C_MQ131_REG_CONFIG_CYCLE_TIME_2048
    
}ADC121C_MQ131CycleTime_t;

typedef enum
{
    ALERT_HOLD_CLEAR            = ADC121C_MQ131_REG_CONFIG_ALERT_HOLD_CLEAR,
    ALERT_HOLD_NOCLEAR          = ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_NOCLEAR
    
} ADC121C_MQ131AlertHold_t;

typedef enum
{
    ALERT_FLAG_DISABLE          = ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_DIS,
    ALERT_FLAG_ENABLE           = ADC121C_MQ131_REG_CONFIG_ALERT_FLAG_EN
    
} ADC121C_MQ131AlertFlag_t;

typedef enum
{
    ALERT_PIN_DISABLE           = ADC121C_MQ131_REG_CONFIG_ALERT_PIN_DIS,
    ALERT_PIN_ENABLE            = ADC121C_MQ131_REG_CONFIG_ALERT_PIN_EN
    
} ADC121C_MQ131AlertPin_t;

typedef enum
{
    POLARITY_LOW                = ADC121C_MQ131_REG_CONFIG_POLARITY_LOW,
    POLARITY_HIGH               = ADC121C_MQ131_REG_CONFIG_POLARITY_HIGH
    
} ADC121C_MQ131Polarity_t;


class ADC121C_MQ131
{
    protected:
        // Instance-specific properties
        uint8_t ADC121C_MQ131_conversionDelay;
        bool ADC121C_MQ131_alertLow;
        bool ADC121C_MQ131_alertHigh;
        ADC121C_MQ131CycleTime_t ADC121C_MQ131_cycletime;
        ADC121C_MQ131AlertHold_t ADC121C_MQ131_alerthold;
        ADC121C_MQ131AlertFlag_t ADC121C_MQ131_alertflag;
        ADC121C_MQ131AlertPin_t ADC121C_MQ131_alertpin;
        ADC121C_MQ131Polarity_t ADC121C_MQ131_polarity;
    
    public:
        uint8_t ADC121C_MQ131_i2cAddress;
        void getAddr_ADC121C_MQ131(uint8_t i2cAddress);
        void begin(void);
        uint16_t Measure_Voltage(void);
        bool getAlertStatus(void);
        bool alertLowTriggered();
        bool alertHighTriggered();
        void clearAlertStatus(void);
        void setCycleTime(ADC121C_MQ131CycleTime_t cycletime);
        ADC121C_MQ131CycleTime_t getCycleTime(void);
        void setAlertHold(ADC121C_MQ131AlertHold_t alerthold);
        ADC121C_MQ131AlertHold_t getAlertHold(void);
        void setAlertFlag(ADC121C_MQ131AlertFlag_t alertflag);
        ADC121C_MQ131AlertFlag_t getAlertFlag(void);
        void setAlertPin(ADC121C_MQ131AlertPin_t alertpin);
        ADC121C_MQ131AlertPin_t getAlertPin(void);
        void setPolarity(ADC121C_MQ131Polarity_t polarity);
        ADC121C_MQ131Polarity_t getPolarity(void);
        uint16_t setAlertLowLimit(uint16_t limit);
        uint16_t setAlertHighLimit(uint16_t limit);
        uint16_t setHysteresis(uint16_t hysteresis);
        uint16_t getLowestConversion(void);
        uint16_t getHighestConversion(void);
        uint16_t clearLowestConversion(void);
        uint16_t clearHighestConversion(void);
        void Calibration();
        void Calibration(float Ro);
        float Measure_RatioRsRo() ;
        inline bool isCalibrated()
        {
            return _stateCalibration;
        };
        inline float Measure_Ro()
        {
            return _Ro;
        };
        float Measure_Scaled(float a, float b) ;
        float Measure_Ozone(float a, float b);
    
    private:
        bool _stateCalibration = false;
        float _Ro = 1.0f;
        float Measure_Rs() ;
        float Measure_Resistance(int rawadc) ;
        // Resistor Installed on the Board (kOhm)
        float Measure_RL()  { return 20.0; }
        // Clean Air Ratio
        float Measure_RoInCleanAir()  { return 20.0; }

};
