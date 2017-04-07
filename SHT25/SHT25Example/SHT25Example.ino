/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT25
        This code is designed to work with the SHT25_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT25_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <SHT25.h>

SHT25 sht;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    sht.getAddr_SHT25(SHT25_DEFAULT_ADDRESS);   // 0x40

    // The Measurement Resolution, VDD Status, On-Chip Heater Status and Heater Current,
    // The Temperature and Humidity Measurement Modes can be changed via the following functions

    sht.setResolution(RESOLUTION_0);             // RH: 12 bit, Temp: 14 bit
    // sht.setResolution(RESOLUTION_1);          // RH: 8 bit, Temp: 12 bit
    // sht.setResolution(RESOLUTION_2);          // RH: 10 bit, Temp: 13 bit
    // sht.setResolution(RESOLUTION_4);          // RH: 11 bit, Temp: 11 bit

    sht.setVoltage(VOLTAGE_OK);                  // VDD OK
    // sht.setVoltage(VOLTAGE_LOW);              // VDD Low

    sht.setHeaterStatus(HEATER_ENABLE);          // On-chip Heater Enable
    // sht.setHeaterStatus(HEATER_DISABLE);      // On-chip Heater Disable

    sht.setOTPStatus(OTP_DISABLE);               // OTP Reload Disable
    // sht.setOTPStatus(OTP_ENABLE);             // OTP Reload Enable

    sht.setTempMode(TEMP_NO_HOLD);               // Measure Temperature, No Hold Master Mode
    // sht.setTempMode(TEMP_HOLD);               // Measure Temperature, Hold Master Mode

    sht.setHumidityMode(HUMIDITY_NO_HOLD);       // Measure Humidity, No Hold Master Mode
    // sht.setHumidityMode(HUMIDITY_HOLD);       // Measure Humidity, Hold Master Mode

    sht.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = sht.sht_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from SHT25");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        cTemp = sht.Measure_Temperature();
        fTemp = cTemp * 1.8 + 32;

        // Read and print out the Relative Humidity, convert it to %RH
        humidity = sht.Measure_Humidity();

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" °F");
        Serial.print("Relative Humidity: ");
        Serial.print(humidity);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("SHT25 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
