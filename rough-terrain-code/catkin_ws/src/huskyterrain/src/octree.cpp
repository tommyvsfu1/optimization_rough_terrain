#include	<ros/ros.h>
#include	<pcl/point_cloud.h>
#include	<pcl_conversions/pcl_conversions.h>
#include	<sensor_msgs/PointCloud2.h>
#include	<pcl/octree/octree.h>
#include <visualization_msgs/Marker.h>
#include <huskyterrain/view_poly.h>
#include <iostream>
int count = 0;
float x,y,z;
float thre = 0.01;
class	cloudHandler
{
public:
				cloudHandler()
				{
								pcl_sub	=	nh.subscribe("/pclprocessing2",1,&cloudHandler::cloudCB,this);
								pcl_pub	=	nh.advertise<sensor_msgs::PointCloud2>("pcl_partitioned",	1);
								vis_pub = nh.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );
				}
				void	cloudCB(const	huskyterrain::view_poly	&input)
				{

								sensor_msgs::PointCloud2 output;
								pcl::PointCloud<pcl::PointXYZ>	cloud_partitioned;
								visualization_msgs::Marker marker;

								for (int i = 0; i < 25; i++) {
								pcl::PointCloud<pcl::PointXYZ>	cloud;
								
								
								pcl::fromROSMsg(input.pointcloud,	cloud);
								float	resolution	=	256.0f;
								pcl::octree::OctreePointCloudSearch<pcl::PointXYZ>	octree	(resolution);
								octree.setInputCloud	(cloud.makeShared());
								octree.addPointsFromInputCloud	();
								pcl::PointXYZ	center_point;
								center_point.z	=	0.0;
								float	radius	=	0.3;
								std::vector<int>	radiusIdx;
								std::vector<float>	radiusSQDist;
								center_point.x	 = input.coorx[i];
								center_point.y	=   input.coory[i];
								std::cout<<i << " ";
								std::cout << center_point.x<< " ";
								std::cout << center_point.y<< std::endl;

								marker.header.frame_id = "odom";
								marker.header.stamp = ros::Time();
								marker.ns = "my_namespace";
								marker.id = input.id[i];
								marker.type = visualization_msgs::Marker::CUBE;
								marker.action = visualization_msgs::Marker::ADD;
								marker.pose.position.x = center_point.x;
								marker.pose.position.y = center_point.y;
								marker.pose.position.z = center_point.z;
								marker.pose.orientation.x = 0.0;
								marker.pose.orientation.y = 0.0;
								marker.pose.orientation.z = 0.0;
							        marker.pose.orientation.w = 1.0;
								marker.scale.x = 1;
								marker.scale.y = 1;
								marker.scale.z = 0.0;
								marker.color.a = 1.0; // Don't forget to set the alpha!
								marker.color.r = 0.0;
								marker.color.g = 0.9;
								marker.color.b = 0.9;
								//only if using a MESH_RESOURCE marker type:
							//	marker.mesh_resource = "package://pr2_description/meshes/base_v0/base.dae";
								//vis_pub.publish( marker );

								if	((octree.radiusSearch	(center_point,	radius,	radiusIdx,	radiusSQDist)	>	0  ))
								{
												for	(size_t	i	=	0;	i	<	radiusIdx.size	();	++i)
												{
																cloud_partitioned.points.push_back(cloud.points[radiusIdx[i]]);
												}
										//if (x != center_point.x) {
										//marker.id = ++count;
										vis_pub.publish( marker );
										
								}
								x = center_point.x;
								y = center_point.y;
								z = center_point.z;
								//count++;
								}
								pcl::toROSMsg(cloud_partitioned,	output);
								output.header.frame_id	=	"odom";
								pcl_pub.publish(output);
				}
protected:
				ros::NodeHandle	nh;
				ros::Subscriber	pcl_sub;
				ros::Publisher	pcl_pub;
				ros::Publisher vis_pub;
};
main(int	argc,	char	**argv)
{
				ros::init(argc,	argv,	"pcl_partitioning");
				cloudHandler	handler;
				ros::spin();
				return	0;
}
