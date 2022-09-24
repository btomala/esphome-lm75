#include "lm75.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lm75 {

static const uint8_t LM75_REGISTER_TEMP = 0x00;   //  Temperature register
static const uint8_t LM75_REGISTER_CONFIG = 0x01; //  Configuration register
static const uint8_t LM75_REGISTER_THYST = 0x02;  //  Hysterisis temp register
static const uint8_t LM75_REGISTER_TOS = 0x03;    //  OS temp register

static const char *const TAG = "lm75";

static const float LM75_CONVERSION_FACTOR = 0.125;
static const uint8_t LM75_REGISTER_DATA_SHIFT = 5;

void LM75Component::setup() { ESP_LOGCONFIG(TAG, "Setting up LM75..."); }

void LM75Component::dump_config() {
  ESP_LOGCONFIG(TAG, "LM75:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with LM75 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  float temperature_os = this->read_temp_(&LM75_REGISTER_TOS);
  ESP_LOGD(TAG, "O.S. Temperature=%.1f°C", temperature_os);
  float temperature_hyst = this->read_temp_(&LM75_REGISTER_THYST);
  ESP_LOGD(TAG, "Hysterisis Temperature=%.1f°C", temperature_hyst);
  LOG_SENSOR("  ", "Temperature", this);
}

float LM75Component::read_temp_(uint8_t temp_register) {
  uint16_t raw_temperature;
  if (this->write(temp_register, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  if (this->read(reinterpret_cast<uint8_t *>(&raw_temperature), 2) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  raw_temperature = i2c::i2ctohs(raw_temperature);
  raw_temperature = raw_temperature >> LM75_REGISTER_DATA_SHIFT;
  return raw_temperature * LM75_CONVERSION_FACTOR;
}

void LM75Component::update() {
  float temperature = this->read_temp_(&LM75_REGISTER_TEMP);
  ESP_LOGD(TAG, "Got Temperature=%.1f°C", temperature);

  this->publish_state(temperature);
  this->status_clear_warning();
}

float LM75Component::get_setup_priority() const { return setup_priority::DATA; }

} // namespace lm75
} // namespace esphome
