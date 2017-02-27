/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BME280
        This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BME280_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <BME280.h>

BME280 bme;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    bme.getAddr_BME280(BME280_DEFAULT_ADDRESS);     // 0x76
    // bme.getAddr_BME280(BME280_ADDRESS_UPDATED);  // 0x77

    /*
    // Humidity, Pressure and Temperature Oversampling, Mode Status
    // Controls Inactive Duration tstandby in Normal Mode
    // Controls the Time Constant of the IIR Filter and SPI Wire Mode.
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    bme.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = bme.bme_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {

        Serial.println("Getting Pressure, Temperature and Humidity Readings from BME280");
        Serial.println(" ");
        // Set up the sensor
        bme.setUpSampling();
        delay(500);
        // Display the results for for Pressure, Temperature and Humidity values
        bme.Measure_Sensor();

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(bme.bme_sensorData.P/100.0);
        Serial.println(" hPa");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(bme.bme_sensorData.T);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(bme.bme_sensorData.T * 1.8 + 32);
        Serial.println(" F");
        Serial.print("Humidity Reading: ");
        Serial.print(bme.bme_sensorData.H);
        Serial.println(" %");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("BME280 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
