cmake_minimum_required(VERSION 2.8.3)
project(lcd_node)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rospy std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  include ${catkin_INCLUDE_DIRS}
)

add_executable(lcd_node src/lcd_i2c.cpp src/lcd_node.cpp)
target_link_libraries(lcd_node ${catkin_LIBRARIES})
