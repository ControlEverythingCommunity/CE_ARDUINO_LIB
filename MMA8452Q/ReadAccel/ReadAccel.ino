/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA8452Q
        This code is designed to work with the MMA8452Q_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <MMA8452Q.h>

MMA8452Q mma;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mma.getAddr_MMA8452Q(MMA8452Q_DEFAULT_ADDRESS_ACCEL);        // 0x1C
    // mma.getAddr_MMA8452Q(MMA8452Q_ADDRESS_ACCEL_UPDATED);        // 0x1D

    // The Accelerometer's Output Buffer Data Format Full Scale and Data Rate Selection
    // can be changed via the following function:

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Output Data High-Pass Filtered, Auto-Wake Sample Frequency When the Device is in Sleep Mode
    // Reduced Noise Reduced Maximum Range Mode, Fast-Read Mode, Fast-Read Mode
    // Self-Test Mode, Software Reset Mode, Sleep Mode Power Scheme Selection
    // Auto-Sleep Enable andActive Mode Power Scheme Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    mma.setAccelRange(ACCEL_RANGE_2G);                  // 2 g
    // mma.setAccelRange(ACCEL_RANGE_4G);               // 4 g
    // mma.setAccelRange(ACCEL_RANGE_8G);               // 8 g

    mma.setAccelBandwidth(ACCEL_BANDWIDTH_800HZ);       // ODR: 800 Hz, Period: 1.25 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_400HZ);    // ODR: 400 Hz, Period: 2.5 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_200HZ);    // ODR: 200 Hz, Period: 5 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_100HZ);    // ODR: 100 Hz, Period: 10 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_50HZ);     // ODR: 50 Hz, Period: 20 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_12_5HZ);   // ODR: 12.5 Hz, Period: 80 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_6_25HZ);   // ODR: 6.25 Hz, Period: 160 ms
    // mma.setAccelBandwidth(ACCEL_BANDWIDTH_1_56HZ);   // ODR: 1.56 Hz, Period: 740 ms



    mma.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MMA8452Q Accelerometer Address
    address = mma.mma_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Data Readings from MMA8452Q");
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
        Serial.println("MMA8452Q Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
