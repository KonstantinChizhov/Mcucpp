import os
import shutil
import sys

def Crc32Table(v):
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	v = ((v >> 1) ^ 0xEDB88320) if (v & 1) else v >> 1
	return v

def Crc32Update(newchar, crcval):
	return (crcval >> 8) ^ Crc32Table((crcval ^ newchar) & 0xff)

def FileCrc(fileName, crcInit):
	crc = crcInit
	with open(fileName, "rb") as f:
		byte = f.read(1)
		while len(byte) >= 1:
			crc = Crc32Update(ord(byte), crc)
			byte = f.read(1)
	return crc
	
def DumpSection(elfFile, sectionName, env):
	dumpFileName = elfFile + sectionName
	app = env['OBJCOPY']
	args = ' -O binary --only-section %s "%s" "%s"' % (sectionName, elfFile, dumpFileName)
	env.Execute(app + args)
	return dumpFileName

def crc_stamp(target, source, env):
	shutil.copy(source[0].abspath, target[0].abspath)
	text = DumpSection(source[0].abspath, ".text", env)
	isr = DumpSection(source[0].abspath, ".isr_vectors", env)
	data = DumpSection(source[0].abspath, ".data", env)
	
	crc = 0XFFFFFFFF
	#crc = FileCrc(isr, crc)
	crc = FileCrc(text, crc)
	#crc = FileCrc(data, crc)
	crc = crc ^ 0XFFFFFFFF
	
	if not "CRC_MARKER" in env:
		raise Exception("CRC_MARKER var is not found in current environment")
	
	crcMarker = env["CRC_MARKER"]
	
	if sys.version_info[0] < 3:
		crcString = chr((crc >> 0) & 0xff) + chr((crc >> 8) & 0xff) + chr((crc >> 16) & 0xff) + chr((crc >> 24) & 0xff)
		markerString = chr((crcMarker >> 0) & 0xff) + chr((crcMarker >> 8) & 0xff) + chr((crcMarker >> 16) & 0xff) + chr((crcMarker >> 24) & 0xff)
	else:
		crcString = crc.to_bytes(4, byteorder='little', signed=False)
		markerString = crcMarker.to_bytes(4, byteorder='little', signed=False)
	
	with open(target[0].abspath, 'rb') as elf_file:
		content = elf_file.read()
	
	patched = content.replace (markerString, crcString)
	if content == patched:
		raise Exception("CRC Marker is not found in: %s" % source[0].abspath)
	with open(target[0].abspath, 'wb') as patchedFile:
		patchedFile.write(patched)
	
	src_base = os.path.basename(source[0].abspath)
	print ('Firmware CRC for %s: %s' % (src_base, hex(crc)) )
	crc_file_name = os.path.join(os.path.dirname(source[0].abspath), "%s_checksum.txt" % (src_base) )
	crc_out = open(crc_file_name , 'w')
	crc_out.write("CRC32 checksum for %s\n" % src_base)
	crc_out.write(hex(crc).upper() + "\n")
	return None

def mcucpp_crc_emitter(target, source, env):
	src_base = os.path.basename(source[0].abspath)
	crc_file_name = os.path.join(os.path.dirname(source[0].abspath), "%s_checksum.txt" % (src_base) )
	target.append(crc_file_name)
	target.append(source[0].abspath + ".text")
	target.append(source[0].abspath + ".isr_vectors")
	target.append(source[0].abspath + ".data")
	return target, source

def exists(env):
	return True

def generate(env, **kw):
	crc_builder = env.Builder(action = crc_stamp, suffix = '.elf', src_suffix = '.elf', emitter = mcucpp_crc_emitter)
	#crc_builder = env.Builder(action = crc_stamp, suffix = '.elf', src_suffix = '.elf')
	env.Append(BUILDERS = {'StampCrc' : crc_builder})
	env['CRC_MARKER'] = 0xFEA90950

