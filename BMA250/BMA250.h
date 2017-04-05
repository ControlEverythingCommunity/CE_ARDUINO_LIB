/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMA250
        This code is designed to work with the BMA250_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-ACCELEROMETER AND GYROSCOPE
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
    #define BMA250_DEFAULT_ADDRESS_ACCEL                        (0x18)
    #define BMA250_ADDRESS_ACCEL_UPDATED                        (0x69)
    #define BMA250_DEFAULT_CHIP_ID                              (0x21)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BMA250_CONVERSIONDELAY                              (100)

/**************************************************************************
    ACCELEROMETER REGISTERS
**************************************************************************/
    #define BMA250_REG_ACCEL_CHIP_ID                            (0x00)      // Chip Identification Number Register
    #define BMA250_REG_ACCEL_ACCEL_X_LSB                        (0x02)      // X-Axis Accelerometer Low Data Register
    #define BMA250_REG_ACCEL_ACCEL_X_MSB                        (0x03)      // X-Axis Accelerometer High Data Register
    #define BMA250_REG_ACCEL_ACCEL_Y_LSB                        (0x04)      // Y-Axis Accelerometer Low Data Register
    #define BMA250_REG_ACCEL_ACCEL_Y_MSB                        (0x05)      // Y-Axis Accelerometer High Data Register
    #define BMA250_REG_ACCEL_ACCEL_Z_LSB                        (0x06)      // Z-Axis Accelerometer Low Data Register
    #define BMA250_REG_ACCEL_ACCEL_Z_MSB                        (0x07)      // Z-Axis Accelerometer High Data Register
    #define BMA250_REG_ACCEL_TEMP                               (0x08)      // Temperature Data Register
    #define BMA250_REG_ACCEL_INTR_STATUS                        (0x09)      // Interrupt Status Register
    #define BMA250_REG_ACCEL_NEW_STATUS                         (0x0A)      // New Data Status Register
    #define BMA250_REG_ACCEL_TAP_SLOPE_STATUS                   (0x0B)      // Tap and Slope Interrupt Status Register
    #define BMA250_REG_ACCEL_FLAT_ORIENT_STATUS                 (0x0C)      // Flat and Orientation Status Register
    #define BMA250_REG_ACCEL_G_RANGE                            (0x0F)      // G-Range Selection Register
    #define BMA250_REG_ACCEL_BANDWIDTH                          (0x10)      // Selection of the Bandwidth for the Acceleration Data Register
    #define BMA250_REG_ACCEL_POWER                              (0x11)      // Configuration of the Power Modes Register
    #define BMA250_REG_ACCEL_SPECIAL_CNTL                       (0x13)      // Selection of the Acceleration Data Acquisition and Data Output Format Register
     #define BMA250_REG_ACCEL_SOFTRESET                         (0x14)      // SoftReset Register
    #define BMA250_REG_ACCEL_INTERRUPT_SETTING1                 (0x16)      // Interrupt Settings Register
    #define BMA250_REG_ACCEL_INTERRUPT_SETTING2                 (0x17)      // Interrupt Settings Register
    #define BMA250_REG_ACCEL_INTERRUPT_MAPPING1                 (0x19)      // Interrupt Mapping Register
    #define BMA250_REG_ACCEL_INTERRUPT_MAPPING2                 (0x1A)      // Interrupt Mapping Register
    #define BMA250_REG_ACCEL_INTERRUPT_MAPPING3                 (0x1B)      // Interrupt Mapping Register
    #define BMA250_REG_ACCEL_INTERRUPT_DATA_SRC                 (0x1E)      // Interrupt Data Source Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_ELEC_BEHAVE              (0x20)      // Interrupt Pin Electrical Behaviour Register
    #define BMA250_REG_ACCEL_INTERRUPT_RESET_MODE               (0x21)      // Interrupt Reset and Mode Selection Register
    #define BMA250_REG_ACCEL_INTERRUPT_DELAY_TIME_L             (0x22)      // Low-g Interrupt Delay Time Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_THRESHOLD_H              (0x23)      // Low-g Interrupt Threshold Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_HYSTERESIS               (0x24)      // Low-g Interrupt Hysteresis Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_DELAY_TIME_H             (0x25)      // High-g Interrupt Delay Time Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_THRESHOLD_H              (0x26)      // High-g Interrupt Threshold Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_SAMPLE_NUMBER            (0x27)      // Slope Interrupt Samples Number Definition Register
    #define BMA250_REG_ACCEL_INTERRUPT_THRESHOLD                (0x28)      // Slope Interrupt Threshold Definition Register
    #define BMA250_REG_ACCEL_TAP_QUIET_SHOCK_DUR                (0x2A)      // Tap Quiet Duration and Tap Shock Duration Register
    #define BMA250_REG_ACCEL_INTERRUPT_SAMPLE_NUMBER1           (0x2B)      // Wake-Up and Threshold Interrupt Samples Number Register
    #define BMA250_REG_ACCEL_INTERRUPT_HYSTERESIS_ORIENT        (0x2C)      // Hysteresis, Blocking for Orientation Interrupt Register
    #define BMA250_REG_ACCEL_THETA_BLOCK_ANGLE                  (0x2D)      // Theta Blocking Angle Register
    #define BMA250_REG_ACCEL_FLAT_THRESHOLD_ANGLE               (0x2E)      // Flat Threshold Angle Register
    #define BMA250_REG_ACCEL_SENSOR_SELF_TEST                   (0x32)      // Sensor Self-Test Activation Settings Register
    #define BMA250_REG_ACCEL_EEPROM_CONTROL                     (0x33)      // EEPROM Control Settings Register
    #define BMA250_REG_ACCEL_DIGITAL_INTERFACE                  (0x34)      // Digital Interface Settings Register
    #define BMA250_REG_ACCEL_OFFSET_COMPENSATION_FAST           (0x36)      // Fast Offset Compensation Settings Register
    #define BMA250_REG_ACCEL_OFFSET_COMPENSATION_SLOW           (0x37)      // Slow Offset Compensation Settings Register
    #define BMA250_REG_ACCEL_FILTER_COMPENSATION_X              (0x38)      // Filtered Data Compensation Settings for X-Axis Register
    #define BMA250_REG_ACCEL_FILTER_COMPENSATION_Y              (0x39)      // Filtered Data Compensation Settings for Y-Axis Register
    #define BMA250_REG_ACCEL_FILTER_COMPENSATION_Z              (0x3A)      // Filtered Data Compensation Settings for Z-Axis Register
    #define BMA250_REG_ACCEL_UNFILTER_COMPENSATION_X            (0x3B)      // Unfiltered Data Compensation Settings for X-Axis Register
    #define BMA250_REG_ACCEL_UNFILTER_COMPENSATION_Y            (0x3C)      // Unfiltered Data Compensation Settings for Y-Axis Register
    #define BMA250_REG_ACCEL_UNFILTER_COMPENSATION_Z            (0x3D)      // Unfiltered Data Compensation Settings for Z-Axis Register

