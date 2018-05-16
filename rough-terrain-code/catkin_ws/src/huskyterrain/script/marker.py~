#!/usr/bin/env python
import rospy
from huskyterrain.msg import imutrans
from visualization_msgs.msg import *
import numpy as np
import math
from numpy import linalg as LA
import threading
class cal():
	def __init__(self):
		self.travesebility = []
		self.C = 0
		self.count = 0
		self.qq = 0
		# Thread
		self.t1 = threading.Thread(target=self.f1,name='t1')
		self.threadC = threading.Condition()	
		self.pub = rospy.Publisher('marker_ground', Marker, queue_size=10) 
		rospy.Subscriber('duckiemap_ground_truth', imutrans, self.callback)
	def callback(self,msg):
			'''
			self.threadC.acquire()
 			if (msg.coorx > 0):
				if (abs(msg.coorx) - abs(int(msg.coorx)) > 0.5) :
					msg.coorx= math.ceil(msg.coorx)
				else :
					msg.coorx=math.floor(msg.coorx)
			else :
				if (abs(msg.coorx) - abs(int(msg.coorx)) > 0.5) :
					msg.coorx=math.floor(msg.coorx)
				else :
					msg.coorx=math.ceil(msg.coorx)
 			if (msg.coory > 0):
				if (abs(msg.coory) - abs(int(msg.coory)) > 0.5) :
					msg.coory=math.ceil(msg.coory)
				else :
					msg.coory =math.floor(msg.coory)
			else :
				if (abs(msg.coory) - abs(int(msg.coory)) > 0.5) :
					msg.coory=math.floor(msg.coory)
				else :
					msg.coory =math.ceil(msg.coory)
			self.threadC.release()
			'''
			x = np.array([msg.angular_acceleration.x, msg.angular_acceleration.y, msg.pose.orientation.x,msg.pose.orientation.y,9.8])
	
			if (len(self.travesebility) <= 30):			
				self.travesebility.append(x)
			else :
					ex_ = sum(self.travesebility)/ float(len(self.travesebility))
					self.C = 0
					for i in range(len(self.travesebility)):
						self.C += (ex_ - self.travesebility[i])**2
					del self.travesebility[:]
					#print len(self.travesebility)
					#print("mark")

					marker = Marker()
					marker.header.frame_id = "odom"
					#marker.header.frame_id = "base_link" #base_link is just to show to Nick 11/7 project (usual to odom !!)
					marker.header.stamp = rospy.Time()
					marker.ns = "my_namespace"
					self.threadC.acquire()
					if (msg.coorx == 0 and msg.coory == 0):
						marker.id = 0
					else :
						#marker.id = int(str(int(msg.coorx))+str(int(msg.coory)))
						marker.id = self.qq
						self.qq +=1
					self.threadC.release()
					marker.type = marker.ARROW
					#marker.text = str(self.C)
					#print (LA.norm(self.C))
					marker.action = marker.ADD
					self.threadC.acquire()
					marker.pose.position.x = msg.coorx
					marker.pose.position.y = msg.coory
					#print(str(msg.coorx))
					#print(str(msg.coory))
					self.threadC.release()
					#marker.pose.position.x = 0.0
					#marker.pose.position.y = 0.0
					marker.pose.position.z = 0
					marker.pose.orientation.x = 0.0
					marker.pose.orientation.y = -1.0
					marker.pose.orientation.z = 0.0
					marker.pose.orientation.w = 1.0
					marker.scale.x = 1.0
					marker.scale.y = 0.1
					marker.scale.z = 0.1
					marker.color.a = 1.0 #// Don't forget to set the alpha!
					marker.color.r = LA.norm(self.C)/float(0.001)
					print LA.norm(self.C)
					#print marker.color.r
					marker.color.g = 0.0
					marker.color.b = 0.0
					self.pub.publish( marker );	
	def f1(self):
		self.threadC.acquire()
		self.threadC.wait()
		self.threadC.release()			
		 
	
 
if __name__ == '__main__':
  rospy.init_node('marker_ground_truth', anonymous=True)  
  kk = cal()
  rospy.spin();  

