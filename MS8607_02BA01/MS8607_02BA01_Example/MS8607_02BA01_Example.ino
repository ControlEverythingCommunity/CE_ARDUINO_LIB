/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS8607-02BA01
        This code is designed to work with the MS8607-02BA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=MS8607-02BA01_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MS8607_02BA01.h>

MS8607_02BA01 ms;

float referencePressure;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    ms.getAddrPT_MS8607_02BA01(MS8607_02BA01_PT_ADDRESS);   // 0x76
    ms.getAddrRH_MS8607_02BA01(MS8607_02BA01_RH_ADDRESS);   // 0x40

    // The OSR Command Value (typ=D1 and D2) to Intiate the Pressure and Temperature Conversion
    // The Measurement Resolution, VDD Status, On-Chip Heater Status and Humidity Measurement Mode
    // can be changed via the following functions

    ms.setinitiatePressure(D1_OSR_256);             // Convert D1 (OSR=256)
    // ms.setinitiatePressure(D1_OSR_512);          // Convert D1 (OSR=512)
    // ms.setinitiatePressure(D1_OSR_1024);         // Convert D1 (OSR=1024)
    // ms.setinitiatePressure(D1_OSR_2048);         // Convert D1 (OSR=2048)
    // ms.setinitiatePressure(D1_OSR_4096);         // Convert D1 (OSR=4096)

    ms.setinitiateTemperature(D2_OSR_256);          // Convert D2 (OSR=256)
    // ms.setinitiateTemperature(D2_OSR_512);       // Convert D2 (OSR=512)
    // ms.setinitiateTemperature(D2_OSR_1024);      // Convert D2 (OSR=2014)
    // ms.setinitiateTemperature(D2_OSR_2048);      // Convert D2 (OSR=2048)
    // ms.setinitiateTemperature(D2_OSR_4096);      // Convert D2 (OSR=4096)

    ms.setResolution(D3_OSR_4096);                  // Resolution (D3): Highest, OSR: 4096
    // ms.setResolution(D3_OSR_2048);               // Resolution (D3): , OSR: 2048
    // ms.setResolution(D3_OSR_1024);               // Resolution (D3): , OSR: 1024
    // ms.setResolution(D3_OSR_256);                // Resolution (D3): Lowest, OSR: 256

    ms.setVoltage(VOLTAGE_OK);                      // VDD OK
    // ms.setVoltage(VOLTAGE_LOW);                  // VDD Low

    ms.setHeaterStatus(HEATER_DISABLE);          // On-chip Heater Disable
    // ms.setHeaterStatus(HEATER_ENABLE);              // On-chip Heater Enable


    ms.setHumidityMode(HUMIDITY_NO_HOLD);           // Measure Humidity, No Hold Master Mode
    // ms.setHumidityMode(HUMIDITY_HOLD);           // Measure Humidity, Hold Master Mode

    ms.begin();

    delay(500);
}


void loop(void)
{
    byte errorPT, errorRH;
    int8_t addressPT, addressRH;

    // MS8607-02BA01 Pressure and Temperature Address
    addressPT = ms.ms_i2cAddressPT;
    // MS8607-02BA01 Relative Humidity Address
    addressRH = ms.ms_i2cAddressRH;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addressPT);
    errorPT = Wire.endTransmission();
    Wire.beginTransmission(addressRH);
    errorRH = Wire.endTransmission();
    if ((errorPT == 0) && (errorRH == 0))
    {
        float cTemp, fTemp, pressure, humidity;

        Serial.println("Getting Readings from MS8607_02BA01");
        Serial.println(" ");
        delay(300);

        // Read the Pressure, Temperature and Relative Hunidity
        ms.Measure_Sensor();

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(ms.ms_sensorData.P);
        Serial.println(" mbar");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(ms.ms_sensorData.T);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(ms.ms_sensorData.T * 1.8 + 32);
        Serial.println(" F");
        Serial.print("Relative Humidity: ");
        Serial.print(ms.ms_sensorData.H);
        Serial.println(" %RH");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MS8607_02BA01 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
