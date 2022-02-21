from Phidget22.Phidget import *
from Phidget22.Devices.RCServo import *
import time

def main():
	rcServo0 = RCServo()

	rcServo0.setDeviceSerialNumber(42685)

	rcServo0.openWaitForAttachment(5000)

	rcServo0.setTargetPosition(120)
	rcServo0.setEngaged(True)

	try:
		input("Press Enter to Stop\n")
	except (Exception, KeyboardInterrupt):
		pass

	rcServo0.close()

main()