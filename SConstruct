import os

def builder_unit_test(target, source, env):
	app = str(source[0].abspath)
	if os.spawnl(os.P_WAIT, app, app)==0:
		open(str(target[0]),'w').write("PASSED\n")
	else:
		return 1
	
testEnv = DefaultEnvironment(
	ENV = {'PATH' : os.environ['PATH']}, 
	CPPPATH=['#/mcucpp', '#/gtest', '#/mcucpp/Test', '#/./'], 
	CPPDEFINES=['_VARIADIC_MAX=10']
	)
bld = Builder(action = builder_unit_test)
testEnv.Append(BUILDERS = {'Test' :  bld})
#/wd4503
Export('testEnv')

SConscript('./SConscript', variant_dir='build', duplicate=0)
