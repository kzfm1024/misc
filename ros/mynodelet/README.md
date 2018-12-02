# mynodelet

Simple example of ROS nodelet.

## How to run with sanitizer

1. Run nodelet manager
  * `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libasan.so.2 /opt/ros/kinetic/lib/nodelet/nodelet manager __name:=standalone_nodelet`

2. Load nodelet class to nodelet manager
  * `roslaunch mynodelet mynodelet_without_manager.launch`
