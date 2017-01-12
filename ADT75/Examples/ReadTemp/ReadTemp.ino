/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADT75
        This code is designed to work with the ADT75_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=ADT75_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ADT75.h>

ADT75 adt;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    adt.getAddr_ADT75(ADT75_DEFAULT_ADDRESS);           // 0x48, 1001 000
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_001);     // 0x49, 1001 001
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_010);     // 0x4A, 1001 010
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_011);     // 0x4B, 1001 011
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_100);     // 0x4C, 1001 100
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_101);     // 0x4D, 1001 101
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_110);     // 0x4E, 1001 110
    // adt.getAddr_ADT75(ADT75_ADDRESS_A2A1A0_111);     // 0x4F, 1001 111

    // The One-Shot Mode, Shutdown Mode and SMBus Alert
    // can be changed via the following functions

    adt.setSM(SM_DISABLED);               // Disabled
    // adt.setSM(SM_ENABLED);               // Enabled

    adt.setOneShot(ONESHOT_NORMAL);         // Normal
    // adt.setOneShot(ONESHOT_ENABLED);     // Enabled

    adt.setShutdown(SHUTDOWN_DISABLE);      // Disable
    // adt.setShutdown(SHUTDOWN_ENABLE);    // Enable

    adt.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = adt.adt_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from ADT75");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = adt.Measure_Temp();
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
        Serial.println("ADT75 Disonnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
