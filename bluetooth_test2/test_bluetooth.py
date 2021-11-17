import asyncio
from bleak import BleakClient
from bleak import BleakScanner
from bleak import discover

async def run():
	devices = await discover()

	for d in devices:
		if 'Tin_Mouse' in d.name:
		    print('Found Arduino Nano 33 BLE Sense Peripheral')
    
	print("Address = ", d.address)

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
