/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9536_BZ
        This code is designed to work with the PCA9536_BZ_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/digital-3-channel-input-output-with-buzzer
*/
/**************************************************************************/

#include <Wire.h>
#include <PCA9536_BZ.h>

PCA9536_BZ pca;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    pca.getAddr_PCA9536_BZ(PCA9536_BZ_DEFAULT_ADDRESS);           // 0x41

    pca.Reset();

    pca.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    // PCA9536_BZ Sensor Address
    address = pca.pca_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Pin State Readings from PCA9536_BZ");
        Serial.println(" ");

        // Set up for Sensor
        pca.setPinMode(PCA9536_BZ_OUTPUT_PIN3, PCA9536_BZ_MODE_OUTPUT);
        delay(1000);
        // pca.setPinMode(PCA9536_BZ_OUTPUT_PIN3, PCA9536_BZ_MODE_INPUT);
        delay(1000);

        // Output Data to Screen
        Serial.println("I/O Pin '3' State is HIGH");
        Serial.println("Buzzer is ON");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("PCA9536_BZ Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
