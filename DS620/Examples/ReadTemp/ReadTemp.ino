/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        DS620
        This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=DS620_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <DS620.h>

DS620 ds;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ds.getAddr_DS620(DS620_DEFAULT_ADDRESS);           // 0x48, 1001 000
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_001);     // 0x49, 1001 001
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_010);     // 0x4A, 1001 010
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_011);     // 0x4B, 1001 011
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_100);     // 0x4C, 1001 100
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_101);     // 0x4D, 1001 101
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_110);     // 0x4E, 1001 110
    // ds.getAddr_DS620(DS620_ADDRESS_A2A1A0_111);     // 0x4F, 1001 111

    // The Conversion Resolution
    // can be changed via the following function

    ds.setResolution(RESOLUTION_13BIT);             // Resolution: 13-bit
    // ds.setResolution(RESOLUTION_10BIT);          // Resolution: 10-bit
    // ds.setResolution(RESOLUTION_11BIT);          // Resolution: 11-bit
    // ds.setResolution(RESOLUTION_12BIT);          // Resolution: 12-bit

    ds.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ds.ds_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t rawtemp;
        float cTemp;

        Serial.println("Getting Temperature Readings from DS620");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        rawtemp = ds.Measure_Temp();
        // 13-Bit Resolution
        cTemp = rawtemp * 0.0625;
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("DS620 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
