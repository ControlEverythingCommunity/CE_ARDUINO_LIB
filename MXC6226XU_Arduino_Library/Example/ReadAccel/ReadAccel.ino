/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6226XU
        This code is designed to work with the MXC6226XU_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=MXC6226XU_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MXC6226XU.h>

MXC6226XU mxc;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mxc.getAddr_MXC6226XU(MXC6226XU_DEFAULT_ADDRESS);        // 0x16

    /*
    // Some parameters can be changed using the following functions
    // Accelerometer:
    // Power Down DTOS, Shake Mode Bit Status, Shake Threshold
    // Shake Events Time Window and Orientation Hysteresis Time Period
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    mxc.begin();
    delay(500);
}

void loop(void)
{
    byte error;
    int8_t address;

    address = mxc.mxc_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Thermal Orientation/Acceleration Readings from MXC6226XU");
        Serial.println(" ");
        // Set up the sensor for Thermal Accelerometer
        mxc.setUpAccelerometer();
        delay(500);
        // Display the results for Thermal Orientation/Acceleration values
        mxc.Measure_Accelerometer();

        // Output data to screen
        Serial.print("Thermal Orientation in X-Axis: ");
        Serial.println(mxc.mxc_accelData.X);
        Serial.print("Thermal Orientation in Y-Axis: ");
        Serial.println(mxc.mxc_accelData.Y);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("MXC6226XU Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
