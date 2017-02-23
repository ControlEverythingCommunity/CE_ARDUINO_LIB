/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAG3110
        This code is designed to work with the MAG3110_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#include <Wire.h>
#include <MAG3110.h>

MAG3110 mag;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mag.getAddr_MAG3110(MAG3110_DEFAULT_ADDRESS_MAG);        // 0x0E

    mag.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MAG3110 Magnetometer Address
    address = mag.mag_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Magnetic Field Data Readings from MAG3110");
        Serial.println(" ");
        // Set up the sensor for Magnetometer
        mag.setUpMagnetometer();
        delay(500);
        // Display the results for Magnetic Field Values
        mag.Measure_Magnetometer();

        // Output Data to Screen
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(mag.mag_Data.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(mag.mag_Data.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(mag.mag_Data.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("MAG3110 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
