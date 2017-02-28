/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC6343A
        This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Compass?sku=HMC6343A_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/
#include <Wire.h>
#include <HMC6343A.h>

HMC6343A hmc;

void setup(void)
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    hmc.getAddr_HMC6343A(HMC6343A_DEFAULT_ADDRESS);   // 0x19

    // The Calculating Compass Data and Calibration Offsets,
    // IIR Heading Filter Used, Run and Standby Mode,
    // Upright Front/Edge and Level Orientation
    // can be changed via functions

    hmc.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = hmc.hmc_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration and Magnetic Field Readings from HMC6343A");
        Serial.println(" ");

        // Set Up the Sensor for Accelerometer and Magnetometer
        hmc.setUpSensor();

        // Display the results for Acceleration and Magnetic Vector Values
        hmc.Measure_Sensor();

        // Output data to screen
        Serial.print("Linear Acceleration in X-Axis: ");
        Serial.println(hmc.hmc_accelData.X);
        Serial.print("Linear Acceleration in Y-Axis: ");
        Serial.println(hmc.hmc_accelData.Y);
        Serial.print("Linear Acceleration in Z-Axis: ");
        Serial.println(hmc.hmc_accelData.Z);
        Serial.println(" ");
        Serial.println("    ***********     ");
        Serial.println(" ");
        delay(500);
        Serial.print("Magnetic Field in X-Axis: ");
        Serial.println(hmc.hmc_magData.X);
        Serial.print("Magnetic Field in Y-Axis: ");
        Serial.println(hmc.hmc_magData.Y);
        Serial.print("Magnetic Field in Z-Axis: ");
        Serial.println(hmc.hmc_magData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("HMC6343A Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
