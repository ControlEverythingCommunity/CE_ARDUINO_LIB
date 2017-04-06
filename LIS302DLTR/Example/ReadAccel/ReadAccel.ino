/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS302DLTR
        This code is designed to work with the LIS302DLTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS302DLTR_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <LIS302DLTR.h>

LIS302DLTR lis;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lis.getAddr_LIS302DLTR(LIS302DLTR_DEFAULT_ADDRESS);        // 0x1C
    // lis.getAddr_LIS302DLTR(LIS302DLTR_ADDRESS_UPDATED);     // 0x1D

    // The Acceleration Data Rate Selection and Acceleration Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Power-Down Mode Enable, Self-Test P, M Enable, Accelerometer X, Y, Z Axes Enable
    // SPI Serial Interface Mode Selection and Reboot Memory Content
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lis.setAccelDataRate(ACCEL_DATARATE_100HZ);             // AODR (Hz): 100
    // lis.setAccelDataRate(ACCEL_DATARATE_400HZ);          // AODR (Hz): 400

    lis.setAccelRange(ACCEL_RANGE_2G);                      // ±2 G
    // lis.setAccelRange(ACCEL_RANGE_8G);                   // ±8 G

    lis.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = lis.lis_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Readings from LIS302DLTR");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        lis.setUpAccelerometer();
        delay(500);
        // Display the results for Linear Acceleration values
        lis.Measure_Accelerometer();

        // Output data to screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(lis.lis_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(lis.lis_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(lis.lis_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("LIS302DLTR Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
