
import os
from SCons.Script import *


def builder_unit_test(target, source, env):
	targetName = str(target[0])
	objects = []
	for src in source:
		objects.append(env.Object('%s-%s' % (targetName, os.path.splitext(str(src))[0]), src))
	
	objects.append(env.Object('%s-%s' % (targetName, 'gtest_main'), '#/gtest/gtest_main.cc'))
	objects.append(env.Object('%s-%s' % (targetName, 'gtest-all'), '#/gtest/gtest-all.cc'))
	testExeNode = env.Program('%s_test' % targetName, objects)
	testApp = str(testExeNode[0].abspath)
	testNode = env.Command('run_test_%s' % targetName, '', testApp)
	env.AlwaysBuild(testNode)
	return 0

	
def generate(env, **kw):
	# TODO autodetect
	env.Tool('mingw')
	#env.Tool('default')
	env['ENV'] = os.environ 
	env.Append(CPPPATH = ['#/mcucpp', '#/gtest', '#/mcucpp/Test', '#/./'])
	
	if env['CC'] == 'cl':
		env.Append(CXXFLAGS = ['/wd4503', '/wd4530'])
		env.Append(CPPDEFINES=['_VARIADIC_MAX=10'])
		env.Append(CPPPATH=['#/tests/include'])
		
	testBuilder = Builder(action = builder_unit_test)
	env.Append(BUILDERS = {'Test' :  testBuilder})
	
	env['CCCOMSTR'] = "Host Compiling C: $TARGET"
	env['CXXCOMSTR'] = "Host Compiling C++: $TARGET"
	env['ASCOMSTR'] = "Host Assembling: $TARGET"
	env['ASPPCOMSTR'] = "Host Assembling: $TARGET"
	env['LINKCOMSTR'] = "Host Linking: $TARGET"

def exists(env):
	return True