/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPU6000
        This code is designed to work with the MPU6000_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <MPU6000.h>

MPU6000 mpu;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mpu.getAddr_MPU6000(MPU6000_DEFAULT_ADDRESS_ACCELGYRO);             // 0x68
    // mpu.getAddr_MPU6000(MPU6000_ADDRESS_ACCELGYRO_UPDATED);          // 0x69

    // The Full Scale Range of the Gyroscope and Accelerometer Outputs
    // Clock Source of the Device
    // can be changed via the following function:

    /*
    // Also some more parameters can be changed using the following functions
    // Resets All Internal Registers, Sleep Mode
    // Cycle Between Sleep Mode and Waking Up to Take Sample Data
    // Disables Cycle Mode and Temperature Sensor
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    mpu.setGyroRange(GYRO_RANGE_2000);                  // ± 2000 °/s
    // mpu.setGyroRange(GYRO_RANGE_250);                // ± 250 °/s
    // mpu.setGyroRange(GYRO_RANGE_500);                // ± 5000 °/s
    // mpu.setGyroRange(GYRO_RANGE_1000);               // ± 1000 °/s

    mpu.setAccelRange(ACCEL_RANGE_16G);                 // 16 g
    // mpu.setAccelRange(ACCEL_RANGE_2G);               // 2 g
    // mpu.setAccelRange(ACCEL_RANGE_4G);               // 4 g
    // mpu.setAccelRange(ACCEL_RANGE_8G);               // 8 g

    mpu.setClockSource(CLOCK_SOURCE_PLL_X_REF);         // PLL with X-Axis Gyroscope Reference
    // mpu.setClockSource(CLOCK_SOURCE_INT_8MHZ_OS);    // Internal 8MHz Oscillator
    // mpu.setClockSource(CLOCK_SOURCE_PLL_Y_REF);      // PLL with Y-Axis Gyroscope Reference
    // mpu.setClockSource(CLOCK_SOURCE_PLL_Z_REF);      // PLL with Z-Axis Gyroscope Reference
    // mpu.setClockSource(CLOCK_SOURCE_EXT_32_768KHZ);  // PLL with External 32.768kHz Reference
    // mpu.setClockSource(CLOCK_SOURCE_EXT_19_2MHZ);    // PLL with External 19.2MHz Reference
    // mpu.setClockSource(CLOCK_SOURCE_STOP);           // Stops the Clock and Keeps the Timing Generator in Reset

    mpu.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MPU6000 Sensor Address
    address = mpu.mpu_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Linear Acceleration and Angular Rate Data Readings from MPU6000");
        Serial.println(" ");
        // Set up the sensor for Accelerometer and Gyroscope
        mpu.setUpSensor();
        delay(500);

        // Display the results for Acceleration and Angular Rate
        mpu.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(mpu.mpu_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(mpu.mpu_accelData.Y);
        Serial.print("Acceleration in Z-Axis: ");
        Serial.println(mpu.mpu_accelData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        Serial.print("Rotation in X-Axis: ");
        Serial.println(mpu.mpu_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(mpu.mpu_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(mpu.mpu_gyroData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        Serial.println("Temperature Readings");
        Serial.println(" ");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(mpu.mpu_tempData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(((mpu.mpu_tempData.T * 1.8) + 32));
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("MPU6000 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
