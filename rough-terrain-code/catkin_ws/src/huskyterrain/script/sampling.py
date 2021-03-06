#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Point
from gazebo_msgs.msg import ModelStates
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
import math
import threading
import tf
import numpy as np
from huskyterrain.msg import imutrans
from huskyterrain.msg import imutransArray
from std_msgs.msg import Bool
from std_msgs.msg import Int8
# Brings in the SimpleActionClient
import actionlib
from move_base_msgs.msg import MoveBaseAction
from huskyterrain.srv import *
# Brings in the messages used by the fibonacci action, including the
# goal message and the result message.
#import actionlib_tutorials.msg
class sampling():
	def __init__(self,x,y):
		#data member
		self.position = Odometry()
		#self.control = Twist()
		self.des = Pose()
		self.des.position.x = x
		self.des.position.y = y
		self.command = Pose()
		self.state = 0
		self.tmp = 0
		self.sensor = []
		self.count = 0

		# ground truth data
		self.r = 0.0
		self.p = 0.0
		self.y = 0.0
		self.accr = 0.0
		self.accp = 0.0
		self.accy = 0.0
		self.ground_truth = imutrans()
		self.ground = imutransArray()
		self.reach = False
		#Parameter
		self.coor = 0.0
		
		# Thread
		self.t1 = threading.Thread(target=self.f1,name='t1')
		self.t2 = threading.Thread(target=self.f2,name='t2')
		self.t3 = threading.Thread(target=self.f3,name='t3')
		self.count = 0
		self.threadC = threading.Condition()	
	
		# Pub/Sub
  		self.pub = rospy.Publisher('cmd_vel', Twist, queue_size = 1)
  		self.duckpub = rospy.Publisher('duckiemap_ground_truth', imutransArray, queue_size = 50)
  		rospy.Subscriber('/ground_truth/state', Odometry, self.cbtrans)
  		#rospy.Subscriber('destination', Point, self.cbdes)
  		rospy.Subscriber('reachgoal',Bool, self.cbreach)
  		rospy.Subscriber('reachgoal1',Bool, self.cbreach1)
		#rospy.Subscriber('/gazebo/model_states',ModelStates, self.cbtrans)

  		rospy.Subscriber('/imu/data', Imu, self.cbimu)
  		rospy.Subscriber('/angularaccerlation',Point, self.cbangular)
		#rospy.Subscriber('/odometry/filtered',Odometry,self.cbworld)
		#MoveBaseClient ac("move_base", true)
		#self.client = actionlib.SimpleActionClient('fibonac', actionlib_tutorials.msg.FibonacciAction)
		#self.client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
		# Thread start		
		self.t1.start()
		self.t2.start()
		# action wait
		#self.client.wait_for_server()

	
		#self.threadC.acquire() 


	def waitforAnswer(self):
    		rospy.wait_for_service('goal')
    		try:
        		wait = rospy.ServiceProxy('goal',reach)
        		resp1 = wait()
        		return resp1.answer
    		except rospy.ServiceException, e:
        		print "Service call failed: %s"%e
			return False
	def cbreach(self,msg):
			self.threadC.acquire()
			self.reach = msg.data
			self.threadC.release()
	def cbreach1(self,msg):
			self.threadC.acquire()
			self.reach = msg.data
			self.threadC.release()
	def cbdes(self,msg):		
			self.threadC.acquire()
			self.des.position.x = msg.x
			self.des.position.y = msg.y
			self.threadC.release()
	def cbtrans(self,msg):# Thread
			self.threadC.acquire()

			# First thought are Action and service , but I use parameter server after because it is not proper in this case
			#if self.client.get_state == "SUCCEEDED" :
			#	rospy.loginfo ("SUCCED")
			# get the gazebo world coordinate
			#if self.waitforAnswer():
			#
			
			self.ground_truth.coorx = msg.pose.pose.position.x
			self.ground_truth.coory = msg.pose.pose.position.y
			# calculate the distance
			#command.x is distance
			#command.y is theta
			if rospy.has_param('/goal') and rospy.has_param('/desx'): # if simple_goal node is on, then do sample
				#rospy.loginfo(rospy.get_param('/goal'))
					if str(rospy.get_param("/goal")) == "going":
						self.des.position.x = float(rospy.get_param("/desx"))
						self.des.position.y = float(rospy.get_param("/desy"))				
						self.command.position.x = math.sqrt( (msg.pose.pose.position.x - self.des.position.x) ** 2 + (msg.pose.pose.position.y - self.des.position.y) ** 2)
						self.command.position.y = math.atan2( (self.des.position.y - msg.pose.pose.position.y ) , (self.des.position.x - msg.pose.pose.position.x ) )
						if (self.command.position.x < 0.6):
						#rospy.loginfo(str(self.reach))
						#rospy.loginfo("the sensor is")
						#rospy.loginfo(str(self.y))
						#rospy.loginfo(str(type(self.ground.data)))
							self.ground_truth.coorx = msg.pose.pose.position.x
							self.ground_truth.coory = msg.pose.pose.position.y
							self.ground_truth.desx = self.des.position.x
							self.ground_truth.desy = self.des.position.y
							# fix the pointer issue
							##DO NOT JUST tt = self.ground_truth, it will end up with all same value
							##https://stackoverflow.com/questions/25437097/put-some-poses-in-the-list-in-ros
							tt = imutrans()
							tt.pose = self.ground_truth.pose
							tt.angular_acceleration = self.ground_truth.angular_acceleration
							tt.coorx = self.ground_truth.coorx
							tt.coory = self.ground_truth.coory
							tt.desx = self.ground_truth.desx
							tt.desy = self.ground_truth.desy		
							#rospy.loginfo(str(self.ground_truth.coorx))
							#rospy.loginfo("lengh is"+str(len(self.ground.data)))
							self.ground.data.append(tt)
							#rospy.loginfo(str(self.ground.data[len(self.ground.data)-1].coorx))
							#self.sensor.append(self.ground_truth)
					elif (str(rospy.get_param("/goal")) == "reaching") and (len(self.ground.data) != 0):
						print "sampleing down"
						#avg = float(sum(sensor))/len(sensor)
						#for i in range(len(sensor)):	
						self.ground.header.stamp = rospy.Time.now()
						self.ground.header.frame_id = "odom"
						#prevent the sychro
						tmp = imutransArray()
						tmp.data = self.ground.data
						tmp.header = self.ground.header
						gg = str(tmp.data[0].desx)
						gg1 = str(tmp.data[0].desy)
						rospy.loginfo("publish the destination"+gg+","+gg1)
						self.duckpub.publish(tmp)
						del self.ground.data[:]
						
					else : # Do nothing
						pass
 						


					# don't use math.atan(), i don't know why it can't work
					#print self.command.position.x
					#print self.command.position.y
			
			self.threadC.release()
	# This is put in the count polygon variable, you should know the whole polygon number in the map, or just to split it	
		
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
	def cbimu(self,msg):
		self.threadC.acquire()
		#self.r,self.p,self.y = tf.transformations.euler_from_quaternion([msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w])
		#print math.degrees(self.y)
		'''
		self.ground_truth.pose.orientation.x = msg.orientation.x
		self.ground_truth.pose.orientation.y = msg.orientation.y
		self.ground_truth.pose.orientation.z = msg.orientation.z
		self.ground_truth.pose.orientation.w = msg.orientation.w
		self.ground_truth.header = msg.header
		#print str(self.ground_truth.pose.orientation.x)
		'''
		self.ground_truth.pose = msg
		#self.ground_truth.header = msg.header
		self.threadC.release()
	def cbangular(self,msg):
		self.threadC.acquire()
		self.accr = msg.x
		self.accp = msg.y
		self.accy = msg.z
		self.ground_truth.angular_acceleration.x  = msg.x
		self.ground_truth.angular_acceleration.y  = msg.y
		self.ground_truth.angular_acceleration.z  = msg.z
		self.threadC.release()
		
		  
if __name__ == '__main__':
  rospy.init_node('sampling', anonymous=True)  
  foo = sampling(1,0)
  rospy.spin()
