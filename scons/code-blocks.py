# Generates project files for Code-Blocks IDE

import xml.etree.ElementTree as ET
import os
from os.path import basename
from xml.dom import minidom
import extensions


def create_project_target(env, currentSource, buildElement, projectFileDir, buildTarget, compiler, project):
	targetElement = ET.SubElement(buildElement, "Target", {"title":buildTarget})
	sources, includes = extensions.get_sources_and_includes(env, currentSource)
	
	outPutRelative = projectFileDir.rel_path(currentSource)
	objectPath = os.path.dirname(outPutRelative)
	if objectPath == "":
		objectPath = "./"
		
	ET.SubElement(targetElement, "Option", {"output":outPutRelative})
	ET.SubElement(targetElement, "Option", {"object_output": objectPath})
	ET.SubElement(targetElement, "Option", {"type":"0" })
	ET.SubElement(targetElement, "Option", {"create_hex":"1"})
	ET.SubElement(targetElement, "Option", {"compiler":compiler })
	ET.SubElement(targetElement, "Option", {"projectDeviceOptionsRelation":"0"})
	
	compilerElem = ET.SubElement(targetElement, "Compiler")
	for flag in env["CCFLAGS"]:
		ET.SubElement(compilerElem, "Add", option=flag)
	for define in env["CPPDEFINES"]:
		if isinstance(define, dict):
			for key in define:
				ET.SubElement(compilerElem, "Add", symbol="%s=%s" %(key, define[key]))
		else:
			ET.SubElement(compilerElem, "Add", symbol=define)
	
	for includePath in env["CPPPATH"]:
		includeDir = env.Dir(includePath).srcnode()
		includeDirRel =  projectFileDir.rel_path(includeDir)
		ET.SubElement(compilerElem, "Add", directory=includeDirRel)
	
	cppElem = ET.SubElement(targetElement, "Cpp")
	for flag in env["CXXFLAGS"] + env["CCFLAGS"]:
		ET.SubElement(cppElem, "Add", option=flag)
		
	asmElem = ET.SubElement(targetElement, "Assembler")
	for flag in env["ASFLAGS"]:
		ET.SubElement(asmElem, "Add", option=flag)
		
	linkerScript = None
	linkElem = ET.SubElement(targetElement, "Linker")
	for flag in env["LINKFLAGS"]:
		flag = flag.rstrip('"').strip('"')
		if flag.startswith("-T"):
			linkerScript = flag
		ET.SubElement(linkElem, "Add", option=flag)
	
	deviceElem = ET.SubElement(targetElement, "Device")
	if 'DEVICE' in env:
		device = env["DEVICE"]
		if "cpu" in device:
			ET.SubElement(deviceElem, "Add", option="$device=" + device["cpu"])

	for src in sources | includes:
		item = projectFileDir.rel_path(src.srcnode())
		filename, file_extension = os.path.splitext(item)
		if file_extension.lower() in env["CPPSUFFIXES"]:
			#unit = ET.SubElement(targetElement, "Unit", { "filename":item})
			unit = ET.SubElement(project, "Unit", { "filename":item})
			if file_extension.lower() in [".c", ".cpp", ".s", ".cxx", ".cc"]:
				ET.SubElement(unit, "Option", { "compilerVar":"CC"})
	
	add_build_commands(targetElement)


def add_build_commands(targetElement):
	makeCommands = ET.SubElement(targetElement, "MakeCommands")
	ET.SubElement(makeCommands, "Build", command="scons.bat -f $makefile $target")
	ET.SubElement(makeCommands, "CompileFile", command="scons.bat -f $makefile $file")
	ET.SubElement(makeCommands, "Clean", command="scons.bat -f $makefile $target -c")
	ET.SubElement(makeCommands, "DistClean", command="scons.bat -f $makefile $target -c")
	ET.SubElement(makeCommands, "SilentBuild", command="scons.bat -f $makefile $target")
	ET.SubElement(makeCommands, "AskRebuildNeeded", command="echo 1")
	
