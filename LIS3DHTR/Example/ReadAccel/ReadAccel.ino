/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3DHTR
        This code is designed to work with the LIS3DHTR_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS3DHTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <LIS3DHTR.h>

LIS3DHTR lis;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lis.getAddr_LIS3DHTR(LIS3DHTR_DEFAULT_ADDRESS);        // 0x18
    // lis.getAddr_LIS3DHTR(LIS3DHTR_ADDRESS_UPDATED);     // 0x19

    // The Acceleration Data Rate Selection and Acceleration Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Power-Down Mode Enable, Accelerometer Power Mode, Accelerometer X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection, High Resolution Output Mode
    // Self-Test Enable and SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lis.setAccelDataRate(ACCEL_DATARATE_1HZ);           // AODR (Hz): 1
    // lis.setAccelDataRate(ACCEL_DATARATE_POWERDOWN);  // Power Down Mode
    // lis.setAccelDataRate(ACCEL_DATARATE_10HZ);       // AODR (Hz): 6.25
    // lis.setAccelDataRate(ACCEL_DATARATE_25HZ);       // AODR (Hz): 25
    // lis.setAccelDataRate(ACCEL_DATARATE_50HZ);       // AODR (Hz): 50
    // lis.setAccelDataRate(ACCEL_DATARATE_100HZ);      // AODR (Hz): 100
    // lis.setAccelDataRate(ACCEL_DATARATE_200HZ);      // AODR (Hz): 200
    // lis.setAccelDataRate(ACCEL_DATARATE_400HZ);      // AODR (Hz): 400
    // lis.setAccelDataRate(ACCEL_DATARATE_1_6KHZ);     // AODR (Hz): 1.6 KHz
    // lis.setAccelDataRate(ACCEL_DATARATE_5KHZ);       // AODR (Hz): 5 KHz

    lis.setAccelRange(ACCEL_RANGE_16G);                 // ±16 G
    // lis.setAccelRange(ACCEL_RANGE_2G);               // ±2 G
    // lis.setAccelRange(ACCEL_RANGE_4G);               // ±4 G
    // lis.setAccelRange(ACCEL_RANGE_8G);               // ±8 G

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
        Serial.println("Getting Acceleration Readings from LIS3DHTR");
        Serial.println(" ");
        // Set up the sensor for ADC, Temperature and Accelerometer
        lis.setUpADCTemperature();
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
        Serial.println("LIS3DHTR Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
