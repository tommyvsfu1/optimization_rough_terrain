#include <iostream>

#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <ros/ros.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
class	cloudHandler
{
public:
				cloudHandler() :viewer("Cloud	Viewer")
				{
					pcl_sub	=	nh.subscribe("/depth_registered/points",	10,	&cloudHandler::cloudCB,	this);
					viewer_timer	=	nh.createTimer(ros::Duration(0.1),&cloudHandler::timerCB,this);
				}
				void	cloudCB(const	sensor_msgs::PointCloud2	&input)
				{
					pcl::PointCloud<pcl::PointXYZ>	source_cloud;
					pcl::fromROSMsg(input,	source_cloud);

  /* Reminder: how transformation matrices work :

           |-------> This column is the translation
    | 1 0 0 x |  \
    | 0 1 0 y |   }-> The identity 3x3 matrix (no rotation) on the left
    | 0 0 1 z |  /
    | 0 0 0 1 |    -> We do not use this line (and it has to stay 0,0,0,1)

    METHOD #1: Using a Matrix4f
    This is the "manual" method, perfect to understand but error prone !
  */
  Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();

  // Define a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)
  float theta = M_PI/4; // The angle of rotation in radians
  transform_1 (0,0) = 0.0;
  transform_1 (0,1) = 0.0;
  transform_1 (0,2) = 1.0;

  transform_1 (1,0) = -1.0;
  transform_1 (1,1) = 0.0;
  transform_1 (1,2) = 0.0;

  transform_1 (2,0) = 0.0;
  transform_1 (2,1) = -1.0;
  transform_1 (2,2) = 0.0;

  //transform_1 (1,0) = sin (theta);
  //transform_1 (1,1) = cos (theta);
  //    (row, column)

  // Define a translation of 2.5 meters on the x axis.
  //transform_1 (0,3) = 2.5;

  // Print the transformation
  printf ("Method #1: using a Matrix4f\n");
  std::cout << transform_1 << std::endl;

  Eigen::Matrix4f transform_2 = Eigen::Matrix4f::Identity();

  // Define a rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)
  //float theta = M_PI/4; // The angle of rotation in radians
  transform_2 (0,0) = 1.0;
  transform_2 (0,1) = 0.0;
  transform_2 (0,2) = 0.0;

  transform_2 (1,0) = 0.0;
  transform_2 (1,1) = 0.0;
  transform_2 (1,2) = -1.0;

  transform_2 (2,0) = 0.0;
  transform_2 (2,1) = 1.0;
  transform_2 (2,2) = 0.0;

  //transform_1 (1,0) = sin (theta);
  //transform_1 (1,1) = cos (theta);
  //    (row, column)

  // Define a translation of 2.5 meters on the x axis.
  //transform_1 (0,3) = 2.5;

  // Print the transformation
  printf ("Method #1: using a Matrix4f\n");
  std::cout << transform_2 << std::endl;

  /*  METHOD #2: Using a Affine3f
    This method is easier and less error prone
  */
/*
  Eigen::Affine3f transform_2 = Eigen::Affine3f::Identity();

  // Define a translation of 2.5 meters on the x axis.
  transform_2.translation() << 2.5, 0.0, 0.0;

  // The same rotation matrix as before; theta radians arround Z axis
  transform_2.rotate (Eigen::AngleAxisf (theta, Eigen::Vector3f::UnitZ()));

  // Print the transformation
  printf ("\nMethod #2: using an Affine3f\n");
  std::cout << transform_2.matrix() << std::endl;
*/
  // Executing the transformation
  pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_cloud (new pcl::PointCloud<pcl::PointXYZ> ());
  // You can either apply transform_1 or transform_2; they are the same
  pcl::transformPointCloud (source_cloud, *transformed_cloud, transform_1);
  pcl::PointCloud<pcl::PointXYZ>::Ptr transformed_cloud1 (new pcl::PointCloud<pcl::PointXYZ> ());
  //pcl::transformPointCloud (*transformed_cloud, *transformed_cloud1, transform_2);
					//pcl::fromROSMsg(input,	cloud);
					viewer.showCloud((*transformed_cloud).makeShared());
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



// This function displays the help
void
showHelp(char * program_name)
{
  std::cout << std::endl;
  std::cout << "Usage: " << program_name << " cloud_filename.[pcd|ply]" << std::endl;
  std::cout << "-h:  Show this help." << std::endl;
}

// This is the main function
int
main (int argc, char** argv)
{
				ros::init	(argc,	argv,	"pcl_visualize");				cloudHandler	handler;
				ros::spin();
				return	0;
/*
  // Show help
  if (pcl::console::find_switch (argc, argv, "-h") || pcl::console::find_switch (argc, argv, "--help")) {
    showHelp (argv[0]);
    return 0;
  }

  // Fetch point cloud filename in arguments | Works with PCD and PLY files
  std::vector<int> filenames;
  bool file_is_pcd = false;

  filenames = pcl::console::parse_file_extension_argument (argc, argv, ".ply");

  if (filenames.size () != 1)  {
    filenames = pcl::console::parse_file_extension_argument (argc, argv, ".pcd");

    if (filenames.size () != 1) {
      showHelp (argv[0]);
      return -1;
    } else {
      file_is_pcd = true;
    }
  }

  // Load file | Works with PCD and PLY files
  pcl::PointCloud<pcl::PointXYZ>::Ptr source_cloud (new pcl::PointCloud<pcl::PointXYZ> ());

  if (file_is_pcd) {
    if (pcl::io::loadPCDFile (argv[filenames[0]], *source_cloud) < 0)  {
      std::cout << "Error loading point cloud " << argv[filenames[0]] << std::endl << std::endl;
      showHelp (argv[0]);
      return -1;
    }
  } else {
    if (pcl::io::loadPLYFile (argv[filenames[0]], *source_cloud) < 0)  {
      std::cout << "Error loading point cloud " << argv[filenames[0]] << std::endl << std::endl;
      showHelp (argv[0]);
      return -1;
    }
  }

/*
  // Visualization
  printf(  "\nPoint cloud colors :  white  = original point cloud\n"
      "                        red  = transformed point cloud\n");
  pcl::visualization::PCLVisualizer viewer ("Matrix transformation example");

   // Define R,G,B colors for the point cloud
  pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> source_cloud_color_handler (source_cloud, 255, 255, 255);
  // We add the point cloud to the viewer and pass the color handler
  viewer.addPointCloud (source_cloud, source_cloud_color_handler, "original_cloud");

  pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> transformed_cloud_color_handler (transformed_cloud, 230, 20, 20); // Red
  viewer.addPointCloud (transformed_cloud, transformed_cloud_color_handler, "transformed_cloud");

  viewer.addCoordinateSystem (1.0, "cloud", 0);
  viewer.setBackgroundColor(0.05, 0.05, 0.05, 0); // Setting background to a dark grey
  viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "original_cloud");
  viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "transformed_cloud");
  //viewer.setPosition(800, 400); // Setting visualiser window position

  while (!viewer.wasStopped ()) { // Display the visualiser until 'q' key is pressed
    viewer.spinOnce ();
  }
*/
 // return 0;
}

