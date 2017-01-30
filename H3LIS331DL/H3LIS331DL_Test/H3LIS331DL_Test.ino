/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        H3LIS331DL
        This code is designed to work with the H3LIS331DL_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=H3LIS331DL_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <H3LIS331DL.h>

H3LIS331DL h3lis;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    h3lis.getAddr_H3LIS331DL(H3LIS331DL_DEFAULT_ADDRESS);        // 0x18
    // h3lis.getAddr_H3LIS331DL(H3LIS331DL_ADDRESS_UPDATED);     // 0x19

    // The Acceleration Data Rate Selection and Acceleration Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // ADC and Temperature Enable
    // Accelerometer:
    // Power-Down Mode Enable, Accelerometer Power Mode, Accelerometer X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection and SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    h3lis.setAccelDataRate(ACCEL_DATARATE_50HZ);                // AODR (Hz): 50
    // h3lis.setAccelDataRate(ACCEL_DATARATE_100HZ);            // AODR (Hz): 100
    // h3lis.setAccelDataRate(ACCEL_DATARATE_400HZ);            // AODR (Hz): 400
    // h3lis.setAccelDataRate(ACCEL_DATARATE_1000HZ);           // AODR (Hz): 1000

    h3lis.setAccelRange(ACCEL_RANGE_100G);                      // ±100 G
    // h3lis.setAccelRange(ACCEL_RANGE_200G);                   // ±200 G
    // h3lis.setAccelRange(ACCEL_RANGE_400G);                   // ±400 G

    h3lis.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = h3lis.h3lis_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Readings from H3LIS331DL");
        Serial.println(" ");
        // Display the results for Linear Acceleration values
        h3lis.Measure_Accelerometer();

        // Output data to screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(h3lis.h3lis_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(h3lis.h3lis_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(h3lis.h3lis_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("H3LIS331DL Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
