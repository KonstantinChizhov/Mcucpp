
def which(program):
	import os
	def is_exe(fpath):
		return os.path.isfile(fpath)  

	fpath, fname = os.path.split(program)
	if fpath:
		if is_exe(program):
			return program
	else:
		for path in os.environ["PATH"].split(os.pathsep):
			path = path.strip('"')
			exe_file = os.path.join(path, program)
			if is_exe(exe_file):
				return exe_file
			if is_exe(exe_file + ".exe"):
				return exe_file + ".exe"
	return None