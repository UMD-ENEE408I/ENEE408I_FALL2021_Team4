import asyncio
from bleak import BleakClient
import struct
import time

address = "C4:D9:AA:CF:80:F5"
MOTOR_CHAR_UUID = "080a43de-6b5e-4004-9c9b-4ac9f67b99be"

async def run(address):
    async with BleakClient(address) as client:
        left_value = -15
        right_value = -20

        t_start = time.time()
        while(1):
            #await client.write_gatt_char(MOTOR_CHAR_UUID, struct.pack('hh', left_value, right_value))

            value = await client.read_gatt_char(MOTOR_CHAR_UUID)
            (left_value, right_value) = struct.unpack('hh', value)
            print("value: {} {}".format(left_value, right_value))

            #left_value += 1
            #right_value += 1
        t_end = time.time()

        #print('{} Hz'.format(40*2 / (t_end-t_start)))

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address))
