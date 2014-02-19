import os
from SCons.Script import *

commInc = ['#/mcucpp']
avrInc = commInc + ['#/mcucpp/AVR']

atmega = {
	'tools' : ['avr-gcc'],
	'REG_WIDTH'  : 8,
	'HAS_HW_MUL' : 8,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

attiny = {
	'tools' : ['avr-gcc'],
	'REG_WIDTH'  : 8,
	'HAS_HW_MUL' : 0,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

msp430 = {
	'tools' : ['msp-gcc'],
	'REG_WIDTH'  : 16,
	'HAS_HW_MUL' : 16,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

cotrexm3 = {
	'tools' : ['arm-eabi-gcc'],
	'REG_WIDTH'  : 32,
	'HAS_HW_MUL' : 32,
	'HAS_HW_DIV' : 32,
	'HAS_FPU'    : 0
}

cotrexm4 = {
	'tools' : ['arm-eabi-gcc'],
	'REG_WIDTH'  : 32,
	'HAS_HW_MUL' : 32,
	'HAS_HW_DIV' : 32,
	'HAS_FPU'    : 32
}


devices = {
	'atmega8': 
	{
		'arch' : atmega, 
		'flash' : 8*1024, 'ram' : 1024, 'eeprom' : 512,
		'includes' : avrInc + ['#/mcucpp/AVR/atmega8'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega8'
	},
	'atmega16': 
	{
		'arch' : atmega, 
		'flash' : 16*1024, 'ram' : 1024, 'eeprom' : 1024,
		'includes' : avrInc + ['#/mcucpp/AVR/atmega16'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega16'
	},
	'atmega128': 
	{
		'arch' : atmega, 
		'flash' : 128*1024, 'ram' : 4*1024, 'eeprom' : 1024,
		'includes' : avrInc + ['#/mcucpp/AVR/atmega128'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega128'
	},
	'atmega168': 
	{
		'arch' : atmega, 
		'flash' : 16*1024, 'ram' : 1*1024, 'eeprom' : 512,
		'includes' : avrInc + ['#/mcucpp/AVR/atmega168'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega168'
	},
	'attiny2313':
	{
		'arch' : attiny, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 128,
		'includes' : avrInc + ['#/mcucpp/AVR/attiny2313'],
		'linkerScript' : None,
		'clock' : 8000000,
		'defines' : [],
		'cpu' : 'attiny2313'
	},
	'attiny26': 
	{
		'arch' : attiny, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 128,
		'includes' : avrInc + ['#/mcucpp/AVR/atmega16'],
		'linkerScript' : None,
		'clock' : 8000000,
		'defines' : [],
		'cpu' : 'attiny26'
	},
	
	'MSP430G2231': 
	{
		'arch' : msp430, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 0,
		'includes' : commInc + ['#/mcucpp/MSP430'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : ['__MSP430_2011__'],
		'cpu' : 'msp430g2231'
	},
	
	'stm32f100': 
	{
		'arch' : cotrexm3,
		'flash' : 128*1024, 'ram' : 8*1024, 'eeprom' : 0,
		'includes' : commInc + ['#/mcucpp/ARM/CortexM3', '#/mcucpp/ARM/Stm32f100' ],
		'linkerScript' : '#/linker_scripts/stm32_100xB.ld',
		'clock' : 24000000,
		'defines' : ['STM32F10X_MD_VL', 'F_OSC=8000000u'],
		'cpu' : 'cortex-m3',
		'startup' : ['#/startups/startup_stm32_100xB.c']
	},
	
	'stm32f407': 
	{
		'arch' : cotrexm4,
		'flash' : 1024*1024, 'ram' : 112*1024, 'eeprom' : 0,
		'includes' : commInc + ['#/mcucpp/ARM/CortexM4', '#/mcucpp/ARM/Stm32F40x' ],
		'linkerScript' : '#/linker_scripts/stm32_40x.ld',
		'clock' : 168000000,
		'defines' : ['STM32F40_41xxx', 'F_OSC=8000000u'],
		'cpu' : 'cortex-m4',
		'startup' : ['#/startups/startup_stm32_40x.c']
	}
}

def generate(env, **kw):
	env['ENV'] = os.environ
	if 'DEVICE_NAME' in env:
		deviceName = env['DEVICE_NAME']
		if deviceName in devices:
			device = devices[deviceName]
			env['DEVICE'] = device
		else :
			raise Exception('Unknown device name: %s' % deviceName)
	else:
		if 'DEVICE' in env:
			device = env['DEVICE']
		else:
			raise Exception('No device name given in DEVICE_NAME nor device definition DEVICE variables')
	if 'arch' not in device:
		raise Exception('No "arch" definition in given device')
	if 'tools' not in device['arch']:
		raise Exception('No "tools" definition in given arch')
		
	for tool in device['arch']['tools']:
		env.Tool(tool)
	

def exists(env):
	return True