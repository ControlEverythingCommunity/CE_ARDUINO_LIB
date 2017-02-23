/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAG3110
        This code is designed to work with the MAG3110_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define MAG3110_DEFAULT_ADDRESS_MAG                     (0x0E)      // 0001110xb
    #define MAG3110_MAG_DEV_ID                              (0xC4)      // 11000100

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MAG3110_CONVERSIONDELAY                         (100)

/**************************************************************************
    MAGNETOMETER REGISTERS
**************************************************************************/
    #define MAG3110_REG_MAG_DR_STATUS                       (0x00)      // Data Ready Status Per Axis Register
    #define MAG3110_REG_MAG_OUT_X_MSB                       (0x01)      // X-Axis Magnetic Data High Register
    #define MAG3110_REG_MAG_OUT_X_LSB                       (0x02)      // X-Axis Magnetic Data Low Register
    #define MAG3110_REG_MAG_OUT_Y_MSB                       (0x03)      // Y-Axis Magnetic Data High Register
    #define MAG3110_REG_MAG_OUT_Y_LSB                       (0x04)      // Y-Axis Magnetic Data Low Register
    #define MAG3110_REG_MAG_OUT_Z_MSB                       (0x05)      // Z-Axis Magnetic Data High Register
    #define MAG3110_REG_MAG_OUT_Z_LSB                       (0x06)      // Z-Axis Magnetic Data Low Register
    #define MAG3110_REG_MAG_WHO_AM_I                        (0x07)      // Device ID Number Register
    #define MAG3110_REG_MAG_SYSMOD                          (0x08)      // ICurrent System Mode Register
    #define MAG3110_REG_MAG_OFF_X_MSB                       (0x09)      // X-Axis User Offset High Register
    #define MAG3110_REG_MAG_OFF_X_LSB                       (0x0A)      // X-Axis User Offset Low Register
    #define MAG3110_REG_MAG_OFF_Y_MSB                       (0x0B)      // Y-Axis User Offset High Register
    #define MAG3110_REG_MAG_OFF_Y_LSB                       (0x0C)      // Y-Axis User Offset Low Register
    #define MAG3110_REG_MAG_OFF_Z_MSB                       (0x0D)      // Z-Axis User Offset High Register
    #define MAG3110_REG_MAG_OFF_Z_LSB                       (0x0E)      // Z-Axis User Offset Low Register
    #define MAG3110_REG_MAG_DIE_TEMP                        (0x0F)      // Temperature Register
    #define MAG3110_REG_MAG_CTRL_REG1                       (0x10)      // Operation Modes Register 1
    #define MAG3110_REG_MAG_CTRL_REG2                       (0x11)      // Operation Modes Register 2

