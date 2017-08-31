import os
from SCons.Script import *
import devices
import extensions


def get_env_id(env, target):
	allOptions = env['CFLAGS'] + env['CCFLAGS'] + env['ASFLAGS'] + env['CPPDEFINES'] + env['LINKFLAGS'] + [str(t.abspath) for t in target]
	if 'DEVICE' in env:
		allOptions = allOptions + [env['DEVICE']['cpu']]
	return abs(hash(str(allOptions)))

def get_lib_sources(target, source, env):
	lib_src_to_add = set()
	for src in source:
		sources, includes = extensions.get_sources_and_includes(env, src)
		for inc in includes:
			dir_name = os.path.dirname(inc.abspath)
			inc_file_name = os.path.splitext(os.path.basename(inc.abspath))[0]
			src_dir = os.path.join(dir_name, "src")
			if os.path.exists(src_dir):
				for lib_src_file in os.listdir(src_dir):
					lib_src_base_name = os.path.splitext(lib_src_file)[0]
					if lib_src_base_name.lower() == inc_file_name.lower():
						lib_src_to_add.add(os.path.join(src_dir, lib_src_file))
				
	return lib_src_to_add
	
def mcucpp_program_emitter(target, source, env):
	""" Emitts library and startup sources to link with user's program """
	
	if 'TARGET_HEAP_SIZE' in env:
		env.Append(CPPDEFINES = {'TARGET_HEAP_SIZE' : env['TARGET_HEAP_SIZE'] })
	objects_to_add = set()
	all_lib_src_to_add = get_lib_sources(target, source, env)
	id = get_env_id(env, target)
	
	if 'DEVICE' in env:
		device = env['DEVICE']
		
		if 'startup' in device and device['startup'] is not None:
			for startupSrc in device['startup']:
				all_lib_src_to_add.add(os.path.normcase(startupSrc % env))
		
		if 'libSources' in device and device['libSources'] is not None:
			for libSrc in device['libSources']:
				all_lib_src_to_add.add(os.path.normcase(libSrc % env))
				
	lib_src_to_add = all_lib_src_to_add
	while len(lib_src_to_add) > 0:
		objects = set()
		for libSrc in lib_src_to_add:
			objName = '%s_%s' % (os.path.splitext(os.path.basename(libSrc))[0], id)
			object = env.Object(objName, libSrc)
			for o in object:
				objects.add(o)
		lib_src_to_add = get_lib_sources(target, objects, env) - all_lib_src_to_add
		objects_to_add = objects_to_add | objects
		all_lib_src_to_add = all_lib_src_to_add | lib_src_to_add
	
	for obj in objects_to_add:
		source.append(obj)
	
	return target, source

def generate(env, **kw):
	env['ENV'] = os.environ
	env['SUPPORTED_DEVICES'] = devices.SupportedDevices
	env['MCUCPP_HOME'] = os.path.abspath(os.path.join( os.path.dirname( os.path.realpath( __file__ ) ), '../'))
	#print "MCUCPP_HOME = %s" % env['MCUCPP_HOME']
	
	env['PROGEMITTER'] = mcucpp_program_emitter
	
	env.Append(CPPPATH = os.path.join(env['MCUCPP_HOME'], '3rdparty\include'))
	
	if 'DEVICE_NAME' in env:
		deviceName = env['DEVICE_NAME']
		if deviceName in devices.SupportedDevices:
			device = devices.SupportedDevices[deviceName]
			env['DEVICE'] = device
		else :
			print 'Unknown device name: %s' % deviceName
			print "List of supported devices:"
			for devId in sorted(devices.SupportedDevices):
				print devId
			raise Exception('Unknown device name: %s' % deviceName)
	else:
		if 'DEVICE' in env:
			device = env['DEVICE']
		else:
			env.Tool('hosted')
			return 
	
	if 'arch' not in device:
		raise Exception('No "arch" definition in given device')
	if 'tools' not in device['arch']:
		raise Exception('No "tools" definition in given arch')
	
	for tool in device['arch']['tools']:
		env.Tool(tool)
	
	env.Tool('code-blocks')
	#env.Tool('serial_tools')


def exists(env):
	return True