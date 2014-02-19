

#SConscript('gtest/SConscript')
#SConscript('tests/UnitTests/SConscript')
#SConscript('examples/SConscript')
#SConscript('tests/Stm32F40x/SConscript')


env = Environment(DEVICE_NAME = 'stm32f407', toolpath = ['#/scons'], tools=['mcucpp'])

env.Program('Hello', 'main.cpp')