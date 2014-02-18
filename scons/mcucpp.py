import os
from SCons.Script import *



def generate(env, **kw):
	env['ENV'] = os.environ
	setup_gnu_tools(env, 'arm-none-eabi-')

def exists(env):
	return env.Detect('arm-none-eabi-gcc')