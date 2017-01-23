/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TH02
        This code is designed to work with the TH02_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <TH02.h>

TH02 th;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    th.getAddr_TH02(TH02_DEFAULT_ADDRESS);           // 0x40

    // The Fast Enable, Heater Enable and Conversion Mode
    // can be changed via the following functions

    th.setFastEnable(FAST_ENABLE_35);        // 35 ms (typical)
    // th.setFastEnable(FAST_ENABLE_18);    // 18 ms (typical)

    th.setHeaterEnable(HEATER_OFF);         // Heater Off
    // th.setHeaterEnable(HEATER_ON);       // Heater On

    th.setConversion(CONVERSION_START);     // Start a conversion
    // th.setConversion(CONVERSION_DONT);   // Do not start a conversion

    th.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = th.th_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from TH02");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        cTemp = th.Measure_Temperature();
        fTemp = cTemp * 1.8 + 32;

        // Read and print out the Relative Humidity, convert it to %RH
        humidity = th.Measure_Humidity();

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
        Serial.println("TH02 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
