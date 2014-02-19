import os
from SCons.Script import *
	
def generate(env, **kw):
	if env.Detect('msp-gcc'): 
		env['TOOL_PREFIX'] = 'msp-'
	else:
		raise Exception('Unable to find build tools "msp-gcc" ')
	
	env.Tool('gnu-tools')
	
	#hexBuilder = Builder(
	#	action = '$OBJCOPY -O ihex -R .eeprom -R .fuse -R .lock -R .signature $SOURCE $TARGET', 
	#	src_suffix = ".elf",
	#	suffix = ".hex")
	
	#env.Append(BUILDERS = {'Hex': hexBuilder})
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		env.Append(ASFLAGS = ["-mmcu=" + device['cpu'] ])
		env.Append(LINKFLAGS = ["-mmcu=" + device['cpu'] ])
		env.Append(CCFLAGS = ["-mmcu=" + device['cpu'] ])
	

def exists(env):
	return env.Detect('msp-gcc')
	