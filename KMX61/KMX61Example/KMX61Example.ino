/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KMX61
        This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <KMX61.h>

KMX61 kmx;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    kmx.getAddr_KMX61(KMX61_DEFAULT_ADDRESS);      // 0x0E

    // The Acceleration Full-Scale Selection,
    // Acceleration and Magnetic Data Rate Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // ASIC_AL to Control the Operating Mode of all of the Connected Devices
    // Controls the Operating Mode of the ASIC_AL’s Magnetometer and Accelerometer
    // Enables the Back to Sleep and Wake up Engine
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    kmx.setAccelRange(ACCEL_RANGE_2G);                      // ±2 g
    // kmx.setAccelRange(ACCEL_RANGE_4G);                   // ±4 g
    // kmx.setAccelRange(ACCEL_RANGE_8G);                   // ±8 g:12-bit
    // kmx.setAccelRange(ACCEL_RANGE_8G_14BIT);             // ±8 g: 14-bit

    kmx.setMagDataRate(MAG_DATA_RATE_12_5HZ);               // Output Data Rate (Hz): 12.5
    // kmx.setMagDataRate(MAG_DATA_RATE_25HZ);              // Output Data Rate (Hz): 25
    // kmx.setMagDataRate(MAG_DATA_RATE_50HZ);              // Output Data Rate (Hz): 50
    // kmx.setMagDataRate(MAG_DATA_RATE_100HZ);             // Output Data Rate (Hz): 100
    // kmx.setMagDataRate(MAG_DATA_RATE_200HZ);             // Output Data Rate (Hz): 200
    // kmx.setMagDataRate(MAG_DATA_RATE_400HZ);             // Output Data Rate (Hz): 800
    // kmx.setMagDataRate(MAG_DATA_RATE_1600HZ);            // Output Data Rate (Hz): 1600
    // kmx.setMagDataRate(MAG_DATA_RATE_0_781HZ);           // Output Data Rate (Hz): 0.781
    // kmx.setMagDataRate(MAG_DATA_RATE_1_583HZ);           // Output Data Rate (Hz): 1.583
    // kmx.setMagDataRate(MAG_DATA_RATE_3_125HZ);           // Output Data Rate (Hz): 3.125
    // kmx.setMagDataRate(MAG_DATA_RATE_6_25HZ);            // Output Data Rate (Hz): 6.25

    kmx.setAccelDataRate(ACCEL_DATA_RATE_12_5HZ);           // Output Data Rate (Hz): 12.5
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_25HZ);          // Output Data Rate (Hz): 25
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_50HZ);          // Output Data Rate (Hz): 50
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_100HZ);         // Output Data Rate (Hz): 100
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_200HZ);         // Output Data Rate (Hz): 200
    // kmx.setAccelDataRate(ACCEL_DATARATE_400HZ);          // Output Data Rate (Hz): 400
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_800HZ);         // Output Data Rate (Hz): 800
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_1600HZ);        // Output Data Rate (Hz): 1600
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_0_781HZ);       // Output Data Rate (Hz): 0.781
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_1_583HZ);       // Output Data Rate (Hz): 1.583
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_3_125HZ);       // Output Data Rate (Hz): 3.125
    // kmx.setAccelDataRate(ACCEL_DATA_RATE_6_25HZ);        // Output Data Rate (Hz): 6.25

    kmx.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // KMX61 Sensor Address
    address = kmx.kmx_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Linear Acceleration and Magnetic Field Data Readings from KMX61");
        Serial.println(" ");
        // Set up the sensor for Accelerometer, and Magnetometer
        kmx.setUpSensor();
        delay(500);
        // Display the results for Linear Acceleration and Magnetic Field Values
        kmx.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(kmx.kmx_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(kmx.kmx_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(kmx.kmx_accelData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        delay(500);
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(kmx.kmx_magData.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(kmx.kmx_magData.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(kmx.kmx_magData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("KMX61 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
