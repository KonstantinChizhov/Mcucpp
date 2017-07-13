
import os
from SCons.Script import *

def unit_test_run(env, target, source):
	testApp = str(target[0].abspath)
	testNode = env.Command('run_test_%s' % str(target[0]), '', testApp)
	
def unit_test_emitter(target, source, env):
	source.append(env.Object('%s/gtest/gtest_main.cc' % env['MCUCPP_HOME']))
	source.append(env.Object('%s/gtest/gtest-all.cc' % env['MCUCPP_HOME']))
	return target, source
	
def generate(env, **kw):
	env['ENV'] = os.environ 
	#if env.Detect('mingw32-gcc'):
	#	env.Tool('mingw')
	#else:
	env.Tool('default')
	
	#print 'Used C compiler "%s"' % env['CC']
	
	env.Append(CPPPATH = [\
		'%s' % env['MCUCPP_HOME'],
		'%s/mcucpp' % env['MCUCPP_HOME'], 
		'%s/gtest' % env['MCUCPP_HOME'], 
		'%s/mcucpp/Test' % env['MCUCPP_HOME'], 
		'#/./'])
	
	if env['CC'] == 'cl':
		env.Append(CXXFLAGS = ['/wd4503', '/wd4530'])
		env.Append(CPPDEFINES=['_VARIADIC_MAX=15'])
		env.Append(CPPPATH=['%s/tests/include' % env['MCUCPP_HOME']])
	
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