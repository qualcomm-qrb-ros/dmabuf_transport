cmake_minimum_required(VERSION 3.8)
project(dmabuf_transport)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -std=c++17)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/type/image.cpp
  src/type/point_cloud2.cpp
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)

  ament_auto_add_gtest(${PROJECT_NAME}_image_test_pub
    test/image_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_image_test_pub PRIVATE "-DTEST_PUBLISHER")

  ament_auto_add_gtest(${PROJECT_NAME}_image_test_sub
    test/image_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_image_test_sub PRIVATE "-DTEST_SUBSCRIBER")

  ament_auto_add_gtest(${PROJECT_NAME}_point_cloud2_test_pub
    test/point_cloud2_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_point_cloud2_test_pub PRIVATE "-DTEST_PUBLISHER")

  ament_auto_add_gtest(${PROJECT_NAME}_point_cloud2_test_sub
    test/point_cloud2_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_point_cloud2_test_sub PRIVATE "-DTEST_SUBSCRIBER")

endif()

ament_auto_package()
