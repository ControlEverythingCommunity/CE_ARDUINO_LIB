/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KXTF9-4100
        This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#include <Wire.h>
#include <KXTF9_4100.h>

KXTF9_4100 kxtf;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    kxtf.getAddr_KXTF9_4100(KXTF9_4100_DEFAULT_ADDRESS);        // 0x0F

    // The Acceleration Range of the Accelerometer Outputs
    // can be changed via the following function:

    /*
    // Also some more parameters can be changed using the following functions
    // Operating and Performance Mode of the KXTF9
    // Reporting of the Availability of New Acceleration Data on the Interrupt
    // Directional TapTM, Wake Up (Motion Detect) and Tilt Position Function
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    kxtf.setAccelRange(ACCEL_RANGE_8G);                  // +/-8 g
    // kxtf.setAccelRange(ACCEL_RANGE_4G);               // +/-4 g
    // kxtf.setAccelRange(ACCEL_RANGE_2G);               // +/-2 g

    kxtf.begin();

    // Set up the sensor for Accelerometer
    kxtf.setUpAccelerometer();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // KXTF9_4100 Accelerometer Address
    address = kxtf.kxtf_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Data Readings from KXTF9-4100");
        Serial.println(" ");
        // Display the results for Acceleration
        kxtf.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(kxtf.kxtf_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(kxtf.kxtf_accelData.Y);
        Serial.print("Acceleration in Z-Axis: ");
        Serial.println(kxtf.kxtf_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("KXTF9-4100 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
