

import xml.etree.ElementTree as ET
import os
from os.path import basename
from xml.dom import minidom
import extensions


def generate_vs_project(env, source):
	projectFileDir = env.Dir('.').srcnode()
	print projectFileDir
	
	sources, includes = extensions.get_sources_and_includes(env, source[0])
	source_list = []
	include_list = []
	
	for src in sources | includes:
		item = projectFileDir.rel_path(src.srcnode())
		filename, file_extension = os.path.splitext(item)
		if file_extension.lower() in env["CPPSUFFIXES"]:
			if file_extension.lower() in [".c", ".cpp", ".s", ".cxx", ".cc", ".asm"]:
				source_list.append(item)
			else:
				include_list.append(item)
	
	env.Append(CPPDEFINES = '_ALLOW_KEYWORD_MACROS')
	env.Append(CPPDEFINES = ['DEBUG', '_DEBUG'])
	env.Append(CCFLAGS='/MDd')
	env.Append(CCFLAGS=['/Zi', '/Fd${TARGET}.pdb'])
	env.Append(LINKFLAGS = ['/DEBUG'])

	proj = env.MSVSProject(target = source[0],
					srcs = source_list,
					incs = include_list,
					buildtarget = source[0],
					variant = 'Debug')
	
	return proj


def exists(env):
	return True
	
def generate(env, **kw):
	env.AddMethod(generate_vs_project, "McucppVsProject")
	