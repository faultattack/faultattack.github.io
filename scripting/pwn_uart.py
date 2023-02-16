from pwn import *
import time
import _thread
import re 

USB_DEVICE_TARGET = '/dev/tty.usbmodem101'
USB_DEVICE_EMP = '/dev/tty.usbmodem11101'

def target(tube):
	#sleep(0.001*0.001)
	print("target us" + str(time.time()*1000*1000))
	tube.send('\n')

def emp(tube):
	#sleep(0.001*0.001)
	print("picemp us" + str(time.time()*1000*1000))
	#
	tube.send('\n')

tio = serialtube(USB_DEVICE_TARGET, baudrate=9600)
eio = serialtube(USB_DEVICE_EMP, baudrate=9600)

tio.sendline('h')
print(tio.recv(2048))
eio.sendline('h')
print(eio.recv(2048))
print("hello world")
for x in range(10):

	eio.send('p')
	tio.send('p')
	_thread.start_new_thread(emp, (eio,))
	_thread.start_new_thread(target, (tio,))
	
	time.sleep(200/1000)
	print(tio.recv(2048))
	print(eio.recv(2048))

#print(timearray)
