#! bin/bash
sudo apt-get update
# IF YOU USE GAZEBO 7 DO NOT USE THIS!!!
problem=$(dpkg -s ros-indigo-gazebo-ros-control | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install ros-indigo-gazebo-ros-control. Setting up"
     sudo apt-get install ros-indigo-gazebo-ros-control 
fi

problem=$(dpkg -s ros-indigo-teleop-twist-keyboard | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install teleop-twist-keyboard . Setting up"
     sudo apt-get install ros-indigo-teleop-twist-keyboard 
fi

problem=$(dpkg -s ros-indigo-ros-control | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install ros-indigo-ros-control. Setting up"
     sudo apt-get install ros-indigo-ros-control
fi

problem=$(dpkg -s ros-indigo-ros-controllers | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install ros-indigo-ros-controllers. Setting up"
     sudo apt-get install ros-indigo-ros-controllers
fi

problem=$(dpkg -s  ros-indigo-pcl-msgs | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install  ros-indigo-pcl-msgs. Setting up"
     sudo apt-get install  ros-indigo-pcl-msgs
fi


problem=$(dpkg -s  ros-indigo-pcl-ros | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install  ros-indigo-pcl-ros. Setting up"
     sudo apt-get install ros-indigo-pcl-ros
fi

problem=$(dpkg -s  libpcl-all | grep installed)
echo Checking : $problem
if [ "" == "$problem" ]; then
     echo "Not install  libpcl-all. Setting up"
     sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-key 19274DEF
     sudo echo "deb http://ppa.launchpad.net/v-launchpad-jochen-sprickerhof-de/pcl/ubuntu maverick main" >> /etc/apt/sources.list	
     sudo apt-get install libpcl-all
fi