def generate_common_project(target, source, env, projectFile, compiler):
	buildTarget = "Default"
	if "CURRENT_BUILD_TARGET" in env:
		buildTarget = env["CURRENT_BUILD_TARGET"]
	
	rootDir = env.Dir("#/").srcnode().abspath
	projectFileDir = env.Dir(os.path.dirname(target[0].abspath))
	targetFilename = basename(source[0].abspath)
	rootConstruct = env.File(os.path.join(rootDir, "SConstruct")).srcnode()
	rootConstructRelative = projectFileDir.rel_path(rootConstruct)
	
	project = ET.SubElement(projectFile, "Project")
	ET.SubElement(project, "Option", { "title":targetFilename})
	ET.SubElement(project, "Option", { "compiler":compiler})
	
	ET.SubElement(project, "Option", makefile=rootConstructRelative)
	ET.SubElement(project, "Option", makefile_is_custom="1")
	ET.SubElement(project, "Option", make_tool="scons.bat")

	buildElement = ET.SubElement(project, "Build")
	for currentSource in source:
		create_project_target(env, currentSource, buildElement, projectFileDir, buildTarget, compiler, project)
		
	add_build_commands(project)
	return project

def get_cb_compiler_name(env):
	if "CC" in env:
		env["CC"]
	return "gcc"
	
def save_project(projectFileNode, targetFilePath):
	xmlstr = minidom.parseString(ET.tostring(projectFileNode, encoding="utf8", method="xml")).toprettyxml(indent="   ")
	with open(targetFilePath, "w") as f:
		f.write(xmlstr)

def add_common_debugger_section(xmlExtensionsNode, debuggerPort):
	debuggerNode = ET.SubElement(xmlExtensionsNode, "debugger") 
	remoteDebuggingNode = ET.SubElement(debuggerNode, "remote_debugging") 
	optionsNode = ET.SubElement(remoteDebuggingNode, "options", conn_type="0", ip_address="localhost", ip_port=debuggerPort)
	return remoteDebuggingNode

def add_st_link_debugging_settings(xmlExtensionsNode, debuggerPort):
	debuggerNode = ET.SubElement(xmlExtensionsNode, "debugger")
	targetDebuggingSettingsNode = ET.SubElement(debuggerNode, "target_debugging_settings", target="servo_build", active_interface="ST-link")
	debugInterfaceNode = ET.SubElement(targetDebuggingSettingsNode, "debug_interface", 
			interface_id="ST-link",
			ip_address="localhost",
			ip_port=debuggerPort,
			path="${EMBITZ}\share\contrib",
			executable="STLinkGDB.exe",
			description="..\..\Mcucpp\svd\STM32F1_svd_V1.0.0\STM32F100.svd",
			dont_start_server="false",
			backoff_time="1000",
			options="0",
			reg_filter="0",
			active_family="STMicroelectronics",
			gdb_before_conn="",
			gdb_after_conn="")
	familyOptionsNode = ET.SubElement(debugInterfaceNode, "family_options", family_id="STMicroelectronics")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_JTAG_SWD", opt_value="swd")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_VECTOR_START", opt_value="0x08000000")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_RESET_TYPE", opt_value="System")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_LOAD_PROGRAM", opt_value="1")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_SEMIHOST_CHECK", opt_value="1")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_RAM_EXEC", opt_value="0")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_VEC_TABLE", opt_value="1")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_DONT_CONN_RESET", opt_value="0")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_ALL_MODE_DEBUG", opt_value="0")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_DEV_ADDR", opt_value="")
	ET.SubElement(familyOptionsNode, "option", opt_id="ID_VERBOSE_LEVEL", opt_value="3")
	return debuggerNode

	
def generate_embitz_project(target, source, env):
	projectFile = ET.Element("EmBitz_project_file")
	ET.SubElement(projectFile, "EmBitzVersion", attrib={"release" : "1.11", "revision" : "0"})
	ET.SubElement(projectFile, "FileVersion", attrib={"major":"1", "minor":"0"}) 
	projectNode = generate_common_project(target, source, env, projectFile, "armgcc_eb")
	extensionsNode = ET.SubElement(projectNode, "Extensions")
	add_st_link_debugging_settings(extensionsNode, "4242")
	save_project(projectFile, target[0].abspath)
	return None

def generate_cb_project(target, source, env):
	projectFile = ET.Element("CodeBlocks_project_file")
	ET.SubElement(projectFile, "FileVersion", attrib={"major":"1", "minor":"6"}) 
	projectNode = generate_common_project(target, source, env, projectFile, get_cb_compiler_name(env))
	extensionsNode = ET.SubElement(projectNode, "Extensions")
	add_common_debugger_section(extensionsNode, "4242")
	save_project(projectFile, target[0].abspath)
	return None

def exists(env):
	return True
	
def generate(env, **kw):
	bld = env.Builder(action = generate_embitz_project, suffix = '.ebp')
	env.Append(BUILDERS = {'EmbitzProject' : bld})
	bld = env.Builder(action = generate_cb_project, suffix = '.cbp')
	env.Append(BUILDERS = {'CodeBlocksProject' : bld})

	