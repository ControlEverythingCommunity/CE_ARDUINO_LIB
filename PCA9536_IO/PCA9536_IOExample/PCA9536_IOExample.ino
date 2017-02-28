/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9536_IO
        This code is designed to work with the PCA9536_IO_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/pca9536-digital-4-channel-input-output-i2c-mini-module
*/
/**************************************************************************/

#include <Wire.h>
#include <PCA9536_IO.h>

PCA9536_IO pca;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    pca.getAddr_PCA9536_IO(PCA9536_IO_DEFAULT_ADDRESS);           // 0x41

    pca.Reset();

    pca.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    // PCA9536_IO Sensor Address
    address = pca.pca_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Pin State Readings from PCA9536_IO");
        Serial.println(" ");

        // Set up for Sensor
        pca.setPinMode(PCA9536_IO_OUTPUT_PIN2, PCA9536_IO_MODE_OUTPUT);
        delay(1000);
        // pca.setPinMode(PCA9536_IO_OUTPUT_PIN2, PCA9536_IO_MODE_INPUT);
        delay(1000);

        // Output Data to Screen
        Serial.println("I/O Pin '2' State is HIGH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("PCA9536_IO Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
