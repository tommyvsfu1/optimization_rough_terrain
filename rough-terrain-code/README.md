# rough-terrain-code  
**still under construction**  
**Instruction is not clear**  
This repository contains all the infrastructure   

## Dependency
### Software
**DO NOT USE VM**  
ros-indigo  
ubuntu 14.04  
gazebo 2    
**if you are using indigo it's deault version is 2**  
> test in gazebo 7 but it should be ok in 2   
> **if you are not using gazebo2 please modify depend.sh**   
```
problem=$(dpkg -s ros-indigo-gazebo**<your version>**-ros-control | grep installed)
```  
## Vitural Hardware
**One robot, husky**    
with **openni-kinect**, **imu (hector team)**, **velodyne(VLP-16)**    
>and we have another robot similar to husky is created by Monica, but still in issue now  
## System    
There are three main launch now 
```
huskyterrain/
            huskycontrol__sample.launch
            huskytraverse_all.launch  
            huskytraverse.launch
```

### huskycontrol.launch --- view all sensor data and world   
which can use keyboard control husky, and view all data(pointcloud, traversibility, laserscan, robotmodel) in rviz     
and it's also doing the sample if the robot is traversing terrain (huskytraverse__sample.launch will use)  
![](https://i.imgur.com/GERFz5B.png)  
### huskytraverse__all.launch --- A - B 
you can use 2D goal in rviz or run the simple node to let the robot move from A to B without map,  
![](https://i.imgur.com/pvlRT6j.png)  
**Another purpose for this launch is to let robot to traverse whole the terrain(set the sample manaully)**
### huskytraverse.launch --- sample the traversability 
based on previous launch , A-B  
write the traversability__sample.yaml  in
'~/rough-terrain-code/catkin__ws/src/huskyterrain/traversability__sample.yaml'  
if your file path is not correct it will cause error, I will fix it soon(just the python I/O)(or you can modify it in huskyterrain/traversability__write.py        

## How to run  
```
git clone git@github.com:ARG-NCTU/rough-terrain-code.git  
cd rough-terrain-code/
source depend.sh
source model.sh
cd catkin_ws/
catkin_make
source devel/setup.bash
```    
then run the three launch  
```  
roslaunch huskyterrain huskycontrol__sample.launch
roslaunch huskyterrain huskytraverse_all.launch  
roslaunch huskyterrain huskytraverse.launch
```  
### Terrain model  
all in huskyterrain/world
![](https://i.imgur.com/lYj43c1.png)  
```
gazebo vrc_terrain.world
gazebo small.world
gazebo small2.world
gazebo apollo15_landing_site.world
gazebo heightmap_dem.world
```
