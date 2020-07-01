import os
from SCons.Script import *
import glob

def espToolTargets(target, source, env):
	src_base = os.path.basename(source[0].abspath)
	return ['%s-0x00000.bin' % (src_base), '%s-0x01010.bin' % (src_base)], source
	
def flashImage(target, source, env):
	command = '$ESPTOOL -p $ESPPORT -b $BAUD write_flash 0x00000 "%s" 0x01010 "%s"' % (source[0].abspath, source[1].abspath)
	return env.Execute(command)

def generate(env, **kw):

	env.PrependENVPath('PATH', os.path.join(env['ESP8266_TOOLCHAIN'], bin))
	if env.Detect('xtensa-lx106-elf-gcc'): 
		env['TOOL_PREFIX'] = 'xtensa-lx106-elf-'
	else:
		raise Exception('Unable to find build tools xtensa-lx106-elf')

	env.Tool('gnu-tools')
	if not 'IDF_PATH' in env:
		sdkPath = glob.glob(env['ESP8266_RTOS_SDK'] + '/ESP8266*SDK')[0]
		print ("SDK path = ", sdkPath)
		env['IDF_PATH'] = sdkPath
		
	if not 'ESPPORT' in env:
		env['ESPPORT'] = "COM1"
	
	if not 'BAUD' in env:
		env['BAUD'] = "230400"
	

	hexBuilder = Builder(
		action = '$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .crc_section $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	env.Append(BUILDERS = {'Hex': hexBuilder})
	
	env['CCFLAGS'] = [
		#"-Wall",
		"-Wextra",
		"-Wpointer-arith",
		"-Wformat",
		"-Wunused",
		"-g",
		"-O2",
		"-Wpointer-arith",
		"-Wundef",
		#"-Werror",
		"-Wl,-EL --longcalls --text-section-literals",
		"-fno-inline-functions",
		"-nostdlib",
		"-mlongcalls",
		"-mtext-section-literals",
		"-mno-serialize-volatile",
	]
	
	# env.Append(LINKFLAGS=[
		# '-nostdlib',
		# '-Wl,--no-check-sections',
		# '-u', 'call_user_start',
		# '-Wl,-static'
		# ])
	libpath = env.Dir(env['SDK'] + 'components/esp8266/lib').srcnode().abspath
	print libpath
	env.Append(LIBPATH=[libpath])
	
	sdkLibs = ['c', 'gcc', 'phy', 'pp', 'net80211', 'lwip',	'wpa', 'main', 'espnow', 'crypto']
	
	env.Append(LINKFLAGS=[
		'"-L'+libpath+'"',

		'-nostdlib',
		'-flto',
		'-Wl,--no-check-sections',
		'-Wl,--gc-sections',
		'-u call_user_start',
		'-Wl,-static',
		'-Wl,--start-group',
		'-lc',
		'-lgcc',
		'-lcore',
		'-lespnow',
		'-lhal',
		'-lnet80211',
		'-lphy',
		'-lpp',
		'-lsmartconfig',
		'-lssc',
		'-lwpa',
		'-lwps',
		
		'-llwip',
		
		'-lcrypto',
		'-ldriver',
		'-Wl,--end-group',
		])
	
	env['ESPTOOL'] = env.File('#/esptool/esptool.py').srcnode().abspath
	
	binImageBuilder = Builder(
		action = '$ESPTOOL elf2image $SOURCE', 
		src_suffix = ".elf",
		suffix = ".bin",
		emitter = espToolTargets )
	
	flasherBuilder = Builder(
		action = flashImage, 
		src_suffix = ".bin" )
	
	env.Append(BUILDERS = {
		'MakeFlashImage': binImageBuilder,
		'Flash': flasherBuilder})
		

def exists(env):
	return env.Detect('xtensa-lx106-elf-gcc')