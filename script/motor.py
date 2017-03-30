#!/usr/bin/env python
#JIN Linhao 2017
#This file is used to set the position movement of the motor
#some useful commands: 
#\x01\x10\x18\x02\x00\x04\x08\x00\x00\x4E\x20\x00\x00\x13\x88\xED\x1A is used to rotate the motor for 20,000 steps in the positive direction, with speed 5000HZ
#\x01\x10\x18\x02\x00\x04\x08\xFF\xFF\x4E\x20\x00\x00\x13\x88\xAD\x11 is used to rotate the motor for 20,000 steps in the negative direction, with speed 5000HZ
#\x01\x06\x00\x7D\x00\x08\x18\x14 is to START ON the positioning operation
#\x01\x06\x00\x7D\x00\x00\x19\xD2 is to START OFF the positioning operation
# \x01\x10\x18\x02\x00\x04\x08\x00\x00\x00\x64\x00\x00\x00\x64\x1E\x46")#100hz 100 stpes
import serial
import time
import binascii

# ser = serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=1, parity=serial.PARITY_EVEN,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)
class Motor:  
	def __init__(self):
		
		self.ser = serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=1, parity=serial.PARITY_EVEN,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)	



	def back_home(self):
		current_position = self.read_position()
		if current_position > 60000:
			#rotate postively to position 1
			self.ser.write("\x01\x10\x18\x00\x00\x0A\x14\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x07\xD0\x00\x00\x05\xDC\x00\x00\x05\xDC\x20\xE1")
			time.sleep(0.1)
			self.ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x08\xF5\x18")
			time.sleep(0.5)
			self.ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x00\xF4\xDE")
			time.sleep(0.1)
		else:
			#rotate negatively to position 1
			self.ser.write("\x01\x10\x18\x00\x00\x0A\x14\x00\x00\x00\x01\x00\x00\x00\x00\xFF\xFF\x07\xD0\x00\x00\x05\xDC\x00\x00\x05\xDC\x44\xE3")
			time.sleep(0.1)
			self.ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x08\xF5\x18")
			time.sleep(2)
			self.ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x00\xF4\xDE")
			time.sleep(0.1)
		print "finish return home operation, check position"
		# home_position = self.read_position()
		# print "home_pos", home_position
		print "--------------------------------------"



	def read_position(self):
		t1 = time.time()
		self.ser.write("\x01\x03\x00\xCC\x00\x02\x04\x34")
		current_binary_all = self.ser.readline()
		t2 = time.time()
		current_hex_all = binascii.hexlify(current_binary_all)
		print "current reading in hex", current_hex_all
		# t2 = time.time()
		# print "t2-t1: ", t2 -t1
		current_decimal = int(current_hex_all[-8:-4], 16)
		print "current position: ", current_decimal

		time.sleep(0.1)
		# print "========================== finish reading =========================="
		return current_decimal

	def move_round_100_500hz(self):
		print "==============="
		home_position = self.read_position()
		# print "home in move", home_position
		self.ser.write("\x01\x10\x18\x02\x00\x04\x08\x00\x00\x00\x64\x00\x00\x01\xF4\x1F\xBA")#500hz 100 stpes
		time.sleep(2)
		# origin = self.read_position()
		self.read_position()
		self.read_position()
		self.read_position()

		for count in range(0, 190):
			# ser.write("\x01\x10\x18\x02\x00\x04\x08\x00\x00\x00\x32\x00\x00\x00\x64\x67\x7B")#100hz 50 stpes
			# time.sleep(0.1)
			print "count ", count	
			expected_position = (count+1)*100 + 1
			print "expected position", expected_position
			self.ser.write("\x01\x06\x00\x7D\x00\x08\x18\x14")
			time.sleep(1)

			self.read_position()
			self.read_position()
			feedback_position = self.read_position()
			print "feedback position", feedback_position
			if feedback_position == 0 or feedback_position == 1024:
				self.ser.write("\x01\x06\x00\x7D\x00\x00\x19\xD2")
				time.sleep(0.2)
				continue
			elif feedback_position <= expected_position + 10 and feedback_position >= expected_position - 10:
				expected_position = feedback_position
				self.ser.write("\x01\x06\x00\x7D\x00\x00\x19\xD2")
				time.sleep(0.2)
				print "--------------------------------------"
			else:
				print "error in iteration"
				break

	def operation(self):
		# self.initialize()
		self.back_home()
		time.sleep(0.1)
		self.move_round_100_500hz()
		time.sleep(0.1)
		# self.back_home()


if __name__ == '__main__':
	motor = Motor()
	motor.operation()
	# motor.read_position()