/**************************************************************************
    ACCELEROMETER G-RANGE SELCTION REGISTER DESCRIPTION
**************************************************************************/
    #define BMA250_REG_ACCEL_G_RANGE_MASK                       (0x0F)      // Selection of the g-Range
    #define BMA250_REG_ACCEL_G_RANGE_2G                         (0x03)      // g-Range: ± 2g
    #define BMA250_REG_ACCEL_G_RANGE_4G                         (0x05)      // g-Range: ± 4g
    #define BMA250_REG_ACCEL_G_RANGE_8G                         (0x08)      // g-Range: ± 8g
    #define BMA250_REG_ACCEL_G_RANGE_16G                        (0x0C)      // g-Range: ± 16g

/**************************************************************************
    ACCELEROMETER BANDWIDTH REGISTER DESCRIPTION
**************************************************************************/
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_MASK                  (0x1F)      // Selection of the Bandwidth for the Acceleration Data
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_7_81HZ                (0x08)      // Bandwidth: 7.81 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_15_63HZ               (0x09)      // Bandwidth: 15.63 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_31_25HZ               (0x0A)      // Bandwidth: 31.25 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_62_5HZ                (0x0B)      // Bandwidth: 62.5 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_125HZ                 (0x0C)      // Bandwidth: 125 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_250HZ                 (0x0D)      // Bandwidth: 250 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_500HZ                 (0x0E)      // Bandwidth: 500 Hz
    #define BMA250_REG_ACCEL_BANDWIDTH_BW_1000HZ                (0x0F)      // Bandwidth: 1000 Hz

typedef enum
{
    ACCEL_RANGE_2G                      = BMA250_REG_ACCEL_G_RANGE_2G,
    ACCEL_RANGE_4G                      = BMA250_REG_ACCEL_G_RANGE_4G,
    ACCEL_RANGE_8G                      = BMA250_REG_ACCEL_G_RANGE_8G,
    ACCEL_RANGE_16G                     = BMA250_REG_ACCEL_G_RANGE_16G
    
} bmaAccelRange_t;

typedef enum
{
    ACCEL_BANDWIDTH_7_81HZ              = BMA250_REG_ACCEL_BANDWIDTH_BW_7_81HZ,
    ACCEL_BANDWIDTH_15_63HZ             = BMA250_REG_ACCEL_BANDWIDTH_BW_15_63HZ,
    ACCEL_BANDWIDTH_31_25HZ             = BMA250_REG_ACCEL_BANDWIDTH_BW_31_25HZ,
    ACCEL_BANDWIDTH_62_5HZ              = BMA250_REG_ACCEL_BANDWIDTH_BW_62_5HZ,
    ACCEL_BANDWIDTH_BW_125HZ            = BMA250_REG_ACCEL_BANDWIDTH_BW_125HZ,
    ACCEL_BANDWIDTH_BW_250HZ            = BMA250_REG_ACCEL_BANDWIDTH_BW_250HZ,
    ACCEL_BANDWIDTH_BW_500HZ            = BMA250_REG_ACCEL_BANDWIDTH_BW_500HZ,
    ACCEL_BANDWIDTH_BW_1000HZ           = BMA250_REG_ACCEL_BANDWIDTH_BW_1000HZ
    
} bmaAccelBandwidth_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} bmaSensorData_t;


class BMA250
{
    protected:
        // Instance-specific properties
        uint8_t bma_conversionDelay;
        bmaAccelRange_t bma_accelrange;
        bmaAccelBandwidth_t bma_accelbandwidth;
    
    public:
        uint8_t bma_i2cAddress;
        bmaSensorData_t bma_accelData;
        void getAddr_BMA250(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_Accelerometer(void);
        void setAccelRange(bmaAccelRange_t accelrange);
        bmaAccelRange_t getAccelRange(void);
        void setAccelBandwidth(bmaAccelBandwidth_t accelbandwidth);
        bmaAccelBandwidth_t getAccelBandwidth(void);
    
    private:
};
