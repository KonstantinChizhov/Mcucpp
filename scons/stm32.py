import os
from SCons.Script import *


def stLinkRun(target, source, env):
	command = '$FLASH_TOOL -c SWD Srst -Run'
	res = env.Execute(command)
	return res
	
def stLinkFlashImage(target, source, env):
	command = '$FLASH_TOOL -c SWD Srst -P %s -V' % (source[0].abspath)
	res = env.Execute(command)
	return res
	
def flashToolTargets(target, source, env):
	return [], source

def generate(env, **kw):

	env.Tool('arm-eabi-gcc')
	#TODO: other flashers, non default location, target verification
	flasher_windows = "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"
	if os.path.isfile(flasher_windows): 
		env['FLASH_TOOL'] = '"%s"' % (flasher_windows)
		flashBuilder = Builder(
			action = stLinkFlashImage, 
			src_suffix = ".hex",
			suffix = ".flash_result")
		runBuilder = Builder(
			action = stLinkRun, 
			src_suffix = ".hex",
			suffix = ".run_result")
		env.Append(BUILDERS = {'Flash': flashBuilder, 'Run': runBuilder})

def exists(env):
	return env.Detect('arm-eabi-gcc')