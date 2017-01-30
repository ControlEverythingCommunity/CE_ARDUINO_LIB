/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        L3G4200D
        This code is designed to work with the L3G4200D_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=L3G4200D_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <L3G4200D.h>

L3G4200D l3g;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    l3g.getAddr_L3G4200D(L3G4200D_DEFAULT_ADDRESS_GYRO);           // 0x68
    // l3g.getAddr_L3G4200D(L3G4200D_ADDRESS_GYRO_UPDATED);        // 0x69

    // The Gyroscope's Output Data Rate and Bandwidth Selection and Full-Scale Selection,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Gyroscope:
    // Power-Down Mode Enable, Gyroscope X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection, Full-Scale Selection,
    // Self-Test Enable, SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    l3g.setGyroDataRate(GYRO_DATARATE_100_12_5);      // ODR (Hz): 100, Cutoff: 12.5
    // l3g.setGyroDataRate(GYRO_DATARATE_100_25);     // ODR (Hz): 100, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_200_12_5);  // ODR (Hz): 200, Cutoff: 12.5
    // l3g.setGyroDataRate(GYRO_DATARATE_200_25);    // ODR (Hz): 200, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_200_50);    // ODR (Hz): 200, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_200_70);    // ODR (Hz): 200, Cutoff: 70
    // l3g.setGyroDataRate(GYRO_DATARATE_400_20);    // ODR (Hz): 400, Cutoff: 20
    // l3g.setGyroDataRate(GYRO_DATARATE_400_25);    // ODR (Hz): 400, Cutoff: 25
    // l3g.setGyroDataRate(GYRO_DATARATE_400_50);    // ODR (Hz): 400, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_400_100);   // ODR (Hz): 400, Cutoff: 100
    // l3g.setGyroDataRate(GYRO_DATARATE_800_30);    // ODR (Hz): 800, Cutoff: 30
    // l3g.setGyroDataRate(GYRO_DATARATE_800_35);    // ODR (Hz): 800, Cutoff: 35
    // l3g.setGyroDataRate(GYRO_DATARATE_800_50);    // ODR (Hz): 800, Cutoff: 50
    // l3g.setGyroDataRate(GYRO_DATARATE_800_100);   // ODR (Hz): 800, Cutoff: 100

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

    // L3G4200D Gyroscope Address
    address = l3g.l3g_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Angular Rate Data Readings from L3G4200D");
        Serial.println(" ");
        // Set up the sensor for Gyroscope
        l3g.setUpGyroscope();
        delay(500);
        // Display the results for Angular Rate Values
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
        Serial.println("L3G4200D Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