/**************************************************************************
    MAGNETOMETER CONTROL REGISTER 1 DESCRIPTION
**************************************************************************/
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_MASK            (0xF8)      // Over-Sampling Ratio and Data Rate
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_80HZ            (0x00)      // Output Rate (Hz): 80.00, Over Sample Ratio: 16, ADC Rate (Hz): 1280, Current Type (microA): 900.0, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_40HZ            (0x08)      // Output Rate (Hz): 40.00, Over Sample Ratio: 32, ADC Rate (Hz): 1280, Current Type (microA): 900.0, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ            (0x10)      // Output Rate (Hz): 20.00, Over Sample Ratio: 64, ADC Rate (Hz): 1280, Current Type (microA): 900.0, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ            (0x18)      // Output Rate (Hz): 10.00, Over Sample Ratio: 128, ADC Rate (Hz): 1280, Current Type (microA): 900.0, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_40HZ1            (0x20)      // Output Rate (Hz): 40.00, Over Sample Ratio: 16, ADC Rate (Hz): 640, Current Type (microA): 550.0, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ1            (0x28)      // Output Rate (Hz): 20.00, Over Sample Ratio: 32, ADC Rate (Hz): 640, Current Type (microA): 550.0, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ1            (0x30)      // Output Rate (Hz): 10.00, Over Sample Ratio: 64, ADC Rate (Hz): 640, Current Type (microA): 550.0, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ             (0x38)      // Output Rate (Hz): 5.00, Over Sample Ratio: 128, ADC Rate (Hz): 640, Current Type (microA): 550.0, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ2            (0x40)      // Output Rate (Hz): 20.00, Over Sample Ratio: 16, ADC Rate (Hz): 320, Current Type (microA): 275.0, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ2            (0x48)      // Output Rate (Hz): 10.00, Over Sample Ratio: 32, ADC Rate (Hz): 320, Current Type (microA): 275.0, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ1             (0x50)      // Output Rate (Hz): 5.00, Over Sample Ratio: 64, ADC Rate (Hz): 320, Current Type (microA): 275.0, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ           (0x58)      // Output Rate (Hz): 2.50, Over Sample Ratio: 128, ADC Rate (Hz): 320, Current Type (microA): 275.0, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ3            (0x60)      // Output Rate (Hz): 10.00, Over Sample Ratio: 16, ADC Rate (Hz): 160, Current Type (microA): 137.5, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ2             (0x68)      // Output Rate (Hz): 5.00, Over Sample Ratio: 32, ADC Rate (Hz): 160, Current Type (microA): 137.5, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ1           (0x70)      // Output Rate (Hz): 2.50, Over Sample Ratio: 64, ADC Rate (Hz): 160, Current Type (microA): 137.5, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ          (0x78)      // Output Rate (Hz): 1.25, Over Sample Ratio: 128, ADC Rate (Hz): 160, Current Type (microA): 137.5, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ3             (0x80)      // Output Rate (Hz): 5.00, Over Sample Ratio: 16, ADC Rate (Hz): 80, Current Type (microA): 68.8, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ2           (0x88)      // Output Rate (Hz): 2.50, Over Sample Ratio: 32, ADC Rate (Hz): 80, Current Type (microA): 68.8, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ1          (0x90)      // Output Rate (Hz): 1.25, Over Sample Ratio: 64, ADC Rate (Hz): 80, Current Type (microA): 68.8, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ          (0x98)      // Output Rate (Hz): 0.63, Over Sample Ratio: 128, ADC Rate (Hz): 80, Current Type (microA): 68.8, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ3           (0xA0)      // Output Rate (Hz): 2.50, Over Sample Ratio: 16, ADC Rate (Hz): 80, Current Type (microA): 34.4, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ2          (0xA8)      // Output Rate (Hz): 1.25, Over Sample Ratio: 32, ADC Rate (Hz): 80, Current Type (microA): 34.4, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ1          (0xB0)      // Output Rate (Hz): 0.63, Over Sample Ratio: 64, ADC Rate (Hz): 80, Current Type (microA): 34.4, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ          (0xB8)      // Output Rate (Hz): 0.31, Over Sample Ratio: 128, ADC Rate (Hz): 80, Current Type (microA): 34.4, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ3          (0xC0)      // Output Rate (Hz): 1.25, Over Sample Ratio: 16, ADC Rate (Hz): 80, Current Type (microA): 17.2, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ2          (0xC8)      // Output Rate (Hz): 0.63, Over Sample Ratio: 32, ADC Rate (Hz): 80, Current Type (microA): 17.2, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ1          (0xD0)      // Output Rate (Hz): 0.31, Over Sample Ratio: 164, ADC Rate (Hz): 80, Current Type (microA): 17.2, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_16HZ          (0xD8)      // Output Rate (Hz): 0.16, Over Sample Ratio: 128, ADC Rate (Hz): 80, Current Type (microA): 17.2, Noise Type (microT rms): 0.25
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ3          (0xE0)      // Output Rate (Hz): 0.63, Over Sample Ratio: 16, ADC Rate (Hz): 80, Current Type (microA): 8.6, Noise Type (microT rms): 0.4
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ2          (0xE8)      // Output Rate (Hz): 0.31, Over Sample Ratio: 32, ADC Rate (Hz): 80, Current Type (microA): 8.6, Noise Type (microT rms): 0.35
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_16HZ1          (0xF0)      // Output Rate (Hz): 0.16, Over Sample Ratio: 64, ADC Rate (Hz): 80, Current Type (microA): 8.6, Noise Type (microT rms): 0.3
    #define MAG3110_REG_MAG_CTRL_REG1_MDROS_0_08HZ          (0xF8)      // Output Rate (Hz): 0.08, Over Sample Ratio: 128, ADC Rate (Hz): 80, Current Type (microA): 8.6, Noise Type (microT rms): 0.25

    #define MAG3110_REG_MAG_CTRL_REG1_FR_MASK               (0x04)      // Fast Read Selection
    #define MAG3110_REG_MAG_CTRL_REG1_FR_FULL               (0x00)      // The Full 16-Bit Values are Read
    #define MAG3110_REG_MAG_CTRL_REG1_FR_FAST               (0x04)      // Fast Read, 8-Bit Values Read from the MSB Registers

    #define MAG3110_REG_MAG_CTRL_REG1_TM_MASK               (0x02)      // Trigger Immediate Measurement
    #define MAG3110_REG_MAG_CTRL_REG1_TM_NORMAL             (0x00)      // Normal Operation Based on AC Condition
    #define MAG3110_REG_MAG_CTRL_REG1_TM_TRIGGER            (0x02)      // Trigger Measurement

    #define MAG3110_REG_MAG_CTRL_REG1_AC_MASK               (0x01)      // Operating Mode Selection
    #define MAG3110_REG_MAG_CTRL_REG1_AC_STANDBY            (0x00)      // STANDBY Mode
    #define MAG3110_REG_MAG_CTRL_REG1_AC_ACTIVE             (0x01)      // ACTIVE Mode


