import os
from SCons.Script import *
	
def generate(env, **kw):
	if env.Detect('avr-gcc'): 
		env['TOOL_PREFIX'] = 'avr-'
	else:
		raise Exception('Unable to find build tools "avr-gcc" ')
	
	env.Tool('gnu-tools')
	
	hexBuilder = Builder(
		action = '$OBJCOPY -O ihex -R .eeprom -R .fuse -R .lock -R .signature $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	env.Append(BUILDERS = {'Hex': hexBuilder})
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		env.Append(ASFLAGS = ["-mmcu=" + device['cpu'] ])
		env.Append(LINKFLAGS = ["-mmcu=" + device['cpu'] ])
		env.Append(CCFLAGS = ["-mmcu=" + device['cpu'] ])
	
	env['SIMULAVR_OUT_PORT'] = '0x24'
	env['SIMULAVR_IN_PORT'] = '0x25'
	env['SIMULAVR_ABORT_PORT'] = '0x22'
	env['SIMULAVR_ERROR_PORT'] = '0x20'
	env['SIMULAVR_OUT_FILE'] = '-'
	env['SIMULAVR_IN_FILE'] = '-'
	simulavrBuilder = Builder(
		action = 'simulavr.exe -V --device $DEVICE_NAME --file $SOURCE --cpufrequency $F_CPU --trace $TARGET -W $SIMULAVR_OUT_PORT,$SIMULAVR_OUT_FILE -R $SIMULAVR_IN_PORT,$SIMULAVR_IN_FILE -a SIMULAVR_ABORT_PORT -e SIMULAVR_ERROR_PORT', 
		src_suffix = ".elf",
		suffix = ".log")
	env.Append(BUILDERS = {'Simulation': simulavrBuilder})
	

def exists(env):
	return env.Detect('avr-gcc')
	