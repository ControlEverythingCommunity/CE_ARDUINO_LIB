/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT31
        This code is designed to work with the SHT31_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT31_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <SHT31.h>

SHT31 sht = SHT31();

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    sht.getAddr_SHT31(SHT31_DEFAULT_ADDRESS);   // 0x44
    // sht.getAddr_SHT31(SHT31_ADDRESS_HIGH);   // 0x45

    // The command to check the Heater Status can be changed via the following functions

    sht.setHeaterStatus(SHT31_CMD_HEATERENABLE);          // Heater enable command
    // sht.setHeaterStatus(SHT31_CMD_HEATERDISABLE);      // Heater disable command

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

        Serial.println("Getting Readings from SHT31");
        Serial.println(" ");
        // Read and print out the temperature in C and F scales
        cTemp = sht.getTemperature();
        fTemp = cTemp * 1.8 + 32;
        if (! isnan(cTemp))
        {
            // check if 'is not a number'
        }
        else
        {
            Serial.println("Failed to read the temperature");
        }

        // Read and print out the Relative Humidity in %RH scale
        humidity = sht.getHumidity();
        if (! isnan(humidity))
        {
            // check if 'is not a number'
        }
        else
        {
            Serial.println("Failed to read the relative humidity");
        }

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.print("Relative Humidity Reading: ");
        Serial.print(humidity);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("SHT31 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(2000);
}
