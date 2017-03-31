cmake_minimum_required(VERSION 2.8.7)

project(bob_perception)
###############################################################################
## Options
option( DEBUG_ACTIVE "Enable Debug build" ON )

if(DEBUG_ACTIVE)
    MESSAGE( "Debug compilation active" )
    set(ROS_BUILD_TYPE Debug)
    set(CMAKE_BUILD_TYPE Debug)
else()
    MESSAGE( "Release compilation active" )
    set(ROS_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Release)
endif()
###############################################################################

find_package(OpenCV 2.4 COMPONENTS core highgui imgproc REQUIRED)
find_package(PCL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system signals)
find_package(catkin REQUIRED COMPONENTS
  image_transport
  roscpp
  rosconsole
  sensor_msgs
  cv_bridge
  dynamic_reconfigure
  laser_geometry
)




catkin_package(
  CATKIN_DEPENDS
    roscpp
    rosconsole
    sensor_msgs
    opencv2
    cv_bridge
    image_transport
    dynamic_reconfigure
)

###############################################################################
# INCLUDES

# Specify locations of header files.
include_directories(
	${catkin_INCLUDE_DIRS} 
	${OpenCV_INCLUDE_DIRS}
	${PCL_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

link_directories(${PCL_LIBRARY_DIRS})

###############################################################################

###############################################################################
# EXECUTABLE

add_definitions(-std=c++11)# -m64) #-Wall)


add_executable(
  pointcloud2image_node
  src/pointcloud2image.cpp
)


target_link_libraries(
  pointcloud2image_node
        ${catkin_LIBRARIES}
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}  
    )

add_executable(
  ros_pc2_node
  src/ros_pc2.cpp
)


target_link_libraries(
  ros_pc2_node
        ${catkin_LIBRARIES}
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}  
    )

add_executable(
  paint_border_node
  src/paint_line.cpp
)
target_link_libraries(
  paint_border_node
        ${catkin_LIBRARIES}
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}  
    )


add_executable(
  pointcloud_sub_node
  src/pointcloud_sub.cpp
)
target_link_libraries(
  pointcloud_sub_node
        ${catkin_LIBRARIES}
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}  
)



#add_executable(
#  laser_to_pointcloud_node
#  src/laserscan_to_pointcloud.cpp
#)
#target_link_libraries(
#  laser_to_pointcloud_node
#        ${catkin_LIBRARIES}
#        ${OpenCV_LIBS}
#        ${PCL_LIBRARIES}  
#)

#add_dependencies(laser_to_pointcloud_node ${PROJECT_NAME}_gencpp -lboost_system)




#add_dependencies(zed_wrapper_node ${PROJECT_NAME}_gencfg)
###############################################################################

#Add all files in subdirectories of the project in
# a dummy_target so qtcreator have access to all files
FILE(GLOB_RECURSE extra_files ${CMAKE_SOURCE_DIR}/*)
add_custom_target(dummy_${PROJECT_NAME} SOURCES ${extra_files})
