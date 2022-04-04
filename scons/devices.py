
commInc = ['%(MCUCPP_HOME)s/mcucpp']
avrInc = commInc + ['%(MCUCPP_HOME)s/mcucpp/AVR', '%(MCUCPP_HOME)s/3rdparty/avr-libstdcpp/include']

atmega = {
    'name': 'AtMega',
    'tools': ['avr-gcc'],
    'REG_WIDTH': 8,
    'HAS_HW_MUL': 8,
    'HAS_HW_DIV': 0,
    'HAS_FPU': 0
}

attiny = {
    'name': 'AtTiny',
    'tools': ['avr-gcc'],
    'REG_WIDTH': 8,
    'HAS_HW_MUL': 0,
    'HAS_HW_DIV': 0,
    'HAS_FPU': 0
}

msp430 = {
    'name': 'Msp430',
    'tools': ['msp-gcc'],
    'REG_WIDTH': 16,
    'HAS_HW_MUL': 16,
    'HAS_HW_DIV': 0,
    'HAS_FPU': 0
}

stm32 = {
    'name': 'Arm-CortexM3',
    'tools': ['stm32'],
    'REG_WIDTH': 32,
    'HAS_HW_MUL': 32,
    'HAS_HW_DIV': 32,
    'HAS_FPU': 0
}

stm32m4 = {
    'name': 'Arm-CortexM4',
    'tools': ['stm32'],
    'REG_WIDTH': 32,
    'HAS_HW_MUL': 32,
    'HAS_HW_DIV': 32,
    'HAS_FPU': 32
}


extensa = {
    'name' : 'extensa',
    'tools' : ['esp8266'],
    'REG_WIDTH'  : 32,
    'HAS_HW_MUL' : 32,
    'HAS_HW_DIV' : 32,
    'HAS_FPU'    : 0
}


