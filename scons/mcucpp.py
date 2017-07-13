import os
from SCons.Script import *
import devices
import extensions

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

def get_env_id(env, target):
	allOptions = env['CFLAGS'] + env['CCFLAGS'] + env['ASFLAGS'] + env['CPPDEFINES'] + env['LINKFLAGS'] + [str(t.abspath) for t in target]
	if 'DEVICE' in env:
		allOptions = allOptions + [env['DEVICE']['cpu']]
	return abs(hash(str(allOptions)))

def get_lib_sources(target, source, env):
	lib_src_to_add = set()
	for src in source:
		sources, includes = extensions.get_sources_and_includes(env, src)
		for inc in includes:
			dir_name = os.path.dirname(inc.abspath)
			inc_file_name = os.path.splitext(os.path.basename(inc.abspath))[0]
			src_dir = os.path.join(dir_name, "src")
			if os.path.exists(src_dir):
				for lib_src_file in os.listdir(src_dir):
					lib_src_base_name = os.path.splitext(lib_src_file)[0]
					if lib_src_base_name.lower() == inc_file_name.lower():
						lib_src_to_add.add(os.path.join(src_dir, lib_src_file))
				
	return lib_src_to_add
	
def mcucpp_program_emitter(target, source, env):
	""" Emitts library and startup sources to link with user's program """
	
	lib_src_to_add = get_lib_sources(target, source, env)
	id = get_env_id(env, target)
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		
		if 'startup' in device and device['startup'] is not None:
			for startupSrc in device['startup']:
				lib_src_to_add.add(os.path.normcase(startupSrc % env))
		
		if 'libSources' in device and device['libSources'] is not None:
			for libSrc in device['libSources']:
				lib_src_to_add.add(os.path.normcase(libSrc % env))
				
	for libSrc in lib_src_to_add:
		objName = '%s_%s' % (os.path.splitext(os.path.basename(libSrc))[0], id)
		source.append(env.Object(objName, libSrc))

	return target, source

def generate(env, **kw):
	env['ENV'] = os.environ
	env['SUPPORTED_DEVICES'] = devices.SupportedDevices
	env['MCUCPP_HOME'] = os.path.abspath(os.path.join( os.path.dirname( os.path.realpath( __file__ ) ), '../'))
	#print "MCUCPP_HOME = %s" % env['MCUCPP_HOME']
	
	serialOut = Builder(
		action = SerialOutput, 
		suffix = ".log")
	
	env.Append(BUILDERS = {'SerialOutput': serialOut})
	
	env['PROGEMITTER'] = mcucpp_program_emitter
	
	if 'DEVICE_NAME' in env:
		deviceName = env['DEVICE_NAME']
		if deviceName in devices.SupportedDevices:
			device = devices.SupportedDevices[deviceName]
			env['DEVICE'] = device
		else :
			print 'Unknown device name: %s' % deviceName
			print "List of supported devices:"
			for devId in sorted(devices.SupportedDevices):
				print devId
			raise Exception('Unknown device name: %s' % deviceName)
	else:
		if 'DEVICE' in env:
			device = env['DEVICE']
		else:
			env.Tool('hosted')
			return 
	
	if 'arch' not in device:
		raise Exception('No "arch" definition in given device')
	if 'tools' not in device['arch']:
		raise Exception('No "tools" definition in given arch')
	
	for tool in device['arch']['tools']:
		env.Tool(tool)
	env.Tool('code-blocks')


def exists(env):
	return True