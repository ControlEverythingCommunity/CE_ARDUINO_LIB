/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7006-A20
        This code is designed to work with the SI7006-A20_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7006-A20_I2CS#tabs-0-product_tabset-2
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
    #define SI7006_A20_DEFAULT_ADDRESS          (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SI7006_A20_CONVERSIONDELAY          (300)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define SI7006_A20_CMD_RH_HOLD              (0xE5)
    #define SI7006_A20_CMD_RH_NO_HOLD           (0xF5)
    #define SI7006_A20_CMD_TEMP_HOLD            (0xE3)
    #define SI7006_A20_CMD_TEMP_NO_HOLD         (0xF3)
    #define SI7006_A20_CMD_TEMP_PREV_RH         (0xE0)
    #define SI7006_A20_CMD_RESET                (0xFE)
    #define SI7006_A20_CMD_WRITE_USER_1         (0xE6)
    #define SI7006_A20_CMD_READ_USER_1          (0xE7)
    #define SI7006_A20_CMD_WRITE_HEATER_CTRL    (0x51)
    #define SI7006_A20_CMD_READ_HEATER_CTRL     (0x11)
    #define SI7006_A20_CMD_READ_HEATER_CTRL     (0x11)
    #define SI7006_A20_CMD_READ_EID_1_0         (0xFA)
    #define SI7006_A20_CMD_READ_EID_1_1         (0x0F)
    #define SI7006_A20_CMD_READ_EID_2_0         (0xFC)
    #define SI7006_A20_CMD_READ_EID_2_1         (0xC9)
    #define SI7006_A20_CMD_READ_FIRM_REV_0      (0x84)
    #define SI7006_A20_CMD_READ_FIRM_REV_1      (0xB8)

/**************************************************************************
    USER REGISTER 1
**************************************************************************/
    #define SI7006_A20_USER1_RES_MASK           (0x81)  // Measurement Resolution
    #define SI7006_A20_USER1_RES_RH12_T14       (0x00)  // RH: 12 bit, Temp: 14 bit
    #define SI7006_A20_USER1_RES_RH8_T12        (0x01)  // RH: 8 bit, Temp: 12 bit
    #define SI7006_A20_USER1_RES_RH10_T13       (0x80)  // RH: 10 bit, Temp: 13 bit
    #define SI7006_A20_USER1_RES_RH11_T11       (0x81)  // RH: 11 bit, Temp: 11 bit

    #define SI7006_A20_USER1_VDDS_MASK          (0x40)  // VDD Status
    #define SI7006_A20_USER1_VDDS_OK            (0x00)  // VDD OK
    #define SI7006_A20_USER1_VDDS_LOW           (0x40)  // VDD LOW

    #define SI7006_A20_USER1_HTRE_MASK          (0x02)  // On-Chip Heater Status
    #define SI7006_A20_USER1_HTRE_ENABLE        (0x00)  // On-chip Heater Enable
    #define SI7006_A20_USER1_HTRE_DISABLE       (0x02)  // On-chip Heater Disable

/**************************************************************************
    HEATER CONTROL REGISTER 3
**************************************************************************/
    #define SI7006_A20_HEATER_MASK              (0x0F)  // Heater Current
    #define SI7006_A20_HEATER_3_09              (0x00)  // 3.09 mA
    #define SI7006_A20_HEATER_9_18              (0x01)  // 9.18 mA
    #define SI7006_A20_HEATER_15_24             (0x02)  // 15.24 mA
    #define SI7006_A20_HEATER_21_33             (0x03)  // 21.33 mA
    #define SI7006_A20_HEATER_27_39             (0x04)  // 27.39 mA
    #define SI7006_A20_HEATER_33_48             (0x05)  // 33.48 mA
    #define SI7006_A20_HEATER_39_54             (0x06)  // 39.54 mA
    #define SI7006_A20_HEATER_45_63             (0x07)  // 45.63 mA
    #define SI7006_A20_HEATER_51_69             (0x08)  // 51.69 mA
    #define SI7006_A20_HEATER_57_78             (0x09)  // 57.78 mA
    #define SI7006_A20_HEATER_63_84             (0x0A)  // 63.84 mA
    #define SI7006_A20_HEATER_69_93             (0x0B)  // 69.93 mA
    #define SI7006_A20_HEATER_75_99             (0x0C)  // 75.99 mA
    #define SI7006_A20_HEATER_82_08             (0x0D)  // 82.08 mA
    #define SI7006_A20_HEATER_88_14             (0x0E)  // 88.14 mA
    #define SI7006_A20_HEATER_94_20             (0x0F)  // 94.20 mA


