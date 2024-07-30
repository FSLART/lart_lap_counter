cmake_minimum_required(VERSION 3.8)
project(lap_counter)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(lart_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# local dependencies
set(SRC_FILES
    src/DataHolder.cpp
    src/GetCones.cpp
    src/GetPulse.cpp
    src/GetMission.cpp
    src/LapCounter.cpp
    src/Launcher.cpp
)

# executables here
add_executable(launch ${SRC_FILES})
ament_target_dependencies(launch rclcpp lart_msgs std_msgs)

# build executable
install(TARGETS
    launch
    DESTINATION lib/${PROJECT_NAME})

# include directories
target_include_directories(launch
    PRIVATE
        include
    PUBLIC
        ${rclcpp_INCLUDE_DIRS}
        ${lart_msgs_INCLUDE_DIRS}
        ${std_msgs_INCLUDE_DIRS}
)

# link libraries
target_link_libraries(launch
    ${rclcpp_LIBRARIES}
    ${lart_msgs_LIBRARIES}
    ${std_msgs_LIBRARIES}
)

ament_package()
