from SCons.Script import *

def SerialOutput(env, target, source):
	import serial
	import serial.tools.list_ports
	import time
	import datetime

	portList = list(serial.tools.list_ports.comports())
	firstPort = portList[0]
	
	portName = firstPort[0]
	if "PORT_NAME" in env:
		portName = env["PORT_NAME"]
	portName = ARGUMENTS.get('PORT_NAME', portName)
	
	baudRate = 115200
	if "BAUD_RATE" in env:
		baudRate = env["BAUD_RATE"]
	baudRate = ARGUMENTS.get('BAUD_RATE', baudRate)

	print "Listening serial port %s:  " % portName, 
	port = serial.Serial(portName, baudRate, bytesize=8, parity='N', stopbits=1, timeout=0.2)
	timeStart = time.time()
	timeout = 10 # seconds
	if 'PORT_TIMEOUT' in env:
		timeout = env['PORT_TIMEOUT']
	
	triggerMessage = ""
	
	if "SERIAL_TRIGGER_MESSAGE" in env:
		triggerMessage = env["SERIAL_TRIGGER_MESSAGE"]
	
	gotResponce = False
	wait = ['-', '\\', '|', '/']
	waitState = 0
	logFile = open(target[0].abspath, 'w')
	logFile.write('Log from %s %s\n' % (portName, datetime.datetime.now() ) )

	if len(triggerMessage) > 0:
		port.write(triggerMessage)
	
	while timeStart + timeout > time.time():
		s = port.read(1)
		if len(s) > 0:
			timeStart = time.time()
			if gotResponce == False:
				print "\bOK"
			gotResponce = True
			if 3 in list(ord(b) for b in s):
				print "----------- End of transfer ---------------"
				logFile.write("----------- End of transfer ---------------")
				break
			sys.stdout.write(s)
			logFile.write(s)
		if gotResponce == False:
			sys.stdout.write('\b')
			sys.stdout.write(wait[waitState])
			waitState = waitState + 1
			if waitState >= len(wait):
				waitState = 0
	if gotResponce == False:
		print "\bTimeout"
		logFile.write("Timeout")
		return "No responce on %s" % portName
	return None
	
def generate(env, **kw):
	serialOut = Builder(
		action = SerialOutput, 
		suffix = ".log")
	env.Append(BUILDERS = {'SerialOutput': serialOut})
	
def exists(env):
	return True