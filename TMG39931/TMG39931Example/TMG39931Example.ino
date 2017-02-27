/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMG39931
        This code is designed to work with the TMG39931_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/light-sensor-gesture-color-als-and-proximity-sensor
*/
/**************************************************************************/

#include <Wire.h>
#include <TMG39931.h>

TMG39931 tmg;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmg.getAddr_TMG39931(TMG39931_DEFAULT_ADDRESS);           // 0x39

    // The Light Sensor Gesture, Color, ALS and Proximity Sensor
    // Internal Integration Time of the Color Channel ADC’s in 2.78 ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Gain
    // Color Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Light Sensor Gesture, Color, ALS and Proximity Sensor:
    // Pattern Burst Enable, Gesture Enable, Proximity and Color Interrupt
    // Wait, Proximity, Color and Power Enable,
    // Color Gain Level, Wait Long
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tmg.setATime(ATIME_712MS);                  // Cycles: 256, Time: 712 ms Max Count: 65535
    // tmg.setATime(ATIME_2_78MS);              // Cycles: 1, Time: 2.78 ms Max Count: 1025
    // tmg.setATime(ATIME_27_8MS);              // Cycles: 10, Time: 27.8 ms Max Count: 10241
    // tmg.setATime(ATIME_103MS);               // Cycles: 42, Time: 103 ms Max Count: 37888
    // tmg.setATime(ATIME_178MS);               // Cycles: 64, Time: 178 ms Max Count: 65535

    tmg.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.78 ms Time (WLONG = 1):  0.03 sec
    // tmg.setWTime(WTIME_85);                  // Wait Time: 74, Time (WLONG = 0): 236 ms Time (WLONG = 1):  2.78 sec
    // tmg.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 712 ms Time (WLONG = 1):  8.34 sec

    tmg.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100%
    // tmg.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50%
    // tmg.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25%
    // tmg.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5%

    tmg.setProximityGain(PGAIN_1X);             // Proximity Gain: 1X
    // tmg.setProximityGain(PGAIN_2X);          // Proximity Gain: 2X
    // tmg.setProximityGain(PGAIN_4X);          // Proximity Gain: 4X
    // tmg.setProximityGain(PGAIN_8X);          // Proximity Gain: 8X

    tmg.setColorGain(AGAIN_1X);                 // Color Gain: 1X
    // tmg.setColorGain(AGAIN_4X);              // Color Gain: 4X
    // tmg.setColorGain(AGAIN_16X);             // Color Gain: 16X
    // tmg.setColorGain(AGAIN_64X);             // Color Gain: 64X

    tmg.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TMG39931 Light Sensor Gesture, Color, ALS and Proximity Sensor Address
    address = tmg.tmg_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Color and Proximity Data Readings from TMG39931");
        Serial.println(" ");
        // Set up for Light Sensor Gesture, Color, ALS and Proximity Sensor
        tmg.setUpColorProximity();
        delay(500);

        // Display the results for Color and Proximity Values
        tmg.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Clear Data Illuminance: ");
        Serial.print(tmg.tmg_colorproxData.C);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(tmg.tmg_colorproxData.R);
        Serial.println(" lx");
        Serial.print("Green Color Illuminance: ");
        Serial.print(tmg.tmg_colorproxData.G);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(tmg.tmg_colorproxData.B);
        Serial.println(" lx");
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tmg.tmg_colorproxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TMG39931 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
