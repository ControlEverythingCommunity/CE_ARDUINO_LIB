/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMG160
        This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-Gyroscope AND GYROSCOPE
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
    #define BMG160_DEFAULT_ADDRESS_GYRO                         (0x68)
    #define BMG160_ADDRESS_GYRO_UPDATED                         (0x69)
    #define BMG160_DEFAULT_CHIP_ID                              (0x0F)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define BMG160_CONVERSIONDELAY                              (100)

/**************************************************************************
    GYROSCOPE REGISTERS
**************************************************************************/
    #define BMG160_REG_GYRO_CHIP_ID                             (0x00)      // Chip Identification Number Register
    #define BMG160_REG_GYRO_RATE_X_LSB                          (0x02)      // X-Axis Gyroscope Low Data Register
    #define BMG160_REG_GYRO_RATE_X_MSB                          (0x03)      // X-Axis Gyroscope High Data Register
    #define BMG160_REG_GYRO_RATE_Y_LSB                          (0x04)      // Y-Axis Gyroscope Low Data Register
    #define BMG160_REG_GYRO_RATE_Y_MSB                          (0x05)      // Y-Axis Gyroscope High Data Register
    #define BMG160_REG_GYRO_RATE_Z_LSB                          (0x06)      // Z-Axis Gyroscope Low Data Register
    #define BMG160_REG_GYRO_RATE_Z_MSB                          (0x07)      // Z-Axis Gyroscope High Data Register
    #define BMG160_REG_GYRO_TEMP                                (0x08)      // Temperature Data Register
    #define BMG160_REG_GYRO_INTR_STATUS                         (0x09)      // Interrupt Status Register
    #define BMG160_REG_GYRO_INTR_STATUS1                        (0x0A)      // Interrupt Status 1 Register
    #define BMG160_REG_GYRO_INTR_STATUS2                        (0x0B)      // Interrupt Status 2 Register
    #define BMG160_REG_GYRO_INTR_STATUS3                        (0x0C)      // Interrupt Status 3 Register
    #define BMG160_REG_GYRO_FIFO_STATUS                         (0x0E)      // FIFO Status Register
    #define BMG160_REG_GYRO_RANGE                               (0x0F)      // Angular Rate Range Measurement Register
    #define BMG160_REG_GYRO_BANDWIDTH                           (0x10)      // Selection of the Angular Rate Data Filter Bandwidth Register
    #define BMG160_REG_GYRO_LPM1                                (0x11)      // Main Power Mode Register
    #define BMG160_REG_GYRO_LPM2                                (0x12)      // Fast Power-Up and External Trigger Configuration Settings Register
    #define BMG160_REG_GYRO_RATE_HODR                            (0x13)      // Angular Rate Data Acquisition and Data Output Format Register
    #define BMG160_REG_GYRO_BGW_SOFTRESET                       (0x14)      // User Triggered SoftReset Register
    #define BMG160_REG_GYRO_BGW_INT_EN_0                        (0x15)      // Interrupt Control Register
    #define BMG160_REG_GYRO_BGW_INT_EN_1                        (0x16)      // Interrupt Pin Configuration Register
    #define BMG160_REG_GYRO_BGW_INT_MAP_0                       (0x17)      // Interrupt Signal Mapped INT1 Control Register
    #define BMG160_REG_GYRO_BGW_INT_MAP_1                       (0x18)      // Interrupt Signal Mapped INT1/INT2 Control Register
    #define BMG160_REG_GYRO_BGW_INT_MAP_0                       (0x19)      // Interrupt Signal Mapped INT2 Control Register
    #define BMG160_REG_GYRO_INTERRUPT_DATA_SRC_SLCT             (0x1A)      // Interrupt with Selectable Data Source Definition Register
    #define BMG160_REG_GYRO_INTERRUPT_DATA_SRC_ANY              (0x1B)      // Fast Offset Compensation with Any Motion Threshold Definition Register
    #define BMG160_REG_GYRO_AWAKE_SAMPLE_DURATION               (0x1C)      // Awake and Sample Duration Register
    #define BMG160_REG_GYRO_FIFO_WATER_MARK_INTR                (0x1E)      // FIFO Water Mark Interrupt Register
    #define BMG160_REG_GYRO_INT_RST_LATCH                       (0x21)      // Interrupt Reset and Mode Selection Register
    #define BMG160_REG_GYRO_HIGH_THS_X                          (0x22)      // High Rate Threshold and Hysteresis Seettings for X-Axis Register
    #define BMG160_REG_GYRO_HIGH_DUR_X                          (0x23)      // High Rate Duration Settings X-Axis Register
    #define BMG160_REG_GYRO_HIGH_THS_Y                          (0x24)      // High Rate Threshold and Hysteresis Seettings for Y-Axis Register
    #define BMG160_REG_GYRO_HIGH_DUR_Y                          (0x25)      // High Rate Duration Settings Y-Axis Register
    #define BMG160_REG_GYRO_HIGH_THS_Z                          (0x26)      // High Rate Threshold and Hysteresis Seettings for Z-Axis Register
    #define BMG160_REG_GYRO_HIGH_DUR_Z                          (0x27)      // High Rate Duration Settings Z-Axis Register
    #define BMG160_REG_GYRO_SOC                                 (0x31)      // Slow Offset Cancellation Settings Register
    #define BMG160_REG_GYRO_A_FOC                               (0x32)      // Fast Offset Cancellation Settings Register
    #define BMG160_REG_GYRO_TRIM_NVM_CTRL                       (0x33)      // NVM Control Settings Register
    #define BMG160_REG_GYRO_BGW_SPI3_WDT                        (0x34)      // Digital Interface Settings Register
    #define BMG160_REG_GYRO_OFC1                                (0x36)      // Offset Compensation Settings Register
    #define BMG160_REG_GYRO_OFC2                                (0x37)      // Offset Compensation Settings for X-Axis Register
    #define BMG160_REG_GYRO_OFC3                                (0x38)      // Offset Compensation Settings for Y-Axis Register
    #define BMG160_REG_GYRO_OFC4                                (0x39)      // Offset Compensation Settings for Z-Axis Register
    #define BMG160_REG_GYRO_TRIM_GP0                            (0x3A)      // NVM General Purpose Data Register
    #define BMG160_REG_GYRO_TRIM_GP1                            (0x3B)      // NVM General Purpose Data Register
    #define BMG160_REG_GYRO_BIST                                (0x3C)      // Built in Self-Test (BIST) Register
    #define BMG160_REG_GYRO_FIFO_CONFIG_0                       (0x3D)      // FIFO Watermark Level Register
    #define BMG160_REG_GYRO_FIFO_CONFIG_1                       (0x3E)      // FIFO Configuration Settings Register
    #define BMG160_REG_GYRO_FIFO_DATA                           (0x3F)      // FIFO Data Readout Register

