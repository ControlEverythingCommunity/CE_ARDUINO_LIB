/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VCNL4010
        This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2
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
    #define VCNL4010_DEFAULT_ADDRESS                            (0x13)      // 0010 011
    #define VCNL4010_ALSPROX_PRODUCT_ID                         (0x21)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define VCNL4010_CONVERSIONDELAY                            (100)

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR REGISTERS
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_COMMAND                        (0x80)      // Specifies Register Address
    #define VCNL4010_REG_ALSPROX_PRODUCT_ID                     (0x81)      // Product ID Revision Register
    #define VCNL4010_REG_ALSPROX_PROX_RATE                      (0x82)      // Rate of Proximity Measurement
    #define VCNL4010_REG_ALSPROX_IR_LED_CURRENT                 (0x83)      // LED Current Setting for Proximity Mode
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR                     (0x84)      // Ambient Light Parameter Register
    #define VCNL4010_REG_ALSPROX_ALSDATAH                       (0x85)      // Ambient Light Result High Data Register
    #define VCNL4010_REG_ALSPROX_ALSDATAL                       (0x86)      // Ambient Light Result Low Data Register
    #define VCNL4010_REG_ALSPROX_PDATAH                         (0x87)      // Proximity Measurement Result High Data Register
    #define VCNL4010_REG_ALSPROX_PDATAL                         (0x88)      // Proximity Measurement Result Low Data Register
    #define VCNL4010_REG_ALSPROX_INT_CTRL                       (0x89)      // Interrupt Control Register
    #define VCNL4010_REG_ALSPROX_LTHRES_L                       (0x8A)      // Low Threshold Low Byte Register
    #define VCNL4010_REG_ALSPROX_LTHRES_H                       (0x8B)      // Low Threshold High Byte Register
    #define VCNL4010_REG_ALSPROX_HTHRES_L                       (0x8C)      // High Threshold Low Byte Register
    #define VCNL4010_REG_ALSPROX_HTHRES_H                       (0x8D)      // High Threshold High Byte Register
    #define VCNL4010_REG_ALSPROX_INTR_STATUS                    (0x8E)      // Interrupt Status Register
    #define VCNL4010_REG_ALSPROX_PROX_MOD                       (0x8F)      // Proximity Modulator Timing Adjustment

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR COMMAND REGISTER DESCRIPTION
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_CMD_CONFIG_LOCK_MASK           (0x80)      // Select Config_Lock
    #define VCNL4010_REG_ALSPROX_CMD_CONFIG_LOCK_SELECT         (0x80)      // Select Config_Lock

    #define VCNL4010_REG_ALSPROX_CMD_ALS_DATA_RDY_MASK          (0x40)      // Select Als_Data_Rdy
    #define VCNL4010_REG_ALSPROX_CMD_ALS_DATA_RDY_SELECT        (0x40)      // Select Als_Data_Rdy

    #define VCNL4010_REG_ALSPROX_CMD_PROX_DATA_RDY_MASK         (0x20)      // Select Prox_Data_Rdy
    #define VCNL4010_REG_ALSPROX_CMD_PROX_DATA_RDY_SELECT       (0x20)      // Select Prox_Data_Rdy

    #define VCNL4010_REG_ALSPROX_CMD_ALS_OD_MASK                (0x10)      // Select Single On-Demand Measurement for Ambient Light
    #define VCNL4010_REG_ALSPROX_CMD_ALS_OD_STOP                (0x00)      // Stops a Single On-Demand Measurement for Ambient Light
    #define VCNL4010_REG_ALSPROX_CMD_ALS_OD_START               (0x10)      // Starts a Single On-Demand Measurement for Ambient Light

    #define VCNL4010_REG_ALSPROX_CMD_PROX_OD_MASK               (0x08)      // Select Single On-Demand Measurement for Proximity
    #define VCNL4010_REG_ALSPROX_CMD_PROX_OD_STOP               (0x00)      // Stops a Single On-Demand Measurement for Proximity
    #define VCNL4010_REG_ALSPROX_CMD_PROX_OD_START              (0x08)      // Starts a Single On-Demand Measurement for Proximity

    #define VCNL4010_REG_ALSPROX_CMD_ALS_EN_MASK                (0x04)      // Periodic ALS Measurement Enable
    #define VCNL4010_REG_ALSPROX_CMD_ALS_EN_DISABLE             (0x00)      // Disables the Periodic ALS Measurement
    #define VCNL4010_REG_ALSPROX_CMD_ALS_EN_ENABLE              (0x04)      // Enables the Periodic ALS Measurement

    #define VCNL4010_REG_ALSPROX_CMD_PROX_EN_MASK               (0x02)      // Periodic Proximity Measurement Enable
    #define VCNL4010_REG_ALSPROX_CMD_PROX_EN_DISABLE            (0x00)      // Disables the Periodic Proximity Measurement
    #define VCNL4010_REG_ALSPROX_CMD_PROX_EN_ENABLE             (0x02)      // Enables the Periodic Proximity Measurement

    #define VCNL4010_REG_ALSPROX_CMD_SELFTIMED_EN_MASK          (0x01)      // State Machine and LP Oscillator for Self Timed Measurements Enable
    #define VCNL4010_REG_ALSPROX_CMD_SELFTIMED_EN_DISABLE       (0x00)      // Disables the State Machine and LP Oscillator for Self Timed Measurements
    #define VCNL4010_REG_ALSPROX_CMD_SELFTIMED_EN_ENABLE        (0x01)      // Activates the State Machine and LP Oscillator for Self Timed Measurements

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR PROXIMITY RATE REGISTER DESCRIPTION
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_PROX_RATE_MASK                 (0x07)      // Rate of Proximity Measurement
    #define VCNL4010_REG_ALSPROX_PROX_RATE_1_95                 (0x00)      // 1.95 Measurements/s (DEFAULT)
    #define VCNL4010_REG_ALSPROX_PROX_RATE_3_90625              (0x01)      // 3.90625 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_7_8125               (0x02)      // 7.8125 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_16_625               (0x03)      // 16.625 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_31_25                (0x04)      // 31.25 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_62_5                 (0x05)      // 62.5 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_125                  (0x06)      // 125 Measurements/s
    #define VCNL4010_REG_ALSPROX_PROX_RATE_250                  (0x07)      // 250 Measurements/s

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR AMBIENT LIGHT PARAMETER REGISTER DESCRIPTION
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_CONT_MASK           (0x80)      // Continuous Conversion Mode
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_CONT_DISABLE        (0x00)      // Disable Continuous Conversion Mode
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_MODE_ENABLE         (0x80)      // Enable Continuous Conversion Mode

    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_MASK           (0x70)      // Ambient Light Measurement Rate
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_1              (0x00)      // 1 Sample/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_2              (0x10)      // 2 Samples/s (DEFAULT)
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_3              (0x20)      // 3 Samples/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_4              (0x30)      // 4 Samples/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_5              (0x40)      // 5 Samples/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_6              (0x50)      // 6 Samples/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_8              (0x60)      // 8 Samples/s
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_RATE_10             (0x70)      // 10 Samples/s

    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_OFFSET_MASK         (0x08)      // Automatic Offset Compensation
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_OFFSET_DISABLE      (0x00)      // Disable Automatic Offset Compensation
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_OFFSET_ENABLE       (0x08)      // Enable Automatic Offset Compensation

    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_MASK       (0x07)      // Averaging Function
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_1          (0x00)      // Number of Conversions = 1 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_2          (0x01)      // Number of Conversions = 2 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_4          (0x02)      // Number of Conversions = 4 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_8          (0x03)      // Number of Conversions = 8 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_16         (0x04)      // Number of Conversions = 16 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_32         (0x05)      // Number of Conversions = 32 Conv (DEFAULT)
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_64         (0x06)      // Number of Conversions = 64 Conv
    #define VCNL4010_REG_ALSPROX_IR_ALS_PAR_AVG_FUNC_128        (0x08)      // Number of Conversions = 128 Conv

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR INTERRUPT CONTROL REGISTER DESCRIPTION
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_MASK            (0xE0)      // The Number of Consecutive Measurements Needed Above/Below the Threshold
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_1               (0x00)      // 1 Count = DEFAULT
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_2               (0x20)      // 2 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_4               (0x40)      // 4 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_8               (0x60)      // 8 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_16              (0x80)      // 16 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_32              (0xA0)      // 32 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_64              (0xC0)      // 64 Counts
    #define VCNL4010_REG_ALSPROX_INT_CTRL_COUNT_128             (0xE0)      // 128 Counts

    #define VCNL4010_REG_ALSPROX_INT_CTRL_PROX_EN_MASK          (0x08)      // Interrupt Generation at Proximity Data Ready
    #define VCNL4010_REG_ALSPROX_INT_CTRL_PROX_EN_DISABLE       (0x00)      // Disables Interrupt Generation at Proximity Data Ready
    #define VCNL4010_REG_ALSPROX_INT_CTRL_PROX_EN_ENABLE        (0x08)      // Enables Interrupt Generation at Proximity Data Ready

    #define VCNL4010_REG_ALSPROX_INT_CTRL_ALS_EN_MASK           (0x04)      // Interrupt Generation at Ambient Data Ready
    #define VCNL4010_REG_ALSPROX_INT_CTRL_ALS_EN_DISABLE        (0x00)      // Disables Interrupt Generation at Ambient Data Ready
    #define VCNL4010_REG_ALSPROX_INT_CTRL_ALS_EN_ENABLE         (0x04)      // Enables Interrupt Generation at Ambient Data Ready

    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_EN_MASK         (0x02)      // Interrupt Generation When High or Low Threshold is Exceeded
    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_EN_DISABLE      (0x00)      // Disables Interrupt Generation When High or Low Threshold is Exceeded
    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_EN_ENABLE       (0x02)      // Enables Interrupt Generation When High or Low Threshold is Exceeded

    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_SEL_MASK        (0x01)      // Thresholds are Applied to Measurements
    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_SEL_PROX        (0x00)      // Thresholds are Applied to Proximity Measurements
    #define VCNL4010_REG_ALSPROX_INT_CTRL_THRES_SEL_ALS         (0x01)      // Thresholds are Applied to ALS Measurements

