/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2561
        This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TSL2561.h>

TSL2561 tsl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tsl.getAddr_TSL2561(TSL2561_DEFAULT_ADDRESS);           // 0x39
    // tsl.getAddr_TSL2561(TSL2561_DEFAULT_LOW);            // 0x29
    // tsl.getAddr_TSL2561(TSL2561_DEFAULT_HIGH);           // 0x49

    // The Light-to-Digital Converter
    // Gain Between Low Gain and High Gain Modes, Integrate Time
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Light-to-Digital Converter:
    // INTR Control Select, Interrupt Persistence and Power ON.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tsl.setGain(GAIN_16X);                                  // 16X
    // tsl.setGain(GAIN_1X);                                // 1X

    tsl.setIntegTime(INTEGRATIONTIME_13_7MS);               // 13.7 ms
    // tsl.setIntegTime(INTEG_PARAM_INTTIME_101MS);         // 101 ms
    // tsl.setIntegTime(INTEG_PARAM_INTTIME_402MS);         // 402 ms

    tsl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TSL2561 Light-to-Digital Converter Address
    address = tsl.tsl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Light Data Readings from TSL2561");
        Serial.println(" ");
        // Set up for Light-to-Digital Converter
        tsl.setUpALS();
        delay(500);

        // Display the results for Light Values
        tsl.getLuminosity();

        // Output Data to Screen
        Serial.print("Full Spectrum Illuminance: ");
        Serial.println(tsl.tsl_alsData.F);
        Serial.print("InfraRed Illuminance: ");
        Serial.println(tsl.tsl_alsData.IR);
        Serial.print("Visible Illuminance: ");
        Serial.println(tsl.tsl_alsData.V);
        Serial.print("Approximate Illuminance: ");
        Serial.print(tsl.Measure_ALS(tsl.tsl_alsData.F, tsl.tsl_alsData.IR));
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TSL2561 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
