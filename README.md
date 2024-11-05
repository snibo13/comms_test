comms_test contains Arduino code to debug the I2C interface
comms_test.py contains minimal communication code for a RaspberryPi to communicate with the I2C slave device

To test:
 - Impact of I2C config speed
 - Adding delays to loop in Arduino code
 - Removing interupt capable function calls and batching response to end (delays, Serial.println, etc)