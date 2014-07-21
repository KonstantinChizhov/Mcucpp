import devices
import sys

if len(sys.argv) > 1:
	if sys.argv[1] == 'list':
		for devId in sorted(devices.SupportedDevices):
			sys.stdout.write("%s%s" %(devId, ";"))
	if sys.argv[1] == 'manufacturers':
		manufacturers = set(v['manufacturer'] for k, v in devices.SupportedDevices.items())
		for devId in sorted(manufacturers):
			sys.stdout.write("%s%s" %(devId, ";"))
	if sys.argv[1] == 'manufacturer':
		if len(sys.argv) < 3:
			exit(1)
		manufacturer = sys.argv[2]
		for devId in sorted(k for k, v in devices.SupportedDevices.items() if (v['manufacturer'] == manufacturer)):
			sys.stdout.write("%s%s" %(devId, ";"))
else:
	#TODO: usage
	exit(0)