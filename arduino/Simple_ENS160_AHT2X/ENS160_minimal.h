#ifndef ENS160_MINIMAL_H
#define ENS160_MINIMAL_H

#include <Arduino.h>
#include <Wire.h>   // USI I2C for ATtiny85

// Alias so we can change later if needed
#define ENS160_WIRE Wire

// ENS160 I2C address (ADDR pin high on most breakout boards)
#define ENS160_I2C_ADDR 0x53

// Registers (short minimal set)
#define ENS160_REG_PART_ID       0x00
#define ENS160_REG_OPMODE        0x10
#define ENS160_REG_DEVICE_STATUS 0x20
#define ENS160_REG_DATA_AQI      0x21  // AQI + TVOC + eCO2 follow

// OPMODE
#define ENS160_OPMODE_DEEPSLEEP  0x00
#define ENS160_OPMODE_IDLE       0x01
#define ENS160_OPMODE_STANDARD   0x02
#define ENS160_OPMODE_RESET      0xF0

// Tiny struct to carry one measurement
struct ENS160_Measurement {
  uint8_t  aqi;   // 1..5, 0 if invalid
  uint16_t tvoc;  // ppb
  uint16_t eco2;  // ppm
};

// Initialize I2C + ENS160 (STANDARD mode).
// Returns true if sensor responds with correct PART_ID.
bool ens160_begin();

// Read AQI / TVOC / eCO2.
// Returns true on NEW + VALID data, false otherwise.
bool ens160_read(ENS160_Measurement &out);

#endif
