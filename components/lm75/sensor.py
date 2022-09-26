import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c, sensor
from esphome.const import (
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
)

DEPENDENCIES = ["i2c"]

# action triggered when in O.S. mode
# sensor enter O.S mode when temperature exceeds the TOS limit, and leaves the active state when the temperature drops below the THYST limit
# CONF_OVER_TEMPERATURE_ACTION = "on_over_temp"
# 80*C by default, actiavate os
CONF_OVER_TEMPERATURE_LIMIT = "over_temp_limit"
# 75*C by default, drop below deactivate os
CONF_HYST_TEMPERATURE_LIMIT = "hyst_temp_limit"
# 1 (00) conversions by default other options 2(01),4(10),6(11)
# define the number of consecutive error conditions that must occur before O.S. is activated
# CONF_OVER_TEMPERATURE_DELAY = "os_delay"
# The most important difference is that in Interrupt mode the O.S. will remain set indefinitely once it has been set.
# To reset O.S. while in Interrupt mode, perform a read from any register in the LM75.
# CONF_OVER_TEMPERATURE_MODE = "mode" # "comparator"(0) default or "interrupt"(1)

lm75_ns = cg.esphome_ns.namespace("lm75")
LM75Component = lm75_ns.class_("LM75Component", cg.PollingComponent, i2c.I2CDevice, sensor.Sensor)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        LM75Component,
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema("0x48"))
    .extend({
        # cv.Optional(CONF_OVER_TEMPERATURE_ACTION): automation.validate_automation(single = True),
        cv.Optional(CONF_OVER_TEMPERATURE_LIMIT, default=80): cv.int_range(min=-55, max=125),
        cv.Optional(CONF_HYST_TEMPERATURE_LIMIT, default=75): cv.int_range(min=-55, max=125),
        # cv.Optional(CONF_OVER_TEMPERATURE_DELAY, default=1): cv.int_range(min=1, max=6),
        # cv.Optional(CONF_OVER_TEMPERATURE_MODE): cv.one_of("comparator", "interrupt") # enum?
    })
)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_over_temperature_limit(config[CONF_OVER_TEMPERATURE_LIMIT]))
    cg.add(var.set_hyst_temperature_limit(config[CONF_HYST_TEMPERATURE_LIMIT]))