/**************************************************************************
    GYROSCOPE RANGE SELCTION REGISTER DESCRIPTION
**************************************************************************/
    #define BMG160_REG_GYRO_G_RANGE_MASK                        (0x07)      // Angular Rate and Resolution
    #define BMG160_REG_GYRO_G_RANGE_2000                        (0x00)      // Full Scale: ± 2000 °/s
    #define BMG160_REG_GYRO_G_RANGE_1000                        (0x01)      // Full Scale: ± 1000 °/s
    #define BMG160_REG_GYRO_G_RANGE_500                         (0x02)      // Full Scale: ± 500 °/s
    #define BMG160_REG_GYRO_G_RANGE_250                         (0x03)      // Full Scale: ± 250 °/s
    #define BMG160_REG_GYRO_G_RANGE_125                         (0x04)      // Full Scale: ± 125 °/s

/**************************************************************************
    GYROSCOPE BANDWIDTH REGISTER DESCRIPTION
**************************************************************************/
    #define BMG160_REG_GYRO_BANDWIDTH_MASK                      (0x07)      // Selection of the Bandwidth for the GYROeration Data
    #define BMG160_REG_GYRO_BANDWIDTH_2000_UNFILTERED           (0x00)      // ODR: 2000 Hz, Filter Bandwidth: Unfiltered (523 Hz)
    #define BMG160_REG_GYRO_BANDWIDTH_2000_230HZ                (0x01)      // ODR: 2000 Hz, Filter Bandwidth: 230 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_1000_116HZ                (0x02)      // ODR: 1000 Hz, Filter Bandwidth: 116 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_400_47HZ                  (0x03)      // ODR: 400 Hz, Filter Bandwidth: 47 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_200_23HZ                  (0x04)      // ODR: 200 Hz, Filter Bandwidth: 23 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_100_12HZ                  (0x05)      // ODR: 100 Hz, Filter Bandwidth: 12 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_200_64HZ                  (0x06)      // ODR: 200 Hz, Filter Bandwidth: 64 Hz
    #define BMG160_REG_GYRO_BANDWIDTH_100_32HZ                  (0x07)      // ODR: 100 Hz, Filter Bandwidth: 32 Hz

