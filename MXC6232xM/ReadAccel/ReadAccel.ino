/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6232xM
        This code is designed to work with the MXC6232xM_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/low-power-2g-dual-axis-accelerometer?variant=25687720907#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <MXC6232xM.h>

MXC6232xM mxc;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mxc.getAddr_MXC6232xM(MXC6232xM_DEFAULT_ADDRESS_ACCEL);        // 0x10

    /*
    // Some parameters can be changed using the following functions
    // Accelerometer:
    // Temperature Out, BandGap Test, Self Test and Power Status.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    mxc.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MXC6232xM Accelerometer Address
    address = mxc.mxc_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Dynamic and Static Acceleration Data Readings from MXC6232xM");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        mxc.setUpAccelerometer();
        delay(500);

        // Display the results for Acceleration
        mxc.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(mxc.mxc_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(mxc.mxc_accelData.Y);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("MXC6232xM Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
