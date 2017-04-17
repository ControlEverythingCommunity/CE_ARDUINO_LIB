/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9531
        This code is designed to work with the PCA9531_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/2-channel-8-bit-pwm-with-8-outputs-gpio-i2c-led-dimmer
*/
/**************************************************************************/

#include <Wire.h>
#include <PCA9531.h>

PCA9531 pca;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    pca.getAddr_PCA9531(PCA9531_DEFAULT_ADDRESS);           // 0x60

    pca.setFreqPSC0(75);        // Period of Blink = 0.5 sec
    pca.setPWM0(128);           // Duty Cycle = 50
    pca.setFreqPSC1(75);        // Period of Blink = 0.5 sec
    pca.setPWM1(128);           // Duty Cycle = 50

    pca.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    // PCA9531 Sensor Address
    address = pca.pca_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting LED State Readings from PCA9531");
        Serial.println(" ");

        // Set up for Sensor for LED Selection
        // Sets the LS0 LED Select Registers, LS0-LED0 to LED3 selector
        // setLEDSelector0(uint8_t LED3, uint8_t LED2, uint8_t LED1, uint8_t LED0)
        pca.setLEDSelector0(PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0);

        // setLEDSelector1(uint8_t LED7, uint8_t LED6, uint8_t LED5, uint8_t LED4)
        // Sets the LS1 LED Select Registers, LS4-LED7 to LED3 selector
        pca.setLEDSelector1(PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0, PCA9531_REG_LS0_PWM0);
        delay(500);

        // Output Data to Screen
        Serial.println("All LED's blinks at PWM0 Rate");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("PCA9531 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
