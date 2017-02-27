/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7660FC
        This code is designed to work with the MMA7660FC_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-orientation-motion-detection-sensor-1-5-g-accelerometer?variant=25687536907#tabs-0-product_tabset-2
*/
/**************************************************************************/

#include <Wire.h>
#include <MMA7660FC.h>

MMA7660FC mma;

void setup(void)
{
    Serial.begin(9600);
    // The address can be changed making the option of connecting multiple devices
    mma.getAddr_MMA7660FC(MMA7660FC_DEFAULT_ADDRESS_ACCEL);        // 0x4C

    /*
    // Some parameters can be changed using the following functions
    // Accelerometer:
    // Interrupt Output Active Status, Interrupt Output Open Drain/Push-Pull Status
    // Prescaler, Auto-Sleep/Wake Functionality, TON and MODE
    // FILT, Auto-Wake/Sleep Mode Sample Rate (AWSR/AMSR).
    // These functions can be modified and added here so that the parameters can be altered as per requirement
    */

    mma.begin();
    delay(1000);
}

void loop(void)
{
    byte error;
    int8_t address;

    // MMA7660FC Accelerometer Address
    address = mma.mma_i2cAddress;

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println("Getting Acceleration Data Readings from MMA7660FC");
        Serial.println(" ");
        // Set up the sensor for Accelerometer
        mma.setUpAccelerometer();
        delay(500);

        // Display the results for Acceleration
        mma.Measure_Accelerometer();

        // Output Data to Screen
        Serial.print("Acceleration in X-Axis: ");
        Serial.println(mma.mma_accelData.X);
        Serial.print("Acceleration in Y-Axis: ");
        Serial.println(mma.mma_accelData.Y);
        Serial.print("Acceleration in Z-Axis: ");
        Serial.println(mma.mma_accelData.Z);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
        delay(1000);
    }
    else
    {
        Serial.println("MMA7660FC Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }


    delay(1000);
}
