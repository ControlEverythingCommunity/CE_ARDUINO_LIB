/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM9DS0
        This code is designed to work with the LSM9DS0_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM9DS0_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <LSM9DS0.h>

LSM9DS0 lsm;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lsm.getAddrG_LSM9DS0(LSM9DS0_DEFAULT_ADDRESS_GYRO);             // 0x6A
    // lsm.getAddrG_LSM9DS0(LSM9DS0_ADDRESS_GYRO_UPDATED);          // 0x6B

    lsm.getAddrXM_LSM9DS0(LSM9DS0_DEFAULT_ADDRESS_ACCELMAG);        // 0x1E
    // lsm.getAddrXM_LSM9DS0(LSM9DS0_ADDRESS_ACCELMAG_UPDATED);     // 0x1D

    // The Gyroscope's Output Data Rate and Bandwidth Selection and Full-Scale Selection,
    // Acceleration Data Rate Selection and Acceleration Full-Scale Selection,
    // Magnetic Data Rate Selection and Magnetic Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Gyroscope:
    // Power-Down Mode Enable, Gyroscope X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection, Full-Scale Selection,
    // Self-Test Enable and SPI Serial Interface Mode Selection
    // Accelerometer and Magmetometer:
    // Block Data Update for Acceleration and Magnetic Data
    // Acceleration X, Y, Z Axes Enable
    // Acceleration Anti-Alias Filter Bandwidth
    // Self-Test Mode Configuration, SPI Serial Interface Mode Selection
    // Temperature Sensor Enable, Magnetic Resolution Selection
    // Latch Interrupt Request on INT2_SRC Register, Latch Interrupt Request on INT1_SRC Register
    // Magnetic Sensor Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

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
    // lsm.setGyroScale(GYRO_SCALE_245DPS);          // 245 dps
    // lsm.setGyroScale(GYRO_SCALE_500DPS);          // 500 dps

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

    lsm.setMagDataRate(MAG_DATARATE_3_125HZ);    // MODR (Hz): 3.125
    // lsm.setMagDataRate(MAG_DATARATE_6_25HZ);  // MODR (Hz): 6.25
    // lsm.setMagDataRate(MAG_DATARATE_12_5HZ);  // MODR (Hz): 12.5
    // lsm.setMagDataRate(MAG_DATARATE_25HZ);    // MODR (Hz): 25
    // lsm.setMagDataRate(MAG_DATARATE_50HZ);    // MODR (Hz): 50
    // lsm.setMagDataRate(MAG_DATARATE_100HZ);   // MODR (Hz): 100

    lsm.setMagGain(MAG_GAIN_12GAUSS);            // ±12 guass
    // lsm.setMagGain(MAG_GAIN_2GAUSS);          // ±2 guass
    // lsm.setMagGain(MAG_GAIN_4GAUSS);          // ±4 guass
    // lsm.setMagGain(MAG_GAIN_8GAUSS);          // ±8 guass

    lsm.begin();
    delay(1000);
}

void loop(void)
{
    byte errorG, errorXM;
    int8_t addressG, addressXM;

    // LSM9DS0 Gyroscope Address
    addressG = lsm.lsm_i2cAddressG;
    // LSM9DS0 Accelerometer/Magnetometer Address
    addressXM = lsm.lsm_i2cAddressXM;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addressG);
    errorG = Wire.endTransmission();
    Wire.beginTransmission(addressXM);
    errorXM = Wire.endTransmission();
    if ((errorG == 0) && (errorXM == 0))
    {
        Serial.println("Getting Angular Rate, Linear Acceleration, and Magnetic Field Data Readings from LSM9DS0");
        Serial.println(" ");
        // Set up the sensor for Gyroscope, Accelerometer, and Magnetometer
        lsm.setUpGyroscope();
        lsm.setUpAccelerometer();
        lsm.setUpMagnetometer();
        delay(500);
        // Display the results for Angular Rate, Acceleration, and Magnetic Field Values
        lsm.Measure_Sensor();
        lsm.Measure_Gyroscope();
        lsm.Measure_Accelerometer();
        lsm.Measure_Magnetometer();

        // Output Data to Screen
        Serial.print("Rotation in X-Axis: ");
        Serial.println(lsm.lsm_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(lsm.lsm_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(lsm.lsm_gyroData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        delay(500);
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
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(lsm.lsm_magData.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(lsm.lsm_magData.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(lsm.lsm_magData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        Serial.println("LSM9DS0 Temperature Readings");
        Serial.println(" ");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(lsm.lsm_tempData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(((lsm.lsm_tempData.T * 1.8) + 32));
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("LSM9DS0 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
