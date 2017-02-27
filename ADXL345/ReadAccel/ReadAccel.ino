/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADXL345
        This code is designed to work with the ADXL345_I2CADC I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-accelerometer-13-bit?variant=25687608267#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    adxl.getAddr_ADXL345(ADXL345_DEFAULT_ADDRESS_ACCEL);          // 0x1D
    // adxl.getAddr_ADXL345(ADXL345_ADDRESS_ACCEL_UPDATED);       // 0x53

    // Acceleration Bandwidth and Output Data Rate and Full-Scale Selection,
    // can be changed via the following functions

    adxl.setAccelDataRate(ADXL345_DATARATE_3200HZ);         // Bandwidth: 1600Hz,   IDD: 140µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_1600HZ);      // Bandwidth: 800Hz,    IDD: 90µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_800HZ);       // Bandwidth: 400Hz,    IDD: 140µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_400HZ);       // Bandwidth: 200Hz,    IDD: 140µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_200HZ);       // Bandwidth: 100Hz,    IDD: 140µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_100HZ);       // Bandwidth: 50Hz,     IDD: 140µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_50HZ);        // Bandwidth: 25Hz,     IDD: 90µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_25HZ);        // Bandwidth: 12.5Hz,   IDD: 60µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_12_5HZ);      // Bandwidth: 6.25Hz,   IDD: 50µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_6_25HZ);      // Bandwidth: 3.13Hz,   IDD: 45µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_3_13HZ);      // Bandwidth: 1.56Hz,   IDD: 40µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_1_56HZ);      // Bandwidth: 0.78Hz,   IDD: 34µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_0_78HZ);      // Bandwidth: 0.39Hz,   IDD: 23µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_0_39HZ);      // Bandwidth: 0.20Hz,   IDD: 23µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_0_20HZ);      // Bandwidth: 0.10Hz,   IDD: 23µA
    // adxl.setAccelDataRate(ADXL345_DATARATE_0_10HZ);      // Bandwidth: 0.05Hz,   IDD: 23µA

    adxl.setAccelRange(ADXL345_RANGE_16G);                  // ±16 g
    // adxl.setAccelRange(ADXL345_RANGE_2G);                // ±2 g
    // adxl.setAccelRange(ADXL345_RANGE_4G);                // ±4 g
    // adxl.setAccelRange(ADXL345_RANGE_8G);                // ±8 g

    adxl.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // ADXL345 Accelerometer Address
    address = adxl.adxl_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Linear Acceleration Data Readings from ADXL345");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        adxl.setUpAccelerometer();
        delay(500);
        // Display the results for Acceleration Values
        adxl.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(adxl.adxl_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(adxl.adxl_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(adxl.adxl_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("ADXL345 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
