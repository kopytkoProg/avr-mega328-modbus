# mega328p-modbus
This is a port FreeModbus lib to mega328p.
## Info
 - Device address: `0x0A`.
 - `Input registers address` from `1000` to `1003` contains own address.
 - `Holdings registers` from `1000` to `1099` can be read and write.
## Example of use witch python [minimalmodbus](https://github.com/pyhys/minimalmodbus) lib

```python
#!/usr/bin/env python
import minimalmodbus

minimalmodbus.BAUDRATE = 38400

# port name, slave address (in decimal)
instrument = minimalmodbus.Instrument('/dev/ttyUSB0', 10, minimalmodbus.MODE_ASCII)

# Display debug info
instrument.debug = True

# Increase timeout because it can provide no response error
instrument.serial.timeout = 0.5

# -----------------------------------------------------

# Read input register
print instrument.read_register(1000, 0, 4)

# Write and read holding registers
for a in range(1000, 1100):
    for i in range(4, 5):
        instrument.write_register(a, a)
        print 'ADDRESS: ', a, ', VALUE: ', instrument.read_register(a)

# Read multiple holding registers
print instrument.read_registers(1000, 100)

# Write multiple holding registers
instrument.write_registers(1000, range(5100, 5200))

# Read multiple holding registers
print instrument.read_registers(1000, 100)
```