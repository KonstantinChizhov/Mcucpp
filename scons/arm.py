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
	env.Append(CPPDEFINES = {})
	if 'defines' in device:
		env.Append(CPPDEFINES = device['defines'])
	if 'clock' in device:
		env.Append(CPPDEFINES = {'F_CPU' : device['clock'] })
	
	env['CFLAGS'] = ["-std=gnu99", "-Wredundant-decls","-Wnested-externs"]
	
	env['CCFLAGS'] = [
		"-mcpu=" + device['cpu'],
		"-gdwarf-2",
		"-funsigned-char",
		"-funsigned-bitfields",
		"-fshort-enums",
		#"-fno-split-wide-types",
		"-ffunction-sections",
		"-fdata-sections",
		"-Wall",
		"-Wformat",
		"-Wextra",
		"-Wpointer-arith",
		"-Wunused",
		#"-pedantic"
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
		"-std=c++03"
	]
	
	env['ASFLAGS'] = [
		"-mcpu=" + device['cpu'],
		"-gdwarf-2",
		"-xassembler-with-cpp",
	]
	
	linkerscript = ""
	if 'linkerscript' in device:
		linkerscript = env.File(device['linkerscript']).srcnode().abspath
		linkerscript = '"-T%s"' % linkerscript
	
	env['LINKFLAGS'] = [
		"-mcpu=" + device['cpu'],
		"-Wl,--gc-sections",
		"-nostartfiles",
		"-Wl,-Map=${TARGET.base}.map",
		linkerscript
	]
	
	if 'archOptions' in device:
		for option in device['archOptions']:
			env.Append(LINKFLAGS = "-%s" % option)
	
	hexBuilder = Builder(
		action = '$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .crc_section $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".hex")
	
	binBuilder = Builder(
		action = '$OBJCOPY -O binary --only-section $SECTION_NAME $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".bin")
	
	disasmBuilder = Builder(
		action = '$OBJDUMP -h -S $SOURCE > $TARGET', 
		src_suffix = ".elf",
		suffix = ".lss")
	
	env.Append(BUILDERS = {
		'Hex': hexBuilder,
		'Disassembly': disasmBuilder,
		'DumpSection': binBuilder})
	
	env.AddMethod(print_size, 'Size')
	
	env['CCCOMSTR'] = "ARM Compiling C: $TARGET"
	env['CXXCOMSTR'] = "ARM Compiling C++: $TARGET"
	env['ASCOMSTR'] = "ARM Assembling: $TARGET"
	env['ASPPCOMSTR'] = "ARM Assembling: $TARGET"
	env['LINKCOMSTR'] = "ARM Linking: $TARGET"

def generate(env, **kw):
	setup_gnu_tools(env, 'arm-none-eabi-')

def exists(env):
	return env.Detect('arm-none-eabi-gcc')