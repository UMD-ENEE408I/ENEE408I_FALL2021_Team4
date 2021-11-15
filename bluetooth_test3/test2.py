import logging
import asyncio
import platform
import ast
import enum

from bleak import BleakClient
from bleak import BleakScanner
from bleak import discover

# These values have been randomly generated - they must match between the Central and Peripheral devices
# Any changes you make here must be suitably made in the Arduino program as well

dataInCharacteristic = '13012F01-F8C3-4F4A-A8F4-15CD926DA146'
#UUID_2 = '13012F02-F8C3-4F4A-A8F4-15CD926DA146'

STATE_IDLE = 0
STATE_F = 1
STATE_L = 2
STATE_R = 3
STATE_U = 4
STATE_END = 5

RED = False


def getValue(val):
	if val == STATE_IDLE:
		return bytearray([0x00])
	elif val == STATE_F:
		return bytearray([0x01])
	elif val == STATE_L:
		return bytearray([0x02])
	elif val == STATE_R:
		return bytearray([0x03])
	elif val == STATE_U:
		return bytearray([0x04])
	elif val == STATE_END:
		return bytearray([0x05])
	else:
		return bytearray([0x00])

async def sendData(client):
	global RED
	userInput = int(input('Enter Instruction to Send :'))
	print(userInput)
    
	if(userInput == STATE_IDLE):
		RED = not RED
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_IDLE))
	elif(userInput == STATE_F):
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_F))
	elif(userInput == STATE_L):
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_L))
	elif(userInput == STATE_R):
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_R))
	elif(userInput == STATE_U):
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_U))
	elif(userInput == STATE_END):
		await client.write_gatt_char(dataInCharacteristic, getValue(STATE_END))
	
	
		
async def run():
	global RED

	print('Arduino Nano BLE LED Peripheral Central Service')
	print('Looking for Arduino Nano 33 BLE Sense Peripheral Device...')

	found = False
	devices = await discover()
	for d in devices:       
		if 'Tin_Mouse' in d.name:
			print('Found Arduino Nano 33 BLE Sense Peripheral')
			found = True
			async with BleakClient(d.address) as client:
				print(f'Connected to {d.address}')
				val = await client.read_gatt_char(dataInCharacteristic)
				print("Val = ", val)

				while True:
					await sendData(client)

	if not found:
		print('Could not find Arduino Nano 33 BLE Sense Peripheral')


loop = asyncio.get_event_loop()
try:
	loop.run_until_complete(run())
except KeyboardInterrupt:
	print('\nRceceived Keyboard Interrupt')
finally:
	print('Program finished')
