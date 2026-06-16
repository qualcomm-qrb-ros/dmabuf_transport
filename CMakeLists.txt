cmake_minimum_required(VERSION 3.8)
project(dmabuf_transport_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

# for end-to-end test
find_package(rclcpp_components REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED)
macro(add_test_component component_name class_name)
  ament_auto_add_library(${PROJECT_NAME}_${component_name}_component SHARED
    test/${component_name}.cpp
  )
  ament_target_dependencies(${PROJECT_NAME}_${component_name}_component
    rclcpp_components
    pcl_conversions
  )
  target_include_directories(${PROJECT_NAME}_${component_name}_component PUBLIC
    ${PCL_INCLUDE_DIRS}
  )
  target_link_libraries(${PROJECT_NAME}_${component_name}_component
    ${PCL_LIBRARIES}
  )
  rclcpp_components_register_nodes(${PROJECT_NAME}_${component_name}_component ${class_name})
endmacro(add_test_component)

add_test_component(test_pub "dmabuf_transport::TestPubComponent")
add_test_component(test_sub "dmabuf_transport::TestSubComponent")
add_test_component(test_dump "dmabuf_transport::TestDumpComponent")

ament_auto_package()
