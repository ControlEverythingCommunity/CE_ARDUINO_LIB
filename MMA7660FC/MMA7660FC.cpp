/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7660FC
        This code is designed to work with the MMA7660FC_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-orientation-motion-detection-sensor-1-5-g-accelerometer?variant=25687536907#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MMA7660FC.h"

/**************************************************************************/
/*
        Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static uint8_t i2cread(void)
{
    #if ARDUINO >= 100
        return Wire.read();
    #else
        return Wire.receive();
    #endif
}

/**************************************************************************/
/*
        Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static void i2cwrite(uint8_t x)
{
    #if ARDUINO >= 100
        Wire.write((uint8_t)x);
    #else
        Wire.send(x);
    #endif
}

/**************************************************************************/
/*
        Writes 8-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits to the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (int8_t)(i2cread());
}


/**************************************************************************/
/*
        Instantiates a new MMA7660FC class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void MMA7660FC::getAddr_MMA7660FC(uint8_t i2cAddress)
{
    mma_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MMA7660FC::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Interrupt Output Active Status
*/
/**************************************************************************/
void MMA7660FC::setAccelInterrupt1(mmaAccelInterrupt1_t accelinterrupt1)
{
    mma_accelinterrupt1 = accelinterrupt1;
}

/**************************************************************************/
/*
        Gets the Interrupt Output Active Status
*/
/**************************************************************************/
mmaAccelInterrupt1_t MMA7660FC::getAccelInterrupt1()
{
    return mma_accelinterrupt1;
}

/**************************************************************************/
/*
        Sets the Interrupt Output Open Drain/Push-Pull Status
*/
/**************************************************************************/
void MMA7660FC::setAccelInterrupt2(mmaAccelInterrupt2_t accelinterrupt2)
{
    mma_accelinterrupt2 = accelinterrupt2;
}

/**************************************************************************/
/*
        Gets the Interrupt Output Open Drain/Push-Pull Status
*/
/**************************************************************************/
mmaAccelInterrupt2_t MMA7660FC::getAccelInterrupt2()
{
    return mma_accelinterrupt2;
}

/**************************************************************************/
/*
        Sets the Prescaler
*/
/**************************************************************************/
void MMA7660FC::setAccelPrescaler(mmaAccelPrescaler_t accelprescaler)
{
    mma_accelprescaler = accelprescaler;
}

/**************************************************************************/
/*
        Gets the Prescaler
*/
/**************************************************************************/
mmaAccelPrescaler_t MMA7660FC::getAccelPrescaler()
{
    return mma_accelprescaler;
}

/**************************************************************************/
/*
        Sets the Auto-Sleep Functionality
*/
/**************************************************************************/
void MMA7660FC::setAccelAutoSleep(mmaAccelAutoSleep_t accelautosleep)
{
    mma_accelautosleep = accelautosleep;
}

/**************************************************************************/
/*
        Gets the Auto-Sleep Functionality
*/
/**************************************************************************/
mmaAccelAutoSleep_t MMA7660FC::getAccelAutoSleep()
{
    return mma_accelautosleep;
}

/**************************************************************************/
/*
        Sets the Auto-Wake Functionality
*/
/**************************************************************************/
void MMA7660FC::setAccelAutoWake(mmaAccelAutoWake_t accelautowake)
{
    mma_accelautowake = accelautowake;
}

/**************************************************************************/
/*
        Gets the Auto-Wake Functionality
*/
/**************************************************************************/
mmaAccelAutoWake_t MMA7660FC::getAccelAutoWake()
{
    return mma_accelautowake;
}

/**************************************************************************/
/*
        Sets the TON Status
*/
/**************************************************************************/
void MMA7660FC::setAccelTon(mmaAccelTon_t accelton)
{
    mma_accelton = accelton;
}

/**************************************************************************/
/*
        Gets the TON Status
*/
/**************************************************************************/
mmaAccelTon_t MMA7660FC::getAccelTon()
{
    return mma_accelton;
}

/**************************************************************************/
/*
        Sets the MODE Status
*/
/**************************************************************************/
void MMA7660FC::setAccelMode(mmaAccelMode_t accelmode)
{
    mma_accelmode = accelmode;
}

/**************************************************************************/
/*
        Gets the MODE Status
*/
/**************************************************************************/
mmaAccelMode_t MMA7660FC::getAccelMode()
{
    return mma_accelmode;
}


/**************************************************************************/
/*
        Sets the FILT
*/
/**************************************************************************/
void MMA7660FC::setAccelFilt(mmaAccelFilt_t accelfilt)
{
    mma_accelfilt = accelfilt;
}

/**************************************************************************/
/*
        Gets the FILT
*/
/**************************************************************************/
mmaAccelFilt_t MMA7660FC::getAccelFilt()
{
    return mma_accelfilt;
}

/**************************************************************************/
/*
        Sets the Auto-Wake Mode Sample Rate (AWSR)
*/
/**************************************************************************/
void MMA7660FC::setAccelWakeSample(mmaAccelWakeSample_t accelwakesample)
{
    mma_accelwakesample = accelwakesample;
}

