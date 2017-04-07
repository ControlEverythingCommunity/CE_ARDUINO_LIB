/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT25
        This code is designed to work with the SHT25_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT25_I2CS#tabs-0-product_tabset-2
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
    #define SHT25_DEFAULT_ADDRESS           (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SHT25_CONVERSIONDELAY           (400)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define SHT25_CMD_TEMP_HOLD             (0xE3)
    #define SHT25_CMD_RH_HOLD               (0xE5)
    #define SHT25_CMD_TEMP_NO_HOLD          (0xF3)
    #define SHT25_CMD_RH_NO_HOLD            (0xF5)
    #define SHT25_CMD_SOFT_RESET            (0xFE)
    #define SHT25_CMD_WRITE_USER            (0xE6)
    #define SHT25_CMD_READ_USER             (0xE7)
    #define SHT25_CMD_READ_EID_1_0          (0xFA)
    #define SHT25_CMD_READ_EID_1_1          (0x0F)
    #define SHT25_CMD_READ_EID_2_0          (0xFC)
    #define SHT25_CMD_READ_EID_2_1          (0xC9)
    #define SHT25_CMD_READ_FIRM_REV_0       (0x84)
    #define SHT25_CMD_READ_FIRM_REV_1       (0xB8)

/**************************************************************************
    USER REGISTER
**************************************************************************/
    #define SHT25_USER_RES_MASK             (0x81)  // Measurement Resolution
    #define SHT25_USER_RES_RH12_T14         (0x00)  // RH: 12 bit, Temp: 14 bit
    #define SHT25_USER_RES_RH8_T12          (0x01)  // RH: 8 bit, Temp: 12 bit
    #define SHT25_USER_RES_RH10_T13         (0x80)  // RH: 10 bit, Temp: 13 bit
    #define SHT25_USER_RES_RH11_T11         (0x81)  // RH: 11 bit, Temp: 11 bit

    #define SHT25_USER_VDDS_MASK            (0x40)  // VDD Status
    #define SHT25_USER_VDDS_OK              (0x00)  // VDD OK
    #define SHT25_USER_VDDS_LOW             (0x40)  // VDD LOW

    #define SHT25_USER_HTRE_MASK            (0x02)  // On-Chip Heater Status
    #define SHT25_USER_HTRE_ENABLE          (0x00)  // On-chip Heater Enable
    #define SHT25_USER_HTRE_DISABLE         (0x02)  // On-chip Heater Disable

    #define SHT25_USER_OTP_MASK             (0x01)  // OTP Reload
    #define SHT25_USER_OTP_ENABLE           (0x00)  // OTP Reload Enable
    #define SHT25_USER_OTP_DISABLE          (0x01)  // OTP Reload Disable


typedef enum
{
    TEMP_HOLD           = SHT25_CMD_TEMP_HOLD,
    TEMP_NO_HOLD        = SHT25_CMD_TEMP_NO_HOLD
} shtTempMode_t;

typedef enum
{
    HUMIDITY_HOLD       = SHT25_CMD_RH_HOLD,
    HUMIDITY_NO_HOLD    = SHT25_CMD_RH_NO_HOLD
} shtHumidityMode_t;

typedef enum
{
    RESOLUTION_0        = SHT25_USER_RES_RH12_T14,
    RESOLUTION_1        = SHT25_USER_RES_RH8_T12,
    RESOLUTION_2        = SHT25_USER_RES_RH10_T13,
    RESOLUTION_4        = SHT25_USER_RES_RH11_T11
} shtResolution_t;

typedef enum
{
    VOLTAGE_OK          = SHT25_USER_VDDS_OK,
    VOLTAGE_LOW         = SHT25_USER_VDDS_LOW
} shtVoltage_t;

typedef enum
{
    HEATER_ENABLE       = SHT25_USER_HTRE_ENABLE,
    HEATER_DISABLE      = SHT25_USER_HTRE_DISABLE
} shtHeaterStatus_t;

typedef enum
{
    OTP_ENABLE          = SHT25_USER_OTP_ENABLE,
    OTP_DISABLE         = SHT25_USER_OTP_DISABLE
} shtOTPStatus_t;


class SHT25
{
    protected:
        // Instance-specific properties
        uint8_t sht_conversionDelay;
        shtTempMode_t sht_tempmode;
        shtHumidityMode_t sht_humiditymode;
        shtResolution_t sht_resolution;
        shtVoltage_t sht_voltage;
        shtHeaterStatus_t sht_heaterstatus;
        shtOTPStatus_t sht_otpstatus;

    
    public:
        uint8_t sht_i2cAddress;
        // Initialize SHT25 library with device address
        // Always returns true
        void getAddr_SHT25(uint8_t i2cAddress);
        bool begin(void);
        float Measure_Temperature(void);
        float Measure_Humidity(void);
        void softReset(void);
        uint8_t CRC8(const uint8_t *data, int len);
        void setTempMode(shtTempMode_t tempmode);
        shtTempMode_t getTempMode(void);
        void setHumidityMode(shtHumidityMode_t tempmode);
        shtHumidityMode_t getHumidityMode(void);
        void setResolution(shtResolution_t resolution);
        shtResolution_t getResolution(void);
        void setVoltage(shtVoltage_t voltage);
        shtVoltage_t getVoltage(void);
        void setHeaterStatus(shtHeaterStatus_t heaterstatus);
        shtHeaterStatus_t getHeaterStatus(void);
        void setOTPStatus(shtOTPStatus_t otpstatus);
        shtOTPStatus_t getOTPStatus(void);
    
    private:
};
