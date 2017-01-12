/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CPS120
        This code is designed to work with the CPS120_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=CPS120_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <CPS120.h>

CPS120 cps;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    cps.getAddr_CPS120(CPS120_DEFAULT_ADDRESS);   // 0x28

    cps.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = cps.cps_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        cps.readRegister(address);
        float cTemp, fTemp, pressure, mbar, hg;

        Serial.println("Getting Readings from CPS120");
        Serial.println(" ");
        // Read and print out the Pressure, convert it to kPa
        pressure = cps.Measure_Pressure();
        mbar = pressure * 10;
        hg = pressure * 760.0 / 101.325;

        // Read and print out the temperature, then convert to C and F scales
        cTemp = cps.Measure_Temperature();
        fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(pressure);
        Serial.println(" kPa");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(mbar);
        Serial.println(" mbar");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(hg);
        Serial.println(" mmHg");
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
        Serial.println("CPS120 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
