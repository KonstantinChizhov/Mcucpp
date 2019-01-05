import os
from SCons.Script import *

def generate(env, **kw):
	if env.Detect('arm-eabi-gcc'): 
		env['TOOL_PREFIX'] = 'arm-eabi-'
	elif env.Detect('arm-none-eabi-gcc'):
		env['TOOL_PREFIX'] = 'arm-none-eabi-'
	else:
		raise Exception('Unable to find build tools neither "arm-none-eabi-gcc" nor "arm-eabi-gcc"')
	
	env.Tool('gnu-tools')
		
	hexBuilder = Builder(
		action = '$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .metadata_section $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	env.Append(BUILDERS = {'Hex': hexBuilder})
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		env.Append(ASFLAGS = ["-mcpu=" + device['cpu'] ])
		env.Append(CCFLAGS = ["-mcpu=" + device['cpu'] ])
		
		if device['cpu'] == 'cortex-m4':
			env.Append(CCFLAGS = ['-mthumb', '-mfloat-abi=softfp', '-mfpu=fpv4-sp-d16' ])
			env.Append(LINKFLAGS = ['-mcpu=cortex-m4', '-mthumb', '-mfloat-abi=softfp', '-mfpu=fpv4-sp-d16'])
		else:
			env.Append(LINKFLAGS = ["-mcpu=" + device['cpu'] ])
			
		if device['cpu'] == 'cortex-m3' or device['cpu'] == 'cortex-m0':
			env.Append(CCFLAGS = ['-mthumb', '-mfloat-abi=soft'])
			env.Append(LINKFLAGS = ['-mthumb', '-mfloat-abi=soft'])
		
	env.Append(CCFLAGS = ['-ffreestanding'])


def exists(env):
	return env.Detect('arm-eabi-gcc')