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
		action = '$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .crc_section $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	env.Append(BUILDERS = {'Hex': hexBuilder})
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		env.Append(ASFLAGS = ["-mcpu=" + device['cpu'] ])
		env.Append(LINKFLAGS = ["-mcpu=" + device['cpu'] ])
		env.Append(CCFLAGS = ["-mcpu=" + device['cpu'] ])
	
	if device['cpu'] == 'cortex-m4':
		env.Append(CCFLAGS = ['-mthumb', '-mfloat-abi=softfp' ])
	if device['cpu'] == 'cortex-m3':
		env.Append(CCFLAGS = ['-mthumb', '-mfloat-abi=soft' ])

def exists(env):
	return env.Detect('arm-eabi-gcc')