
import os
from SCons.Script import *


def builder_unit_test(env, target, source):
	targetName = str(target)
	objects = []
	for src in source:
		objects.append(env.Object('%s-%s' % (targetName, os.path.splitext(str(src))[0]), src))
	
	objects.append(env.Object('%s-%s' % (targetName, 'gtest_main'), '%s/gtest/gtest_main.cc' % env['MCUCPP_HOME']))
	objects.append(env.Object('%s-%s' % (targetName, 'gtest-all'), '%s/gtest/gtest-all.cc' % env['MCUCPP_HOME']))
	testExeNode = env.Program('%s_test' % targetName, objects)
	env.Default(testExeNode)
	testApp = str(testExeNode[0].abspath)
	testNode = env.Command('run_test_%s' % targetName, '', testApp)
	env.Default(testNode)
	env.AlwaysBuild(testNode)
	return 0

	
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
		
	#testBuilder = Builder(action = builder_unit_test)
	#env.Append(BUILDERS = {'Test' :  testBuilder})
	
	env.AddMethod(builder_unit_test, "Test");
	
	env['CCCOMSTR'] = "Host Compiling C: $TARGET"
	env['CXXCOMSTR'] = "Host Compiling C++: $TARGET"
	env['ASCOMSTR'] = "Host Assembling: $TARGET"
	env['ASPPCOMSTR'] = "Host Assembling: $TARGET"
	env['LINKCOMSTR'] = "Host Linking: $TARGET"

def exists(env):
	return True