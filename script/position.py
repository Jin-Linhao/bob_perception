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
import rospy  
import tf
#ls /dev
t0 = time.time()
ser = serial.Serial(port='/dev/ttyUSB0',baudrate=115200,timeout=1, parity=serial.PARITY_EVEN,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)
t1 = time.time()

rospy.init_node('motor_tf_broadcaster')  



ser.write("\x01\x03\x00\xcc\x00\x02\x04\x34")
# t2 = time.time()
origin_raw = ser.readline()
# t3 = time.time()
origin = binascii.hexlify(origin_raw)
origin = int(origin[-8:-4], 16)
print "origin: ", origin
time.sleep(0.1)
print "--------------------------------------"

ser.write("\x01\x10\x18\x02\x00\x04\x08\x00\x00\x00\x64\x00\x00\x01\xF4\x1F\xBA")#500hz 100 stpes
time.sleep(0.1)
for count in range(0, 200):
	# ser.write("\x01\x10\x18\x02\x00\x04\x08\x00\x00\x00\x32\x00\x00\x00\x64\x67\x7B")#100hz 50 stpes
	# time.sleep(0.1)
	print count	
	expected_position = (count + 1)*100 + origin
	print "expected position", expected_position
	ser.write("\x01\x06\x00\x7D\x00\x08\x18\x14")
	time.sleep(1)

	ser.write("\x01\x03\x00\xcc\x00\x02\x04\x34")
	# t2 = time.time()
	data_raw = ser.readline()
	# t3 = time.time()
	response = binascii.hexlify(data_raw)
	feedback_position = int(response[-8:-4], 16)
	print "feedback position", feedback_position
	if feedback_position == 0 or feedback_position == 1024:
		ser.write("\x01\x06\x00\x7D\x00\x00\x19\xD2")
		time.sleep(0.2)
		br = tf.TransformBroadcaster()
		br.sendTransform((0, 0, 0),  
					 tf.transformations.quaternion_from_euler(0, 0, feedback_position*0.01),  
					 rospy.Time.now(),  
					 "laser",  "world")  
		continue
	elif feedback_position <= expected_position + 10 and feedback_position >= expected_position - 10:
		expected_position = feedback_position
		ser.write("\x01\x06\x00\x7D\x00\x00\x19\xD2")
		time.sleep(0.2)
		br = tf.TransformBroadcaster()
		br.sendTransform((0, 0, 0),  
					 tf.transformations.quaternion_from_euler(0, -feedback_position*0.01*0.018, 0),  
					 rospy.Time.now(),  
					 "laser",  "world")  
		print "--------------------------------------"
	else:
		print "error in iteration"
		break
ser.write("\x01\x10\x18\x00\x00\x0A\x14\x00\x00\x00\x01\x00\x00\x00\x00\xFF\xFF\x07\xD0\x00\x00\x05\xDC\x00\x00\x05\xDC\x44\xE3")
time.sleep(0.1)
ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x08\xF5\x18")
time.sleep(5)
ser.write("\x01\x10\x00\x7C\x00\x02\x04\x00\x00\x00\x00\xF4\xDE")


