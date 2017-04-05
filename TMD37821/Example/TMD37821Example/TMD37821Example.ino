/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD37821
        This code is designed to work with the TMD37821_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TMD37821_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <TMD37821.h>

TMD37821 tmd;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tmd.getAddr_TMD37821(TMD37821_DEFAULT_ADDRESS);           // 0x39

    // The Digital Color and Proximity Sensor
    //Internal Integration Time of the Color Channel ADC’s in 2.38 ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Output Selector
    // Color Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Digital Color and Proximity Sensor:
    // Proximity and Color Interrupt, Wait , Proximity, Color and Power Enable,
    // Color Gain Level, Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    // tmd.setATime(ATIME_2_38MS);                 // Cycles: 1, Time: 2.38 ms Max Count: 1024
    // tmd.setATime(ATIME_24MS);                // Cycles: 10, Time: 24 ms Max Count: 10240
    // tmd.setATime(ATIME_100MS);               // Cycles: 42, Time: 100 ms Max Count: 43008
    // tmd.setATime(ATIME_152MS);               // Cycles: 64, Time: 152 ms Max Count: 65535
    tmd.setATime(ATIME_609MS);               // Cycles: 256, Time: 609 ms Max Count: 65535

    tmd.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.38 ms Time (WLONG = 1):  0.03 sec
    // tmd.setWTime(WTIME_85);                  // Wait Time: 74, Time (WLONG = 0): 202 ms Time (WLONG = 1):  2.43 sec
    // tmd.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 609 ms Time (WLONG = 1):  7.31 sec

    tmd.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100%, LED Strength – PDL=1: 11.1%
    // tmd.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50%, LED Strength – PDL=1: 5.6%
    // tmd.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25%, LED Strength – PDL=1: 2.8%
    // tmd.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5%, LED Strength – PDL=1: 1.4%

    tmd.setProximityOutput(PSAT_0);             // PDATA Output Regardless of Ambient Light Level
    // tmd.setProximityOutput(PSAT_1);          // PDATA Output Equal to Dark Current Value if Saturated

    tmd.setColorGain(AGAIN_1X);                 // Color Gain: 1X
    // tmd.setColorGain(AGAIN_4X);              // Color Gain: 4X
    // tmd.setColorGain(AGAIN_16X);             // Color Gain: 16X
    // tmd.setColorGain(AGAIN_60X);             // Color Gain: 60X

    tmd.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TMD37821 Digital Color and Proximity Sensor Address
    address = tmd.tmd_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Color and Proximity Data Readings from TMD37821");
        Serial.println(" ");
        // Set up for Digital Color and Proximity Sensor
        tmd.setUpColorProximity();
        delay(500);

        // Display the results for Color and Proximity Values
        tmd.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Clear Data Illuminance: ");
        Serial.print(tmd.tmd_colorproxData.C);
        Serial.println(" lx");
        Serial.print("Red Color Illuminance: ");
        Serial.print(tmd.tmd_colorproxData.R);
        Serial.println(" lx");
        Serial.print("Green Color Illuminance: ");
        Serial.print(tmd.tmd_colorproxData.G);
        Serial.println(" lx");
        Serial.print("Blue Color Illuminance: ");
        Serial.print(tmd.tmd_colorproxData.B);
        Serial.println(" lx");
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tmd.tmd_colorproxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TMD37821 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
