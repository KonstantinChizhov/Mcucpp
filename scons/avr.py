import os
from SCons.Script import *

def print_size(env, source, alias='size'):
	action = Action("$SIZE %s" % source[0].path, cmdstr="Used section sizes:")
	return env.AlwaysBuild(env.Alias(alias, source, action))


def setup_gnu_tools(env, prefix):
	gnu_tools = ['gcc', 'g++', 'gnulink', 'ar', 'gas']
	for tool in gnu_tools:
		env.Tool(tool)
	env['CC'] = prefix+'gcc'
	env['CXX'] = prefix+'g++'
	env['AR'] = prefix+'ar'
	env['AS'] = prefix+'as'
	env['OBJCOPY'] = prefix+'objcopy'
	env['OBJDUMP'] = prefix+'objdump'
	env['NM'] = prefix+"nm"
	env['RANLIB'] = prefix+"ranlib"
	env['SIZE'] = prefix+"size"
	env['PROGSUFFIX'] = '.elf'
	
	device = env['DEVICE']
	env['DEVICE_NAME'] = device['name']
	
	env.Append(CPPDEFINES = {})
	if 'defines' in device:
		env.Append(CPPDEFINES = device['defines'])
	if 'clock' in device:
		env.Append(CPPDEFINES = {'F_CPU' : device['clock'] })
		env['F_CPU'] = device['clock']
	
	env['CFLAGS'] = ["-std=gnu99", "-Wredundant-decls","-Wnested-externs"]
	
	env['CCFLAGS'] = [
		"-mmcu=" + device['cpu'],
		"-gdwarf-2",
		"-funsigned-char",
		"-funsigned-bitfields",
		"-fshort-enums",
		"-ffunction-sections",
		"-fdata-sections",
		"-Wall",
		"-Wformat",
		"-Wextra",
		"-Wpointer-arith",
		"-Wunused",
		"-ffunction-sections",
		#"-pedantic",
		'-fpack-struct'
	]
	if 'archOptions' in device:
		for option in device['archOptions']:
			env.Append(CCFLAGS = "-%s" % option)
	
	env['CXXFLAGS'] = [
		"-fno-exceptions",
		"-nostdlib",
		"-fno-threadsafe-statics",
		"-fno-rtti",
		"-fuse-cxa-atexit",
		"-Woverloaded-virtual",
	]
	
	env['ASFLAGS'] = [
		"-mmcu=" + device['cpu'],
		"-gdwarf-2",
		"-xassembler-with-cpp",
	]
	
	
	linkerscript = ""
	if 'linkerscript' in device:
		linkerscript = os.path.join(env.Dir('.').srcnode().abspath, device['linkerscript'])
		linkerscript = '"-T%s"' % linkerscript
	
	env['LINKFLAGS'] = [
		"-mmcu=" + device['cpu'],
		"-Wl,--gc-sections",
		"-Wl,-Map=${TARGET.base}.map,--cref",
		linkerscript
	]
	
	hexBuilder = Builder(
		action = '$OBJCOPY -O ihex -R .eeprom -R .fuse -R .lock -R .signature $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	disasmBuilder = Builder(
		action = '$OBJDUMP -h -S $SOURCE > $TARGET', 
		src_suffix = ".elf",
		suffix = ".lss")
	
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
	
	env.Append(BUILDERS = {
		'Hex': hexBuilder,
		'Disassembly': disasmBuilder,
		'Simulation': simulavrBuilder})
	
	env.AddMethod(print_size, 'Size')
	
def generate(env, **kw):
	setup_gnu_tools(env, 'avr-')

def exists(env):
	return env.Detect('avr-gcc')