import asyncio
from bleak import BleakClient
import struct
import time

address = "B0:73:A1:FA:50:C4"		#Mouse's ID
MOUSE1_UUID = "8d527f4b-29f7-4946-b03f-2cd7ee85bcce"	#randomly generated UUID

async def run(address):
	async with BleakClient(address) as client:
		val_to_send = 99

		#Will send 'val_to_send' to the 'serial_buffer' from the arduino code
		await client.write_gatt_char(MOUSE1_UUID, struct.pack('h',val_to_send))
			#struct.pack('hh', left, right) #IGNORE

		#Will read value of 'serial_buffer' from the arduino code
		value_received = await client.read_gatt_char(MOUSE1_UUID)
		value_received = struct.unpack('h', value_received) #IGNORE

		print("value: {}".format(val_received))

loop = asyncio.get_event_loop()
loop.run_until_complete(run(address))
