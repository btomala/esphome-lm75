#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace lm75 {

class LM75Component : public PollingComponent, public i2c::I2CDevice, public sensor::Sensor {
  public:
    void setup() override;
    void dump_config() override;
    void update() override;
    float get_setup_priority() const override;

  protected:
    optional<float> read_temp_(const uint8_t *temp_register);
};
} //namespace lm75
} //namespace esphome
