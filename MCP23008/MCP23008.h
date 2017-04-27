/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP23008
        This code is designed to work with the MCP23008_I2CR8G5LE_10A_D I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/8-channel-relay-controller-i2c#tabs-0-product_tabset-2
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
                                                                    //         A2  A1  A0
    #define MCP23008_DEFAULT_ADDRESS                    (0x20)      // 0100    0    0   0
    #define MCP23008_ADDRESS_A2A1A0_001                 (0x21)      // 0100    0    0   1
    #define MCP23008_ADDRESS_A2A1A0_010                 (0x22)      // 0100    0    1   0
    #define MCP23008_ADDRESS_A2A1A0_011                 (0x23)      // 0100    0    1   1
    #define MCP23008_ADDRESS_A2A1A0_100                 (0x24)      // 0100    1    0   0
    #define MCP23008_ADDRESS_A2A1A0_101                 (0x25)      // 0100    1    0   1
    #define MCP23008_ADDRESS_A2A1A0_110                 (0x26)      // 0100    1    1   0
    #define MCP23008_ADDRESS_A2A1A0_111                 (0x27)      // 0100    1    1   1

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MCP23008_CONVERSIONDELAY                    (100)       // define the time interal(in ms) between each conversion

/**************************************************************************
    REGISTER
**************************************************************************/
    #define MCP23008_REG_IODIR                          (0x00)      // I/O DIRECTION (IODIR) REGISTER
    #define MCP23008_REG_IPOL                           (0x01)      // INPUT POLARITY (IPOL) REGISTER
    #define MCP23008_REG_GPINTEN                        (0x02)      // INTERRUPT-ON-CHANGE CONTROL (GPINTEN) REGISTER
    #define MCP23008_REG_DEFVAL                         (0x03)      // DEFAULT COMPARE (DEFVAL) REGISTER FOR INTERRUPT-ONCHANGE
    #define MCP23008_REG_INTCON                         (0x04)      // INTERRUPT CONTROL (INTCON) REGISTER
    #define MCP23008_REG_IOCON                          (0x05)      // CONFIGURATION (IOCON) REGISTER
    #define MCP23008_REG_GPPU                           (0x06)      // PULL-UP RESISTOR CONFIGURATION (GPPU) REGISTER
    #define MCP23008_REG_INTF                           (0x07)      // INTERRUPT FLAG (INTF) REGISTER
    #define MCP23008_REG_INTCAP                         (0x08)      // INTERRUPT CAPTURE (INTCAP) REGISTER
    #define MCP23008_REG_GPIO                           (0x09)      // PORT (GPIO) REGISTER
    #define MCP23008_REG_OLAT                           (0x0A)      // OUTPUT LATCH REGISTER (OLAT)


class MCP23008
{
    protected:
        // Instance-specific properties
        uint8_t mcp_conversionDelay;
    
    public:
        uint8_t mcp_i2cAddress;
        void getAddr_MCP23008(uint8_t i2cAddress);
        void begin(void);
        void pinMode(uint8_t pin, uint8_t mode);
        void pullUpRegister(uint8_t pin, uint8_t mode);
        uint8_t readGPIO(void);
        void writeGPIO(uint8_t gpio);
        uint8_t digitalRead(uint8_t pin);
        void digitalWrite(uint8_t pin, uint8_t mode);

    private:
};
