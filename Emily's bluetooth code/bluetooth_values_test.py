import asyncio
from bleak import BleakClient
import struct
import time

address = "C4:D9:AA:CF:80:F5"#B0:73:A1:FA:50:C4" 
#
MOTOR_CHAR_UUID = "080a43de-6b5e-4004-9c9b-4ac9f67b99be"

async def run(address):
    async with BleakClient(address) as client:
        #left_value = -15
        #right_value = -20
        direction = 1 # choice out of 1, 2, 3, 4, 5
        t_start = time.time()
        while(1):
            await client.write_gatt_char(MOTOR_CHAR_UUID, struct.pack('hh', direction, 0))

            value = await client.read_gatt_char(MOTOR_CHAR_UUID)
            (encoder, four_bits) = struct.unpack('hh', value)
            print("encoder: {}, LSR: {}, finished: {}".format(encoder, (four_bits>>1)&0x0000000e, four_bits&0x00000001))

            #left_value += 1
            #right_value += 1
        t_end = time.time()

        #print('{} Hz'.format(40*2 / (t_end-t_start)))

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address))
