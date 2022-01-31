from Phidget22.Phidget import *
from Phidget22.Devices.VoltageInput import *
import time

def onVoltageChange(self, voltage):
	print("Voltage: " + str(voltage))

def main():
	voltageInput3 = VoltageInput()

	voltageInput3.setChannel(3)

	voltageInput3.setOnVoltageChangeHandler(onVoltageChange)

	voltageInput3.openWaitForAttachment(5000)

	try:
		input("Press Enter to Stop\n")
	except (Exception, KeyboardInterrupt):
		pass

	voltageInput3.close()

main()
