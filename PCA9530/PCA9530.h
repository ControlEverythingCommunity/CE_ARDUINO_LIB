/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9530
        This code is designed to work with the PCA9530_I2CPWM I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
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
    #define PCA9530_DEFAULT_ADDRESS         (0x60)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define PCA9530_CONVERSIONDELAY         (100)

/**************************************************************************
    CONTROL REGISTERS
**************************************************************************/
    #define PCA9530_REG_INPUT               (0x00)      // Input Register
    #define PCA9530_REG_PSC0                (0x01)      // frequency Prescaler 0 Register
    #define PCA9530_REG_PWM0                (0x02)      // PWM Register 0
    #define PCA9530_REG_PSC1                (0x03)      // frequency Prescaler 1 Register
    #define PCA9530_REG_PWM1                (0x04)      // PWM Register 1
    #define PCA9530_REG_LS0                 (0x05)      // LED Selector Register

/**************************************************************************
    LED SELECTOR REGISTER 0
**************************************************************************/
    #define PCA9530_REG_LS0_LED_OFF         (0x00)      // Output is Set High-Impedance, LED OFF
    #define PCA9530_REG_LS0_LED_ON          (0x01)      // Output is Set LOW, LED ON
    #define PCA9530_REG_LS0_PWM0            (0x02)      // Output Blinks at PWM0 Rate
    #define PCA9530_REG_LS0_PWM1            (0x03)      // Output Blinks at PWM1 Rate


class PCA9530
{
    protected:
        // Instance-specific properties
        uint8_t pca_conversionDelay;

    public:
        uint8_t pca_i2cAddress;
        void getAddr_PCA9530(uint8_t i2cAddress);
        bool begin();
        void setFreqPSC0(uint8_t period);
        void setPWM0(uint8_t dutycycle);
        void setFreqPSC1(uint8_t period);
        void setPWM1(uint8_t dutycycle);
        void setLEDSelector(uint8_t LED1, uint8_t LED0);
    
    private:
};
