/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AIS328DQTR
        This code is designed to work with the AIS328DQTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=AIS328DQTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <AIS328DQTR.h>

AIS328DQTR ais;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    ais.getAddr_AIS328DQTR(AIS328DQTR_DEFAULT_ADDRESS);        // 0x18
    // ais.getAddr_AIS328DQTR(AIS328DQTR_ADDRESS_UPDATED);     // 0x19

    // The Acceleration Data Rate Selection and Acceleration Full-Scale Selection
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Accelerometer:
    // Power-Down Mode Enable, Accelerometer Power Mode, Accelerometer X, Y, Z Axes Enable,
    // Block Data Update, Big/Little Endian Data Selection, Self Test Sign
    // Self-Test Enable and SPI Serial Interface Mode Selection
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    ais.setAccelDataRate(ACCEL_DATARATE_50HZ);              // AODR (Hz): 50
    // ais.setAccelDataRate(ACCEL_DATARATE_100HZ);          // AODR (Hz): 100
    // ais.setAccelDataRate(ACCEL_DATARATE_400HZ);          // AODR (Hz): 400
    // ais.setAccelDataRate(ACCEL_DATARATE_1000HZ);         // AODR (Hz): 1000

    ais.setAccelRange(ACCEL_RANGE_2G);                      // ±2 G
    // ais.setAccelRange(ACCEL_RANGE_4G);                   // ±4 G
    // ais.setAccelRange(ACCEL_RANGE_8G);                   // ±8 G

    ais.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ais.ais_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Readings from AIS328DQTR");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        ais.setUpAccelerometer();
        delay(500);
        // Display the results for Linear Acceleration values
        ais.Measure_Accelerometer();

        // Output data to screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(ais.ais_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(ais.ais_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(ais.ais_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("AIS328DQTR Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
