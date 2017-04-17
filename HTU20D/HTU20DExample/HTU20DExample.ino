/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTU20D
        This code is designed to work with the HTU20D_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HTU20D_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <HTU20D.h>

HTU20D htu;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    htu.getAddr_HTU20D(HTU20D_DEFAULT_ADDRESS);   // 0x40

    // The Measurement Resolution, VDD Status, On-Chip Heater Status and Heater Current,
    // The Temperature and Humidity Measurement Modes can be changed via the following functions

    htu.setResolution(RESOLUTION_0);             // RH: 12 bit, Temp: 14 bit
    // htu.setResolution(RESOLUTION_1);          // RH: 8 bit, Temp: 12 bit
    // htu.setResolution(RESOLUTION_2);          // RH: 10 bit, Temp: 13 bit
    // htu.setResolution(RESOLUTION_4);          // RH: 11 bit, Temp: 11 bit

    htu.setVoltage(VOLTAGE_OK);                  // VDD OK
    // htu.setVoltage(VOLTAGE_LOW);              // VDD Low

    htu.setHeaterStatus(HEATER_ENABLE);          // On-chip Heater Enable
    // htu.setHeaterStatus(HEATER_DISABLE);      // On-chip Heater Disable

    htu.setOTPStatus(OTP_DISABLE);               // OTP Reload Disable
    // htu.setOTPStatus(OTP_ENABLE);             // OTP Reload Enable


    htu.setTempMode(TEMP_NO_HOLD);               // Measure Temperature, No Hold Master Mode
    // htu.setTempMode(TEMP_HOLD);               // Measure Temperature, Hold Master Mode

    htu.setHumidityMode(HUMIDITY_NO_HOLD);       // Measure Humidity, No Hold Master Mode
    // htu.setHumidityMode(HUMIDITY_HOLD);       // Measure Humidity, Hold Master Mode

    htu.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = htu.htu_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from HTU20D");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        cTemp = htu.Measure_Temperature();
        fTemp = cTemp * 1.8 + 32;

        // Read and print out the Relative Humidity, convert it to %RH
        humidity = htu.Measure_Humidity();

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
        Serial.println("HTU20D Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
