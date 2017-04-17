/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTU20D
        This code is designed to work with the HTU20D_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HTU20D_I2CS#tabs-0-product_tabset-2
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
    #define HTU20D_DEFAULT_ADDRESS           (0x40)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HTU20D_CONVERSIONDELAY           (400)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define HTU20D_CMD_TEMP_HOLD             (0xE3)
    #define HTU20D_CMD_RH_HOLD               (0xE5)
    #define HTU20D_CMD_TEMP_NO_HOLD          (0xF3)
    #define HTU20D_CMD_RH_NO_HOLD            (0xF5)
    #define HTU20D_CMD_WRITE_USER            (0xE6)
    #define HTU20D_CMD_READ_USER             (0xE7)
    #define HTU20D_CMD_SOFT_RESET            (0xFE)

/**************************************************************************
    USER REGISTER
**************************************************************************/
    #define HTU20D_USER_RES_MASK             (0x81)  // Measurement Resolution
    #define HTU20D_USER_RES_RH12_T14         (0x00)  // RH: 12 bit, Temp: 14 bit
    #define HTU20D_USER_RES_RH8_T12          (0x01)  // RH: 8 bit, Temp: 12 bit
    #define HTU20D_USER_RES_RH10_T13         (0x80)  // RH: 10 bit, Temp: 13 bit
    #define HTU20D_USER_RES_RH11_T11         (0x81)  // RH: 11 bit, Temp: 11 bit

    #define HTU20D_USER_VDDS_MASK            (0x40)  // VDD Status
    #define HTU20D_USER_VDDS_OK              (0x00)  // VDD>2.25V
    #define HTU20D_USER_VDDS_LOW             (0x40)  // VDD<2.25V

    #define HTU20D_USER_HTRE_MASK            (0x02)  // On-Chip Heater Status
    #define HTU20D_USER_HTRE_ENABLE          (0x00)  // On-chip Heater Enable
    #define HTU20D_USER_HTRE_DISABLE         (0x02)  // On-chip Heater Disable

    #define HTU20D_USER_OTP_MASK             (0x01)  // OTP Reload
    #define HTU20D_USER_OTP_ENABLE           (0x00)  // OTP Reload Enable
    #define HTU20D_USER_OTP_DISABLE          (0x01)  // OTP Reload Disable


typedef enum
{
    TEMP_HOLD           = HTU20D_CMD_TEMP_HOLD,
    TEMP_NO_HOLD        = HTU20D_CMD_TEMP_NO_HOLD
} htuTempMode_t;

typedef enum
{
    HUMIDITY_HOLD       = HTU20D_CMD_RH_HOLD,
    HUMIDITY_NO_HOLD    = HTU20D_CMD_RH_NO_HOLD
} htuHumidityMode_t;

typedef enum
{
    RESOLUTION_0        = HTU20D_USER_RES_RH12_T14,
    RESOLUTION_1        = HTU20D_USER_RES_RH8_T12,
    RESOLUTION_2        = HTU20D_USER_RES_RH10_T13,
    RESOLUTION_4        = HTU20D_USER_RES_RH11_T11
} htuResolution_t;

typedef enum
{
    VOLTAGE_OK          = HTU20D_USER_VDDS_OK,
    VOLTAGE_LOW         = HTU20D_USER_VDDS_LOW
} htuVoltage_t;

typedef enum
{
    HEATER_ENABLE       = HTU20D_USER_HTRE_ENABLE,
    HEATER_DISABLE      = HTU20D_USER_HTRE_DISABLE
} htuHeaterStatus_t;

typedef enum
{
    OTP_ENABLE          = HTU20D_USER_OTP_ENABLE,
    OTP_DISABLE         = HTU20D_USER_OTP_DISABLE
} htuOTPStatus_t;


class HTU20D
{
    protected:
        // Instance-specific properties
        uint8_t htu_conversionDelay;
        htuTempMode_t htu_tempmode;
        htuHumidityMode_t htu_humiditymode;
        htuResolution_t htu_resolution;
        htuVoltage_t htu_voltage;
        htuHeaterStatus_t htu_heaterstatus;
        htuOTPStatus_t htu_otpstatus;

    
    public:
        uint8_t htu_i2cAddress;
        // Initialize HTU20D library with device address
        // Always returns true
        void getAddr_HTU20D(uint8_t i2cAddress);
        bool begin(void);
        float Measure_Temperature(void);
        float Measure_Humidity(void);
        void softReset(void);
        uint8_t CRC8(const uint8_t *data, int len);
        void setTempMode(htuTempMode_t tempmode);
        htuTempMode_t getTempMode(void);
        void setHumidityMode(htuHumidityMode_t tempmode);
        htuHumidityMode_t getHumidityMode(void);
        void setResolution(htuResolution_t resolution);
        htuResolution_t getResolution(void);
        void setVoltage(htuVoltage_t voltage);
        htuVoltage_t getVoltage(void);
        void setHeaterStatus(htuHeaterStatus_t heaterstatus);
        htuHeaterStatus_t getHeaterStatus(void);
        void setOTPStatus(htuOTPStatus_t otpstatus);
        htuOTPStatus_t getOTPStatus(void);
    
    private:
};
