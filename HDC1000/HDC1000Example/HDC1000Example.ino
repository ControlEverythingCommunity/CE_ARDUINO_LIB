/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HDC1000
        This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <HDC1000.h>

HDC1000 hdc;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    hdc.getAddr_HDC1000(HDC1000_DEFAULT_ADDRESS);   // 0x40
    // hdc.getAddr_HDC1000(HDC1000_0_1_ADDRESS);    // 0x41
    // hdc.getAddr_HDC1000(HDC1000_1_0_ADDRESS);    // 0x42
    // hdc.getAddr_HDC1000(HDC1000_1_1_ADDRESS);    // 0x43

    // The Software Reset, Heater Status, Mode of Acquisition, Battery Status
    // Temperature and Humidity Measurement Resolution can be changed via the functions

    hdc.setReset(RESET_DONE);                       // Software Reset
    // hdc.setReset(RESET_NORMAL);                  // Normal Operation

    hdc.setHeaterStatus(HEATER_ENABLE);             // Heater Enable
    // hdc.setHeaterStatus(HEATER_DISABLE);         // Heater Disable

    hdc.setMode(TEMP_OR_HUMIDITY);                  // Temperature or Humidity is Acquired
    // hdc.setMode(TEMP_HUM_BOTH);                  // Temperature and Humidity are Acquired in Sequence, Temperature First

    hdc.setVoltage(VOLTAGE_HIGH);                   // Battery Voltage > 2.8V
    // hdc.setVoltage(VOLTAGE_LOW);                 // Battery Voltage < 2.8V

    hdc.setTempResolution(TEMP_RESOLUTION_14);      // Temperature Measurement Resolution: 14-Bit
    // hdc.setTempResolution(TEMP_RESOLUTION_11);   // Temperature Measurement Resolution: 11-Bit

    hdc.setHumResolution(HUM_RESOLUTION_14);        // Humidity Measurement Resolution: 14-Bit
    // hdc.setHumResolution(HUM_RESOLUTION_11);     // Humidity Measurement Resolution: 11-Bit
    // hdc.setHumResolution(HUM_RESOLUTION_8);      // Humidity Measurement Resolution: 8-Bit

    hdc.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = hdc.hdc_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from HDC1000");
        Serial.println(" ");

        // Set up for Temperature and Humidity Sensor
        hdc.setConfig();
        delay(500);

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(hdc.hdc_sensorData.T);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(hdc.hdc_sensorData.T * 1.8 + 32);
        Serial.println(" °F");
        Serial.print("Relative Humidity: ");
        Serial.print(hdc.hdc_sensorData.H);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("HDC1000 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
