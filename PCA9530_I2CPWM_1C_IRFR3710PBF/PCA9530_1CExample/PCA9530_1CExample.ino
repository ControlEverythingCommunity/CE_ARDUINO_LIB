/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9530_1C
        This code is designed to work with the PCA9530_I2CPWM_1C_IRFR3710PBF I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#include <Wire.h>
#include <PCA9530_1C.h>

PCA9530_1C pca;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    pca.getAddr_PCA9530_1C(PCA9530_1C_DEFAULT_ADDRESS);           // 0x60

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

    // PCA9530_1C Sensor Address
    address = pca.pca_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting LED State Readings from PCA9530_1C");
        Serial.println(" ");

        // Set up for Sensor for LED Selection
        // Sets the LS0 LED Select Registers, LS0-LED selector
        // setLEDSelector(uint8_t LED0)
        pca.setLEDSelector(PCA9530_1C_REG_LS0_PWM0);
        delay(500);

        // Output Data to Screen
        Serial.println("The LED blinks at PWM0 Rate");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("PCA9530_1C Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
