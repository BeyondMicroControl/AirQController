#include "AHT21_minimal.h"

// Small helpers

static bool aht21_write_cmd3(uint8_t cmd, uint8_t d1, uint8_t d2) {
  AHT_WIRE.beginTransmission(AHT21_I2C_ADDR);
  AHT_WIRE.write(cmd);
  AHT_WIRE.write(d1);
  AHT_WIRE.write(d2);
  return (AHT_WIRE.endTransmission() == 0);
}

static bool aht21_write_u8(uint8_t cmd) {
  AHT_WIRE.beginTransmission(AHT21_I2C_ADDR);
  AHT_WIRE.write(cmd);
  return (AHT_WIRE.endTransmission() == 0);
}

bool aht21_begin() {
  // Start I2C
  AHT_WIRE.begin();

  // Simple presence check: send a soft reset, expect ACK
  if (!aht21_write_u8(AHT21_CMD_SOFTRESET)) {
    return false;
  }
  delay(20);   // datasheet: >20 ms after soft reset

  // Init command: 0xBE 0x08 0x00
  if (!aht21_write_cmd3(AHT21_CMD_INIT, 0x08, 0x00)) {
    return false;
  }
  delay(10);   // allow init to complete

  // From here on, sensor will auto-calibrate after first few measurements.
  return true;
}

bool aht21_read(AHT21_Measurement &out) {
  // Trigger measurement: 0xAC, 0x33, 0x00
  if (!aht21_write_cmd3(AHT21_CMD_TRIGGER, 0x33, 0x00)) {
    return false;
  }

  // Wait for measurement to complete.
  // For AHT20/AHT21, ~80 ms is safe.
  delay(80);

  // Read 6 bytes: status + 5 data bytes
  uint8_t buf[6];
  AHT_WIRE.requestFrom(AHT21_I2C_ADDR, (uint8_t)6);
  for (uint8_t i = 0; i < 6; i++) {
    if (!AHT_WIRE.available()) return false;
    buf[i] = AHT_WIRE.read();
  }

  uint8_t status = buf[0];

  // Optional: check busy bit (bit7). If still busy, abort.
  if (status & 0x80) {
    return false;
  }

  // Humidity: 20 bits from buf[1..3]
  //  raw_h = (b1 << 12) | (b2 << 4) | (b3 >> 4)
  uint32_t raw_h = ((uint32_t)buf[1] << 12) |
                   ((uint32_t)buf[2] << 4)  |
                   ((uint32_t)buf[3] >> 4);

  // Temperature: 20 bits from buf[3..5]
  //  raw_t = ((b3 & 0x0F) << 16) | (b4 << 8) | b5
  uint32_t raw_t = (((uint32_t)buf[3] & 0x0F) << 16) |
                   ((uint32_t)buf[4] << 8) |
                   ((uint32_t)buf[5]);

  // Convert to fixed-point without floats.
  // Formulas from datasheet:
  //   RH[%]   = raw_h / 2^20 * 100
  //   T[°C]   = raw_t / 2^20 * 200 - 50
  //
  // We want:
  //   RH*10   = raw_h * 1000 / 2^20
  //   T*10    = raw_t * 2000 / 2^20 - 500

  uint32_t rh10_u32 = (raw_h * 1000UL + (1UL << 19)) >> 20;   // rounded
  uint32_t t10_u32  = (raw_t * 2000UL + (1UL << 19)) >> 20;   // rounded
  int32_t  t10_i32  = (int32_t)t10_u32 - 500;                 // subtract 50.0°C -> 500 in *10

  if (rh10_u32 > 1000) rh10_u32 = 1000;   // clamp to 100.0%RH

  out.humidity10    = (uint16_t)rh10_u32;
  out.temperature10 = (int16_t)t10_i32;

  return true;
}