/**************************************************************************
    PROXIMITY AND AMBIENT LIGHT SENSOR PROXIMITY MODULATOR TIMING ADJUSTMENT REGISTER DESCRIPTION
**************************************************************************/
    #define VCNL4010_REG_ALSPROX_PROX_MOD_DELAY_TIME_MASK       (0xE0)      // Compensation of Delays from IR LED and IR Photo Diode
    #define VCNL4010_REG_ALSPROX_PROX_MOD_DELAY_TIME_DEFAULT    (0x00)      // Setting a Delay Time Between IR LED Signal and IR Input Signal Evaluation

    #define VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_MASK        (0x18)      // Setting the Proximity IR Test Signal Frequency
    #define VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_390_625K    (0x00)      // 390.625 kHz (DEFAULT)
    #define VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_781_25K     (0x08)      // 781.25 kHz
    #define VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_1_5625M     (0x10)      // 1.5625 MHz
    #define VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_3_125M      (0x18)      // 3.125 MHz

    #define VCNL4010_REG_ALSPROX_PROX_MOD_DEAD_TIME_MASK        (0x07)      // Reducing of Possible Disturbance Effects and Signal Level
    #define VCNL4010_REG_ALSPROX_PROX_MOD_DEAD_TIME_DEFAULT     (0x07)      // Setting a Dead Time in Evaluation of IR Signal at the Slopes of the IR Signal


typedef enum
{
    FREQUENCY_390_625KHZ            = VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_390_625K,
    FREQUENCY_781_25KHZ             = VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_781_25K,
    FREQUENCY_1_5625MHZ             = VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_1_5625M,
    FREQUENCY_3_125MHZ              = VCNL4010_REG_ALSPROX_PROX_MOD_PROX_FREQ_3_125M
    
} vcnlFrequency_t;


class VCNL4010
{
    protected:
        // Instance-specific properties
        uint8_t vcnl_conversionDelay;
        vcnlFrequency_t vcnl_frequency;

    public:
        uint8_t vcnl_i2cAddress;
        void getAddr_VCNL4010(uint8_t i2cAddress);
        bool begin(void);
        uint16_t Measure_ALS(void);
        uint16_t Measure_Proximity(void);
        void setLEDCurrent(uint8_t current);
        void setFrequency(vcnlFrequency_t frequency);
    
    private:
};
