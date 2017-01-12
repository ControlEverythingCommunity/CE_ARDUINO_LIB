/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7005
        This code is designed to work with the SI7005_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7005_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <SI7005.h>

SI7005 si;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    si.getAddr_SI7005(SI7005_DEFAULT_ADDRESS);           // 0x40

    // The Fast Enable, Heater Enable and Conversion Mode
    // can be changed via the following functions

    si.setFastEnable(FAST_ENABLE_35);        // 35 ms (typical)
    // si.setFastEnable(FAST_ENABLE_18);     // 18 ms (typical)

    si.setHeaterEnable(HEATER_OFF);      // Heater Off
    // si.setHeaterEnable(HEATER_ON);    // Heater On

    si.setConversion(CONVERSION_START);       // Start a conversion
    // si.setConversion(CONVERSION_DONT);     // Do not start a conversion

    si.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = si.si_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from SI7005");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        cTemp = si.Measure_Temperature();
        fTemp = cTemp * 1.8 + 32;

        // Read and print out the Relative Humidity, convert it to %RH
        humidity = si.Measure_Humidity();

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.print("Relative Humidity Reading: ");
        Serial.print(humidity);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("SI7005 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
