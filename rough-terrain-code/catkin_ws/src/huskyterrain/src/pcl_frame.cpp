#define PCL_NO_PRECOMPILE
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



/*
struct PointTypeWithDescriptor
{
	PCL_ADD_POINT4D;
	PCL_ADD_RGB;
	int id;
	//static const int DESCRIPTOR_SIZE = 64;
	//float descriptor[DESCRIPTOR_SIZE];

	PointTypeWithDescriptor() {}

	PointTypeWithDescriptor(const PointTypeWithDescriptor& input)
	{
		this->x = input.x;
		this->y = input.y;
		this->z = input.z;
		this->rgb = input.rgb;
		this->id = input.id;
		//for(int i = 0; i < DESCRIPTOR_SIZE; ++i) { this->descriptor[i] = input.descriptor[i]; } // Ugly, as I was lazy
	}

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT (PointTypeWithDescriptor,
                                   (float, x, x)
                                   (float, y, y)
                                   (float, z, z)
                                   (float, rgb, rgb)
								   (int, id, id)
);
*/

struct PointTypeWithDescriptor
{
        PCL_ADD_POINT4D;
        PCL_ADD_RGB; //preferred way of adding a XYZRGB + padding
        int ID;
        float Stiffness;
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW ;//Make sure our new allocators are aligned
}EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT (PointTypeWithDescriptor,
               (float, x, x)
               (float, y, y)
               (float, z, z)
               (float, rgb, rgb)
               (int, ID, ID)
               (float, Stiffness, Stiffness)
)








struct pointCloud {
	pcl::PointCloud<pcl::PointXYZRGB> cloud;
	int id;
	int jd;
};

