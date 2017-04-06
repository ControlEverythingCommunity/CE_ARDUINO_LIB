/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMA250
        This code is designed to work with the BMA250_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <BMA250.h>

BMA250 bma;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    bma.getAddr_BMA250(BMA250_DEFAULT_ADDRESS_ACCEL);             // 0x18
    // bma.getAddr_BMA250(BMA250_ADDRESS_ACCELGYRO_UPDATED);          // 0x69

    // The g-Range and Selection of the Bandwidth for the Accelerometer Data
    // can be changed via the following function:

    bma.setAccelRange(ACCEL_RANGE_2G);                     // 2 g
    // bma.setAccelRange(ACCEL_RANGE_4G);                   // 4 g
    // bma.setAccelRange(ACCEL_RANGE_8G);                   // 8 g
    // bma.setAccelRange(ACCEL_RANGE_16G);                  // 16 g

    bma.setAccelBandwidth(ACCEL_BANDWIDTH_7_81HZ);          // Bandwidth: 7.81 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_15_63HZ);      // Bandwidth: 15.63 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_31_25HZ);      // Bandwidth: 31.25 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_62_5HZ);       // Bandwidth: 62.5 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_BW_125HZ);     // Bandwidth: 125 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_BW_250HZ);     // Bandwidth: 250 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_BW_500HZ);     // Bandwidth: 500 Hz
    // bma.setAccelBandwidth(ACCEL_BANDWIDTH_BW_1000HZ);    // Bandwidth: 1000 Hz

    bma.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // BMA250 Sensor Address
    address = bma.bma_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Data Readings from BMA250");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        bma.setUpSensor();
        delay(500);

        // Display the results for Acceleration
        bma.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(bma.bma_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(bma.bma_accelData.Y);
        Serial.print("Acceleration in Z-Axis: ");
        Serial.println(bma.bma_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("BMA250 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
