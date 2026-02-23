#ifndef AHT21_MINIMAL_H
#define AHT21_MINIMAL_H

#include <Arduino.h>

//#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
//  #include <TinyWireM.h>
//  #define AHT_WIRE TinyWireM
//#else
  #include <Wire.h>
  #define AHT_WIRE Wire
//#endif

// AHT20 / AHT21 share the same I2C address and commands
#define AHT21_I2C_ADDR      0x38
#define AHT21_CMD_INIT      0xBE
#define AHT21_CMD_TRIGGER   0xAC
#define AHT21_CMD_SOFTRESET 0xBA

// Result struct with fixed-point values
// temperature10: °C * 10  (e.g. 253 = 25.3°C)
// humidity10:    %RH * 10 (e.g. 654 = 65.4%RH)
struct AHT21_Measurement {
  int16_t  temperature10;
  uint16_t humidity10;
};

// Initialize AHT20 / AHT21 (soft reset + init).
// Returns true if the sensor ACKs the commands.
bool aht21_begin();

// Trigger a measurement and read values into 'out'.
// Returns true on success.
bool aht21_read(AHT21_Measurement &out);

#endif
