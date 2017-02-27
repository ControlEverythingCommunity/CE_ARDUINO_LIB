/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7455
        This code is designed to work with the MMA7455_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/
    #define MMA7455_DEFAULT_ADDRESS_ACCEL                   (0x1D)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MMA7455_CONVERSIONDELAY                         (100)

/**************************************************************************
    ACCELNETOMETER REGISTERS
**************************************************************************/
    #define MMA7455_REG_ACCEL_XOUTL                         (0x00)      // 10-Bits X-Axis Acceleration Data Low Register
    #define MMA7455_REG_ACCEL_XOUTH                         (0x01)      // 10-Bits X-Axis Acceleration Data High Register
    #define MMA7455_REG_ACCEL_YOUTL                         (0x02)      // 10-Bits Y-Axis Acceleration Data Low Register
    #define MMA7455_REG_ACCEL_YOUTH                         (0x03)      // 10-Bits Y-Axis Acceleration Data High Register
    #define MMA7455_REG_ACCEL_ZOUTL                         (0x04)      // 10-Bits Z-Axis Acceleration Data Low Register
    #define MMA7455_REG_ACCEL_ZOUTH                         (0x05)      // 10-Bits Z-Axis Acceleration Data High Register
    #define MMA7455_REG_ACCEL_XOUT                          (0x06)      // 8-Bits X-Axis Acceleration Data Register
    #define MMA7455_REG_ACCEL_YOUT                          (0x07)      // 8-Bits Y-Axis Acceleration Data Register
    #define MMA7455_REG_ACCEL_ZOUT                          (0x08)      // 8-Bits Z-Axis Acceleration Data Register
    #define MMA7455_REG_ACCEL_STATUS                        (0x09)      // Status Registers
    #define MMA7455_REG_ACCEL_DETSRC                        (0x0A)      // Detection Source Registers
    #define MMA7455_REG_ACCEL_TOUT                          (0x0B)      // Temperature Output Value Register
    #define MMA7455_REG_ACCEL_I2CAD                         (0x0D)      // I2C Device Address Register
    #define MMA7455_REG_ACCEL_USRINF                        (0x0E)      // User Information Register
    #define MMA7455_REG_ACCEL_WHO_AM_I                      (0x0F)      // Device ID Number Register
    #define MMA7455_REG_ACCEL_XOFFL                         (0x10)      // X-Axis Offset Drift Low Register
    #define MMA7455_REG_ACCEL_XOFFH                         (0x11)      // X-Axis Offset Drift High Register
    #define MMA7455_REG_ACCEL_YOFFL                         (0x12)      // Y-Axis Offset Drift Low Register
    #define MMA7455_REG_ACCEL_YOFFH                         (0x13)      // Y-Axis Offset Drift High Register
    #define MMA7455_REG_ACCEL_ZOFFL                         (0x14)      // Z-Axis Offset Drift Low Register
    #define MMA7455_REG_ACCEL_ZOFFH                         (0x15)      // Z-Axis Offset Drift High Register
    #define MMA7455_REG_ACCEL_MCTL                          (0x16)      // Mode Control Register
    #define MMA7455_REG_ACCEL_INTRST                        (0x17)      // Interrupt Latch Reset Register
    #define MMA7455_REG_ACCEL_CTL1                          (0x18)      // Control Register 1
    #define MMA7455_REG_ACCEL_CTL2                          (0x19)      // Control Register 2
    #define MMA7455_REG_ACCEL_LDTH                          (0x1A)      // Level Detection Threshold Limit Value Register
    #define MMA7455_REG_ACCEL_PDTH                          (0x1B)      // Pulse Detection Threshold Limit Value Register
    #define MMA7455_REG_ACCEL_PW                            (0x1C)      // Pulse Duration Value Register
    #define MMA7455_REG_ACCEL_LT                            (0x1D)      // Latency Time Value Register
    #define MMA7455_REG_ACCEL_TW                            (0x1E)      // Time Window for 2nd Pulse Value Register