typedef enum
{
    TEMP_HOLD           = SI7006_A20_CMD_TEMP_HOLD,
    TEMP_NO_HOLD        = SI7006_A20_CMD_TEMP_NO_HOLD
} siTempMode_t;

typedef enum
{
    HUMIDITY_HOLD       = SI7006_A20_CMD_RH_HOLD,
    HUMIDITY_NO_HOLD    = SI7006_A20_CMD_RH_NO_HOLD
} siHumidityMode_t;

typedef enum
{
    RESOLUTION_0        = SI7006_A20_USER1_RES_RH12_T14,
    RESOLUTION_1        = SI7006_A20_USER1_RES_RH8_T12,
    RESOLUTION_2        = SI7006_A20_USER1_RES_RH10_T13,
    RESOLUTION_4        = SI7006_A20_USER1_RES_RH11_T11
} siResolution_t;

typedef enum
{
    VOLTAGE_OK          = SI7006_A20_USER1_VDDS_OK,
    VOLTAGE_LOW         = SI7006_A20_USER1_VDDS_LOW
} siVoltage_t;

typedef enum
{
    HEATER_ENABLE       = SI7006_A20_USER1_HTRE_ENABLE,
    HEATER_DISABLE      = SI7006_A20_USER1_HTRE_DISABLE
} siHeaterStatus_t;

typedef enum
{
    HEATER_3_09         = SI7006_A20_HEATER_3_09,
    HEATER_9_18         = SI7006_A20_HEATER_9_18,
    HEATER_15_24        = SI7006_A20_HEATER_15_24,
    HEATER_21_33        = SI7006_A20_HEATER_21_33,
    HEATER_27_39        = SI7006_A20_HEATER_27_39,
    HEATER_33_48        = SI7006_A20_HEATER_33_48,
    HEATER_39_54        = SI7006_A20_HEATER_39_54,
    HEATER_45_63        = SI7006_A20_HEATER_45_63,
    HEATER_51_69        = SI7006_A20_HEATER_51_69,
    HEATER_57_78        = SI7006_A20_HEATER_57_78,
    HEATER_63_84        = SI7006_A20_HEATER_63_84,
    HEATER_69_93        = SI7006_A20_HEATER_69_93,
    HEATER_75_99        = SI7006_A20_HEATER_75_99,
    HEATER_82_08        = SI7006_A20_HEATER_82_08,
    HEATER_88_14        = SI7006_A20_HEATER_88_14,
    HEATER_94_20        = SI7006_A20_HEATER_94_20
} siHeaterCurrent_t;


class SI7006_A20
{
    protected:
        // Instance-specific properties
        uint8_t si_conversionDelay;
        siTempMode_t si_tempmode;
        siHumidityMode_t si_humiditymode;
        siResolution_t si_resolution;
        siVoltage_t si_voltage;
        siHeaterStatus_t si_heaterstatus;
        siHeaterCurrent_t si_heatercurrent;
    
    public:
        uint8_t si_i2cAddress;
        // Initialize SI7006 library with device address
        // Always returns true
        void getAddr_SI7006_A20(uint8_t i2cAddress);
        void begin(void);
        float Measure_Temperature(void);
        float Measure_OldTemperature(void);
        float Measure_Humidity(void);
        void Reset(void);
        void setTempMode(siTempMode_t tempmode);
        siTempMode_t getTempMode(void);
        void setHumidityMode(siHumidityMode_t tempmode);
        siHumidityMode_t getHumidityMode(void);
        void setResolution(siResolution_t resolution);
        siResolution_t getResolution(void);
        void setVoltage(siVoltage_t voltage);
        siVoltage_t getVoltage(void);
        void setHeaterStatus(siHeaterStatus_t heaterstatus);
        siHeaterStatus_t getHeaterStatus(void);
        void setHeaterCurrent(siHeaterCurrent_t heatercurrent);
        siHeaterCurrent_t getHeaterCurrent(void);
    
    private:
};
