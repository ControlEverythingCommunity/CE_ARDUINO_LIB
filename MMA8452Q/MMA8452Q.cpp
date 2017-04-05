/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA8452Q
        This code is designed to work with the MMA8452Q_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MMA8452Q.h"

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
    return (uint8_t)(i2cread());
}


/**************************************************************************/
/*
        Instantiates a new MMA8452Q class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void MMA8452Q::getAddr_MMA8452Q(uint8_t i2cAddress)
{
    mma_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MMA8452Q::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(mma_i2cAddress, MMA8452Q_REG_ACCEL_WHO_AM_I);
    if (devid != MMA8452Q_DEFAULT_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data High-Pass Filtered
*/
/**************************************************************************/
void MMA8452Q::setAccelHPF(mmaAccelHPF_t accelhpf)
{
    mma_accelhpf = accelhpf;
}

/**************************************************************************/
/*
        Gets the Output Data High-Pass Filtered
*/
/**************************************************************************/
mmaAccelHPF_t MMA8452Q::getAccelHPF()
{
    return mma_accelhpf;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void MMA8452Q::setAccelRange(mmaAccelRange_t accelrange)
{
    mma_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
mmaAccelRange_t MMA8452Q::getAccelRange()
{
    return mma_accelrange;
}

/**************************************************************************/
/*
        Sets the Configures the Auto-Wake Sample Frequency 
        When the Device is in Sleep Mode
*/
/**************************************************************************/
void MMA8452Q::setAccelFrequency(mmaAccelFrequency_t accelfrequency)
{
    mma_accelfrequency = accelfrequency;
}

/**************************************************************************/
/*
        Gets the Configures the Auto-Wake Sample Frequency 
        When the Device is in Sleep Mode
*/
/**************************************************************************/
mmaAccelFrequency_t MMA8452Q::getAccelFrequency()
{
    return mma_accelfrequency;
}

/**************************************************************************/
/*
        Sets the Data Rate Selection
*/
/**************************************************************************/
void MMA8452Q::setAccelBandwidth(mmaAccelBandwidth_t accelbandwidth)
{
    mma_accelbandwidth = accelbandwidth;
}

/**************************************************************************/
/*
        Gets the Data Rate Selection
*/
/**************************************************************************/
mmaAccelBandwidth_t MMA8452Q::getAccelBandwidth()
{
    return mma_accelbandwidth;
}

/**************************************************************************/
/*
        Sets the Reduced Noise Reduced Maximum Range Mode
*/
/**************************************************************************/
void MMA8452Q::setAccelNoise(mmaAccelNoise_t accelnoise)
{
    mma_accelnoise = accelnoise;
}

/**************************************************************************/
/*
        Gets the Reduced Noise Reduced Maximum Range Mode
*/
/**************************************************************************/
mmaAccelNoise_t MMA8452Q::getAccelNoise()
{
    return mma_accelnoise;
}

/**************************************************************************/
/*
        Sets the Fast-Read Mode
*/
/**************************************************************************/
void MMA8452Q::setAccelFastRead(mmaAccelFastRead_t accelfastread)
{
    mma_accelfastread = accelfastread;
}

/**************************************************************************/
/*
        Gets the Fast-Read Mode
*/
/**************************************************************************/
mmaAccelFastRead_t MMA8452Q::getAccelFastRead()
{
    return mma_accelfastread;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection
*/
/**************************************************************************/
void MMA8452Q::setAccelMode(mmaAccelMode_t accelmode)
{
    mma_accelmode = accelmode;
}

/**************************************************************************/
/*
        Gets the Full-Scale Selection
*/
/**************************************************************************/
mmaAccelMode_t MMA8452Q::getAccelMode()
{
    return mma_accelmode;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void MMA8452Q::setAccelSelfTest(mmaAccelSelfTest_t accelselftest)
{
    mma_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
mmaAccelSelfTest_t MMA8452Q::getAccelSelfTest()
{
    return mma_accelselftest;
}

/**************************************************************************/
/*
        Sets the Software Reset Mode
*/
/**************************************************************************/
void MMA8452Q::setAccelReset(mmaAccelReset_t accelreset)
{
    mma_accelreset = accelreset;
}

/**************************************************************************/
/*
        Gets the Software Reset Mode
*/
/**************************************************************************/
mmaAccelReset_t MMA8452Q::getAccelReset()
{
    return mma_accelreset;
}

/**************************************************************************/
/*
        Sets the Sleep Mode Power Scheme Selection
*/
/**************************************************************************/
void MMA8452Q::setAccelSleepPower(mmaAccelSleepPower_t accelsleeppower)
{
    mma_accelsleeppower = accelsleeppower;
}

/**************************************************************************/
/*
        Gets the Sleep Mode Power Scheme Selection
*/
/**************************************************************************/
mmaAccelSleepPower_t MMA8452Q::getAccelSleepPower()
{
    return mma_accelsleeppower;
}

/**************************************************************************/
/*
        Sets the Auto-Sleep Enable
*/
/**************************************************************************/
void MMA8452Q::setAccelAutoSleep(mmaAccelAutoSleep_t accelautosleep)
{
    mma_accelautosleep = accelautosleep;
}

/**************************************************************************/
/*
        Gets the Auto-Sleep Enable
*/
/**************************************************************************/
mmaAccelAutoSleep_t MMA8452Q::getAccelAutoSleep()
{
    return mma_accelautosleep;
}

/**************************************************************************/
/*
        Sets the Active Mode Power Scheme Selection
*/
/**************************************************************************/
void MMA8452Q::setAccelActivePower(mmaAccelActivePower_t accelactivepower)
{
    mma_accelactivepower = accelactivepower;
}

/**************************************************************************/
/*
        Gets the Active Mode Power Scheme Selection
*/
/**************************************************************************/
mmaAccelActivePower_t MMA8452Q::getAccelActivePower()
{
    return mma_accelactivepower;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void MMA8452Q::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer HPF Data Out and Dynamic Range Settings Register
    /*
     // Output Data High-Pass Filtered
     uint8_t config = mma_accelhpf;
    
     // Set the Output Buffer Data Format Full Scale
     config |= mma_accelrange;
    */
    
    uint8_t config = MMA8452Q_REG_ACCEL_XYZ_DATA_CFG_HPF_OUT_DISABLE;   // Disable Output Data High-Pass
    
    // Set the Output Buffer Data Format Full Scale
    config |= mma_accelrange;
    
    // Write the configuration to the Accelerometer HPF Data Out and Dynamic Range Settings Register
    writeRegister(mma_i2cAddress, MMA8452Q_REG_ACCEL_XYZ_DATA_CFG, config);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
     // Set the Configures the Auto-Wake Sample Frequency When the Device is in Sleep Mode
     uint8_t control1 = mma_accelfrequency;
     
     // Set the Data Rate Selection
     control1 |= mma_accelbandwidth;
     
     // Set the Reduced Noise Reduced Maximum Range Mode
     control1 |= mma_accelnoise;
     
     // Set the Fast-Read Mode
     control1 |= mma_accelfastread;
     
     // Set the Full-Scale Selection
     control1 |= mma_accelmode;
    */
    
    uint8_t control1 =  MMA8452Q_REG_ACCEL_CTRL_REG1_ASLP_RATE_50HZ     |   // Frequency (Hz): 50
                        MMA8452Q_REG_ACCEL_CTRL_REG1_LNOISE_NORMAL      |   // Reduced Noise: Normal Mode
                        MMA8452Q_REG_ACCEL_CTRL_REG1_F_READ_NORMAL      |   // Fast-Read: Normal Mode
                        MMA8452Q_REG_ACCEL_CTRL_REG1_F_ACTIVE_ACTIVE;       // Full-Scale Selection: Full-Scale Selection
    
    // Set the Data Rate Selection
    control1 |= mma_accelbandwidth;
    
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(mma_i2cAddress, MMA8452Q_REG_ACCEL_CTRL_REG1, control1);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 2
    /*
     // Set the Self-Test Mode
     uint8_t control2 = mma_accelselftest;
     
     // Set the Software Reset Mode
     control2 |= mma_accelreset;
     
     // Set the Sleep Mode Power Scheme Selection
     control2 |= mma_accelaleeppower;
     
     // Set the Auto-Sleep Enable
     control2 |= mma_accelautosleep;
     
     // Set the Active Mode Power Scheme Selection
     control2 |= mma_accelactivepower;
     */
    
    uint8_t control2 =  MA8452Q_REG_ACCEL_CTRL_REG2_ST_DISABLE      |   // Self-Test Disabled
                        MA8452Q_REG_ACCEL_CTRL_REG2_RST_DISABLE     |   // Device Reset Disabled
                        MA8452Q_REG_ACCEL_CTRL_REG2_SMODS_NORMAL    |   // Power Mode: Normal
                        MA8452Q_REG_ACCEL_CTRL_REG2_SLPE_DISABLE    |   // Auto-Sleep Disabled
                        MA8452Q_REG_ACCEL_CTRL_REG2_MODS_NORMAL;        // Power Mode: Normal
    
    // Write the configuration to the Accelerometer Control Register 2
    writeRegister(mma_i2cAddress, MMA8452Q_REG_ACCEL_CTRL_REG2, control2);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 12 bit as two’s complement
*/
/**************************************************************************/
void MMA8452Q::Measure_Accelerometer()
{
    uint8_t buffer[7];
    // Read the Accelerometer
    Wire.beginTransmission(mma_i2cAddress);
    i2cwrite((uint8_t)MMA8452Q_REG_ACCEL_STATUS);
    Wire.endTransmission();
    
    Wire.requestFrom(mma_i2cAddress, (uint8_t)7);
    buffer[0] = i2cread();    // Reading the Status Data Register
    buffer[1] = i2cread();    // Reading the High X-Axis Acceleration Data Register
    buffer[2] = i2cread();    // Reading the Low X-Axis Acceleration Data Register
    buffer[3] = i2cread();    // Reading the High Y-Axis Acceleration Data Register
    buffer[4] = i2cread();    // Reading the Low Y-Axis Acceleration Data Register
    buffer[5] = i2cread();    // Reading the High Z-Axis Acceleration Data Register
    buffer[6] = i2cread();    // Reading the Low Z-Axis Acceleration Data Register

    // Conversion of the result
    // 12-bit signed result for X-Axis Acceleration Data of MMA8452Q
    mma_accelData.X = (int16_t)((buffer[1] << 8) | buffer[2]);
    mma_accelData.X >>= 4;
    
    // 12-bit signed result for Y-Axis Acceleration Data of MMA8452Q
    mma_accelData.Y = (int16_t)((buffer[3] << 8) | buffer[4]);
    mma_accelData.Y >>= 4;
    
    // 12-bit signed result for Z-Axis Acceleration Data of MMA8452Q
    mma_accelData.Z = (int16_t)((buffer[5] << 8) | buffer[6]);
    mma_accelData.Z >>= 4;
    
}

