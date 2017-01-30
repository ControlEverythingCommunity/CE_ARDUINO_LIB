/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A3G4250DTR
        This code is designed to work with the A3G4250DTR_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=A3G4250DTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <A3G4250DTR.h>

A3G4250DTR a3g;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    a3g.getAddr_A3G4250DTR(A3G4250DTR_DEFAULT_ADDRESS_GYRO);           // 0x68
    // a3g.getAddr_A3G4250DTR(A3G4250DTR_ADDRESS_GYRO_UPDATED);        // 0x69

    // The Gyroscope's Output Data Rate and Bandwidth Selection,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Gyroscope:
    // Power-Down Mode Enable, Gyroscope X, Y, Z Axes Enable,
    // Big/Little Endian Data Selection,
    // Self-Test Enable, SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    a3g.setGyroDataRate(GYRO_DATARATE_100_12_5);        // ODR (Hz): 100, Cutoff: 12.5
    // a3g.setGyroDataRate(GYRO_DATARATE_100_25);       // ODR (Hz): 100, Cutoff: 25
    // a3g.setGyroDataRate(GYRO_DATARATE_200_12_5);     // ODR (Hz): 200, Cutoff: 12.5
    // a3g.setGyroDataRate(GYRO_DATARATE_200_25);       // ODR (Hz): 200, Cutoff: 25
    // a3g.setGyroDataRate(GYRO_DATARATE_200_50);       // ODR (Hz): 200, Cutoff: 50
    // a3g.setGyroDataRate(GYRO_DATARATE_200_70);       // ODR (Hz): 200, Cutoff: 70
    // a3g.setGyroDataRate(GYRO_DATARATE_400_20);       // ODR (Hz): 400, Cutoff: 20
    // a3g.setGyroDataRate(GYRO_DATARATE_400_25);       // ODR (Hz): 400, Cutoff: 25
    // a3g.setGyroDataRate(GYRO_DATARATE_400_50);       // ODR (Hz): 400, Cutoff: 50
    // a3g.setGyroDataRate(GYRO_DATARATE_400_100);      // ODR (Hz): 400, Cutoff: 100
    // a3g.setGyroDataRate(GYRO_DATARATE_800_30);       // ODR (Hz): 800, Cutoff: 30
    // a3g.setGyroDataRate(GYRO_DATARATE_800_35);       // ODR (Hz): 800, Cutoff: 35
    // a3g.setGyroDataRate(GYRO_DATARATE_800_50);       // ODR (Hz): 800, Cutoff: 50
    // a3g.setGyroDataRate(GYRO_DATARATE_800_100);      // ODR (Hz): 800, Cutoff: 100

    a3g.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // A3G4250DTR Gyroscope Address
    address = a3g.a3g_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Angular Rate Data Readings from A3G4250DTR");
        Serial.println(" ");
        // Set up the sensor for Gyroscope
        a3g.setUpGyroscope();
        delay(500);
        // Display the results for Angular Rate Values
        a3g.Measure_Sensor();
        a3g.Measure_Gyroscope();

        // Output Data to Screen
        Serial.print("Rotation in X-Axis: ");
        Serial.println(a3g.a3g_gyroData.X);
        Serial.print("Rotation in Y-Axis: ");
        Serial.println(a3g.a3g_gyroData.Y);
        Serial.print("Rotation in Z-Axis: ");
        Serial.println(a3g.a3g_gyroData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("A3G4250DTR Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