/**************************************************************************
    ACCELEROMETER MODE CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define MMA7455_REG_ACCEL_MCTL_DRPD_MASK                (0x40)      // DRPD
    #define MMA7455_REG_ACCEL_MCTL_DRPD_OUTPUT              (0x00)      // Data Ready Status is Output to INT1/DRDY PIN
    #define MMA7455_REG_ACCEL_MCTL_DRPD_NO_OUTPUT           (0x40)      // Data Ready Status is Not Output to INT1/DRDY PIN

    #define MMA7455_REG_ACCEL_MCTL_SPI3W_MASK               (0x20)      // SPI Serial Interface Mode Selection
    #define MMA7455_REG_ACCEL_MCTL_SPI3W_4WIRE              (0x00)      // 4-Wire Interface
    #define MMA7455_REG_ACCEL_MCTL_SPI3W_3WIRE              (0x20)      // 3-Wire Interface

    #define MMA7455_REG_ACCEL_MCTL_STON_MASK                (0x10)      // Self-Test Mode
    #define MMA7455_REG_ACCEL_MCTL_STON_DISABLE             (0x00)      // Self-Test Disabled
    #define MMA7455_REG_ACCEL_MCTL_STON_ENABLE              (0x10)      // Self-Test Enabled

    #define MMA7455_REG_ACCEL_MCTL_GLVL_MASK                (0x0C)      // Acceleration Measurement Range
    #define MMA7455_REG_ACCEL_MCTL_GLVL_8G                  (0x00)      // 8g
    #define MMA7455_REG_ACCEL_MCTL_GLVL_4G                  (0x08)      // 4g
    #define MMA7455_REG_ACCEL_MCTL_GLVL_2G                  (0x04)      // 2g

    #define MMA7455_REG_ACCEL_MCTL_MODE_MASK                (0x03)      // Operating Mode
    #define MMA7455_REG_ACCEL_MCTL_MODE_STANDBY             (0x00)      // Standby Mode
    #define MMA7455_REG_ACCEL_MCTL_MODE_MEAS                (0x01)      // Measurement Mode
    #define MMA7455_REG_ACCEL_MCTL_MODE_LEVEL               (0x02)      // Level Detection Mode
    #define MMA7455_REG_ACCEL_MCTL_MODE_PULSE               (0x03)      // Pulse Detection Mode

/**************************************************************************
    ACCELEROMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define MMA7455_REG_ACCEL_CTL1_DFBW_MASK                (0x80)      // Digital Filter Band Width
    #define MMA7455_REG_ACCEL_CTL1_DFBW_62_5HZ              (0x00)      // Digital Filter Band Width: 62.5 Hz
    #define MMA7455_REG_ACCEL_CTL1_DFBW_125HZ               (0x80)      // Digital Filter Band Width: 125 Hz

    #define MMA7455_REG_ACCEL_CTL1_THOPT_MASK               (0x40)      // THOPT
    #define MMA7455_REG_ACCEL_CTL1_THOPT_ABSOLUTE           (0x00)      // Threshold Value is Absolute Only
    #define MMA7455_REG_ACCEL_CTL1_THOPT_INTEGER            (0x40)      // Integer Value is Available

    #define MMA7455_REG_ACCEL_CTL1_ZDA_MASK                 (0x20)      // Acceleration Z-Axis Enable
    #define MMA7455_REG_ACCEL_CTL1_ZDA_ENABLE               (0x00)      // Acceleration Z-Axis Enabled
    #define MMA7455_REG_ACCEL_CTL1_ZDA_DISABLE              (0x20)      // Acceleration Z-Axis Disabled

    #define MMA7455_REG_ACCEL_CTL1_YDA_MASK                 (0x10)      // Acceleration Y-Axis Enable
    #define MMA7455_REG_ACCEL_CTL1_YDA_ENABLE               (0x00)      // Acceleration Y-Axis ENabled
    #define MMA7455_REG_ACCEL_CTL1_YDA_DISABLE              (0x10)      // Acceleration Y-Axis Disabled

    #define MMA7455_REG_ACCEL_CTL1_XDA_MASK                 (0x08)      // Acceleration X-Axis Enable
    #define MMA7455_REG_ACCEL_CTL1_XDA_ENABLE               (0x00)      // Acceleration X-Axis Enabled
    #define MMA7455_REG_ACCEL_CTL1_XDA_DISABLE              (0x08)      // Acceleration X-Axis Disabled

    #define MMA7455_REG_ACCEL_CTL1_INTREG_MASK              (0x0E)      // Interrupt Settings
    #define MMA7455_REG_ACCEL_CTL1_INTREG_LEVEL             (0x00)      // Level Detection
    #define MMA7455_REG_ACCEL_CTL1_INTREG_PULSE             (0x02)      // Pulse Detection
    #define MMA7455_REG_ACCEL_CTL1_INTREG_PULSE_SINGLE      (0x04)      // Single/Double Pulse Detection


typedef enum
{
    ACCEL_DRPD_OUTPUT                   = MMA7455_REG_ACCEL_MCTL_DRPD_OUTPUT,
    ACCEL_DRPD_NO_OUTPUT                = MMA7455_REG_ACCEL_MCTL_DRPD_NO_OUTPUT
    
} mmaAccelDRPD_t;

typedef enum
{
    ACCEL_SPI_WIRE_4                    = MMA7455_REG_ACCEL_MCTL_SPI3W_4WIRE,
    ACCEL_SPI_WIRE_3                    = MMA7455_REG_ACCEL_MCTL_SPI3W_3WIRE
    
} mmaAccelSPIWire_t;

typedef enum
{
    ACCEL_SELF_TEST_DISABLED            = MMA7455_REG_ACCEL_MCTL_STON_DISABLE,
    ACCEL_SELF_TEST_ENABLED             = MMA7455_REG_ACCEL_MCTL_STON_ENABLE
    
} mmaAccelSelfTest_t;

typedef enum
{
    ACCEL_RANGE_8G                      = MMA7455_REG_ACCEL_MCTL_GLVL_8G,
    ACCEL_RANGE_4G                      = MMA7455_REG_ACCEL_MCTL_GLVL_4G,
    ACCEL_RANGE_2G                      = MMA7455_REG_ACCEL_MCTL_GLVL_2G
    
} mmaAccelRange_t;

typedef enum
{
    ACCEL_MODE_STANDBY                  = MMA7455_REG_ACCEL_MCTL_MODE_STANDBY,
    ACCEL_MODE_MEAS                     = MMA7455_REG_ACCEL_MCTL_MODE_MEAS,
    ACCEL_MODE_LEVEL                    = MMA7455_REG_ACCEL_MCTL_MODE_LEVEL,
    ACCEL_MODE_PULSE                    = MMA7455_REG_ACCEL_MCTL_MODE_PULSE
    
} mmaAccelMode_t;

typedef enum
{
    ACCEL_BANDWIDTH_62_5HZ              = MMA7455_REG_ACCEL_CTL1_DFBW_62_5HZ,
    ACCEL_BANDWIDTH_125HZ               = MMA7455_REG_ACCEL_CTL1_DFBW_125HZ
    
} mmaAccelBandwidth_t;

typedef enum
{
    ACCEL_THRESHOLD_ABSOLUTE            = MMA7455_REG_ACCEL_CTL1_THOPT_ABSOLUTE,
    ACCEL_THRESHOLD_INTEGER             = MMA7455_REG_ACCEL_CTL1_THOPT_INTEGER
    
} mmaAccelThreshold_t;

typedef enum
{
    ACCEL_Z_AXIS_DISABLED               = MMA7455_REG_ACCEL_CTL1_ZDA_DISABLE,
    ACCEL_Z_AXIS_ENABLED                = MMA7455_REG_ACCEL_CTL1_ZDA_ENABLE
    
} mmaAccelZda_t;

typedef enum
{
    ACCEL_Y_AXIS_DISABLED               = MMA7455_REG_ACCEL_CTL1_YDA_DISABLE,
    ACCEL_Y_AXIS_ENABLED                = MMA7455_REG_ACCEL_CTL1_YDA_ENABLE
    
} mmaAccelYda_t;

typedef enum
{
    ACCEL_X_AXIS_DISABLED               = MMA7455_REG_ACCEL_CTL1_XDA_DISABLE,
    ACCEL_X_AXIS_ENABLED                = MMA7455_REG_ACCEL_CTL1_XDA_ENABLE
    
} mmaAccelXda_t;

typedef enum
{
    ACCEL_INTERRUPT_LEVEL               = MMA7455_REG_ACCEL_CTL1_INTREG_LEVEL,
    ACCEL_INTERRUPT_PULSE               = MMA7455_REG_ACCEL_CTL1_INTREG_PULSE,
    ACCEL_INTERRUPT_PULSE_SINGLE        = MMA7455_REG_ACCEL_CTL1_INTREG_PULSE_SINGLE,
    
    
} mmaAccelInterrupt_t;


typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} mmaSensorData_t;


class MMA7455
{
    protected:
        // Instance-specific properties
        uint8_t mma_conversionDelay;
        mmaAccelDRPD_t mma_acceldrpd;
        mmaAccelSPIWire_t mma_accelspiwire;
        mmaAccelSelfTest_t mma_accelselftest;
        mmaAccelRange_t mma_accelrange;
        mmaAccelMode_t mma_accelmode;
        mmaAccelBandwidth_t mma_accelbandwidth;
        mmaAccelThreshold_t mma_accelthreshold;
        mmaAccelZda_t mma_accelzda;
        mmaAccelYda_t mma_accelyda;
        mmaAccelXda_t mma_accelxda;
        mmaAccelInterrupt_t mma_accelinterrupt;

    public:
        uint8_t mma_i2cAddress;
        mmaSensorData_t mma_accelData;
        void getAddr_MMA7455(uint8_t i2cAddress);
        bool begin(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelDRPD(mmaAccelDRPD_t acceldrpd);
        mmaAccelDRPD_t getAccelDRPD(void);
        void setAccelSPIWire(mmaAccelSPIWire_t accelspiwire);
        mmaAccelSPIWire_t getAccelSPIWire(void);
        void setAccelSelfTest(mmaAccelSelfTest_t accelselftest);
        mmaAccelSelfTest_t getAccelSelfTest(void);
        void setAccelRange(mmaAccelRange_t accelrange);
        mmaAccelRange_t getAccelRange(void);
        void setAccelMode(mmaAccelMode_t accelmode);
        mmaAccelMode_t getAccelMode(void);
        void setAccelBandwidth(mmaAccelBandwidth_t accelbandwidth);
        mmaAccelBandwidth_t getAccelBandwidth(void);
        void setAccelThreshold(mmaAccelThreshold_t accelthreshold);
        mmaAccelThreshold_t getAccelThreshold(void);
        void setAccelZda(mmaAccelZda_t accelzda);
        mmaAccelZda_t getAccelZda(void);
        void setAccelYda(mmaAccelYda_t accelyda);
        mmaAccelYda_t getAccelYda(void);
        void setAccelXda(mmaAccelXda_t accelzda);
        mmaAccelXda_t getAccelXda(void);
        void setAccelInterrupt(mmaAccelInterrupt_t accelinterrupt);
        mmaAccelInterrupt_t getAccelInterrupt(void);
    
    private:
};
