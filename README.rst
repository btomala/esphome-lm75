esphome-LM75
============

Basic esphome support for LM75.

The LM75 is a 2-wire digital temperature sensor with an analog-to-digital converter and I2C interface. 
The LM75 provides readings with accuracy of +/-2°C from -25°C to 100°C and +/- 3°C from -55°C to 125°C.

The `I²C Bus <https://esphome.io/components/i2c.html>`_ is required to be set up in your configuration for this sensor to work.

Inspired by `esphome-PCT2075 <https://github.com/ell249/esphome-PCT2075>`_ and `tmp102 <https://esphome.io/components/sensor/tmp102.html>` implementations.

**Support only reading temperature!**

Instalation
-----------
Configure external compoenet:

.. code-block:: yaml

external_components:
  - source: github://btomala/esphome-LM75@v1
    components: [ lm75 ]


And addit to the sensors section:

.. code-blosk:: yaml

sensor:
  - platform: lm75
    id: temperature
    name: "Board temperature messurement"
    update_interval: 60s
    address: 0x48

Configuration variables:
------------------------

- **name** (**Required**, string): The name for the temperature sensor.
- **update_interval** (*Optional*, `Time <https://esphome.io/guides/configuration-types.html#config-time>`_): The interval to check the sensor. Defaults to ``60s``.
- **address** (*Optional*, int): The I²C address of the sensor. Defaults to ``0x48``.
- **id** (*Optional*, `ID <https://esphome.io/guides/configuration-types.html#config-id>`_): Set the ID of this sensor for use in lambdas.
- All other options from `Sensor <https://esphome.io/components/sensor/index.html#config-sensor>`_.

TODO
----

Support for:
- setting up configuration
- hysteresis
- over-temperature output (O.S.)

See Also
--------

- `LM75 from Texas Instruments <https://www.ti.com/lit/ds/symlink/lm75b.pdf>`_
