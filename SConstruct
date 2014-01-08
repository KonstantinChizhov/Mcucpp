import os

def builder_unit_test(target, source, env):
	app = str(source[0].abspath)
	if os.spawnl(os.P_WAIT, app, app)==0:
		open(str(target[0]),'w').write("PASSED\n")
	else:
		return 1
	
testEnv = DefaultEnvironment(tools = ['mingw'],
	ENV = {'PATH' : os.environ['PATH']}, 
	CPPPATH=['#/mcucpp', '#/gtest', '#/mcucpp/Test', '#/./']
	)
	
bld = Builder(action = builder_unit_test)
testEnv.Append(BUILDERS = {'Test' :  bld})

if testEnv['CC'] == 'cl':
	testEnv.Append(CXXFLAGS = ['/wd4503', '/wd4530'])
	testEnv.Append(CPPDEFINES=['_VARIADIC_MAX=10'])
	testEnv.Append(CPPPATH=['#/tests/include'])
	
testEnv['CCCOMSTR'] = "Compiling C: $TARGET"
testEnv['CXXCOMSTR'] = "Compiling C++: $TARGET"
testEnv['ASCOMSTR'] = "Assembling: $TARGET"
testEnv['ASPPCOMSTR'] = "Assembling: $TARGET"
testEnv['LINKCOMSTR'] = "Linking: $TARGET"
	
Export('testEnv')

SConscript('./SConscript', variant_dir='build', duplicate=0)
