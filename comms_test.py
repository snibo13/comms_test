import smbus
import time

bus = smbus.SMBus(1) #Initialise I2C interface 1
# Confirm pin out

# Test command write
bus.write_i2c_block_data(address, 0x01, [0x00, 0x01 ^ 0x00])
# Command 0x01, Param 0x00 and XOR Checksum
time.sleep(2)

bus.write_i2c_block_data(address, 0x00, [0x01, 0x01])
# Command 0x00 Oaram 0x01 Invalid Checksum
time.sleep(2)

while True:
    try:
        data = bus.read_i2c_block_data(address, 0x00, 3)
        print(f"Data read: {data}")
    except:
        #Handle case where no data is abailable or an error occurs
        print("No data available or error occured during I2C read.")