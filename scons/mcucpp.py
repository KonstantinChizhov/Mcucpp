import os
from SCons.Script import *
import devices

def generate(env, **kw):
	env['ENV'] = os.environ
	env['SUPPORTED_DEVICES'] = devices.SupportedDevices
	
	if 'DEVICE_NAME' in env:
		deviceName = env['DEVICE_NAME']
		if deviceName in devices.SupportedDevices:
			device = devices.SupportedDevices[deviceName]
			env['DEVICE'] = device
		else :
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
		


def exists(env):
	return True