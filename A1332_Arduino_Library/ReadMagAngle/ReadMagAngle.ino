/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A1332
        This code is designed to work with the A1332_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/precision-hall-effect-rotational-angle-0-to-360-sensor
*/
/**************************************************************************/

#include <Wire.h>
#include <A1332.h>

A1332 a1332;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    a1332.getAddr_A1332(A1332_DEFAULT_ADDRESS);   // 0x0C

    a1332.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = a1332.a1332_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        a1332.readRegister(address);
        float magAngle;

        Serial.println("Getting Hall Effect Rotational Angle Readings from A1332");
        Serial.println(" ");
        // Read and print out the Magnetic Angle, convert it to degrees
        magAngle = a1332.Measure_HallSensor();

        // Output data to screen
        Serial.print("Magnetic Angle Value: ");
        Serial.print(magAngle);
        Serial.println(" degrees");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("A1332 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
