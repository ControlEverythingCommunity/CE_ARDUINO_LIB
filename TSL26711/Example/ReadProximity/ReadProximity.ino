/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL26711
        This code is designed to work with the TSL26711_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <TSL26711.h>

TSL26711 tsl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    tsl.getAddr_TSL26711(TSL26711_DEFAULT_ADDRESS);           // 0x39

    // The Infrared Digital Proximity Detector
    //Internal Integration Time of the Proximity ADC’s in 2.73ms Increments,
    // Wait Time, Proximity LED Drive Strength, Proximity Diode Selector
    // Proximity Gain
    // can be changed via the following functions

    /*
    // Also, some more parameters can be changed using the following functions
    // Infrared Digital Proximity Detector:
    // Sleep After, Proximity Interrupt, Wait , Proximity and Power Enable,
    // Wait Long.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    tsl.setPTime(PTIME_2_72MS);                 // Cycles: 1, Time: 2.72 ms Max Count: 1023

    tsl.setWTime(WTIME_1);                      // Wait Time: 1, Time (WLONG = 0): 2.73 ms Time (WLONG = 1):  0.032 sec
    // tsl.setWTime(WTIME_74);                  // Wait Time: 74, Time (WLONG = 0): 201 ms Time (WLONG = 1):  2.4 sec
    // tsl.setWTime(WTIME_256);                 // Wait Time: 256, Time (WLONG = 0): 696 ms Time (WLONG = 1):  8.3 sec

    tsl.setProximityDrive(PDRIVE_100);          // LED Strength – PDL=0: 100mA
    // tsl.setProximityDrive(PDRIVE_50);        // LED Strength – PDL=0: 50mA
    // tsl.setProximityDrive(PDRIVE_25);        // LED Strength – PDL=0: 25mA
    // tsl.setProximityDrive(PDRIVE_12_5);      // LED Strength – PDL=0: 12.5mA

    tsl.setProximityDiode(PDIODE_CH1);          // Proximity Uses the CH1 Diode
    // tsl.setProximityDiode(PDIODE_CH0);       // Proximity Uses the CH0 Diode
    // tsl.setProximityDiode(PDIODE_NONE);      // Proximity Uses the Neither Diode

    tsl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // TSL26711 Infrared Digital Proximity Detector Address
    address = tsl.tsl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Proximity Data Readings from TSL26711");
        Serial.println(" ");
        // Set up for Infrared Digital Proximity Detector
        tsl.setUpProximity();
        delay(500);

        // Display the results for Proximity Values
        tsl.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Proximity of the Sensor: ");
        Serial.println(tsl.tsl_proxData.P);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("TSL26711 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
