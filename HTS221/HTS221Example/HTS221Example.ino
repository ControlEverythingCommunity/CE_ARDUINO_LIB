/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTS221
        This code is designed to work with the HTS221_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Humidity and Temperature Sensor?sku=HTS221_I2CS#tabs-0-product_tabset-2
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <HTS221.h>

HTS221 hts;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    hts.getAddr_HTS221(HTS221_DEFAULT_ADDRESS);           // 0x5F

    // The Humidity and Temperature Sensor's Output Data Rate Configuration,
    // can be changed via the following functions

    /*
    // Also some more parameters can be changed using the following functions
    // Humidity and Temperature Sensor:
    // Numbers of Averaged Temperature and Humidity Samples, Power-Down Mode Control
    // Block Data Update, Reboot Memory Content, Heater and One-Shot Enable.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    hts.setDataRate(HUMTEMP_DATARATE_1HZ);             // Humidity (Hz): 1, Temperature (Hz): 1
    // hts.setDataRate(HUMTEMP_DATARATE_ONE_SHOT);     // One-Shot Mode Enabled
    // hts.setDataRate(HUMTEMP_DATARATE_7HZ);          // Humidity (Hz): 7, Temperature (Hz): 7
    // hts.setDataRate(HUMTEMP_DATARATE_12_5HZ);       // Humidity (Hz): 12.5, Temperature (Hz): 12.5

    hts.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    // HTS221 Humidity and Temperature Sensor Address
    address = hts.hts_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Humidity and Temperature Readings from HTS221");
        Serial.println(" ");
        // Set up the sensor for Humidity and Temperature Sensor
        hts.setUpSensor();
        delay(500);

        // Display the results for Humidity and Temperature Values
        hts.Measure_Sensor();

        // Output Data to Screen
        Serial.print("Relative Humidity Reading: ");
        Serial.print(hts.hts_sensorData.H);
        Serial.println(" %RH");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(hts.hts_sensorData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(((hts.hts_sensorData.T * 1.8) + 32));
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("HTS221 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
