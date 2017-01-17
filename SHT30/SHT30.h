/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT30
        This code is designed to work with the SHT30_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT30_I2CS#tabs-0-product_tabset-2
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
    #define SHT30_DEFAULT_ADDRESS               (0x44)
    #define SHT30_ADDRESS_HIGH                  (0x45)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SHT30_CONVERSIONDELAY               (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define SHT30_MEAS_HIGHREP_STRETCH_EN       (0x2C06)    // High repeatability measurement with clock stretching enabled
    #define SHT30_MEAS_MEDREP_STRETCH_EN        (0x2C0D)    // Medium repeatability measurement with clock stretching enabled
    #define SHT30_MEAS_LOWREP_STRETCH_EN        (0x2C10)    // Low repeatability measurement with clock stretching enabled
    #define SHT30_MEAS_HIGHREP_STRETCH_DIS      (0x2400)    // High repeatability measurement with clock stretching disabled
    #define SHT30_MEAS_MEDREP_STRETCH_DIS       (0x240B)    // Medium repeatability measurement with clock stretching disabled
    #define SHT30_MEAS_LOWREP_STRETCH_DIS       (0x2416)    // Low repeatability measurement with clock stretching disabled
    #define SHT30_CMD_READSTATUS                (0xF32D)    // Command to read out the status register
    #define SHT30_CMD_CLEARSTATUS               (0x3041)    // Command to clear the status register
    #define SHT30_CMD_SOFTRESET                 (0x30A2)    // Soft reset command
    #define SHT30_CMD_HEATERENABLE              (0x306D)    // Heater enable command
    #define SHT30_CMD_HEATERDISABLE             (0x3066)    // Heater disable command


class SHT30
{
    protected:
        // Instance-specific properties
        uint8_t sht_conversionDelay;
        float humidity, temperature;
    
    public:
        SHT30();
        uint8_t sht_i2cAddress;
        void getAddr_SHT30(uint8_t i2cAddress);
        void begin(void);
        float getTemperature(void);
        float getHumidity(void);
        bool Measure_TemperatureAndHumidity(void);
        uint16_t readStatus(void);
        void Reset(void);
        void setHeaterStatus(uint16_t heaterstatus);
        uint8_t CRC8(const uint8_t *data, int len);

    
    private:
};
