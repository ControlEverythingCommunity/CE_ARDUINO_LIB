/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34725
        This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34725_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TCS34725.h>

TCS34725 tcs;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tcs.getAddr_TCS34725(TCS34725_DEFAULT_ADDRESS);           // 0x29

    // The Digital Color Sensor
    // Internal Integration Time of the Color Channel ADC’s in 2.4 ms Increments,
    // Wait Time and Color Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Digital Color Sensor:
    // Color Interrupt, Wait, Color and Power Enable,
    // Color Gain Level, Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    // tcs.setATime(ATIME_2_4MS);               // Cycles: 1, Time: 2.4 ms Max Count: 1024
    // tcs.setATime(ATIME_24MS);                // Cycles: 10, Time: 24 ms Max Count: 10240
    // tcs.setATime(ATIME_101MS);               // Cycles: 42, Time: 101 ms Max Count: 43008
    // tcs.setATime(ATIME_154MS);               // Cycles: 64, Time: 154 ms Max Count: 65535
    tcs.setATime(ATIME_700MS);                  // Cycles: 256, Time: 700 ms Max Count: 65535

    tcs.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.4 ms Time (WLONG = 1):  0.029 sec
    // tcs.setWTime(WTIME_85);                  // Wait Time: 74, Time (WLONG = 0): 204 ms Time (WLONG = 1):  2.45 sec
    // tcs.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 614 ms Time (WLONG = 1):  7.4 sec

    tcs.setColorGain(AGAIN_1X);                 // Color Gain: 1X
    // tcs.setColorGain(AGAIN_4X);              // Color Gain: 4X
    // tcs.setColorGain(AGAIN_16X);             // Color Gain: 16X
    // tcs.setColorGain(AGAIN_60X);             // Color Gain: 60X

    tcs.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TCS34725 Digital Color Sensor Address
    address = tcs.tcs_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Color Data Readings from TCS34725");
        Serial.println(" ");
        // Set up for Digital Color Sensor
        tcs.setUpSensor();
        delay(500);

        // Display the results for Color Values
        tcs.Measure_Color();

        // Output Data to Screen
        Serial.print("Clear Data Illuminance: ");
        Serial.print(tcs.tcs_colorData.C);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.R);
        Serial.println(" lx");
        Serial.print("Green Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.G);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(tcs.tcs_colorData.B);
        Serial.println(" lx");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TCS34725 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
