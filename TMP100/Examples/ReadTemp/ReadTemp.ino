/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP100
        This code is designed to work with the TMP100_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP100_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMP100.h>

TMP100 tmp;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    tmp.getAddr_TMP100(TMP100_DEFAULT_ADDRESS);         // 0x4F, 1001111
    // tmp.getAddr_TMP100(TMP100_0_0_ADDRESS);          // 0x48, 1001000
    // tmp.getAddr_TMP100(TMP100_0_FLOAT_ADDRESS);      // 0x49, 1001001
    // tmp.getAddr_TMP100(TMP100_0_1_ADDRESS);          // 0x4A, 1001010
    // tmp.getAddr_TMP100(TMP100_1_0_ADDRESS);          // 0x4B, 1001011
    // tmp.getAddr_TMP100(TMP100_1_FLOAT_ADDRESS);      // 0x4C, 1001100
    // tmp.getAddr_TMP100(TMP100_1_1_ADDRESS);          // 0x4D, 1001101
    // tmp.getAddr_TMP100(TMP100_FLOAT_0_ADDRESS);      // 0x4E, 1001110

    // The One-Shot Mode, Shutdown Mode and Resolution
    // can be changed via the following functions

    tmp.setOneShot(ONESHOT_DISABLED);        // Disabled
    // tmp.setOneShot(ONESHOT_ENSABLED);     // Enabled

    tmp.setShutdown(SHUTDOWN_DISABLE);      // Disable
    // tmp.setShutdown(SHUTDOWN_ENABLE);    // Enable

    tmp.setResolution(RESOLUTION_12);       // 12 bit, data shifted right by 4 bits, multiply by 0.0625, (___>> 4) * 0.0625
    // tmp.setResolution(RESOLUTION_9);     // 9 bit, , data shifted right by 7 bits, multiply by 0.5, (___>> 7) * 0.5
    // tmp.setResolution(RESOLUTION_10);    // 10 bit, data shifted right by 6 bits, multiply by 0.125, (___>> 6) * 0.125
    // tmp.setResolution(RESOLUTION_11);    // 11 bit, data shifted right by 5 bits, multiply by 0.25, (___>> 5) * 0.25

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

        Serial.println("Getting Temperature Readings from TMP100");
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
        Serial.println("TMP100 Disconnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