/**************************************************************************/
/*
        Gets Auto-Wake Mode Sample Rate (AWSR)
*/
/**************************************************************************/
mmaAccelWakeSample_t MMA7660FC::getAccelWakeSample()
{
    return mma_accelwakesample;
}

/**************************************************************************/
/*
        Sets the Auto-Sleep Mode Sample Rate (AMSR)
*/
/**************************************************************************/
void MMA7660FC::setAccelSleepSample(mmaAccelSleepSample_t accelsleepsample)
{
    mma_accelsleepsample = accelsleepsample;
}

/**************************************************************************/
/*
        Gets Auto-Sleep Mode Sample Rate (AMSR)
*/
/**************************************************************************/
mmaAccelSleepSample_t MMA7660FC::getAccelSleepSample()
{
    return mma_accelsleepsample;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void MMA7660FC::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Mode Register
    /*
     // Set the Interrupt Output Active Status
     uint8_t mode = mma_accelinterrupt1;
    
     // Set the Interrupt Output Open Drain/Push-Pull Status
     mode |= mma_accelinterrupt2;
    
     // Set the Prescaler
     mode |= mma_accelprescaler;
    
     // Set the Auto-Sleep Functionality
     mode |= mma_accelautosleep;
     
     // Set the Auto-Wake Functionality
     mode |= mma_accelautowake;
     
     // Set the TON
     mode |= mma_accelton;
     
     // Set the MODE
     mode |= mma_accelmode;
    */
    
    uint8_t mode =  MMA7660FC_REG_ACCEL_MODE_IAH_LOW            |   // Interrupt Output INT is Active Low
                    MMA7660FC_REG_ACCEL_MODE_IPP_OPEN_DRAIN     |   // Interrupt Output INT is Active Open Drain
                    MMA7660FC_REG_ACCEL_MODE_SCPS_1             |   // The Prescaler is Divide-by-1
                    MMA7660FC_REG_ACCEL_MODE_ASE_DISABLED       |   // Auto-Sleep Disabled
                    MMA7660FC_REG_ACCEL_MODE_AWE_DISABLED       |   // Auto-Wake Disabled
                    MMA7660FC_REG_ACCEL_MODE_TON_STANDBY        |   // Standby Mode or Active Mode Depending on State of MODE
                    MMA7660FC_REG_ACCEL_MODE_MODE_ACTIVE;           // Active Mode
    
    // Write the configuration to the Accelerometer Mode Control Register
    writeRegister(mma_i2cAddress, MMA7660FC_REG_ACCEL_MODE, mode);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Sample Rate Register
    /*
     // Set the FILT
     uint8_t sample_rate = mma_accelbandwidth;
     
     // Set the Auto-Wake Mode Sample Rate (AWSR)
     sample_rate |= mma_accelthreshold;
     
     // Set the Auto-Sleep Mode Sample Rate (AMSR)
     sample_rate |= mma_accelyda;
    */
    
    uint8_t sample_rate =   MMA7660FC_REG_ACCEL_SR_FILT_DISABLED        |   // Tilt Debounce Filtering is Disabled
                            MMA7660FC_REG_ACCEL_SR_AWSR_32              |   // 32 Samples/Second Auto-Wake Mode
                            MMA7660FC_REG_ACCEL_SR_AMSR_1;                  // 1 Sample/Second Active and Auto-Sleep Mode
    
    // Write the configuration to the Accelerometer Sample Rate Register
    writeRegister(mma_i2cAddress, MMA7660FC_REG_ACCEL_SR, sample_rate);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 6 bit as two’s complement
*/
/**************************************************************************/
void MMA7660FC::Measure_Accelerometer()
{
    // Read the Accelerometer
    int8_t xAccel, yAccel, zAccel;
    
    // Read the Data
    // Reading the 3 Axes Acceleration Data Register
    Wire.beginTransmission(mma_i2cAddress);
    i2cwrite((uint8_t)MMA7660FC_REG_ACCEL_XOUT);
    Wire.endTransmission();
    Wire.requestFrom(mma_i2cAddress, (uint8_t)3);
    xAccel = i2cread();
    yAccel = i2cread();
    zAccel = i2cread();
    
    // Conversion of the data
    if (xAccel > 31)
    {
        xAccel -= 64;
    }
    // 6-bit signed result for X-Axis Acceleration Data of MMA7660FC
    mma_accelData.X = (int8_t)(xAccel);
    
    // Conversion of the data
    if (yAccel > 31)
    {
        yAccel -= 64;
    }
    // 6-bit signed result for Y-Axis Acceleration Data of MMA7660FC
    mma_accelData.Y = (int8_t)(yAccel);
    
    // Conversion of the data
    if (zAccel > 31)
    {
        zAccel -= 64;
    }
    // 6-bit signed result for Z-Axis Acceleration Data of MMA7660FC
    mma_accelData.Z = (int8_t)(zAccel);
}

