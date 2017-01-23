/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HCPA-5V-U3
        This code is designed to work with the HCPA-5V-U3_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HCPA-5V-U3_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <HCPA_5V_U3.h>

HCPA_5V_U3 hcpa = HCPA_5V_U3();

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    hcpa.getAddr_HCPA_5V_U3(HCPA_5V_U3_DEFAULT_ADDRESS);   // 0x28

    hcpa.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = hcpa.hcpa_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {

        float cTemp, fTemp, humidity;

        Serial.println("Getting Readings from HCPA_5V_U3");
        Serial.println(" ");

        // Read and print out the Relative Humidity in %RH scale
        humidity = hcpa.getHumidity();
        if (! isnan(humidity))
        {
            // check if 'is not a number'
        }
        else
        {
            Serial.println("Failed to read the relative humidity");
        }

        // Read and print out the temperature in C and F scales
        cTemp = hcpa.getTemperature();
        fTemp = cTemp * 1.8 + 32;
        if (! isnan(cTemp))
        {
            // check if 'is not a number'
        }
        else
        {
            Serial.println("Failed to read the temperature");
        }

        // Output data to screen
        Serial.print("Relative Humidity Reading: ");
        Serial.print(humidity);
        Serial.println(" %RH");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" °C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" °F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("HCPA_5V_U3 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(2000);
}
