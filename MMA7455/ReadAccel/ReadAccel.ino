/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7455
        This code is designed to work with the MMA7455_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <MMA7455.h>

MMA7455 mma;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mma.getAddr_MMA7455(MMA7455_DEFAULT_ADDRESS_ACCEL);        // 0x1D

    // The Accelerometer's Measurement Range can be changed via the following function:

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Data Ready Status, SPI Serial Interface Mode Selection, Self-Test Mode
    // Operating Mode, Digital Filter Band Width, Threshold Value
    // Acceleration X, Y and Z-Axes Enable and Interrupt Settings.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    // mma.setAccelRange(ACCEL_RANGE_8G);                 // 8 g
    // mma.setAccelRange(ACCEL_RANGE_4G);               // 4 g
    mma.setAccelRange(ACCEL_RANGE_2G);               // 2 g

    mma.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MMA7455 Accelerometer Address
    address = mma.mma_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Data Readings from MMA7455");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        mma.setUpAccelerometer();
        delay(500);

        // Display the results for Acceleration
        mma.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(mma.mma_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(mma.mma_accelData.Y);
        Serial.print("Acceleration in Z-Axis: ");
        Serial.println(mma.mma_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("MMA7455 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
