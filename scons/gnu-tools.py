import os
from SCons.Script import *


def GetEnvId(env):
	allOptions = env['CFLAGS'] + env['CCFLAGS'] + env['ASFLAGS'] + env['CPPDEFINES'] + env['LINKFLAGS']
	return abs(hash(str(allOptions)))


def overrideProgramBuilder(env, target, source):
	startupObjects = []
	if 'DEVICE' in env:
		device = env['DEVICE']
		id = GetEnvId(env)
		if 'startup' in device and device['startup'] is not None:
			for startupSrc in device['startup']:
				objName = '%s_%s' % (os.path.splitext(os.path.basename(startupSrc))[0], id)
				startupObjects += env.Object(objName, startupSrc)
		
		if 'libSources' in device and device['libSources'] is not None:
			for libSrc in device['libSources']:
				objName = '%s_%s' % (os.path.splitext(os.path.basename(libSrc))[0], id)
				startupObjects += env.Object(objName, libSrc)
	
	originalProgramBuilder = env['OriginalProgramBuilder']
	res = originalProgramBuilder(env, target, source + startupObjects)
	env.Command(None, res, "$SIZE $SOURCE")
	if 'Hex' in env['BUILDERS']:
		env.Hex(res)
	env.Disassembly(res)
	return res
	

def setup_gnu_tools(env, prefix):

	for tool in ['gcc', 'g++', 'gnulink', 'ar', 'gas']:
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
	
	env['CFLAGS'] = ["-std=gnu99", "-Wredundant-decls","-Wnested-externs"]
	
	env['CCFLAGS'] = [
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
	]
		
	env['CXXFLAGS'] = [
		"-fno-exceptions",
		"-nostdlib",
		"-fno-threadsafe-statics",
		"-fno-rtti",
		"-fuse-cxa-atexit",
		"-Woverloaded-virtual"
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
			linkerscript = env.File(device['linkerScript']).srcnode().abspath
			linkerscript = '"-T%s"' % linkerscript
		
		if 'includes' in device and device['includes'] is not None:
			env.Append(CPPPATH = device['includes'])
		
		if 'startup' in device and device['startup'] is not None:
			startFiles = '-nostartfiles'
		
	originalProgramBuilder = env['BUILDERS']['Program']
	env['BUILDERS']['Program'] = overrideProgramBuilder
	env['OriginalProgramBuilder'] = originalProgramBuilder
	
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