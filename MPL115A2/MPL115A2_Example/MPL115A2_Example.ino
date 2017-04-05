/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPL115A2
        This code is designed to work with the MPL115A2_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/collections/barometers/products/mpl115a2-digital-barometer-500-to-1150-mbar
*/
/**************************************************************************/

#include <Wire.h>
#include <MPL115A2.h>

MPL115A2 mpl;

double referencePressure;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
     mpl.getAddr_MPL115A2(MPL115A2_ADDRESS_DEFAULT);        // 0x60

    mpl.begin();

    delay(500);
}


void loop(void)
{
    byte error;
    int8_t address;

    address = mpl.mpl_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Readings from MPL115A2");
        Serial.println(" ");
        delay(300);

        // Read the Pressure and Temperature
        mpl.Measure_PressureTemperature();

        // Output data to screen
        Serial.print("Digital Pressure Reading: ");
        Serial.print(mpl.mpl_sensorData.P);
        Serial.println(" mbar");
        Serial.print("Temperature Reading in Celsius: ");
        Serial.print(mpl.mpl_sensorData.T);
        Serial.println(" C");
        Serial.print("Temperature Reading in Fahrenheit: ");
        Serial.print(mpl.mpl_sensorData.T * 1.8 + 32);
        Serial.println(" F");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MPL115A2 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
