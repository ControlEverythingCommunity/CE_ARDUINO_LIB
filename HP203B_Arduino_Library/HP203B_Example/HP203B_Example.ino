/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HP203B
        This code is designed to work with the HP203B_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/precision-barometer-and-altimeter?variant=25687549067#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <HP203B.h>

HP203B hp;

double referencePressure;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    hp.getAddr_HP203B(HP203B_DEFAULT_ADDRESS);          // 0x76
    // hp.getAddr_HP203B(HP203B_ADDRESS_UPDATED);       // 0x77

    // The OSR command value to intiate the Pressure, Altitude and Temperature Conversions
    // can be changed via the following functions

    hp.setOSR(OSR_4096);            // OSR=4096
    // hp.setOSR(OSR_2048);         // OSR=2048
    // hp.setOSR(OSR_1024);         // OSR=1024
    // hp.setOSR(OSR_512);          // OSR=512
    // hp.setOSR(OSR_256);          // OSR=256
    // hp.setOSR(OSR_128);          // OSR=128

    hp.begin();

    delay(500);
}


void loop(void)
{
    byte error;
    int8_t address;

    address = hp.hp_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {

        Serial.println("Getting Pressure, Altitude and Temperature Readings from HP203B");
        Serial.println(" ");
        delay(300);

        // Read the Pressure, Altitude and Temperature
        hp.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(hp.hp_sensorData.P);
        Serial.println(" hPa");
        Serial.print("Digital Altitude Reading: ");
        Serial.print(hp.hp_sensorData.A);
        Serial.println(" m");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(hp.hp_sensorData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(hp.hp_sensorData.T * 1.8 + 32);
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("HP203B Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
