/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP9805
        This code is designed to work with the MCP9805_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=MCP9805_I2CS#tabs-0-product_tabset-2
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
/*
                                                                  Address
                                                                   Code    A2  A1  A0
*/
    #define MCP9805_DEFAULT_ADDRESS             (0x18)          // 0011     0    0   0
    #define MCP9805_ADDRESS_A2A1A0_001          (0x19)          // 0011     0    0   1
    #define MCP9805_ADDRESS_A2A1A0_010          (0x1A)          // 0011     0    1   0
    #define MCP9805_ADDRESS_A2A1A0_011          (0x1B)          // 0011     0    1   1
    #define MCP9805_ADDRESS_A2A1A0_100          (0x1C)          // 0011     1    0   0
    #define MCP9805_ADDRESS_A2A1A0_101          (0x1D)          // 0011     1    0   1
    #define MCP9805_ADDRESS_A2A1A0_110          (0x1E)          // 0011     1    1   0
    #define MCP9805_ADDRESS_A2A1A0_111          (0x1F)          // 0011     1    1   1

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define MCP9805_CONVERSIONDELAY             (100)

/**************************************************************************
    POINTER REGISTER
**************************************************************************/
    #define MCP9805_REG_POINTER_MASK            (0x08)
    #define MCP9805_REG_POINTER_RFU             (0x00)
    #define MCP9805_REG_POINTER_CONFIG          (0x01)
    #define MCP9805_REG_POINTER_TUPPER          (0x02)
    #define MCP9805_REG_POINTER_TLOWER          (0x03)
    #define MCP9805_REG_POINTER_TCRIT           (0x04)
    #define MCP9805_REG_POINTER_TEMP            (0x05)
    #define MCP9805_REG_POINTER_MANUFID         (0x06)
    #define MCP9805_REG_POINTER_DEVID           (0x07)
    #define MCP9805_REG_POINTER_RES             (0x08)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/

    #define MCP9805_REG_CONFIG_THYST_MASK       (0x0600)    // TUPPER and TLOWER Limit Hysteresis bits
    #define MCP9805_REG_CONFIG_THYST_0C         (0x0000)    // 0°C (power-up default)
    #define MCP9805_REG_CONFIG_THYST_1_5C       (0x0200)    // +1.5°C
    #define MCP9805_REG_CONFIG_THYST_3C         (0x0400)    // +3.0°C
    #define MCP9805_REG_CONFIG_THYST_6C         (0x0600)    // +6.0°C

    #define MCP9805_REG_CONFIG_SHDN_MASK        (0x0100)    // Shutdown Mode bit
    #define MCP9805_REG_CONFIG_SHDN_CONT        (0x0000)    // Continuous conversion (power-up default)
    #define MCP9805_REG_CONFIG_SHDN_SHUT        (0x0100)    // Shutdown (Low-Power mode)

    #define MCP9805_REG_CONFIG_TCRIT_MASK       (0x0080)    // TCRIT Lock bit
    #define MCP9805_REG_CONFIG_TCRIT_UNLOCK     (0x0000)    // Unlocked. TCRIT register can be written (power-up default)
    #define MCP9805_REG_CONFIG_TCRIT_LOCK       (0x0080)    // Locked; TUPPER and TLOWER registers can not be written

    #define MCP9805_REG_CONFIG_TWIN_MASK        (0x0040)    // TUPPER and TLOWER Window Lock bit
    #define MCP9805_REG_CONFIG_TWIN_UNLOCK      (0x0000)    // Unlocked; TUPPER and TLOWER registers can be written (power-up default)
    #define MCP9805_REG_CONFIG_TWIN_LOCK        (0x0040)    //  Locked; TUPPER and TLOWER registers can not be written

    #define MCP9805_REG_CONFIG_INTCL_MASK       (0x0020)    // Interrupt Clear bit
    #define MCP9805_REG_CONFIG_INTCL_NOEFF      (0x0000)    // No effect (power-up default)
    #define MCP9805_REG_CONFIG_INTCL_CLEAR      (0x0020)    // Clear interrupt output; when read, this bit returns to ‘0’

    #define MCP9805_REG_CONFIG_ALERTSTAT_MASK   (0x0010)    // Alert Output Status bit
    #define MCP9805_REG_CONFIG_ALERTSTAT_NOASRT (0x0000)    // Alert output is not asserted by the device (power-up default)
    #define MCP9805_REG_CONFIG_ALERTSTAT_ASRT   (0x0010)    // Alert output is asserted as a comparator/Interrupt or critical temperature output

    #define MCP9805_REG_CONFIG_ALERTCNT_MASK    (0x0008)    // Alert Output Control bit
    #define MCP9805_REG_CONFIG_ALERTCNT_DISABLE (0x0000)    // Disabled (power-up default)
    #define MCP9805_REG_CONFIG_ALERTCNT_ENABLE  (0x0008)    // Enabled

    #define MCP9805_REG_CONFIG_ALERTSEL_MASK    (0x0004)    // Alert Output Select bit
    #define MCP9805_REG_CONFIG_ALERTSEL_ALL     (0x0000)    // Alert output for TUPPER, TLOWER and TCRIT (power-up default)
    #define MCP9805_REG_CONFIG_ALERTSEL_SELCT   (0x0004)    // TA > TCRIT only (TUPPER and TLOWER temperature boundaries are disabled)

    #define MCP9805_REG_CONFIG_ALERTPOL_MASK    (0x0002)    // Alert Output Polarity bit
    #define MCP9805_REG_CONFIG_ALERTPOL_LOW     (0x0000)    // Active-low (power-up default; pull-up resistor required)
    #define MCP9805_REG_CONFIG_ALERTPOL_HIGH    (0x0002)    // Active-high

    #define MCP9805_REG_CONFIG_ALERTMOD_MASK    (0x0001)    // Alert Output Mode bit
    #define MCP9805_REG_CONFIG_ALERTMOD_COMP    (0x0000)    // Comparator output (power-up default)
    #define MCP9805_REG_CONFIG_ALERTMOD_INTR    (0x0001)    // Interrupt output

