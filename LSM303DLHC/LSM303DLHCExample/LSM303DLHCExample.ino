/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM303DLHC
        This code is designed to work with the LSM303DLHC_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <LSM303DLHC.h>

LSM303DLHC lsm;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lsm.getAddrX_LSM303DLHC(LSM303DLHC_DEFAULT_ADDRESS_ACCEL);      // 0x19
    lsm.getAddrM_LSM303DLHC(LSM303DLHC_DEFAULT_ADDRESS_MAG);        // 0x1E

    // The Acceleration Data Rate Selection and Full-Scale Selection,
    // Magnetic Data Rate Selection and Magnetic Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Low Power Mode Enable, Accelerometer X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection,
    // SHigh Resolution Enable and SPI Serial Interface Mode Selection,
    // Magmetometer:
    // Temperature Sensor Enable, Magnetic Sensor Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lsm.setAccelDataRate(ACCEL_DATARATE_1HZ);               // AODR (Hz): 1
    // lsm.setAccelDataRate(ACCEL_DATARATE_POWERDOWN);      // Power Down Mode
    // lsm.setAccelDataRate(ACCEL_DATARATE_10HZ);           // AODR (Hz): 10
    // lsm.setAccelDataRate(ACCEL_DATARATE_25HZ);           // AODR (Hz): 25
    // lsm.setAccelDataRate(ACCEL_DATARATE_50HZ);           // AODR (Hz): 50
    // lsm.setAccelDataRate(ACCEL_DATARATE_100HZ);          // AODR (Hz): 100
    // lsm.setAccelDataRate(ACCEL_DATARATE_200HZ);          // AODR (Hz): 200
    // lsm.setAccelDataRate(ACCEL_DATARATE_400HZ);          // AODR (Hz): 400
    // lsm.setAccelDataRate(ACCEL_DATARATE_1600HZ);         // AODR (Hz): 1600
    // lsm.setAccelDataRate(ACCEL_DATARATE_1344HZ);         // AODR (Hz): 1344

    lsm.setAccelRange(ACCEL_RANGE_16G);                     // ±16 g
    // lsm.setAccelRange(ACCEL_RANGE_2G);                   // ±2 g
    // lsm.setAccelRange(ACCEL_RANGE_4G);                   // ±4 g
    // lsm.setAccelRange(ACCEL_RANGE_8G);                   // ±8 g

    lsm.setMagDataRate(MAG_DATARATE_0_75HZ);                // MODR (Hz): 0.75
    // lsm.setMagDataRate(MAG_DATARATE_1_5HZ);              // MODR (Hz): 1.5
    // lsm.setMagDataRate(MAG_DATARATE_3HZ);                // MODR (Hz): 3
    // lsm.setMagDataRate(MAG_DATARATE_7_5HZ);              // MODR (Hz): 7.5
    // lsm.setMagDataRate(MAG_DATARATE_15HZ);               // MODR (Hz): 15
    // lsm.setMagDataRate(MAG_DATARATE_30HZ);               // MODR (Hz): 30
    // lsm.setMagDataRate(MAG_DATARATE_75HZ);               // MODR (Hz): 75
    // lsm.setMagDataRate(MAG_DATARATE_220HZ);              // MODR (Hz): 220

    lsm.setMagGain(MAG_GAIN_1_3GAUSS);                      // ±1.3 guass
    // lsm.setMagGain(MAG_GAIN_1_9GAUSS);                   // ±1.9 guass
    // lsm.setMagGain(MAG_GAIN_2_5GAUSS);                   // ±2.5 guass
    // lsm.setMagGain(MAG_GAIN_4GAUSS);                     // ±4.0 guass
    // lsm.setMagGain(MAG_GAIN_4_7GAUSS);                   // ±4.7 guass
    // lsm.setMagGain(MAG_GAIN_5_6GAUSS);                   // ±5.6 guass
    // lsm.setMagGain(MAG_GAIN_8_1GAUSS);                   // ±8.1 guass

    lsm.begin();
    delay(1000);
}

void loop(void)
{
    byte errorX, errorM;
    int8_t addressX, addressM;

    // LSM303DLHC Accelerometer Address
    addressX = lsm.lsm_i2cAddressX;
    // LSM303DLHC Magnetometer Address
    addressM = lsm.lsm_i2cAddressM;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addressX);
    errorX = Wire.endTransmission();
    Wire.beginTransmission(addressM);
    errorM = Wire.endTransmission();
    if ((errorX == 0) && (errorM == 0))
    {
        Serial.println("Getting Linear Acceleration and Magnetic Field Data Readings from LSM303DLHC");
        Serial.println(" ");
        // Set up the sensor for Accelerometer, and Magnetometer
        lsm.setUpAccelerometer();
        lsm.setUpMagnetometer();
        delay(500);
        // Display the results for Linear Acceleration and Magnetic Field Values
        lsm.Measure_Sensor();
        lsm.Measure_Accelerometer();
        lsm.Measure_Magnetometer();

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
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(lsm.lsm_magData.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(lsm.lsm_magData.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(lsm.lsm_magData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("LSM303DLHC Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
