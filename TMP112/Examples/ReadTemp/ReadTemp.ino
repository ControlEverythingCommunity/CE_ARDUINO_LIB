/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP112
        This code is designed to work with the TMP112_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP112_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMP112.h>

TMP112 tmp;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    tmp.getAddr_TMP112(TMP112_DEFAULT_ADDRESS);         // 0x48, 1001000
    // tmp.getAddr_TMP112(TMP112_ADDRESS_VPLUS);        // 0x49, 1001001

    // The One-Shot Mode, Shutdown Mode
    // can be changed via the following functions

    tmp.setOneShot(ONESHOT_DISABLED);        // Disabled
    // tmp.setOneShot(ONESHOT_ENSABLED);     // Enabled

    tmp.setShutdown(SHUTDOWN_DISABLE);      // Disable
    // tmp.setShutdown(SHUTDOWN_ENABLE);    // Enable


    tmp.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = tmp.tmp_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from TMP112");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = tmp.Measure_Temp();
        // 12-bit resolution
        temp = temp >> 4;
        float cTemp = temp * 0.0625;
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("TMP112 Disconnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