/**************************************************************************
    RESOLUTION REGISTER
**************************************************************************/

    #define MCP9805_REG_RES_MASK                (0x03)      // Resolution bits
    #define MCP9805_REG_RES_0_5                 (0x00)      // +0.5°C (tCONV = 30 ms typical)
    #define MCP9805_REG_RES_0_25                (0x01)      // +0.25°C (tCONV = 65 ms typical)
    #define MCP9805_REG_RES_0_125               (0x02)      // +0.125°C (tCONV = 130 ms typical)
    #define MCP9805_REG_RES_0_0625              (0x03)      // +0.0625°C (power-up default, tCONV = 250 ms typical)


typedef enum
{
    THYST_0C            = MCP9805_REG_CONFIG_THYST_0C,
    THYST_1_5C          = MCP9805_REG_CONFIG_THYST_1_5C,
    THYST_3C            = MCP9805_REG_CONFIG_THYST_3C,
    THYST_6C            = MCP9805_REG_CONFIG_THYST_6C
} mcpTHyst_t;

typedef enum
{
    SHDN_CONT           = MCP9805_REG_CONFIG_SHDN_CONT,
    SHDN_SHUT           = MCP9805_REG_CONFIG_SHDN_SHUT
} mcpTShdn_t;

typedef enum
{
    TCRIT_UNLOCK        = MCP9805_REG_CONFIG_TCRIT_UNLOCK,
    TCRIT_LOCK          = MCP9805_REG_CONFIG_TCRIT_LOCK
} mcpTCrit_t;

typedef enum
{
    TWIN_UNLOCK         = MCP9805_REG_CONFIG_TWIN_UNLOCK,
    TWIN_LOCK           = MCP9805_REG_CONFIG_TWIN_LOCK
} mcpTWin_t;

