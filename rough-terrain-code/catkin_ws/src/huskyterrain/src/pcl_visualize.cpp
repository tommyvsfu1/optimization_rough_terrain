#include <iostream>
#include <ros/ros.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <Eigen/Geometry>
#include <pcl/point_types.h>
#include <pcl/impl/instantiate.hpp>

float offset = 0.88727;

class	cloudHandler
{
public:
	cloudHandler() :viewer("Cloud	Viewer")
	{
		pcl_sub	=	nh.subscribe("/camera/depth/points",	10,	&cloudHandler::cloudCB,	this);
		viewer_timer	=	nh.createTimer(ros::Duration(0.1),&cloudHandler::timerCB,this);
	}
	void	cloudCB(const	sensor_msgs::PointCloud2	&input)
	{
		pcl::PointCloud<pcl::PointXYZRGB>	cloud;
		pcl::fromROSMsg(input,	cloud);

		pcl::PointCloud<pcl::PointXYZRGB> tt;
  	        tt.width = cloud.width;
  	        tt.height = cloud.height;
  	        tt.is_dense = cloud.is_dense;
  	        tt.points.resize (tt.width * tt.height);
  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
	     	      		tt.points[i].z = cloud.points[i].z;
	     	      		tt.points[i].x = cloud.points[i].x;
	     	      		tt.points[i].y = cloud.points[i].y-offset;
	     	      		tt.points[i].rgb = cloud.points[i].rgb;
  	        }
		
  		// Transformation
  	        pcl::PointCloud<pcl::PointXYZRGB> tmp;
  	        tmp.width = cloud.width;
  	        tmp.height = cloud.height;
  	        tmp.is_dense = cloud.is_dense;
  	        tmp.points.resize (tmp.width * tmp.height);
		Eigen::Quaterniond q(0,0.0,-0.25852,0.966); // theta = 0.523(RADIANS)
		Eigen::Isometry3d T(q);



  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
				Eigen::Vector3d point;
				point[2] = tt.points[i].z;
				point[1] = tt.points[i].y;
				point[0] = tt.points[i].x;
				Eigen::Vector3d pointWorld = T * point;
				
	     	      		tmp.points[i].x = pointWorld[0];
	     	      		tmp.points[i].y = pointWorld[1];
	     	      		tmp.points[i].z = pointWorld[2];
	     	      		tmp.points[i].rgb = cloud.points[i].rgb;
		}


		pcl::PointCloud<pcl::PointXYZRGB> t;
		for (int i = 0; i < 4; i++) {
		t.width = cloud.width;
		t.height = cloud.height;
		t.is_dense = cloud.is_dense;
		t.points.resize(t.width * t.height);			
		}
		


		//Point cloud divison into patch, array implementation
		const double Kinect_view_Front_cutoff = 1.9;
		const double Kinect_view_Rear_cutoff = 8;
		const double Define_grid = 0.9;
		pcl::PointCloud<pcl::PointXYZRGB>* ID[6];
