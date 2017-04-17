/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_ACS714
        This code is designed to work with the ADC121C_I2CS_ACS714 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/1-channel-dc-current-monitor-for-i2c
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC121C_ACS714.h>

ADC121C_ACS714 acs714;


void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_DEFAULT_ADDRESS);         // 0x50, 1010000
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_FLGND);        // 0x51, 1010001
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_FLVA);         // 0x52, 1010010
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_GNDFL);        // 0x54, 1010100
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_GNDGND);       // 0x55, 1010101
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_GNDVA);        // 0x56, 1010110
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_VAFL);         // 0x58, 1011000
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_VAGND);        // 0x59, 1011001
    // acs714.getAddr_ADC121C_ACS714(ADC121C_ACS714_ADDRESS_VAVA);         // 0x5A, 1011010


    // The Automatic Conversion Mode, Alert Hold, Alert Flag Enable,
    // Alert Pin Enable and ALERT pin Polarity
    // can be changed via the following functions

    acs714.setCycleTime(CYCLE_TIME_32);                    // Tconvert x 32, 27 ksps
    // acs714.setCycleTime(AUTOMATIC_MODE_DISABLED);       // Automatic Mode Disabled, 0 ksps
    // acs714.setCycleTime(CYCLE_TIME_64);                 // Tconvert x 64, 13.5 ksps
    // acs714.setCycleTime(CYCLE_TIME_128);                // Tconvert x 128, 6.7 ksps
    // acs714.setCycleTime(CYCLE_TIME_256);                // Tconvert x 256, 3.4 ksps
    // acs714.setCycleTime(CYCLE_TIME_512);                // Tconvert x 512, 1.7 ksps
    // acs714.setCycleTime(CYCLE_TIME_1024);               // Tconvert x 1024, 0.9 ksps
    // acs714.setCycleTime(CYCLE_TIME_2048);               // Tconvert x 2048, 0.4 ksps

    acs714.setAlertHold(ALERT_HOLD_CLEAR);                 // Alerts will self-clear
    // acs714.setAlertHold(ALERT_HOLD_NOCLEAR);            // Alerts will not self-clear

    acs714.setAlertFlag(ALERT_FLAG_DISABLE);               // Disables alert status bit in the Conversion Result register
    // acs714.setAlertFlag(ALERT_FLAG_ENABLE);             // Enables alert status bit in the Conversion Result register

    acs714.setAlertPin(ALERT_PIN_DISABLE);                 // Disables the ALERT output pin
    // acs714.setAlertPin(ALERT_PIN_ENABLE);               // Enables the ALERT output pin

    acs714.setPolarity(POLARITY_LOW);                      // Sets the ALERT pin to active low
    // acs714.setPolarity(POLARITY_HIGH);                  // Sets the ALERT pin to active high

    acs714.begin();
}

void loop(void)
{
    byte error;
    int8_t address;

    address = acs714.ADC121C_ACS714_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t result;

        Serial.println("Getting Current Readings from ADC121C_ACS714");
        Serial.println(" ");
        result = acs714.Measure_Current();
        Serial.print("Instantaneous Current Value: ");
        Serial.print(result, 6);
        Serial.println(" A");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC121C_ACS714 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
