// https://learn.adafruit.com/scanning-i2c-addresses/arduino

#include "I2C_functions.h"

void i2c_scanner(TwoWire &wire, const char *name) {
    byte error, address;
    int n_devices;

    Serial.print("Scanning for I2C devices ");
    Serial.println(name);
    n_devices = 0;
    for (address = 1; address < 127; address++) {
        wire.beginTransmission(address);
        error = wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
            n_devices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (n_devices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("I2C scan complete\n");

    delay(5000);
}


void reset_i2c_bus() {
    Wire.end();         // Stop communication on the I2C bus
    delay(100);         // Wait for the bus to settle
    Wire.begin();       // Restart the I2C bus
    delay(100);         // Allow the bus to stabilize
}