struct fuck {
	pcl::PointXYZRGB points;
	int i ;
	int j;
};
void view(pcl::PointCloud<pcl::PointXYZ> cloud) {
  pcl::PointCloud<pcl::PointXYZ> tmp;
  tmp.width = cloud.width;
  tmp.height = cloud.height;
  tmp.is_dense = cloud.is_dense;
  tmp.points.resize (tmp.width * tmp.height);
  for (size_t i = 0; i < cloud.points.size(); ++i) {
	if (cloud.points[i].z > 0.5) {
	     tmp.points[i].z = cloud.points[i].z;
	     tmp.points[i].x = cloud.points[i].x;
	     tmp.points[i].y = cloud.points[i].y;
	}
  }
  //viewer.showCloud(tmp);
} 



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

		pointCloud tt;
  	        tt.cloud.width = cloud.width;
  	        tt.cloud.height = cloud.height;
  	        tt.cloud.is_dense = cloud.is_dense;
  	        tt.cloud.points.resize (tt.cloud.width * tt.cloud.height);
  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
		     //if (cloud.points[i].z > 0.8 && cloud.points[i].z < 1.8) {
			// if (cloud.points[i].x > 0.0 && cloud.points[i].x < 0.5) {
	     	      		tt.cloud.points[i].z = cloud.points[i].z;
	     	      		tt.cloud.points[i].x = cloud.points[i].x;
	     	      		tt.cloud.points[i].y = cloud.points[i].y-offset;
	     	      		tt.cloud.points[i].rgb = cloud.points[i].rgb;
	     	      		//tt.points[i].rgb[1] = cloud.points[i].rgb[1];
	     	      		//tt.points[i].rgb[2] = cloud.points[i].rgb[2];
				//std::cout << tt.cloud.points[i].y << std::endl;
			 //}
		     //}
  	        }

  		Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();

  		// Define a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)
  		float theta = M_PI/4; // The angle of rotation in radians
  		transform_1 (1,1) = sin (-0.523);
  		transform_1 (1,2) = cos (-0.523);
  		transform_1 (2,1) = cos (-0.523);
  		transform_1 (2,2) = -sin (-0.523);
  		//    (row, column)

  		// Print the transformation
  		//printf ("Method #1: using a Matrix4f\n");
  		//std::cout << transform_1 << std::endl;


  	        //pcl::PointCloud<pcl::PointXYZRGB> tmp;
		pointCloud tmp;
  	        tmp.cloud.width = cloud.width;
  	        tmp.cloud.height = cloud.height;
  	        tmp.cloud.is_dense = cloud.is_dense;
  	        tmp.cloud.points.resize (tmp.cloud.width * tmp.cloud.height);
		Eigen::Quaterniond q(0,0.0,-0.25852,0.966);
		Eigen::Isometry3d T(q);



  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
		     //if (cloud.points[i].z > 2 && cloud.points[i].z < 3) {
			// if (cloud.points[i].y > 3.0 && cloud.points[i].y < 4.0 ) {
				Eigen::Vector3d point;
				point[2] = tt.cloud.points[i].z;
				point[1] = tt.cloud.points[i].y;
				point[0] = tt.cloud.points[i].x;
				Eigen::Vector3d pointWorld = T * point;
				
	     	      		tmp.cloud.points[i].x = pointWorld[0];
	     	      		tmp.cloud.points[i].y = pointWorld[1];
	     	      		tmp.cloud.points[i].z = pointWorld[2];
	     	      		tmp.cloud.points[i].rgb = cloud.points[i].rgb;
	     	      		//tt.points[i].rgb[1] = cloud.points[i].rgb[1];
	     	      		//tt.points[i].rgb[2] = cloud.points[i].rgb[2];
				//std::cout << tt.cloud.points[i].y << std::endl;
			// }
		     //}
		}


		pcl::PointCloud<pcl::PointXYZRGB> t;
		//for (int i = 0; i < 4; i++) {
		t.width = cloud.width;
		t.height = cloud.height;
		t.is_dense = cloud.is_dense;
		t.points.resize(t.width * t.height);			
		//}
		


		// Ideal point cloud divison into patch, array implementation
		const double Kinect_view_Front_cutoff = 1.8;
		const double Kinect_view_Rear_cutoff = 8;
		const double Define_grid = 0.9;
		pcl::PointCloud<pcl::PointXYZRGB>* ID[6];
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
  	        for (size_t i = 0; i < cloud.points.size(); ++i) {
		     if (tmp.cloud.points[i].z > (1+0.8) ) {
			if (tmp.cloud.points[i].x > -0.9 && tmp.cloud.points[i].x < 0.9) {
				t.points[i].x = tmp.cloud.points[i].x;
				t.points[i].y = tmp.cloud.points[i].y;
				t.points[i].z = tmp.cloud.points[i].z;
				t.points[i].rgb = tmp.cloud.points[i].rgb;
				
			 }
		     }
/*
		     if (tmp.cloud.points[i].z > (3+0.5) && tmp.cloud.points[i].z < (4+0.5)) {
			 if (tmp.cloud.points[i].x > -0.5 && tmp.cloud.points[i].x < 0.5 ) {
				t.points[i].x = tmp.cloud.points[i].x;
				t.points[i].y = tmp.cloud.points[i].y;
				t.points[i].z = tmp.cloud.points[i].z;
				t.points[i].rgb = tmp.cloud.points[i].rgb;
				
			 }
		     }
		     if (tmp.cloud.points[i].z > (5+0.5) && tmp.cloud.points[i].z < (6+0.5)) {
			 if (tmp.cloud.points[i].x > -0.5 && tmp.cloud.points[i].x < 0.5 ) {
				t.points[i].x = tmp.cloud.points[i].x;
				t.points[i].y = tmp.cloud.points[i].y;
				t.points[i].z = tmp.cloud.points[i].z;
				t.points[i].rgb = tmp.cloud.points[i].rgb;
				
			 }
		     }
		     if (tmp.cloud.points[i].z > (7+0.5) && tmp.cloud.points[i].z < (8+0.5)) {
			 if (tmp.cloud.points[i].x > -0.5 && tmp.cloud.points[i].x < 0.5 ) {
				t.points[i].x = tmp.cloud.points[i].x;
				t.points[i].y = tmp.cloud.points[i].y;
				t.points[i].z = tmp.cloud.points[i].z;
				t.points[i].rgb = tmp.cloud.points[i].rgb;
				
			 }
		     }
*/
		}



		viewer.showCloud(ID[0][1].makeShared());
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
	ros::Timer	viewer_timer;
};
int main(int argc,char **argv)
{
	ros::init(argc,	argv,	"pcl_frame");
	cloudHandler handler;
	ros::spin();
	return	0;
}
