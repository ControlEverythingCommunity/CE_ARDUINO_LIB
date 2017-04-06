/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3MDL
        This code is designed to work with the LIS3MDL_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Magnetic-Sensor?sku=LIS3MDL_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <LIS3MDL.h>

LIS3MDL lis;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lis.getAddr_LIS3MDL(LIS3MDL_DEFAULT_ADDRESS);        // 0x1C
    // lis.getAddr_LIS3MDL(LIS3MDL_ADDRESS_UPDATED);     // 0x1D

    // The Magnetic Data Rate Selection and Magnetic Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Magnetometer:
    // Temperature Sensor Enable, X, Y and Z Axes Operative Mode Selection, FAST_ODR Enable
    // Self-Test Enable, Reboot Memory Content, Low-Power Mode Configuration,
    // SPI Serial Interface Mode Selection, Operating Mode Selection
    // Z-Axis Operative Mode Selection, Big/Little Endian Data Selection
    // FAST_READ Enable and Block Data Update for Magnetic Data
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lis.setMagDataRate(MAG_DATARATE_0_625HZ);           // ODR [Hz]: 0.625
    // lis.setMagDataRate(MAG_DATARATE_1_25HZ);         // ODR [Hz]: 1.25
    // lis.setMagDataRate(MAG_DATARATE_2_5HZ);          // ODR [Hz]: 2.5
    // lis.setMagDataRate(MAG_DATARATE_5HZ);            // ODR [Hz]: 5
    // lis.setMagDataRate(MAG_DATARATE_10HZ);           // ODR [Hz]: 10
    // lis.setMagDataRate(MAG_DATARATE_20HZ);           // ODR [Hz]: 20
    // lis.setMagDataRate(MAG_DATARATE_40HZ);           // ODR [Hz]: 40
    // lis.setMagDataRate(MAG_DATARATE_80HZ);           // ODR [Hz]: 80

    lis.setMagScale(MAG_SCALE_4G);                      // ±4 Gauss
    // lis.setMagScale(MAG_SCALE_8G);                   // ±8 Gauss
    // lis.setMagScale(MAG_SCALE_12G);                  // ±12 Gauss
    // lis.setMagScale(MAG_SCALE_16G);                  // ±16 Gauss

    lis.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = lis.lis_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Magnetic Data Readings from LIS3MDL");
        Serial.println(" ");
        // Set up the sensor for Magnetometer
        lis.setUpMagnetometer();
        delay(500);
        // Display the results for Magnetic Data Values
        lis.Measure_Magnetometer();

        // Output data to screen
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(lis.lis_magData.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(lis.lis_magData.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(lis.lis_magData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("LIS3MDL Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
