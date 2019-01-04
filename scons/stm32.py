import os
from SCons.Script import *
import datetime
import which

ocdInterfaces = {
	'f1' : 'interface/stlink-v2.cfg',
	'f1' : 'interface/stlink-v1.cfg',
	'f2' : 'interface/stlink-v1.cfg',
	'f3' : 'interface/stlink-v2.cfg',
	'f4' : 'interface/stlink-v2.cfg'
	}

ocdTargets = {
	'f0' : 'target/stm32f0x.cfg',
	'f1' : 'target/stm32f1x.cfg',
	'f2' : 'target/stm32f2x.cfg',
	'f3' : 'target/stm32f3x.cfg',
	'f4' : 'target/stm32f4x.cfg'
	}

def getFamilyFromName(deviceName):
	return deviceName.lower().replace('stm32', '')[0:2]

def stLinkRun(target, source, env):
	command = '$FLASH_TOOL -c SWD -NoPrompt -Q Srst -Run'
	res = env.Execute(command)
	return res
	
def stLinkFlashImage(target, source, env):
	command = '$FLASH_TOOL -c SWD Srst -ME -P %s -V -NoPrompt -Q' % (source[0].abspath)
	res = env.Execute(command)
	logFile = open(target[0].abspath, 'w')
	logFile.write('Flash tool executed\r\n%s\r\n%s\r\nResult: %s\n' % (command, datetime.datetime.now(), res ) )
	return res

def openOcdRun(target, source, env):
	command = '"$FLASH_TOOL" '
	if os.path.isdir(os.path.join(env['FLASH_TOOL_PATH'], '../share/openocd/scripts')):
		command += '-s "%(FLASH_TOOL_PATH)s/../share/openocd/scripts" ' 
	command += '-f %(OCD_INTERFACE)s ' 
	command += '-f %(OCD_TARGET)s '
	command += '-c "init" -c "reset run" -c "shutdown"'
	command = command % env
	
	res = env.Execute(command)
	return res
	
def openOcdFlashImage(target, source, env):
	command = '"$FLASH_TOOL" '
	if os.path.isdir(os.path.join(env['FLASH_TOOL_PATH'], '../share/openocd/scripts')):
		command += '-s "%(FLASH_TOOL_PATH)s/../share/openocd/scripts" ' 
	command += '-f %(OCD_INTERFACE)s ' 
	command += '-f %(OCD_TARGET)s '
	command += '-c "init" -c "reset init" -c "flash write_image erase %s" -c "reset" -c "shutdown"' % \
			(source[0].abspath.replace('\\', '/'))
	
	command = command % env
	res = env.Execute(command)
	logFile = open(target[0].abspath, 'w')
	logFile.write('Flash tool executed\r\n%s\r\n%s\r\nResult: %s\n' % (command, datetime.datetime.now(), res ) )
	return res
	
	
def openOcdDebug(target, source, env):
	import subprocess
	scriptsPath = os.path.abspath(os.path.join(env['FLASH_TOOL_PATH'], '../share/openocd/scripts'))
	openOcdArgs = [env['FLASH_TOOL']]
	if os.path.isdir(scriptsPath):
		openOcdArgs += ['-s', scriptsPath] 
	openOcdArgs += ['-f', env['OCD_INTERFACE']]
	openOcdArgs += ['-f', env['OCD_TARGET']]
	ocdOut = open(target[0].abspath, 'w')
	openOcdProcess = subprocess.Popen(openOcdArgs, stdout=ocdOut, stderr=ocdOut, stdin=None)
	gdb = '$GDB --eval-command="target remote localhost:3333" %s' % (source[0].abspath)
	res = env.Execute(gdb)
	openOcdProcess.terminate()
	ocdOut.close()
	return res

def generate(env, **kw):
	env.Tool('arm-eabi-gcc')
	
	flasherFound = False
	flasher_windows = "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
	
	if not flasherFound and env.Detect("openocd") and False:
		if not 'DEVICE_NAME' in env and not 'OCD_INTERFACE' in env:
			raise Exception("No device name specified. Set DEVICE_NAME with valid device name.")
		
		devFmily = getFamilyFromName(env['DEVICE_NAME'])
		ocdInterface = ocdInterfaces[devFmily]
		ocdTarget = ocdTargets[devFmily]
		env["OCD_INTERFACE"] = ocdInterface
		env["OCD_TARGET"] = ocdTarget
	
		openOcdLocation = which.which('openocd')
		if openOcdLocation is not None:
			openOcdPath, fname = os.path.split(openOcdLocation)

		env['FLASH_TOOL'] = os.path.abspath(openOcdLocation)
		env['FLASH_TOOL_PATH'] = openOcdPath
		
		flashBuilder = Builder(
			action = openOcdFlashImage, 
			src_suffix = ".hex",
			suffix = "_flash.log")
		runBuilder = Builder(
			action = openOcdRun, 
			src_suffix = ".hex",
			suffix = "_run.log")
		debugBuilder = Builder(
			action = openOcdDebug, 
			src_suffix = ".elf",
			suffix = "_debug.log")
		env.Append(BUILDERS = {'Flash': flashBuilder, 'Run': runBuilder, 'Debug' : debugBuilder})
		flasherFound = True
		
	if not flasherFound:
		if os.path.isfile(flasher_windows): 
			env['FLASH_TOOL'] = '"%s"' % (flasher_windows)
			flashBuilder = Builder(
				action = stLinkFlashImage, 
				src_suffix = ".hex",
				suffix = "_flash.log")
			runBuilder = Builder(
				action = stLinkRun, 
				src_suffix = ".hex",
				suffix = "_run.log")
			env.Append(BUILDERS = {'Flash': flashBuilder, 'Run': runBuilder})
			flasherFound = True
		else:
			print ("ST-LINK Utility is not found in default location")
	
	if not flasherFound:
		print ("No supported flashing tool found. ")

def exists(env):
	return env.Detect('arm-eabi-gcc')