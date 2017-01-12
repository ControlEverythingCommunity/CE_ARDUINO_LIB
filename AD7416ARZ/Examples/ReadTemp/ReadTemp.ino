/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7416ARZ
        This code is designed to work with the AD7416ARZ_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=AD7416ARZ_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <AD7416ARZ.h>

AD7416ARZ ad7416arz;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_DEFAULT_ADDRESS);           // 0x48, 1001 000
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_001);     // 0x49, 1001 001
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_010);     // 0x4A, 1001 010
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_011);     // 0x4B, 1001 011
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_100);     // 0x4C, 1001 100
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_101);     // 0x4D, 1001 101
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_110);     // 0x4E, 1001 110
    // ad7416arz.getAddr_AD7416ARZ(AD7416ARZ_ADDRESS_A2A1A0_111);     // 0x4F, 1001 111

    // The Channel Selection and Shutdown Mode
    // can be changed via the following functions

    ad7416arz.setChannel(CHANNEL_0);            // Channel 0
    // ad7416arz.setChannel(CHANNEL_1);         // Channel 1
    // ad7416arz.setChannel(CHANNEL_2);         // Channel 2
    // ad7416arz.setChannel(CHANNEL_3);         // Channel 3
    // ad7416arz.setChannel(CHANNEL_4);         // Channel 4

    ad7416arz.setShutdown(SHUTDOWN_DISABLE);    // Disable
    // ad7416arz.setShutdown(SHUTDOWN_ENABLE);  // Enable

    ad7416arz.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ad7416arz.ad7416arz_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from AD7416ARZ");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = ad7416arz.Measure_Temp();
        // 10-bit resolution
        temp = temp >> 6;
        float cTemp = temp * 0.25;
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
        Serial.println("AD7416ARZ Disconnected !");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
