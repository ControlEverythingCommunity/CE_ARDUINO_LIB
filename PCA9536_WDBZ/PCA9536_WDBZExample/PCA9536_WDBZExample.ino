/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9536_WDBZ
        This code is designed to work with the PCA9536_WDBZ_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/water-detect-sensor-with-buzzer
*/
/**************************************************************************/

#include <Wire.h>
#include <PCA9536_WDBZ.h>

PCA9536_WDBZ pca;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    pca.getAddr_PCA9536_WDBZ(PCA9536_WDBZ_DEFAULT_ADDRESS);           // 0x41

    pca.Reset();

    pca.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    uint8_t address;

    // PCA9536_WDBZ Sensor Address
    address = pca.pca_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint8_t state;
        Serial.println("Getting Water Detection Information from PCA9536_WDBZ");
        Serial.println(" ");

        // Set up for Sensor
        pca.setPinMode(PCA9536_WDBZ_OUTPUT_PIN0, PCA9536_WDBZ_MODE_OUTPUT);
        state = pca.getPinState(PCA9536_WDBZ_OUTPUT_PIN0);
        delay(1000);

        if (state == 0)
        {
            pca.setPinMode(PCA9536_WDBZ_OUTPUT_PIN3, PCA9536_WDBZ_MODE_OUTPUT);
            pca.setPinState(PCA9536_WDBZ_OUTPUT_PIN3, PCA9536_WDBZ_STATE_HIGH);

            // Output Data to Screen
            Serial.println("I/O Pin '3' State is HIGH");
            Serial.println("Buzzer is ON");
            Serial.println("I/O Pin '0' State is LOW");
            Serial.println("Water Detected");
            Serial.println(" ");
            Serial.println("        ***************************        ");
            Serial.println(" ");
            delay(1000);

        }
        else
        {
            pca.setPinMode(PCA9536_WDBZ_OUTPUT_PIN3, PCA9536_WDBZ_MODE_OUTPUT);
            pca.setPinState(PCA9536_WDBZ_OUTPUT_PIN3, PCA9536_WDBZ_STATE_LOW);

            // Output Data to Screen
            Serial.println("I/O Pin '3' State is LOW");
            Serial.println("Buzzer is OFF");
            Serial.println("I/O Pin '0' State is HIGH");
            Serial.println("No Water Present");
            Serial.println(" ");
            Serial.println("        ***************************        ");
            Serial.println(" ");
            delay(500);
        }
    }
    else
    {
        Serial.println("PCA9536_WDBZ Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
