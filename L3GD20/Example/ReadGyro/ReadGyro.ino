/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        L3GD20
        This code is designed to work with the L3GD20_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <L3GD20.h>

L3GD20 l3g;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    l3g.getAddr_L3GD20(L3GD20_DEFAULT_ADDRESS_GYRO);           // 0x6A
    // l3g.getAddr_L3GD20(L3GD20_ADDRESS_GYRO_UPDATED);        // 0x6B

    // The Gyroscope's Output Data Rate and Bandwidth Selection and Full-Scale Selection,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Gyroscope:
    // Power-Down Mode Enable, Gyroscope X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection,
    // SPI Serial Interface Mode Selection.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    l3g.setGyroDataRate(GYRO_DATARATE_95_12_5);      // ODR (Hz): 100, Cutoff: 12.5
    // l3g.setGyroDataRate(GYRO_DATARATE_95_25);     // ODR (Hz): 100, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_190_12_5);  // ODR (Hz): 200, Cutoff: 12.5
    // l3g.setGyroDataRate(GYRO_DATARATE_190_25);    // ODR (Hz): 200, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_190_50);    // ODR (Hz): 200, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_190_70);    // ODR (Hz): 200, Cutoff: 70
    // l3g.setGyroDataRate(GYRO_DATARATE_380_20);    // ODR (Hz): 400, Cutoff: 20
    // l3g.setGyroDataRate(GYRO_DATARATE_380_25);    // ODR (Hz): 400, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_380_50);    // ODR (Hz): 400, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_380_100);   // ODR (Hz): 400, Cutoff: 100
    // l3g.setGyroDataRate(GYRO_DATARATE_760_30);    // ODR (Hz): 800, Cutoff: 30
    // l3g.setGyroDataRate(GYRO_DATARATE_760_35);    // ODR (Hz): 800, Cutoff: 35
    // l3g.setGyroDataRate(GYRO_DATARATE_760_50);    // ODR (Hz): 800, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_760_100);   // ODR (Hz): 800, Cutoff: 100

    l3g.setGyroScale(GYRO_SCALE_2000DPS);            // 2000 dps
    // l3g.setGyroScale(GYRO_SCALE_250DPS);          // 250 dps
    // l3g.setGyroScale(GYRO_SCALE_500DPS);          // 500 dps

    l3g.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // L3GD20 Gyroscope Address
    address = l3g.l3g_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Angular Rate Data Readings from L3GD20");
        Serial.println(" ");
        // Set up the sensor for Gyroscope
        l3g.setUpGyroscope();
        delay(500);
        // Display the results for Angular Rate Values
        l3g.setUpGyroscope();
        l3g.Measure_Sensor();
        l3g.Measure_Gyroscope();

        // Output Data to Screen
        Serial.print("Rotation in X-Axis: ");
        Serial.println(l3g.l3g_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(l3g.l3g_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(l3g.l3g_gyroData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("L3GD20 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