typedef enum
{
    INTCL_NOEFF         = MCP9805_REG_CONFIG_INTCL_NOEFF,
    INTCL_CLEAR         = MCP9805_REG_CONFIG_INTCL_CLEAR
} mcpInctl_t;

typedef enum
{
    ALERTSTAT_NOASRT    = MCP9805_REG_CONFIG_ALERTSTAT_NOASRT,
    ALERTSTAT_ASRT      = MCP9805_REG_CONFIG_ALERTSTAT_ASRT
} mcpAlertStat_t;

typedef enum
{
    ALERTCNT_DISABLE    = MCP9805_REG_CONFIG_ALERTCNT_DISABLE,
    ALERTCNT_ENABLE     = MCP9805_REG_CONFIG_ALERTCNT_ENABLE
} mcpAlertCtrl_t;

typedef enum
{
    ALERTSEL_ALL        = MCP9805_REG_CONFIG_ALERTSEL_ALL,
    ALERTSEL_SELCT      = MCP9805_REG_CONFIG_ALERTSEL_SELCT
} mcpAlertSlct_t;

typedef enum
{
    ALERTPOL_LOW        = MCP9805_REG_CONFIG_ALERTPOL_LOW,
    ALERTPOL_HIGH       = MCP9805_REG_CONFIG_ALERTPOL_HIGH
} mcpAlertPol_t;

typedef enum
{
    ALERTMOD_COMP       = MCP9805_REG_CONFIG_ALERTMOD_COMP,
    ALERTMOD_INTR       = MCP9805_REG_CONFIG_ALERTMOD_INTR
} mcpAlertMod_t;

typedef enum
{
    RES_0_5             = MCP9805_REG_RES_0_5,
    RES_0_25            = MCP9805_REG_RES_0_25,
    RES_0_125           = MCP9805_REG_RES_0_125,
    RES_0_0625          = MCP9805_REG_RES_0_0625
    
} mcpRes_t;


class MCP9805
{
    protected:
        // Instance-specific properties
        uint8_t mcp_conversionDelay;
        mcpTHyst_t mcp_thyst;
        mcpTShdn_t mcp_tshdn;
        mcpTCrit_t mcp_tcrit;
        mcpTWin_t mcp_twin;
        mcpInctl_t mcp_inctl;
        mcpAlertStat_t mcp_alertstat;;
        mcpAlertCtrl_t mcp_alertctrl;
        mcpAlertSlct_t mcp_alertslct;
        mcpAlertPol_t mcp_alertpol;
        mcpAlertMod_t mcp_alertmod;
        mcpRes_t mcp_res;

    public:
        uint8_t mcp_i2cAddress;
        void getAddr_MCP9805(uint8_t i2cAddress);
        boolean begin(void);
        int16_t Measure_Temp(void);
        void setTHyst(mcpTHyst_t thyst);
        mcpTHyst_t getTHyst(void);
        void setTShdn(mcpTShdn_t tshdn);
        mcpTShdn_t getTShdn(void);
        void setTCrit(mcpTCrit_t tcrit);
        mcpTCrit_t getTCrit(void);
        void setTWin(mcpTWin_t twin);
        mcpTWin_t getTWin(void);
        void setInctl(mcpInctl_t inctl);
        mcpInctl_t getInctl(void);
        void setAlertStat(mcpAlertStat_t alertstat);
        mcpAlertStat_t getAlertStat(void);
        void setAlertCtrl(mcpAlertCtrl_t alertctrl);
        mcpAlertCtrl_t getAlertCtrl(void);
        void setAlertSlct(mcpAlertSlct_t alertslct);
        mcpAlertSlct_t getAlertSlct(void);
        void setAlertPol(mcpAlertPol_t alertpol);
        mcpAlertPol_t getAlertPol(void);
        void setAlertMod(mcpAlertMod_t alertmod);
        mcpAlertMod_t getAlertMod(void);
        void setRes(mcpRes_t res);
        mcpRes_t getRes(void);

    private:
};
