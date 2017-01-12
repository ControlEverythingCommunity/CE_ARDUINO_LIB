/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7050
        This code is designed to work with the SI7050_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7050_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <SI7050.h>

SI7050 si;

void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    si.getAddr_SI7050(SI7050_DEFAULT_ADDRESS);   // 0x40, 1000 000

    // The Temperature Measurement Mode can be changed via the following function

    si.setTempMode(TEMP_NO_HOLD);        // Measure Temperature, No Hold Master Mode
    // si.setTempMode(TEMP_HOLD);        // Measure Temperature, Hold Master Mode

    si.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = si.si_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {

        int16_t temp;

        Serial.println("Getting Temperature Readings from SI7050");
        Serial.println(" ");
        // Read and print out the temperature, then convert to C and F scales
        temp = si.Measure_Temp();
        float cTemp = ((175.72 * temp) / 65536.0) - 46.85;
        float fTemp = cTemp * 1.8 + 32;

        // Output data to screen
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(cTemp);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(fTemp);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("SI7050 Disconnected !");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
