/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM330
        This code is designed to work with the LSM330_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM330_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <LSM330.h>

LSM330 lsm;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lsm.getAddrX_LSM330(LSM330_DEFAULT_ADDRESS_ACCEL);          // 0x1E
    // lsm.getAddrX_LSM330(LSM330_ADDRESS_ACCEL_UPDATED);       // 0x1D

    lsm.getAddrG_LSM330(LSM330_DEFAULT_ADDRESS_GYRO);           // 0x6A
    // lsm.getAddrG_LSM330(LSM330_ADDRESS_GYRO_UPDATED);        // 0x6B

    // The Gyroscope's Output Data Rate and Bandwidth Selection and Full-Scale Selection,
    // Acceleration Data Rate Selection and Acceleration Full-Scale Selection,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Gyroscope:
    // Power-Down Mode Enable, Gyroscope X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection, Full-Scale Selection,
    // SPI Serial Interface Mode Selection
    // Accelerometer:
    // Block Data Update for Acceleration Data
    // Acceleration X, Y, Z Axes Enable
    // Acceleration Anti-Alias Filter Bandwidth
    // Self-Test Mode Configuration, SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lsm.setAccelDataRate(ACCEL_DATARATE_3_125HZ);        // AODR (Hz): 3.125
    // lsm.setAccelDataRate(ACCEL_DATARATE_POWERDOWN);   // Power Down Mode
    // lsm.setAccelDataRate(ACCEL_DATARATE_6_25HZ);      // AODR (Hz): 6.25
    // lsm.setAccelDataRate(ACCEL_DATARATE_12_5HZ);      // AODR (Hz): 12.5
    // lsm.setAccelDataRate(ACCEL_DATARATE_25HZ);        // AODR (Hz): 25
    // lsm.setAccelDataRate(ACCEL_DATARATE_50HZ);        // AODR (Hz): 50
    // lsm.setAccelDataRate(ACCEL_DATARATE_100HZ);       // AODR (Hz): 100
    // lsm.setAccelDataRate(ACCEL_DATARATE_200HZ);       // AODR (Hz): 200
    // lsm.setAccelDataRate(ACCEL_DATARATE_400HZ);       // AODR (Hz): 400
    // lsm.setAccelDataRate(ACCEL_DATARATE_800HZ);       // AODR (Hz): 800
    // lsm.setAccelDataRate(ACCEL_DATARATE_1600HZ);      // AODR (Hz): 1600

    lsm.setAccelRange(ACCEL_RANGE_16G);          // ±16 g
    // lsm.setAccelRange(ACCEL_RANGE_2G);        // ±2 g
    // lsm.setAccelRange(ACCEL_RANGE_4G);        // ±4 g
    // lsm.setAccelRange(ACCEL_RANGE_6G);        // ±6 g
    // lsm.setAccelRange(ACCEL_RANGE_8G);        // ±8 g

    lsm.setGyroDataRate(GYRO_DATARATE_95_12_5);      // ODR (Hz): 95, Cutoff: 12.5
    // lsm.setGyroDataRate(GYRO_DATARATE_95_25);     // ODR (Hz): 95, Cutoff: 25
    // lsm.setGyroDataRate(GYRO_DATARATE_190_12_5);  // ODR (Hz): 190, Cutoff: 12.5
    // lsm.setGyroDataRate(GYRO_DATARATE_190_25);    // ODR (Hz): 190, Cutoff: 25
    // lsm.setGyroDataRate(GYRO_DATARATE_190_50);    // ODR (Hz): 190, Cutoff: 50
    // lsm.setGyroDataRate(GYRO_DATARATE_190_70);    // ODR (Hz): 190, Cutoff: 70
    // lsm.setGyroDataRate(GYRO_DATARATE_380_20);    // ODR (Hz): 380, Cutoff: 20
    // lsm.setGyroDataRate(GYRO_DATARATE_380_25);    // ODR (Hz): 380, Cutoff: 25
    // lsm.setGyroDataRate(GYRO_DATARATE_380_50);    // ODR (Hz): 380, Cutoff: 50
    // lsm.setGyroDataRate(GYRO_DATARATE_380_100);   // ODR (Hz): 380, Cutoff: 100
    // lsm.setGyroDataRate(GYRO_DATARATE_760_30);    // ODR (Hz): 760, Cutoff: 30
    // lsm.setGyroDataRate(GYRO_DATARATE_760_35);    // ODR (Hz): 760, Cutoff: 35
    // lsm.setGyroDataRate(GYRO_DATARATE_760_50);    // ODR (Hz): 760, Cutoff: 50
    // lsm.setGyroDataRate(GYRO_DATARATE_760_100);   // ODR (Hz): 760, Cutoff: 100

    lsm.setGyroScale(GYRO_SCALE_2000DPS);            // 2000 dps
    // lsm.setGyroScale(GYRO_SCALE_250DPS);          // 250 dps
    // lsm.setGyroScale(GYRO_SCALE_500DPS);          // 500 dps

    lsm.begin();
    delay(1000);
}

void loop(void)
{
    byte errorX, errorG;
    int8_t addressX, addressG;

    // LSM330 Accelerometer Address
    addressX = lsm.lsm_i2cAddressX;

    // LSM330 Gyroscope Address
    addressG = lsm.lsm_i2cAddressG;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addressX);
    errorX = Wire.endTransmission();
    Wire.beginTransmission(addressG);
    errorG = Wire.endTransmission();
    if ((errorX == 0) && (errorG == 0))
    {
        Serial.println("Getting Linear Acceleration and Angular Rate Data Readings from LSM330");
        Serial.println(" ");
        // Set up the sensor for Accelerometer and Gyroscope
        lsm.setUpAccelerometer();
        lsm.setUpGyroscope();
        delay(500);
        // Display the results for Acceleration and Angular Rate Values
        lsm.Measure_Sensor();
        lsm.Measure_Accelerometer();
        lsm.Measure_Gyroscope();

        // Output Data to Screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(lsm.lsm_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(lsm.lsm_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(lsm.lsm_accelData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        delay(500);
        Serial.print("Rotation in X-Axis: ");
        Serial.println(lsm.lsm_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(lsm.lsm_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(lsm.lsm_gyroData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("LSM330 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
