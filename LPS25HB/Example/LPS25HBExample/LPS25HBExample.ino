/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LPS25HB
        This code is designed to work with the LPS25HB_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=LPS25HB_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <LPS25HB.h>

LPS25HB lps;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    lps.getAddr_LPS25HB(LPS25HB_DEFAULT_ADDRESS_BARO);           // 0x5C
    // lps.getAddr_LPS25HB(LPS25HB_ADDRESS_BARO_UPDATED);        // 0x5D

    // The Barometer's Output Data Rate Configuration,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Barometer:
    // Power-Down Mode Control, Interrupt Generation Enable,
    // Block Data Update, Reset Autozero Function,
    // SPI Serial Interface Mode Selection.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    lps.setBaroDataRate(BARO_DATARATE_1HZ);             // Pressure (Hz): 1, Temperature (Hz): 1
    // lps.setBaroDataRate(BARO_DATARATE_ONE_SHOT);     // One-Shot Mode Enabled
    // lps.setBaroDataRate(BARO_DATARATE_7HZ);          // Pressure (Hz): 7, Temperature (Hz): 7
    // lps.setBaroDataRate(BARO_DATARATE_12_5HZ);       // Pressure (Hz): 12.5, Temperature (Hz): 12.5
    // lps.setBaroDataRate(BARO_DATARATE_25HZ);         // Pressure (Hz): 25, Temperature (Hz): 25

    lps.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // LPS25HB Barometer Address
    address = lps.lps_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Pressure and Temperature Readings from LPS25HB");
        Serial.println(" ");
        // Set up the sensor for Barometer
        lps.setUpBarometer();
        delay(500);
        // Display the results for Pressure and Temperature Values
        lps.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(lps.lps_baroData.P);
        Serial.println(" hPa");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(lps.lps_baroData.P);
        Serial.println(" mbar");
        Serial.print("Digital Pressure Reading: ");
        Serial.print(lps.lps_baroData.P * 0.75);
        Serial.println(" mmHg");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(lps.lps_baroData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(((lps.lps_baroData.T * 1.8) + 32));
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("LPS25HB Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
