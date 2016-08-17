cmake_minimum_required(VERSION 2.8.3)
add_compile_options(-std=c++11)
project(ctrl_serial_comm)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)


## find yaml parser lib
include_directories("/usr/local/include/")
file(GLOB self_LIBRARIES /usr/local/lib/*.so)

catkin_package(
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(commWrite src/commSend.cxx src/commCtrl.cxx)
target_link_libraries(commWrite ${catkin_LIBRARIES} ${self_LIBRARIES})

add_executable(commRead src/commRecv.cxx src/commCtrl.cxx)
target_link_libraries(commRead ${catkin_LIBRARIES} ${self_LIBRARIES})
