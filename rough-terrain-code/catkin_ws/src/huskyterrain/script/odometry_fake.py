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
		self.world.pose.pose.orientation.w = 1
		self.odomFilter = Odometry()
		# Thread
		self.t1 = threading.Thread(target=self.f1,name='t1')
		self.t2 = threading.Thread(target=self.f2,name='t2')
		self.t3 = threading.Thread(target=self.f3,name='t3')
		self.count = 0
		self.threadC = threading.Condition()	
		self.pub = rospy.Publisher('odom', Odometry, queue_size=50) 
    		#rospy.Subscriber('/odometry/filtered',Odometry,self.cbOdom)
    		rospy.Subscriber('/ground_truth/state',Odometry,self.cbfake)
		self.bool = True
		self.br = tf.TransformBroadcaster()
		#self.t1.start()
		#self.t2.start()

 	def cbfake(self,msg):
		self.threadC.acquire()
		self.world.pose.pose.position.x = msg.pose.pose.position.x
		self.world.pose.pose.position.y = msg.pose.pose.position.y
		self.world.pose.pose.position.z = msg.pose.pose.position.z
		self.world.pose.pose.orientation.x = msg.pose.pose.orientation.x
		self.world.pose.pose.orientation.y = msg.pose.pose.orientation.y
		self.world.pose.pose.orientation.z = msg.pose.pose.orientation.z
		self.world.pose.pose.orientation.w = msg.pose.pose.orientation.w	
		tmp1 = Odometry()	
		tmp1.child_frame_id = "base_link"
		tmp1.pose = msg.pose
		tmp1.twist = msg.twist 
		tmp1.header.frame_id = "odom"
		self.br.sendTransform((self.world.pose.pose.position.x,self.world.pose.pose.position.y,self.world.pose.pose.position.z)\
,(self.world.pose.pose.orientation.x,self.world.pose.pose.orientation.y,self.world.pose.pose.orientation.z,self.world.pose.pose.orientation.w),rospy.Time.now(),"base_link","odom")
		self.pub.publish(tmp1)
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
     rospy.init_node('odom_fake')
     foo = data_transfer()
     rospy.spin()

