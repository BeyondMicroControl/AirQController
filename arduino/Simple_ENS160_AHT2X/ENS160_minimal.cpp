#include "ENS160_minimal.h"

// --- small helpers (no dynamic memory) ---

static void ens160_write_u8(uint8_t reg, uint8_t value) {
  ENS160_WIRE.beginTransmission(ENS160_I2C_ADDR);
  ENS160_WIRE.write(reg);
  ENS160_WIRE.write(value);
  ENS160_WIRE.endTransmission();
}

static uint8_t ens160_read_u8(uint8_t reg) {
  ENS160_WIRE.beginTransmission(ENS160_I2C_ADDR);
  ENS160_WIRE.write(reg);
  ENS160_WIRE.endTransmission(false);  // repeated start

  ENS160_WIRE.requestFrom(ENS160_I2C_ADDR, (uint8_t)1);
  if (ENS160_WIRE.available()) {
    return ENS160_WIRE.read();
  }
  return 0;
}

static void ens160_read_bytes(uint8_t reg, uint8_t *buf, uint8_t len) {
  ENS160_WIRE.beginTransmission(ENS160_I2C_ADDR);
  ENS160_WIRE.write(reg);
  ENS160_WIRE.endTransmission(false);  // repeated start

  ENS160_WIRE.requestFrom(ENS160_I2C_ADDR, len);
  uint8_t i = 0;
  while (ENS160_WIRE.available() && i < len) {
    buf[i++] = ENS160_WIRE.read();
  }
}

// --- public API ---

bool ens160_begin() {
  // USI I2C init
  ENS160_WIRE.begin();

  // Read PART_ID (2 bytes, LSB first, should be 0x0160)
  uint8_t part[2] = {0, 0};
  ens160_read_bytes(ENS160_REG_PART_ID, part, 2);
  uint16_t part_id = (uint16_t)part[1] << 8 | part[0];

  if (part_id != 0x0160) {
    return false;
  }

  // Soft reset
  ens160_write_u8(ENS160_REG_OPMODE, ENS160_OPMODE_RESET);
  delay(10);

  // Idle then Standard
  ens160_write_u8(ENS160_REG_OPMODE, ENS160_OPMODE_IDLE);
  delay(10);
  ens160_write_u8(ENS160_REG_OPMODE, ENS160_OPMODE_STANDARD);

  // Warmup is handled later via STATUS; here we just say "OK, sensor is there".
  return true;
}

bool ens160_read(ENS160_Measurement &out) {
  // Check DEVICE_STATUS:
  // bit1 = NEWDAT, bits3:2 = VALIDITY_FLAG
  uint8_t status = ens160_read_u8(ENS160_REG_DEVICE_STATUS);

  // No new data?
  if ((status & 0x02) == 0) {
    return false;
  }

  uint8_t validity = (status >> 2) & 0x03;
  if (validity == 3) {
    // invalid output
    return false;
  }

  // Read 5 bytes: AQI, TVOC_L, TVOC_H, eCO2_L, eCO2_H
  uint8_t buf[5];
  ens160_read_bytes(ENS160_REG_DATA_AQI, buf, 5);

  ENS160_Measurement m;
  m.aqi  = buf[0] & 0x07;
  m.tvoc = (uint16_t)buf[2] << 8 | buf[1];
  m.eco2 = (uint16_t)buf[4] << 8 | buf[3];

  if (m.aqi == 0) {
    // spec defines 1..5; 0 is "not ready" in practice
    return false;
  }

  out = m;
  return true;
}
