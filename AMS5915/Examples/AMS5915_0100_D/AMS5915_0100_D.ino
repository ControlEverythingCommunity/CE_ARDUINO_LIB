/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AMS5915_0100-D
        This code is designed to work with the AMS5915_I2CS_0100-D I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Pressure?sku=AMS5915_I2CS_0100-D#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <AMS5915.h>

AMS5915 ams;
const float AMS5915_0100_D_P_MIN = 0.0;     // minimum pressure, millibar
const float AMS5915_0100_D_P_MAX = 100.0;   // maximum pressure, millibar

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ams.getAddr_AMS5915(AMS5915_DEFAULT_ADDRESS);   // 0x28

    ams.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ams.ams_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        ams.Measure_PressureAndTemperature(AMS5915_0100_D_P_MIN, AMS5915_0100_D_P_MAX);
        float cTemp, fTemp, pressure, kPa, hg;

        Serial.println("Getting Readings from AMS5915_0100-D");
        Serial.println(" ");
        // Read and print out the Pressure, convert it to mbar, PSI, kPa, mmHg
        pressure = ams.getPressure();
        PSI = pressure * 0.0145038;
        kPa = pressure / 10;
        hg = pressure * 76.0 / 101.325;

        // Read and print out the temperature, then convert to C and F scales
        cTemp = ams.getTemperature();
        fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(pressure);
        Serial.println(" mbar");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(PSI);
        Serial.println(" PSI");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(kPa);
        Serial.println(" kPa");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(hg);
        Serial.println(" mmHg");
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
        Serial.println("AMS5915_0100-D Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