/*
		for (double distance = Kinect_view_Front_cutoff+0.9; distance < Kinect_view_Rear_cutoff; distance += Define_grid) {
			const int NumOFPatch = 2 * int((distance-0.9) / (std::sqrt(3) * 0.9));
			ID[int((distance-Kinect_view_Front_cutoff-0.9)/0.9)] = new pcl::PointCloud<pcl::PointXYZRGB>[NumOFPatch];
			std::cout << "the iteration is" <<  (distance-Kinect_view_Front_cutoff-0.9)/0.9 << std::endl;
			std::cout << "number of patch" << NumOFPatch << std::endl;
			int i = (distance-Kinect_view_Front_cutoff-0.9)/0.9;
			std::cout << "i is " << i << std::endl;

			for (int j = 0; j < NumOFPatch; j++) {
				ID[i][j].width = tmp.cloud.width;
				ID[i][j].height = tmp.cloud.height;
				ID[i][j].is_dense = tmp.cloud.is_dense;
				ID[i][j].points.resize( ID[i][j].width * ID[i][j].height );
			
				// filter
				for (size_t k = 0; k < tmp.cloud.points.size(); ++k) {
					//std::cout << "distance " <<  distance << std::endl;
					if (tmp.cloud.points[k].z >= distance-0.9 && tmp.cloud.points[k].z <= distance) {
						// question:: 0.9*( -(NumOFPatch/2) +j is this correct?
						// question1:: the x offset problem? (I think is the camera problem)
						std::cout << (0.9*( -(NumOFPatch/2) +j)) << " " << (0.9*( -(NumOFPatch/2) +j+1 ) ) << std::endl;
						 if (tmp.cloud.points[k].x > (0.9*( -(NumOFPatch/2) +j ) ) && tmp.cloud.points[k].x < (0.9*( -(NumOFPatch/2) +j+1 ) ) ) {
								std::cout << "fuck" << std::endl;
	     	      						ID[i][j].points[k].x = tmp.cloud.points[k].x;
	     	      						ID[i][j].points[k].y = tmp.cloud.points[k].y;
	     	      						ID[i][j].points[k].z = tmp.cloud.points[k].z;
	     	      						ID[i][j].points[k].rgb = tmp.cloud.points[k].rgb;		
									
						}
					}					
				}
				std::string tommy = std::to_string(i);
				tommy += std::to_string(j);
				std::string name = tommy + "test.pcd";
				pcl::io::savePCDFileASCII (name, ID[i][j]);

			}
		}
*/
  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
		     if (tmp.points[i].z > (1+0.9) ) {
			if (tmp.points[i].x > -0.9 && tmp.points[i].x < 0.9) {
				t.points[i].x = tmp.points[i].x;
				t.points[i].y = tmp.points[i].y;
				t.points[i].z = tmp.points[i].z;
				t.points[i].rgb = tmp.points[i].rgb;
				
			 }
		     }


		     if (tmp.points[i].z > (3+0.5) && tmp.points[i].z < (4+0.5)) {
			 if (tmp.points[i].x > -0.5 && tmp.points[i].x < 0.5 ) {
				t.points[i].x = tmp.points[i].x;
				t.points[i].y = tmp.points[i].y;
				t.points[i].z = tmp.points[i].z;
				t.points[i].rgb = tmp.points[i].rgb;
				
			 }
		     }
		     if (tmp.points[i].z > (5+0.5) && tmp.points[i].z < (6+0.5)) {
			 if (tmp.points[i].x > -0.5 && tmp.points[i].x < 0.5 ) {
				t.points[i].x = tmp.points[i].x;
				t.points[i].y = tmp.points[i].y;
				t.points[i].z = tmp.points[i].z;
				t.points[i].rgb = tmp.points[i].rgb;
				
			 }
		     }
		     if (tmp.points[i].z > (7+0.5) && tmp.points[i].z < (8+0.5)) {
			 if (tmp.points[i].x > -0.5 && tmp.points[i].x < 0.5 ) {
				t.points[i].x = tmp.points[i].x;
				t.points[i].y = tmp.points[i].y;
				t.points[i].z = tmp.points[i].z;
				t.points[i].rgb = tmp.points[i].rgb;
				
			 }
		     }

		}



		viewer.showCloud(t.makeShared());
	}
	void	timerCB(const	ros::TimerEvent&)
	{
		if	(viewer.wasStopped())
		{
			ros::shutdown();
		}
	}
protected:
	ros::NodeHandle	nh;
	ros::Subscriber	pcl_sub;
	pcl::visualization::CloudViewer	viewer;
	ros::Timer viewer_timer;
};
int main(int argc,char **argv)
{
	ros::init(argc,	argv, "pcl_visualize");
	cloudHandler handler;
	ros::spin();
	return	0;
}
