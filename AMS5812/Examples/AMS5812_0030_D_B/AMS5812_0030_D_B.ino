/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AMS5812_0030-D-B
        This code is designed to work with the AMS5812_I2CS_0030-D-B I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Pressure?sku=AMS5812_I2CS_0030-D-B#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <AMS5812.h>

AMS5812 ams;
const float AMS5812_0030_D_B_P_MIN = -3.0;        // minimum pressure, PSI
const float AMS5812_0030_D_B_P_MAX = 3.0;         // maximum pressure, PSI
const float AMS5812_0030_D_B_T_MIN = -25.0;       // minimum temperature, C
const float AMS5812_0030_D_B_T_MAX = 85.0;        // maximum temperature, C

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ams.getAddr_AMS5812(AMS5812_DEFAULT_ADDRESS);   // 0x78

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
        ams.Measure_PressureAndTemperature(AMS5812_0030_D_B_P_MIN, AMS5812_0030_D_B_P_MAX, AMS5812_0030_D_B_T_MIN, AMS5812_0030_D_B_T_MAX);
        float cTemp, fTemp, pressure, mbar, kPa, hg;

        Serial.println("Getting Readings from AMS5812_0030-D-B");
        Serial.println(" ");
        // Read and print out the Pressure, convert it to PSI, mbar, kPa, mmHg
        pressure = ams.getPressure();
        mbar = pressure * 68.9476;
        kPa = pressure / 10;
        hg = pressure * 76.0 / 101.325;

        // Read and print out the temperature, then convert to C and F scales
        cTemp = ams.getTemperature();
        fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(pressure);
        Serial.println(" PSI");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(mbar);
        Serial.println(" mbar");
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
        Serial.println("AMS5812_0030-D-B Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
