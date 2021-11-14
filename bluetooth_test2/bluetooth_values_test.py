import logging
import asyncio
import platform
import ast

from bleak import BleakClient
from bleak import BleakScanner
from bleak import discover

CAR1_ADDR = "B0:73:A1:FA:50:C4"		#Mouse's ID
MOUSE_DATA_UUID = "1974d751-f3e1-49ef-9ac7-874dcced8416"
MOUSE_INST_UUID = "acd961a8-451e-11ec-81d3-0242ac130003"

async def sendMessage(client):
	global dataToSend

	userInput = input("What to send?")
	print("Input = ", userInput)
	
	if('u' in userInput):
		await client.write_gatt_char(MOUSE_INST_UUID, getValue(dataToSend))


async def run():
	global dataToSend
	async with BleakClient(CAR1_ADDR) as client:
		dataReceived = await client.read_gatt_char(MOUSE_DATA_UUID)
		print("value: {}".format(value))

	while 1:
		await sendMessage(client)


loop = asyncio.get_event_loop()
loop.run_until_complete(run())

