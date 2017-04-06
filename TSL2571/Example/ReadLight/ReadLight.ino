/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2571
        This code is designed to work with the TSL2571_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2571_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <TSL2571.h>

TSL2571 tsl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tsl.getAddr_TSL2571(TSL2571_DEFAULT_ADDRESS);           // 0x39

    // The Light-to-Digital Converter
    // Internal Integration Time of the ALS Channel ADC’s in 2.72ms Increments,
    // Wait Time, ALS Gain
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Light-to-Digital Converter:
    // ALS Interrupt, Wait, ALS and Power Enable,
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tsl.setATime(ATIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms Max Count: 1024
    // tsl.setATime(ATIME_27_2MS);              // Cycles: 10, Time: 27.2 ms Max Count: 10240
    // tsl.setATime(ATIME_101MS);               // Cycles: 37, Time: 101 ms Max Count: 37888
    // tsl.setATime(ATIME_174MS);               // Cycles: 64, Time: 174 ms Max Count: 65535
    // tsl.setATime(ATIME_696MS);               // Cycles: 256, Time: 696 ms Max Count: 65535

    tsl.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.72 ms Time (WLONG = 1):  0.032 sec
    // tsl.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 200 ms Time (WLONG = 1):  2.4 sec
    // tsl.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 700 ms Time (WLONG = 1):  8.3 sec

    tsl.setALSGain(AGAIN_1X);                   // ALS Gain: 1X
    // tsl.setALSGain(AGAIN_8X);                // ALS Gain: 8X
    // tsl.setALSGain(AGAIN_16X);               // ALS Gain: 16X
    // tsl.setALSGain(AGAIN_120X);              // ALS Gain: 120X

    tsl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TSL2571 Light-to-Digital Converter Address
    address = tsl.tsl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting ALS Data Readings from TSL2571");
        Serial.println(" ");
        // Set up for Light-to-Digital Converter
        tsl.setUpALS();
        delay(500);

        // Display the results for ALS Values
        tsl.Measure_ALS();

        // Output Data to Screen
        Serial.print("Ambient Light Illuminance: ");
        Serial.print(tsl.tsl_alsData.L);
        Serial.println(" lx");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TSL2571 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
