/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AT30TS74
        This code is designed to work with the AT30TS74_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <AT30TS74.h>

AT30TS74 at;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    at.getAddr_AT30TS74(AT30TS74_DEFAULT_ADDRESS);           // 0x48, 1001 000
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_001);     // 0x49, 1001 001
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_010);     // 0x4A, 1001 010
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_011);     // 0x4B, 1001 011
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_100);     // 0x4C, 1001 100
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_101);     // 0x4D, 1001 101
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_110);     // 0x4E, 1001 110
    // at.getAddr_AT30TS74(AT30TS74_ADDRESS_A2A1A0_111);     // 0x4F, 1001 111

    // The One-Shot Mode, Shutdown Mode and Resolution
    // can be changed via the following functions

    at.setOneShot(ONESHOT_DISABLED);        // Disabled (Power-up default)
    // at.setOneShot(ONESHOT_ENSABLED);     // Enabled

    at.setShutdown(SHUTDOWN_DISABLE);       // Disable (Power-up default)
    // at.setShutdown(SHUTDOWN_ENABLE);     // Enable

    at.setResolution(RESOLUTION_12);        // 12 bit, data shifted right by 4 bits, multiply by 0.0625, (___>> 4) * 0.0625
    // at.setResolution(RESOLUTION_9);      // 9 bit (Power-up default), , data shifted right by 7 bits, multiply by 0.5, (___>> 7) * 0.5
    // at.setResolution(RESOLUTION_10);     // 10 bit, data shifted right by 6 bits, multiply by 0.125, (___>> 6) * 0.125
    // at.setResolution(RESOLUTION_11);     // 11 bit, data shifted right by 5 bits, multiply by 0.25, (___>> 5) * 0.25

    at.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = at.at_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from AT30TS74");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = at.Measure_Temp();
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
        Serial.println("AT30TS74 Disconnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
