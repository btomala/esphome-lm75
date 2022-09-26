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

void LM75Component::setup() {
   ESP_LOGCONFIG(TAG, "Setting up LM75...");
 }

void LM75Component::dump_config() {
  ESP_LOGCONFIG(TAG, "LM75:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with LM75 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  optional<float> temperature_os = this->read_temp_(&LM75_REGISTER_TOS);
  if(temperature_os)
    ESP_LOGCONFIG(TAG, "  O.S. Temperature: %.1f°C", *temperature_os);
  optional<float> temperature_hyst = this->read_temp_(&LM75_REGISTER_THYST);
  if(temperature_hyst)
    ESP_LOGCONFIG(TAG, "  Hysterisis Temperature: %.1f°C", *temperature_hyst);
  LOG_SENSOR("  ", "Temperature", this);
}

void LM75Component::set_over_temperature_limit(int temperature){
  ESP_LOGD(TAG, "Setting Over Temperature: %.1f°C", temperature);
  optional<float> new_temperature = this->write_temp_(&LM75_REGISTER_TOS, temperature);
  if(new_temperature)
    ESP_LOGD(TAG, "New Over Temperature: %.1f°C", *new_temperature);
}
void LM75Component::set_hyst_temperature_limit(int temperature){
  ESP_LOGD(TAG, "Setting Hyst Temperature: %.1f°C", temperature);
  optional<float> new_temperature = this->write_temp_(&LM75_REGISTER_THYST, temperature);
  if(new_temperature)
    ESP_LOGD(TAG, "New Hyst Temperature: %.1f°C", *new_temperature);
}

optional<float> LM75Component::write_temp_(const uint8_t *temp_register, int temperature) {
  uint16_t raw_temperature;
  bool stop = false;
  raw_temperature = temperature / LM75_CONVERSION_FACTOR;
  raw_temperature = raw_temperature << LM75_REGISTER_DATA_SHIFT;
  if (this->write(temp_register, 1, stop) != i2c::ERROR_OK) {
    this->status_set_warning();
    return {};
  }
  if (this->write(reinterpret_cast<uint8_t *>(&raw_temperature), 2) != i2c::ERROR_OK) {
    this->status_set_warning();
    return {};
  }
  return this->read_temp_(temp_register);
}

optional<float> LM75Component::read_temp_(const uint8_t *temp_register) {
  uint16_t raw_temperature;
  if (this->write(temp_register, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return {};
  }
  if (this->read(reinterpret_cast<uint8_t *>(&raw_temperature), 2) != i2c::ERROR_OK) {
    this->status_set_warning();
    return {};
  }
  raw_temperature = i2c::i2ctohs(raw_temperature);
  raw_temperature = raw_temperature >> LM75_REGISTER_DATA_SHIFT;
  return raw_temperature * LM75_CONVERSION_FACTOR;
}


void LM75Component::update() {
  optional<float> temperature = this->read_temp_(&LM75_REGISTER_TEMP);
  if (temperature) {
    ESP_LOGD(TAG, "Got Temperature=%.1f°C", *temperature);
    this->publish_state(*temperature);
    this->status_clear_warning();
  }
}

float LM75Component::get_setup_priority() const { return setup_priority::DATA; }

} // namespace lm75
} // namespace esphome
