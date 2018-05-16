#!/usr/bin/env python  
import roslib
import rospy
import tf
import math
from time import time, sleep
from sensor_msgs.msg import Imu
import tf
from sensor_msgs.msg import Image
from sensor_msgs.msg import CameraInfo
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped
import threading
class data_transfer():
	def __init__(self):
		
		self.world = Odometry()
		self.odomFilter = Odometry()
		# Thread
		self.t1 = threading.Thread(target=self.f1,name='t1')
		self.t2 = threading.Thread(target=self.f2,name='t2')
		self.t3 = threading.Thread(target=self.f3,name='t3')
		self.count = 0
		self.threadC = threading.Condition()	
		self.pub = rospy.Publisher('/set_pose', PoseWithCovarianceStamped, queue_size=50) 
    		rospy.Subscriber('/odometry/filtered',Odometry,self.cbOdom)
    		rospy.Subscriber('/ground_truth/state',Odometry,self.cbOdomWorld)
		self.bool = True
		#self.br = tf.TransformBroadcaster()
		#self.t1.start()
		#self.t2.start()
	def cbOdom(self,msg):
		self.threadC.acquire()
		self.odomFilter.header = msg.header
		self.odomFilter.child_frame_id = msg.child_frame_id
		self.odomFilter.pose.pose.position.x = msg.pose.pose.position.x
		self.odomFilter.pose.pose.position.y = msg.pose.pose.position.y
		self.odomFilter.pose.pose.position.z = msg.pose.pose.position.z
		self.odomFilter.pose.pose.orientation.x = msg.pose.pose.orientation.x
		self.odomFilter.pose.pose.orientation.y = msg.pose.pose.orientation.y
		self.odomFilter.pose.pose.orientation.z = msg.pose.pose.orientation.z
		self.odomFilter.pose.pose.orientation.w = msg.pose.pose.orientation.w
		self.odomFilter.pose.covariance = msg.pose.covariance
		self.odomFilter.twist = msg.twist
		rospy.loginfo(str(abs(self.odomFilter.pose.pose.position.x - self.world.pose.pose.position.x)))
		if (((self.world.pose.pose.position.x) -2.0) > 0.1) and self.bool  :
			self.fix()
			self.bool = False
			rospy.loginfo("enter fixing space")
	
		self.threadC.release()

 	def fix(self):
		#self.threadC.acquire()		
		tmp1 = PoseWithCovarianceStamped()
		tmp1.header = self.odomFilter.header
		tmp1.pose.pose.position.x = self.world.pose.pose.position.x
		tmp1.pose.pose.position.y = self.odomFilter.pose.pose.position.y
		tmp1.pose.pose.position.z = self.odomFilter.pose.pose.position.z
		tmp1.pose.pose.orientation.x = self.odomFilter.pose.pose.orientation.x
		tmp1.pose.pose.orientation.y = self.odomFilter.pose.pose.orientation.y
		tmp1.pose.pose.orientation.z = self.odomFilter.pose.pose.orientation.z
		tmp1.pose.pose.orientation.w = self.odomFilter.pose.pose.orientation.w
		tmp1.pose.covariance = self.odomFilter.pose.covariance
		#tmp1 = msg
		#tmp1.header.frame_id = "odom"
		#self.br.sendTransform((0.0,0.0,0.0),(0.0,0.0,0.0,1.0),rospy.Time.now(),"base_link","odom")
		self.pub.publish(tmp1)
					
	def cbOdomWorld(self,msg):
		self.threadC.acquire()
		self.world.pose.pose.position.x = msg.pose.pose.position.x
		self.world.pose.pose.position.y = msg.pose.pose.position.y
		self.world.pose.pose.position.z = msg.pose.pose.position.z
		self.threadC.release()				
	def f1(self):
		self.threadC.acquire()
		self.threadC.wait()
		self.threadC.release()
	def f2(self):
		self.threadC.acquire()
		self.threadC.wait()
		self.threadC.release()
	def f3(self):
		self.threadC.acquire()
		self.control()
		self.threadC.release()
		self.t3 = threading.Thread(target=self.f3,name='t3')
if __name__ == '__main__':
     rospy.init_node('data_transfer')
     foo = data_transfer()
     rospy.spin()

