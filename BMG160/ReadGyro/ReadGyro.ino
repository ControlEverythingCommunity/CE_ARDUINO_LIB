/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMG160
        This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-Gyroscope
*/
/**************************************************************************/

#include <Wire.h>
#include <BMG160.h>

BMG160 bmg;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    bmg.getAddr_BMG160(BMG160_DEFAULT_ADDRESS_GYRO);        // 0x68
    // bmg.getAddr_BMG160(BMG160_ADDRESS_GYRO_UPDATED);     // 0x69

    // The Angular Rate Range Measurement and Selection of the Angular Rate Data Filter Bandwidth
    // can be changed via the following function:

    bmg.setGyroRange(GYRO_RANGE_2000);                      // ± 2000 °/s
    // bmg.setGyroRange(GYRO_RANGE_1000);                   // ± 1000 °/s
    // bmg.setGyroRange(GYRO_RANGE_500);                    // ± 500 °/s
    // bmg.setGyroRange(GYRO_RANGE_250);                    // ± 250 °/s
    // bmg.setGyroRange(GYRO_RANGE_125);                    // ± 125 °/s

    bmg.setGyroBandwidth(GYRO_BANDWIDTH_200_23HZ);       // ODR: 200 Hz, Filter Bandwidth: 23 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_2000_UNFILTERED);   // ODR: 2000 Hz, Filter Bandwidth: Unfiltered (523 Hz)
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_2000_230HZ);     // ODR: 2000 Hz, Filter Bandwidth: 230 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_1000_116HZ);     // ODR: 1000 Hz, Filter Bandwidth: 116 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_400_47HZ);       // ODR: 400 Hz, Filter Bandwidth: 47 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_100_12HZ);       // ODR: 100 Hz, Filter Bandwidth: 12 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_200_64HZ);       // ODR: 200 Hz, Filter Bandwidth: 64 Hz
    // bmg.setGyroBandwidth(GYRO_BANDWIDTH_100_32HZ);       // ODR: 100 Hz, Filter Bandwidth: 32 Hz

    bmg.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // BMG160 Sensor Address
    address = bmg.bmg_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Gyroscope Data Readings from BMG160");
        Serial.println(" ");
        // Set up the sensor for Gyroscope
        bmg.setUpSensor();
        delay(500);

        // Display the results for Gyroscope
        bmg.Measure_Gyroscope();

        // Output Data to Screen
        Serial.print("Rotation in X-Axis: ");
        Serial.println(bmg.bmg_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(bmg.bmg_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(bmg.bmg_gyroData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(500);
    }
    else
    {
        Serial.println("BMG160 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
