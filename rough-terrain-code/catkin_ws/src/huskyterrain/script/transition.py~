#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from huskyterrain.msg import imutrans
from huskyterrain.msg import pcl_polygon
from sensor_msgs.msg import PointCloud2
import threading


class dataMix():
	def __init__(self):
		self.data = pcl_polygon()
		# Thread
		self.t1 = threading.Thread(target=self.f1,name='t1')
		self.t2 = threading.Thread(target=self.f2,name='t2')
		#self.t3 = threading.Thread(target=self.f3,name='t3')
		self.count = 0
		self.threadC = threading.Condition()
		
		# Pub/Sub
		self.pub = rospy.Publisher('pclprocessing1', pcl_polygon, queue_size = 1)	
		rospy.Subscriber('cluster', PointCloud2, self.cbCloud)
		rospy.Subscriber('duckiemap_ground_truth',imutrans,self.cbimu)	
		self.t1.start()
		self.t2.start()
	def cbCloud(self,msg):
		self.threadC.acquire()
		self.data.pointcloud = msg  	
		self.pub.publish(self.data)	
		self.threadC.release()
	def cbimu(self,msg):
		self.threadC.acquire()
		self.data.header = msg.header
		self.data.pose = msg.pose
		self.data.angular_acceleration = msg.angular_acceleration
		self.data.coorx = msg.coorx
		self.data.coory = msg.coory
		self.data.id = msg.id
		self.data.sampling_rate = msg.sampling_rate
		self.threadC.release()	
	#def PUB(self):
	#	self.threadC.acquire()
	#	self.pub.publish(self.data)

	def f1(self):
		self.threadC.acquire()
		self.threadC.wait()
		self.threadC.release()
	def f2(self):
		self.threadC.acquire()
		self.threadC.wait()
		self.threadC.release()
 


if __name__ == '__main__':
  rospy.init_node('listener', anonymous=True)
  qq = dataMix()
  rospy.spin()
