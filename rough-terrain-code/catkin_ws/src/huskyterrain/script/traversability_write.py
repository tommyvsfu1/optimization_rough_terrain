#!/usr/bin/env python
from os.path import expanduser
import rospy
from sensor_msgs.msg import Imu
from huskyterrain.msg import imutrans
from huskyterrain.msg import imutransArray
import tf
import math
import numpy as np
import yaml
def callback(msg):
	rospy.loginfo("start")
	traversability =[0,0,0,0,0]
	array = []
	rospy.loginfo("the data is coming ")
	for i in msg.data:
		x = np.array([i.angular_acceleration.x, i.angular_acceleration.y, i.pose.orientation.x,i.pose.orientation.y,9.8])
		array.append(x)
	rospy.loginfo(len(array))
	if (len(array) != 0):
		ex_ = sum(array)/ float(len(array))
		for i in range(len(array)):
			traversability += (ex_ - array[i])**2
	del array[:]
	
	rospy.loginfo(str(msg.data[0].desx)+" "+str(msg.data[0].desy) + " "+str(traversability))
	
	
	#data = dict(
	#		str(traversability)
	#)
	Mylist = [msg.data[0].desx, msg.data[0].desy,traversability]
	home = expanduser("~")
	with open(home+'/catkin_ws/src/huskyterrain/traversability_sample.yaml', 'a') as outfile:
    		#yaml.dump(data, outfile, default_flow_style=False)
		outfile.write(str(Mylist)+'\n')
		#pickle.dump(traversability, outfile)
	
if __name__ == '__main__':
  rospy.init_node('traversability_write', anonymous=True)  
  rospy.Subscriber('duckiemap_ground_truth', imutransArray, callback)
  rospy.spin()
