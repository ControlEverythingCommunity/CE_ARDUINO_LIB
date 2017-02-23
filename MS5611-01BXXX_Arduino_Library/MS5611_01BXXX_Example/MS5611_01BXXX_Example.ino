/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS5611-01BXXX
        This code is designed to work with the MS5611-01BXXX_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/variometer-10-to-1200-mbar?variant=25687736203#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MS5611_01BXXX.h>

MS5611_01BXXX ms;

double referencePressure;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
     ms.getAddr_MS5611_01BXXX(MS5611_01BXXX_ADDRESS_LOW);        // 0x76
   // ms.getAddr_MS5611_01BXXX(MS5611_01BXXX_ADDRESS_HIGH);    // 0x77

    // The OSR command value (typ=D1 and D2) to intiate the pressure and temperature conversion
    // can be changed via the following functions

    ms.setinitiatePressure(D1_OSR_256);           // Convert D1 (OSR=256)
    // ms.setinitiatePressure(D1_OSR_512);        // Convert D1 (OSR=512)
    // ms.setinitiatePressure(D1_OSR_1024);       // Convert D1 (OSR=1024)
    // ms.setinitiatePressure(D1_OSR_2048);       // Convert D1 (OSR=2048)
    // ms.setinitiatePressure(D1_OSR_4096);       // Convert D1 (OSR=4096)

    ms.setinitiateTemperature(D2_OSR_256);        // Convert D2 (OSR=256)
    // ms.setinitiateTemperature(D2_OSR_512);     // Convert D2 (OSR=512)
    // ms.setinitiateTemperature(D2_OSR_1024);    // Convert D2 (OSR=2014)
    // ms.setinitiateTemperature(D2_OSR_2048);    // Convert D2 (OSR=2048)
    // ms.setinitiateTemperature(D2_OSR_4096);    // Convert D2 (OSR=4096)

    ms.begin();

    delay(500);
}


void loop(void)
{
    byte error;
    int8_t address;

    address = ms.ms_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        float cTemp, fTemp, pressure;

        Serial.println("Getting Readings from MS5611_01BXXX");
        Serial.println(" ");
        delay(300);

        // Read the Pressure and Temperature
        // Read true temperature & Pressure
        double realTemperature = ms.Measure_Temperature();
        double realPressure = ms.Measure_Pressure();

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(realPressure);
        Serial.println(" mbar");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(realTemperature);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(realTemperature * 1.8 + 32);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MS5611_01BXXX Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
