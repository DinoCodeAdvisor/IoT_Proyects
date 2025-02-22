import machine

# Reading the internal diode in our Raspberry Pi Pico W
adc = machine.ADC(4) 

def read_temperature():
    adc_value = adc.read_u16()
    volt = (3.3/65535) * adc_value
    temperature = 27 - (volt - 0.706)/0.001721
    return temperature
