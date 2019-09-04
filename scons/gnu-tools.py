import os
from SCons.Script import *


def setup_gnu_tools(env, prefix):

	for tool in ['gcc', 'g++', 'gnulink', 'ar', 'gas']:
		env.Tool(tool)
		
	env['CC'] = prefix+'gcc'
	env['CXX'] = prefix+'g++'
	env['AR'] = prefix+'ar'
	env['AS'] = prefix+'as'
	env['LINK'] = prefix+'gcc'
	env['OBJCOPY'] = prefix+'objcopy'
	env['OBJDUMP'] = prefix+'objdump'
	env['NM'] = prefix+"nm"
	env['RANLIB'] = prefix+"ranlib"
	env['SIZE'] = prefix+"size"
	env['PROGSUFFIX'] = '.elf'
	env['GDB'] = prefix + 'gdb'
	
	env['CFLAGS'] = ["-std=gnu11", "-Wredundant-decls","-Wnested-externs"]
	
	env['CCFLAGS'] = [
		"-g",
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
		"-nostdlib"
	]
		
	env['CXXFLAGS'] = [
		"-fno-exceptions",
		"-fno-threadsafe-statics",
		"-fno-rtti",
		"-fuse-cxa-atexit",
		"-Woverloaded-virtual",
		"-std=c++17"
		#"-std=c++03"
	]
	
	env['ASFLAGS'] = [
		"-gdwarf-2",
		"-xassembler-with-cpp",
	]
	
	linkerscript = ""
	startFiles = ""
	env.Append(CPPDEFINES = {})
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		
		if 'defines' in device:
			env.Append(CPPDEFINES = device['defines'])
		
		if 'clock' in device:
			env.Append(CPPDEFINES = {'F_CPU' : device['clock'] })
		if 'linkerScript' in device and device['linkerScript'] is not None:
			formattedLinkerScript = device['linkerScript'] % env
			linkerscript = env.File(formattedLinkerScript).srcnode().abspath
			linkerscript = '"-T%s"' % linkerscript
		
		if 'includes' in device and device['includes'] is not None:
			formattedIncludes = [os.path.abspath( include % env) for include in device['includes']]
			env.Append(CPPPATH = formattedIncludes)
		
		if 'startup' in device and device['startup'] is not None:
			startFiles = '-nostartfiles'
	
	env['LINKFLAGS'] = [
		"-Wl,--gc-sections",
		"-Wl,-Map=${TARGET.base}.map",
		linkerscript,
		startFiles
	]
	
	binBuilder = Builder(
		action = '$OBJCOPY -O binary --only-section $SECTION_NAME $SOURCE $TARGET', 
		src_suffix = ".elf",
		suffix = ".bin")
	
	disasmBuilder = Builder(
		action = '$OBJDUMP -h -S $SOURCE > $TARGET', 
		src_suffix = ".elf",
		suffix = ".lss")
	
	env.Append(BUILDERS = {
		'Disassembly': disasmBuilder,
		'DumpSection': binBuilder})
	
	prettyPrefix = prefix
	if prettyPrefix.endswith('-'):
		prettyPrefix = prettyPrefix[:-1]
	
	env['CCCOMSTR'] = "Compiling C (%s): $TARGET" % prettyPrefix
	env['CXXCOMSTR'] = "Compiling C++ (%s): $TARGET" % prettyPrefix
	env['ASCOMSTR'] = "Assembling (%s): $TARGET" % prettyPrefix
	env['ASPPCOMSTR'] = "Assembling (%s): $TARGET" % prettyPrefix
	env['LINKCOMSTR'] = "Linking (%s): $TARGET" % prettyPrefix

def generate(env, **kw):
	prefix = env['TOOL_PREFIX']
	setup_gnu_tools(env, prefix)

def exists(env):
	prefix = env['TOOL_PREFIX']
	return env.Detect('%s-gcc' % prefix)