SupportedDevices = {
    'atmega8':
        {
            'name' : 'atmega8',
            'manufacturer': 'Atmel',
            'arch': atmega,
            'flash': 8*1024, 'ram': 1024, 'eeprom': 512,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega8'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 8000000,
            'defines': [],
            'cpu': 'atmega8'
        },
    'atmega16':
        {
            'name' : 'atmega16',
            'manufacturer': 'Atmel',
            'arch': atmega,
            'flash': 16*1024, 'ram': 1024, 'eeprom': 1024,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega16'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 16000000,
            'defines': [],
            'cpu': 'atmega16'
        },
    'atmega128':
        {
            'name' : 'atmega128',
            'manufacturer': 'Atmel',
            'arch': atmega,
            'flash': 128*1024, 'ram': 4*1024, 'eeprom': 1024,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega128'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 16000000,
            'defines': [],
            'cpu': 'atmega128'
        },
    'atmega168':
        {
            'name' : 'atmega168',
            'manufacturer': 'Atmel',
            'arch': atmega,
            'flash': 16*1024, 'ram': 1*1024, 'eeprom': 512,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega168'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 16000000,
            'defines': [],
            'cpu': 'atmega168'
        },
    'atmega1284p':
        {
            'name' : 'atmega1284p',
            'manufacturer': 'Atmel',
            'arch': atmega,
            'flash': 128*1024, 'ram': 16*1024, 'eeprom': 4*1024,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/atmega1284p'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 20000000,
            'defines': [],
            'cpu': 'atmega1284p'
        },
    'attiny2313':
        {
            'name' : 'attiny2313',
            'manufacturer': 'Atmel',
            'arch': attiny,
            'flash': 2*1024, 'ram': 128, 'eeprom': 128,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/attiny2313'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/AVR/src/usart.cpp'],
            'linkerScript': None,
            'clock': 8000000,
            'defines': [],
            'cpu': 'attiny2313'
        },
        'attiny26':
        {
            'name' : 'attiny26',
            'manufacturer': 'Atmel',
            'arch': attiny,
            'flash': 2*1024, 'ram': 128, 'eeprom': 128,
            'includes': avrInc + ['%(MCUCPP_HOME)s/mcucpp/AVR/attiny26'],
            'linkerScript': None,
            'clock': 8000000,
            'defines': [],
            'cpu': 'attiny26'
        },

        # 'msp430g2231':
        # {
    # 'manufacturer' : 'TI',
    # 'arch' : msp430,
    # 'flash' : 2*1024, 'ram' : 128, 'eeprom' : 0,
    # 'includes' : commInc + ['%(MCUCPP_HOME)s/mcucpp/MSP430'],
    # 'libSources' : [],
    # 'linkerScript' : None,
    # 'clock' : 16000000,
    # 'defines' : ['__MSP430_2011__'],
    # 'cpu' : 'msp430x2231'
        # },

        'stm32f100':
        {
            'name' : 'stm32f100',
            'manufacturer': 'ST',
            'arch': stm32,
            'flash': 128*1024, 'ram': 8*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32f100', '%(MCUCPP_HOME)s/mcucpp/ARM/CortexM3', '%(MCUCPP_HOME)s/startup', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32F1xx/Include'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_100xB.ld',
            'clock': 24000000,
            'defines': ['STM32F100xB', 'STM32F100XB', 'STM32F10X_MD_VL', 'F_OSC=8000000u'],
            'cpu': 'cortex-m3',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32_100xB.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },

        'stm32f103':
        {
            'name' : 'stm32f103',
            'manufacturer': 'ST',
            'arch': stm32,
            'flash': 64*1024, 'ram': 20*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32f100', 
					'%(MCUCPP_HOME)s/mcucpp/ARM/CortexM3', 
					'%(MCUCPP_HOME)s/startup', 
					'%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32F1xx/Include', 
					'%(MCUCPP_HOME)s/3rdparty/STM32_USB-FS-Device_Driver/inc'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_103xB.ld',
            'clock': 72000000,
            'defines': ['F_OSC=8000000u', 'STM32F103xB', 'STM32F103XB', 'STM32F10X_MD'],
            'cpu': 'cortex-m3',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32_103.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },

        'stm32f407':
        {
            'name' : 'stm32f407',
            'manufacturer': 'ST',
            'arch': stm32m4,
            'flash': 1024*1024, 'ram': 112*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32F40x', '%(MCUCPP_HOME)s/mcucpp/ARM/CortexM4',  '%(MCUCPP_HOME)s/startup', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Include', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32F4xx/Include'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_40x.ld',
            'clock': 168000000,
            'defines': ['STM32F40_41xxx', 'F_OSC=8000000u', '__FPU_PRESENT=1'],
            'cpu': 'cortex-m4',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32_40x.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },

        'stm32f429':
        {
            'name' : 'stm32f429',
            'manufacturer': 'ST',
            'arch': stm32m4,
            'flash': 2*1024*1024, 'ram': 256*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32F40x', '%(MCUCPP_HOME)s/mcucpp/ARM/CortexM4', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Include', '%(MCUCPP_HOME)s/startup', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32F4xx/Include'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_40x.ld',
            'clock': 180000000,
            'defines': ['STM32F429_439xx', 'F_OSC=8000000u', '__FPU_PRESENT=1'],
            'cpu': 'cortex-m4',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32_40x.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },
        'stm32l471':
        {
            'name' : 'stm32l471',
            'manufacturer': 'ST',
            'arch': stm32m4,
            'flash': 1*1024*1024, 'ram': 128*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32L4x1', '%(MCUCPP_HOME)s/mcucpp/ARM/CortexM4', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Include', '%(MCUCPP_HOME)s/startup', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32L4xx/Include'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_471.ld',
            'clock': 80000000,
            'defines': ['STM32L471xx', 'F_OSC=8000000u', '__FPU_PRESENT=1'],
            'cpu': 'cortex-m4',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32l471.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },
        'stm32f411':
        {
            'name' : 'stm32f411',
            'manufacturer': 'ST',
            'arch': stm32m4,
            'flash': 512*1024, 'ram': 128*1024, 'eeprom': 0,
            'includes': commInc + ['%(MCUCPP_HOME)s/mcucpp/ARM/Stm32F40x', '%(MCUCPP_HOME)s/mcucpp/ARM/CortexM4',  '%(MCUCPP_HOME)s/startup', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Include', '%(MCUCPP_HOME)s/3rdparty/CMSIS/Device/ST/STM32F4xx/Include'],
            'linkerScript': '%(MCUCPP_HOME)s/linker_scripts/stm32_411.ld',
            'clock': 100000000,
            'defines': ['STM32F40_41xxx', 'F_OSC=8000000u', '__FPU_PRESENT=1'],
            'cpu': 'cortex-m4',
            'startup': ['%(MCUCPP_HOME)s/startups/startup_stm32_40x.c'],
            'libSources': ['%(MCUCPP_HOME)s/mcucpp/src/memory_management.cpp']
        },
        'esp8266' : 
        {
            'manufacturer' : 'Espressif',
            'arch' : extensa, 
            'flash' : 1024*1024, 'ram' : 80*1024,
            'includes' : ['%(IDF_PATH)s/components/esp8266/include', 
                    '%(IDF_PATH)s/components/lwip/include'],
            'libSources' : [],
            'linkerScript' : '%(IDF_PATH)s/components/esp8266/ld/esp8266.common.ld',
            'clock' : 80000000,
            'defines' : [ \
                #'LWIP_OPEN_SRC', 
                'PBUF_RSV_FOR_WLAN', 
                'EBUF_LWIP', 
                #'ICACHE_FLASH', 
                '__ets__'],
            'cpu' : 'extensa',
            'packages' : [{
                'name': 'ESP8266_RTOS_SDK',
                'searchUrl' : 'https://github.com/espressif/ESP8266_RTOS_SDK/releases/latest',
                'searchPattern' : r'.*espressif/ESP8266_RTOS_SDK/archive/.*\.zip',
                'verifyPath' : 'ESP8266_RTOS_SDK-3.1.2/components/esp8266/include/esp_wifi.h',
                'type' : 'lib'
                },
                {
                'name': 'ESP8266_TOOLCHAIN',
                'dpwnloadUrl' : 'https://dl.espressif.com/dl/xtensa-lx106-elf-win32-1.22.0-100-ge567ec7-5.2.0.zip',
                'verifyPath' : 'xtensa-lx106-elf-win32-1.22.0-100-ge567ec7-5.2.0/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc.exe',
                'type' : 'tool'
                }]
        }
}
