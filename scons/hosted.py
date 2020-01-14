
import os
from SCons.Script import *
import subprocess

def unit_test_run(env, target, source):
	print('unit_test_run', target, source)
	testApp = str(target[0].abspath)
	subprocess.call([testApp])
	#testNode = env.Command('run_test_%s' % str(target[0]), '', testApp)
	#print (testNode)
	
def unit_test_emitter(target, source, env):
	build_dir = os.path.dirname(target[0].abspath) 
	source.append(env.Object(os.path.join(build_dir, 'gmock_main'), 
				'%s/3rdparty/googletest/googlemock/src/gmock_main.cc' % env['MCUCPP_HOME']))
	source.append(env.Object(os.path.join(build_dir, 'gtest-all'), 
				'%s/3rdparty/googletest/googletest/src/gtest-all.cc' % env['MCUCPP_HOME']))
	source.append(env.Object(os.path.join(build_dir, 'gmock-all'), 
				'%s/3rdparty/googletest/googlemock/src/gmock-all.cc' % env['MCUCPP_HOME']))

	return target, source
	
def generate(env, **kw):
	env['ENV'] = os.environ

	if env['PLATFORM'] == 'win32' and (env.Detect('mingw32-gcc') or env.Detect('gcc')):
		env.Tool('mingw')
	else:
		env.Tool('default')
	
	print( 'Used C compiler "%s"' % env['CC'])
	
	env.Append(CPPPATH = [\
		'%s/3rdparty/googletest/googletest/include' % env['MCUCPP_HOME'],
		'%s/3rdparty/googletest/googlemock/include' % env['MCUCPP_HOME'],
		'%s/3rdparty/googletest/googletest' % env['MCUCPP_HOME'],
		'%s/3rdparty/googletest/googlemock' % env['MCUCPP_HOME'],
		'%s/mcucpp' % env['MCUCPP_HOME'], 
		'%s/mcucpp/Test' % env['MCUCPP_HOME'], 
		'#/./'])
	
	if env['CC'] == 'cl':
		env.Append(CXXFLAGS = ['/wd4503', '/wd4530'])
		env.Append(CPPDEFINES=['_VARIADIC_MAX=15'])
		env.Append(CPPPATH=['%s/tests/include' % env['MCUCPP_HOME']])
	
	if env['CC'] == 'gcc':
		env.Append(CXXFLAGS = ['-std=gnu++14', '-O0', '-g'])
		env.Append(CXXFLAGS = ['-static', '-static-libgcc', '-static-libstdc++' ])
		
	
	env.Append(CPPDEFINES={'GTEST_OS_WINDOWS':'1', 'GTEST_HAS_PTHREAD':'0', 'EMULATE_GLIBC':'1', '_EMULATE_GLIBC':'1'})
	
	programBuilder = env['BUILDERS']['Program']
	
	testBuilder = Builder(action = [programBuilder.action, unit_test_run], 
			src_suffix = programBuilder.src_suffix,
			suffix = programBuilder.suffix,
			emitter = [programBuilder.emitter, unit_test_emitter])
	
	env.Append(BUILDERS = {'Test' :  testBuilder})
	
	env['CCCOMSTR'] = "Host Compiling C: $TARGET"
	env['CXXCOMSTR'] = "Host Compiling C++: $TARGET"
	env['ASCOMSTR'] = "Host Assembling: $TARGET"
	env['ASPPCOMSTR'] = "Host Assembling: $TARGET"
	env['LINKCOMSTR'] = "Host Linking: $TARGET"

def exists(env):
	return True