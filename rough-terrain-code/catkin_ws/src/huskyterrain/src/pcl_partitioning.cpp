#include	<ros/ros.h>
#include	<pcl/point_cloud.h>
#include	<pcl_conversions/pcl_conversions.h>
#include	<sensor_msgs/PointCloud2.h>
#include	<pcl/octree/octree.h>
#include "huskyterrain/view_poly.h"
class	cloudHandler
{
public:
				cloudHandler()
				{
								pcl_sub	=	nh.subscribe("/cluster",	10,	&cloudHandler::cloudCB,	
this);
								pcl_pub	=	nh.advertise<sensor_msgs::PointCloud2>("pcl_partitioned",	1);
				}
				void	cloudCB(const	huskyterrain::view_poly	&input)
				{
								pcl::PointCloud<pcl::PointXYZ>	cloud;
								pcl::PointCloud<pcl::PointXYZ>	cloud_partitioned;
								sensor_msgs::PointCloud2	output;
								pcl::fromROSMsg(input.pointcloud,cloud);
								float	resolution	=	128.0f;
								pcl::octree::OctreePointCloudSearch<pcl::PointXYZ>	octree	(resolution);
								octree.setInputCloud	(cloud.makeShared());
								octree.addPointsFromInputCloud	();
								pcl::PointXYZ	center_point;
								center_point.x	=	3.0	;
								center_point.y	=	1.0;
								center_point.z	=	0.0;
								float	radius	=	0.3;								std::vector<int>	radiusIdx;
								std::vector<float>	radiusSQDist;
								if	(octree.radiusSearch	(center_point,	radius,	radiusIdx,	radiusSQDist)	
>	0)
								{
												for	(size_t	i	=	0;	i	<	radiusIdx.size	();	++i)
												{
																cloud_partitioned.points.push_back	
(cloud.points[radiusIdx[i]]);
												}
								}
								pcl::toROSMsg(cloud_partitioned,	output);
								output.header.frame_id	=	"odom";
								pcl_pub.publish(output);
				}
protected:
				ros::NodeHandle	nh;
				ros::Subscriber	pcl_sub;
				ros::Publisher	pcl_pub;
};
main(int	argc,	char	**argv)
{
				ros::init(argc,	argv,	"pcl_partitioning");
				cloudHandler	handler;
				ros::spin();
				return	0;
}
