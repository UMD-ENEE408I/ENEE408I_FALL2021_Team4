import asyncio
from bleak import BleakClient
import struct
import time

address = "B0:73:A1:FA:50:C4"		#Mouse's ID
MOTOR_CHAR_UUID = "8d527f4b-29f7-4946-b03f-2cd7ee85bcce"	#randomly generated UUID

async def run(address):
    async with BleakClient(address) as client:
        left_value = -15
        right_value = -20

        for i in range(40):
            await client.write_gatt_char(MOTOR_CHAR_UUID, struct.pack('hh', left_value, right_value))

            value = await client.read_gatt_char(MOTOR_CHAR_UUID)
			
            (left_value, right_value) = struct.unpack('hh', value) #struct.unpack converts value to C binary
			
            print("value: {} {}".format(left_value, right_value))

            left_value += 1
            right_value += 1

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address))
