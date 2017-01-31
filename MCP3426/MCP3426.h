/****************************************************************************
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP3426
        This code is designed to work with the MCP3426_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3426_I2CADC#tabs-0-product_tabset-2
*/
/****************************************************************************/

#include <Wire.h>
#include <math.h>

class MCP3426
{
    protected:
        long raw_adc;
        uint8_t SPS;
        bool MODE;
        uint8_t i;
        uint8_t testvar;
        uint8_t config;
        uint8_t GAIN;
        uint8_t no_of_bytes;
        uint8_t data[3];
    
    public:
        uint8_t devAddr;
        MCP3426(uint8_t i2cAddress);
        ~MCP3426();
        bool testConnection(void);
        void SetConfiguration(uint8_t channel, uint8_t resolution, bool mode, uint8_t PGA);
        bool CheckConversion();
        long readADC();

    private:
};
