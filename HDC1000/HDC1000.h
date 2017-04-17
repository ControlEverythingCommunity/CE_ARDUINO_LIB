/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HDC1000
        This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2
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
//                                                                      ADR1    ADR0    SLAVE ADDRESS
    #define HDC1000_DEFAULT_ADDRESS                 (0x40)      //      0       0      1000000
    #define HDC1000_0_1_ADDRESS                     (0x41)      //      0       1      1000001
    #define HDC1000_1_0_ADDRESS                     (0x42)      //      1       0      1000010
    #define HDC1000_1_1_ADDRESS                     (0x43)      //      1       1      1000100
    #define HDC1000_MANUF_ID                        (0x5449)
    #define HDC1000_DEV_ID                          (0x1000)


/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HDC1000_CONVERSIONDELAY                 (300)

/**************************************************************************
    REGISTER MAP DESCRIPTION
**************************************************************************/
    #define HDC1000_REG_TEMP                        (0x00)      // Temperature Measurement Output
    #define HDC1000_REG_HUMIDITY                    (0x01)      // Relative Humidity Measurement Output
    #define HDC1000_REG_CONFIG                      (0x02)      // Configuration and Status
    #define HDC1000_REG_SERIAL_ID_FIRST             (0xFB)      // First 2 Bytes of the Serial ID of the Part
    #define HDC1000_REG_SERIAL_ID_MID               (0xFC)      // Mid 2 Bytes of the Serial ID of the Part
    #define HDC1000_REG_SERIAL_ID_LAST              (0xFD)      // Last Byte of the Serial ID of the Part
    #define HDC1000_REG_MANUF_ID                    (0xFE)      // ID of Texas Instruments
    #define HDC1000_REG_DEVICE_ID                   (0xFF)      // ID of HDC1000 Device

/**************************************************************************
    CONFIGURATION REGISTER
**************************************************************************/
    #define HDC1000_REG_CONFIG_RST_MASK             (0x8000)  // Software Reset
    #define HDC1000_REG_CONFIG_RST_NORMAL           (0x0000)  // Normal Operation
    #define HDC1000_REG_CONFIG_RST_RESET            (0x8000)  // Software Reset

    #define HDC1000_REG_CONFIG_HEAT_MASK            (0x2000)  // Heater Status
    #define HDC1000_REG_CONFIG_HEAT_ENABLE          (0x0000)  // Heater Enable
    #define HDC1000_REG_CONFIG_HEAT_DISABLE         (0x2000)  // Heater Disable

    #define HDC1000_REG_CONFIG_MODE_MASK            (0x1000)  // Mode of Acquisition
    #define HDC1000_REG_CONFIG_MODE_TEMP_OR_HUM     (0x0000)  // Temperature or Humidity is Acquired
    #define HDC1000_REG_CONFIG_MODE_TEMP_HUM_BOTH   (0x1000)  // Temperature and Humidity are Acquired in Sequence, Temperature First

    #define HDC1000_REG_CONFIG_BTST_MASK            (0x0800)  // Battery Status
    #define HDC1000_REG_CONFIG_BTST_HIGH            (0x0000)  // Battery Voltage > 2.8V
    #define HDC1000_REG_CONFIG_BTST_LOW             (0x0800)  // Battery Voltage < 2.8V

    #define HDC1000_REG_CONFIG_TRES_MASK            (0x0400)  // Temperature Measurement Resolution
    #define HDC1000_REG_CONFIG_TRES_14BIT           (0x0000)  // 14 Bit
    #define HDC1000_REG_CONFIG_TRES_11BIT           (0x0400)  // 11 Bit

    #define HDC1000_REG_CONFIG_HRES_MASK            (0x0300)  // Humidity Measurement Resolution
    #define HDC1000_REG_CONFIG_HRES_14BIT           (0x0000)  // 14 Bit
    #define HDC1000_REG_CONFIG_HRES_11BIT           (0x0100)  // 11 Bit
    #define HDC1000_REG_CONFIG_HRES_8BIT            (0x0200)  // 8 Bit


typedef enum
{
    RESET_NORMAL            = HDC1000_REG_CONFIG_RST_NORMAL,
    RESET_DONE              = HDC1000_REG_CONFIG_RST_RESET
    
} hdcReset_t;

typedef enum
{
    HEATER_ENABLE           = HDC1000_REG_CONFIG_HEAT_ENABLE,
    HEATER_DISABLE          = HDC1000_REG_CONFIG_HEAT_DISABLE
    
} hdcHeaterStatus_t;

typedef enum
{
    TEMP_OR_HUMIDITY        = HDC1000_REG_CONFIG_MODE_TEMP_OR_HUM,
    TEMP_HUM_BOTH           = HDC1000_REG_CONFIG_MODE_TEMP_HUM_BOTH
    
} hdcMode_t;

typedef enum
{
    VOLTAGE_HIGH            = HDC1000_REG_CONFIG_BTST_HIGH,
    VOLTAGE_LOW             = HDC1000_REG_CONFIG_BTST_LOW
    
} hdcVoltage_t;

typedef enum
{
    TEMP_RESOLUTION_14      = HDC1000_REG_CONFIG_TRES_14BIT,
    TEMP_RESOLUTION_11      = HDC1000_REG_CONFIG_TRES_11BIT
    
} hdcTempResolution_t;

typedef enum
{
    HUM_RESOLUTION_14       = HDC1000_REG_CONFIG_HRES_14BIT,
    HUM_RESOLUTION_11       = HDC1000_REG_CONFIG_HRES_11BIT,
    HUM_RESOLUTION_8        = HDC1000_REG_CONFIG_HRES_8BIT
    
} hdcHumResolution_t;

typedef struct
{
    float T;
    float H;
    
} hdcSensorData_t;

class HDC1000
{
    protected:
        // Instance-specific properties
        uint8_t hdc_conversionDelay;
        hdcReset_t hdc_reset;
        hdcHeaterStatus_t hdc_heaterstatus;
        hdcMode_t hdc_mode;
        hdcVoltage_t hdc_voltage;
        hdcTempResolution_t hdc_tempresolution;
        hdcHumResolution_t hdc_humresolution;
    
    public:
        uint8_t hdc_i2cAddress;
        hdcSensorData_t hdc_sensorData;
        // Initialize library with device address
        // Always returns true
        void getAddr_HDC1000(uint8_t i2cAddress);
        bool begin(void);
        void Measure_Temperature(void);
        void Measure_Humidity(void);
        void setConfig(void);
        void setReset(hdcReset_t reset);
        hdcReset_t getReset(void);
        void setHeaterStatus(hdcHeaterStatus_t heaterstatus);
        hdcHeaterStatus_t getHeaterStatus(void);
        void setMode(hdcMode_t mode);
        hdcMode_t getMode(void);
        void setVoltage(hdcVoltage_t voltage);
        hdcVoltage_t getVoltage(void);
        void setTempResolution(hdcTempResolution_t tempresolution);
        hdcTempResolution_t getTempResolution(void);
        void setHumResolution(hdcHumResolution_t humresolution);
        hdcHumResolution_t getHumResolution(void);
    
    private:
};
