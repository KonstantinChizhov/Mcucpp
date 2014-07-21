
commInc = ['%(MCUCPP_HOME)s/mcucpp']
avrInc = commInc + ['%(MCUCPP_HOME)s/mcucpp/AVR']

atmega = {
	'name' : 'AtMega',
	'tools' : ['avr-gcc'],
	'REG_WIDTH'  : 8,
	'HAS_HW_MUL' : 8,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

attiny = {
	'name' : 'AtTiny',
	'tools' : ['avr-gcc'],
	'REG_WIDTH'  : 8,
	'HAS_HW_MUL' : 0,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

msp430 = {
	'name' : 'Msp430',
	'tools' : ['msp-gcc'],
	'REG_WIDTH'  : 16,
	'HAS_HW_MUL' : 16,
	'HAS_HW_DIV' : 0,
	'HAS_FPU'    : 0
}

cotrexm3 = {
	'name' : 'Arm-CortexM3',
	'tools' : ['arm-eabi-gcc'],
	'REG_WIDTH'  : 32,
	'HAS_HW_MUL' : 32,
	'HAS_HW_DIV' : 32,
	'HAS_FPU'    : 0
}

cotrexm4 = {
	'name' : 'Arm-CortexM4',
	'tools' : ['arm-eabi-gcc'],
	'REG_WIDTH'  : 32,
	'HAS_HW_MUL' : 32,
	'HAS_HW_DIV' : 32,
	'HAS_FPU'    : 32
}


SupportedDevices = {
	'atmega8': 
	{
		'manufacturer' : 'Atmel',
		'arch' : atmega, 
		'flash' : 8*1024, 'ram' : 1024, 'eeprom' : 512,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega8'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega8'
	},
	'atmega16': 
	{
		'manufacturer' : 'Atmel',
		'arch' : atmega, 
		'flash' : 16*1024, 'ram' : 1024, 'eeprom' : 1024,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega16'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega16'
	},
	'atmega128': 
	{
		'manufacturer' : 'Atmel',
		'arch' : atmega, 
		'flash' : 128*1024, 'ram' : 4*1024, 'eeprom' : 1024,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega128'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega128'
	},
	'atmega168': 
	{
		'manufacturer' : 'Atmel',
		'arch' : atmega, 
		'flash' : 16*1024, 'ram' : 1*1024, 'eeprom' : 512,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega168'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : [],
		'cpu' : 'atmega168'
	},
	'attiny2313':
	{
		'manufacturer' : 'Atmel',
		'arch' : attiny, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 128,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/attiny2313'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 8000000,
		'defines' : [],
		'cpu' : 'attiny2313'
	},
	'attiny26': 
	{
		'manufacturer' : 'Atmel',
		'arch' : attiny, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 128,
		'includes' : avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/attiny26'],
		'linkerScript' : None,
		'clock' : 8000000,
		'defines' : [],
		'cpu' : 'attiny26'
	},
	
	'msp430g2231': 
	{
		'manufacturer' : 'TI',
		'arch' : msp430, 
		'flash' : 2*1024, 'ram' : 128, 'eeprom' : 0,
		'includes' : commInc + ['%(MCUCPP_HOME)s/mcucpp/MSP430'],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
		'linkerScript' : None,
		'clock' : 16000000,
		'defines' : ['__MSP430_2011__'],
		'cpu' : 'msp430x2231'
	},
	
	'stm32f100': 
	{
		'manufacturer' : 'ST',
		'arch' : cotrexm3,
		'flash' : 128*1024, 'ram' : 8*1024, 'eeprom' : 0,
		'includes' : commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/CortexM3', '%(MCUCPP_HOME)s/mcucpp/ARM/Stm32f100' ],
		'linkerScript' : '%(MCUCPP_HOME)s/linker_scripts/stm32_100xB.ld',
		'clock' : 24000000,
		'defines' : ['STM32F10X_MD_VL', 'F_OSC=8000000u'],
		'cpu' : 'cortex-m3',
		'startup' : ['%(MCUCPP_HOME)s/startups/startup_stm32_100xB.c']
	},
	
	'stm32f407': 
	{
		'manufacturer' : 'ST',
		'arch' : cotrexm4,
		'flash' : 1024*1024, 'ram' : 112*1024, 'eeprom' : 0,
		'includes' : commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/CortexM4', '%(MCUCPP_HOME)s/mcucpp/ARM/Stm32F40x' ],
		'libSources' : ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32F40x/src/dma.cpp'],
		'linkerScript' : '%(MCUCPP_HOME)s/linker_scripts/stm32_40x.ld',
		'clock' : 168000000,
		'defines' : ['STM32F40_41xxx', 'F_OSC=8000000u'],
		'cpu' : 'cortex-m4',
		'startup' : ['%(MCUCPP_HOME)s/startups/startup_stm32_40x.c']
	}
}
