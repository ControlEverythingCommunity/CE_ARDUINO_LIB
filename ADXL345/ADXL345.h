/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADXL345
        This code is designed to work with the ADXL345_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-accelerometer-13-bit?variant=25687608267#tabs-0-product_tabset-2
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
    #define ADXL345_DEFAULT_ADDRESS_ACCEL                       (0x1D)
    #define ADXL345_ADDRESS_ACCEL_UPDATED                       (0x53)
    #define ADXL345_ACCEL_DEV_ID                                (0xE5)      // 11100101

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADXL345_CONVERSIONDELAY                             (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define ADXL345_REG_ACCEL_DEVID                             (0x00)    // Device ID
    #define ADXL345_REG_ACCEL_THRESH_TAP                        (0x1D)    // Tap Threshold
    #define ADXL345_REG_ACCEL_OFSX                              (0x1E)    // X-Axis Offset
    #define ADXL345_REG_ACCEL_OFSY                              (0x1F)    // Y-Axis Offset
    #define ADXL345_REG_ACCEL_OFSZ                              (0x20)    // Z-Axis Offset
    #define ADXL345_REG_ACCEL_DUR                               (0x21)    // Tap Duration
    #define ADXL345_REG_ACCEL_LATENT                            (0x22)    // Tap Latency
    #define ADXL345_REG_ACCEL_WINDOW                            (0x23)    // Tap Window
    #define ADXL345_REG_ACCEL_THRESH_ACT                        (0x24)    // Activity Threshold
    #define ADXL345_REG_ACCEL_THRESH_INACT                      (0x25)    // Inactivity Threshold
    #define ADXL345_REG_ACCEL_TIME_INACT                        (0x26)    // Inactivity Time
    #define ADXL345_REG_ACCEL_ACT_INACT_CTL                     (0x27)    // Axis Enable Control for Activity and Inactivity Detection
    #define ADXL345_REG_ACCEL_THRESH_FF                         (0x28)    // Free-Fall Threshold
    #define ADXL345_REG_ACCEL_TIME_FF                           (0x29)    // Free-Dall Time
    #define ADXL345_REG_ACCEL_TAP_AXES                          (0x2A)    // Axis Control for Single/Double Tap
    #define ADXL345_REG_ACCEL_ACT_TAP_STATUS                    (0x2B)    // Source for Single/Double Tap
    #define ADXL345_REG_ACCEL_BW_RATE                           (0x2C)    // Data Rate and Power Mode Control
    #define ADXL345_REG_ACCEL_POWER_CTL                         (0x2D)    // Power-Saving Features Control
    #define ADXL345_REG_ACCEL_INT_ENABLE                        (0x2E)    // Interrupt Enable Control
    #define ADXL345_REG_ACCEL_INT_MAP                           (0x2F)    // Interrupt Mapping Control
    #define ADXL345_REG_ACCEL_INT_SOURCE                        (0x30)    // Source of Interrupts
    #define ADXL345_REG_ACCEL_DATA_FORMAT                       (0x31)    // Data Format Control
    #define ADXL345_REG_ACCEL_DATAX0                            (0x32)    // X-Axis Data 0
    #define ADXL345_REG_ACCEL_DATAX1                            (0x33)    // X-Axis Data 1
    #define ADXL345_REG_ACCEL_DATAY0                            (0x34)    // Y-Axis Data 0
    #define ADXL345_REG_ACCEL_DATAY1                            (0x35)    // Y-Axis Data 1
    #define ADXL345_REG_ACCEL_DATAZ0                            (0x36)    // Z-Axis Data 0
    #define ADXL345_REG_ACCEL_DATAZ1                            (0x37)    // Z-Axis Data 1
    #define ADXL345_REG_ACCEL_FIFO_CTL                          (0x38)    // FIFO Control
    #define ADXL345_REG_ACCEL_FIFO_STATUS                       (0x39)    // FIFO Status

/**************************************************************************
    ACCELEROMETER BW_RATE REGISTER DESCRIPTION
**************************************************************************/
    #define ADXL345_REG_ACCEL_BW_RATE_LOW_POWER_MASK            (0x10)      // Low_Power Bit
    #define ADXL345_REG_ACCEL_BW_RATE_LOW_POWER_NORMAL          (0x00)      // Normal Operation
    #define ADXL345_REG_ACCEL_BW_RATE_LOW_POWER_REDUCED         (0x10)      // Reduced Operation

    #define ADXL345_REG_ACCEL_BW_RATE_MASK                      (0x0F)      // Device Bandwidth and Output Data Rate
    #define ADXL345_REG_ACCEL_BW_RATE_3200_HZ                   (0x0F)      // Bandwidth: 1600Hz,   IDD: 140µA
    #define ADXL345_REG_ACCEL_BW_RATE_1600_HZ                   (0x0E)      // Bandwidth: 800Hz,    IDD: 90µA
    #define ADXL345_REG_ACCEL_BW_RATE_800_HZ                    (0x0D)      // Bandwidth: 400Hz,    IDD: 140µA
    #define ADXL345_REG_ACCEL_BW_RATE_400_HZ                    (0x0C)      // Bandwidth: 200Hz,    IDD: 140µA
    #define ADXL345_REG_ACCEL_BW_RATE_200_HZ                    (0x0B)      // Bandwidth: 100Hz,    IDD: 140µA
    #define ADXL345_REG_ACCEL_BW_RATE_100_HZ                    (0x0A)      // Bandwidth: 50Hz,     IDD: 140µA
    #define ADXL345_REG_ACCEL_BW_RATE_50_HZ                     (0x09)      // Bandwidth: 25Hz,     IDD: 90µA
    #define ADXL345_REG_ACCEL_BW_RATE_25_HZ                     (0x08)      // Bandwidth: 12.5Hz,   IDD: 60µA
    #define ADXL345_REG_ACCEL_BW_RATE_12_5_HZ                   (0x07)      // Bandwidth: 6.25Hz,   IDD: 50µA
    #define ADXL345_REG_ACCEL_BW_RATE_6_25_HZ                   (0x06)      // Bandwidth: 3.13Hz,   IDD: 45µA
    #define ADXL345_REG_ACCEL_BW_RATE_3_13_HZ                   (0x05)      // Bandwidth: 1.56Hz,   IDD: 40µA
    #define ADXL345_REG_ACCEL_BW_RATE_1_56_HZ                   (0x04)      // Bandwidth: 0.78Hz,   IDD: 34µA
    #define ADXL345_REG_ACCEL_BW_RATE_0_78_HZ                   (0x03)      // Bandwidth: 0.39Hz,   IDD: 23µA
    #define ADXL345_REG_ACCEL_BW_RATE_0_39_HZ                   (0x02)      // Bandwidth: 0.20Hz,   IDD: 23µA
    #define ADXL345_REG_ACCEL_BW_RATE_0_20_HZ                   (0x01)      // Bandwidth: 0.10Hz,   IDD: 23µA
    #define ADXL345_REG_ACCEL_BW_RATE_0_10_HZ                   (0x00)      // Bandwidth: 0.05Hz,   IDD: 23µA

/**************************************************************************
    ACCELEROMETER POWER_CTL REGISTER DESCRIPTION
**************************************************************************/
    #define ADXL345_REG_ACCEL_POWER_CTL_LINK_MASK               (0x20)      // Link Bit
    #define ADXL345_REG_ACCEL_POWER_CTL_LINK_CONCURRENT         (0x00)      // The Inactivity and Activity Functions are Concurrent
    #define ADXL345_REG_ACCEL_POWER_CTL_LINK_ENABLED            (0x20)      // Enabled Delays the Start of the Activity Function Until Inactivity is Detected

    #define ADXL345_REG_ACCEL_POWER_CTL_AUTO_SLEEP_MASK         (0x10)      // Auto-Sleep Bit
    #define ADXL345_REG_ACCEL_POWER_CTL_AUTO_SLEEP_DISABLE      (0x00)      // Disables Automatic Switching to Sleep Mode
    #define ADXL345_REG_ACCEL_POWER_CTL_AUTO_SLEEP_ENABLE       (0x10)      // Enables Automatic Switching to Sleep Mode

    #define ADXL345_REG_ACCEL_POWER_CTL_MEASURE_MASK            (0x08)      // Measurement Mode
    #define ADXL345_REG_ACCEL_POWER_CTL_MEASURE_STANDBY         (0x00)      // Standby Mode
    #define ADXL345_REG_ACCEL_POWER_CTL_MEASURE_NORMAL          (0x08)      // Normal Mode

    #define ADXL345_REG_ACCEL_POWER_CTL_SLEEP_MASK              (0x04)      // Auto-Sleep Bit
    #define ADXL345_REG_ACCEL_POWER_CTL_SLEEP_DISABLE           (0x00)      // Normal Mode of Operation
    #define ADXL345_REG_ACCEL_POWER_CTL_SLEEP_ENABLE            (0x04)      // Sleep Mode of Operation

    #define ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_MASK             (0x03)      // Wakeup Bits
    #define ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_8_HZ             (0x00)      // Frequency of Readings in Sleep Mode: 8Hz
    #define ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_4_HZ             (0x01)      // Frequency of Readings in Sleep Mode: 4Hz
    #define ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_2_HZ             (0x02)      // Frequency of Readings in Sleep Mode: 2Hz
    #define ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_1_HZ             (0x03)      // Frequency of Readings in Sleep Mode: 1Hz

/**************************************************************************
    ACCELEROMETER DATA_FORMAT REGISTER DESCRIPTION
**************************************************************************/
    #define ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_MASK            (0x03)      // Full Scale Range
    #define ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_2G              (0x00)      // g Range: ±2 g
    #define ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_4G              (0x01)      // g Range: ±4 g
    #define ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_8G              (0x02)      // g Range: ±8 g
    #define ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_16G             (0x03)      // g Range: ±16 g

typedef enum
{
    ADXL345_DATARATE_3200HZ        = ADXL345_REG_ACCEL_BW_RATE_3200_HZ,
    ADXL345_DATARATE_1600HZ        = ADXL345_REG_ACCEL_BW_RATE_1600_HZ,
    ADXL345_DATARATE_800HZ         = ADXL345_REG_ACCEL_BW_RATE_800_HZ,
    ADXL345_DATARATE_400HZ         = ADXL345_REG_ACCEL_BW_RATE_400_HZ,
    ADXL345_DATARATE_200HZ         = ADXL345_REG_ACCEL_BW_RATE_200_HZ,
    ADXL345_DATARATE_100HZ         = ADXL345_REG_ACCEL_BW_RATE_100_HZ,
    ADXL345_DATARATE_50HZ          = ADXL345_REG_ACCEL_BW_RATE_50_HZ,
    ADXL345_DATARATE_25HZ          = ADXL345_REG_ACCEL_BW_RATE_25_HZ,
    ADXL345_DATARATE_12_5HZ        = ADXL345_REG_ACCEL_BW_RATE_12_5_HZ,
    ADXL345_DATARATE_6_25HZ         = ADXL345_REG_ACCEL_BW_RATE_6_25_HZ,
    ADXL345_DATARATE_3_13HZ        = ADXL345_REG_ACCEL_BW_RATE_3_13_HZ,
    ADXL345_DATARATE_1_56HZ        = ADXL345_REG_ACCEL_BW_RATE_1_56_HZ,
    ADXL345_DATARATE_0_78HZ        = ADXL345_REG_ACCEL_BW_RATE_0_78_HZ,
    ADXL345_DATARATE_0_39HZ        = ADXL345_REG_ACCEL_BW_RATE_0_39_HZ,
    ADXL345_DATARATE_0_20HZ        = ADXL345_REG_ACCEL_BW_RATE_0_20_HZ,
    ADXL345_DATARATE_0_10HZ        = ADXL345_REG_ACCEL_BW_RATE_0_10_HZ
    
} adxlAccelDataRate_t;

typedef enum
{
    ADXL345_RANGE_2G               = ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_2G,
    ADXL345_RANGE_4G               = ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_4G,
    ADXL345_RANGE_8G               = ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_8G,
    ADXL345_RANGE_16G              = ADXL345_REG_ACCEL_DATA_FORMAT_RANGE_16G
    
} adxlAccelRange_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} adxlSensorData_t;


class ADXL345
{
    protected:
        // Instance-specific properties
        uint8_t adxl_conversionDelay;
        adxlAccelDataRate_t adxl_acceldatarate;
        adxlAccelRange_t adxl_accelrange;

    public:
        uint8_t adxl_i2cAddress;
        adxlSensorData_t adxl_accelData;
        void getAddr_ADXL345(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpAccelerometer(void);
        void Measure_Accelerometer(void);
        void setAccelDataRate(adxlAccelDataRate_t acceldatarate);
        adxlAccelDataRate_t getAccelDataRate(void);
        void setAccelRange(adxlAccelRange_t accelrange);
        adxlAccelRange_t getAccelRange(void);

    private:
};
