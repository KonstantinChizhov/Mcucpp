

#SConscript('gtest/SConscript')
#SConscript('tests/UnitTests/SConscript')
#SConscript('examples/SConscript')
#SConscript('tests/Stm32F40x/SConscript')


env = Environment(DEVICE_NAME = 'msp430g2231', toolpath = ['#/scons'], tools=['mcucpp'])


env.Program('Hello', 'main.cpp')