typedef enum
{
    MAG_DATARATE_80HZ               = MAG3110_REG_MAG_CTRL_REG1_MDROS_80HZ,
    MAG_DATARATE_40HZ               = MAG3110_REG_MAG_CTRL_REG1_MDROS_40HZ,
    MAG_DATARATE_20HZ               = MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ,
    MAG_DATARATE_10HZ               = MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ,
    MAG_DATARATE_40HZ1              = MAG3110_REG_MAG_CTRL_REG1_MDROS_40HZ1,
    MAG_DATARATE_20HZ1              = MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ1,
    MAG_DATARATE_10HZ1              = MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ1,
    MAG_DATARATE_5HZ                = MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ,
    MAG_DATARATE_20HZ2              = MAG3110_REG_MAG_CTRL_REG1_MDROS_20HZ2,
    MAG_DATARATE_10HZ2              = MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ2,
    MAG_DATARATE_5HZ1               = MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ1,
    MAG_DATARATE_2_5HZ              = MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ,
    MAG_DATARATE_10HZ3              = MAG3110_REG_MAG_CTRL_REG1_MDROS_10HZ3,
    MAG_DATARATE_5HZ2               = MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ2,
    MAG_DATARATE_2_5HZ1             = MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ1,
    MAG_DATARATE_1_25HZ             = MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ,
    MAG_DATARATE_5HZ3               = MAG3110_REG_MAG_CTRL_REG1_MDROS_5HZ3,
    MAG_DATARATE_2_5HZ2             = MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ2,
    MAG_DATARATE_1_25HZ1            = MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ1,
    MAG_DATARATE_0_63HZ             = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ,
    MAG_DATARATE_2_5HZ3             = MAG3110_REG_MAG_CTRL_REG1_MDROS_2_5HZ3,
    MAG_DATARATE_1_25HZ2            = MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ2,
    MAG_DATARATE_0_63HZ1            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ1,
    MAG_DATARATE_0_31HZ             = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ,
    MAG_DATARATE_1_25HZ3            = MAG3110_REG_MAG_CTRL_REG1_MDROS_1_25HZ3,
    MAG_DATARATE_0_63HZ2            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ2,
    MAG_DATARATE_0_31HZ1            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ1,
    MAG_DATARATE_0_16HZ             = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_16HZ,
    MAG_DATARATE_0_63HZ3            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_63HZ3,
    MAG_DATARATE_0_31HZ2            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_31HZ2,
    MAG_DATARATE_0_16HZ1            = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_16HZ1,
    MAG_DATARATE_0_08HZ             = MAG3110_REG_MAG_CTRL_REG1_MDROS_0_08HZ
    
} magDataRate_t;

typedef enum
{
    MAG_FULL_READ                   = MAG3110_REG_MAG_CTRL_REG1_FR_FULL,
    MAG_FAST_READ                   = MAG3110_REG_MAG_CTRL_REG1_FR_FAST
    
} magRead_t;

typedef enum
{
    MAG_NORMAL                      = MAG3110_REG_MAG_CTRL_REG1_TM_NORMAL,
    MAG_TRIGGER                     = MAG3110_REG_MAG_CTRL_REG1_TM_TRIGGER
    
} magTrigger_t;

typedef enum
{
    MAG_STANDBY_MODE                = MAG3110_REG_MAG_CTRL_REG1_AC_STANDBY,
    MAG_ACTIVE_MODE                 = MAG3110_REG_MAG_CTRL_REG1_AC_ACTIVE
    
} magMode_t;

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    int8_t T;
    
} magSensorData_t;


class MAG3110
{
    protected:
        // Instance-specific properties
        uint8_t mag_conversionDelay;
        magDataRate_t mag_datarate;
        magRead_t mag_read;
        magTrigger_t mag_trigger;
        magMode_t mag_mode;

    public:
        uint8_t mag_i2cAddress;
        magSensorData_t mag_Data;
        magSensorData_t mag_tempData;
        void getAddr_MAG3110(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Sensor(void);
        void setUpMagnetometer(void);
        void Measure_Magnetometer(void);
        void Measure_Temperature(void);
        void setMagDataRate(magDataRate_t datarate);
        magDataRate_t getMagDataRate(void);
        void setRead(magRead_t read);
        magRead_t getRead(void);
        void setTrigger(magTrigger_t trigger);
        magTrigger_t getTrigger(void);
        void setMagMode(magMode_t mode);
        magMode_t getMagMode(void);
    
    private:
};