typedef enum
{
    GYRO_RANGE_2000                     = BMG160_REG_GYRO_G_RANGE_2000,
    GYRO_RANGE_1000                     = BMG160_REG_GYRO_G_RANGE_1000,
    GYRO_RANGE_500                      = BMG160_REG_GYRO_G_RANGE_500,
    GYRO_RANGE_250                      = BMG160_REG_GYRO_G_RANGE_250,
    GYRO_RANGE_125                      = BMG160_REG_GYRO_G_RANGE_125
    
} bmgGyroRange_t;

typedef enum
{
    GYRO_BANDWIDTH_2000_UNFILTERED      = BMG160_REG_GYRO_BANDWIDTH_2000_UNFILTERED,
    GYRO_BANDWIDTH_2000_230HZ           = BMG160_REG_GYRO_BANDWIDTH_2000_230HZ,
    GYRO_BANDWIDTH_1000_116HZ           = BMG160_REG_GYRO_BANDWIDTH_1000_116HZ,
    GYRO_BANDWIDTH_400_47HZ             = BMG160_REG_GYRO_BANDWIDTH_400_47HZ,
    GYRO_BANDWIDTH_200_23HZ             = BMG160_REG_GYRO_BANDWIDTH_200_23HZ,
    GYRO_BANDWIDTH_100_12HZ             = BMG160_REG_GYRO_BANDWIDTH_100_12HZ,
    GYRO_BANDWIDTH_200_64HZ             = BMG160_REG_GYRO_BANDWIDTH_200_64HZ,
    GYRO_BANDWIDTH_100_32HZ             = BMG160_REG_GYRO_BANDWIDTH_100_32HZ
    
} bmgGyroBandwidth_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    
} bmgSensorData_t;


class BMG160
{
    protected:
        // Instance-specific properties
        uint8_t bmg_conversionDelay;
        bmgGyroRange_t bmg_gyrorange;
        bmgGyroBandwidth_t bmg_gyrobandwidth;
    
    public:
        uint8_t bmg_i2cAddress;
        bmgSensorData_t bmg_gyroData;
        void getAddr_BMG160(uint8_t i2cAddress);
        bool begin(void);
        void setUpSensor(void);
        void Measure_Gyroscope(void);
        void setGyroRange(bmgGyroRange_t gyrorange);
        bmgGyroRange_t getGyroRange(void);
        void setGyroBandwidth(bmgGyroBandwidth_t gyrobandwidth);
        bmgGyroBandwidth_t getGyroBandwidth(void);
    
    private:
};
