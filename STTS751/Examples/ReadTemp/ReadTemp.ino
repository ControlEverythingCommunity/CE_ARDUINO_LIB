/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        STTS751
        This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <STTS751.h>

STTS751 stts;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    stts.getAddr_STTS751(STTS751_DEFAULT_ADDRESS);         // 0x39

    // The MASK1, RUN/STOP, Conversion Resolution and Conversion Rate
    // can be changed via the following functions

    stts.setMask1(MASK1_ENABLED);           // EVENT is Enabled
    // stts.setMask1(MASK1_DISABLED);       // EVENT is Disabled

    stts.setMode(MODE_CONTIN);              // Device is running in continuous conversion mode
    // stts.setMode(MODE_STANDBY);          // Device is in standby mode drawing minimum power

    stts.setTres(TRES_12BIT);               // LSB Step Size (°C): 0.0625
    // stts.setTres(TRES_9BIT);             // LSB Step Size (°C): 0.5
    // stts.setTres(TRES_10BIT);            // LSB Step Size (°C): 0.25
    // stts.setTres(TRES_11BIT);            // LSB Step Size (°C): 0.125

    stts.setConvRate(CONVRATE_0_0625);      // Conversions Per Second: 0.0625
    // stts.setConvRate(CONVRATE_0_125);    // Conversions Per Second: 0.125
    // stts.setConvRate(CONVRATE_0_25);     // Conversions Per Second: 0.25
    // stts.setConvRate(CONVRATE_0_5);      // Conversions Per Second: 0.5
    // stts.setConvRate(CONVRATE_1);        // Conversions Per Second: 1
    // stts.setConvRate(CONVRATE_2);        // Conversions Per Second: 2
    // stts.setConvRate(CONVRATE_4);        // Conversions Per Second: 4
    // stts.setConvRate(CONVRATE_8);        // Conversions Per Second: 8
    // stts.setConvRate(CONVRATE_16);       // Conversions Per Second: 16
    // stts.setConvRate(CONVRATE_32);       // Conversions Per Second: 32

    stts.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = stts.stts_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t temp;

        Serial.println("Getting Temperature Readings from STTS751");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = stts.Measure_Temp();
        // 12-bit resolution
        temp = temp >> 4;
        float cTemp = temp * 0.0625;
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
        Serial.println("STTS751 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
