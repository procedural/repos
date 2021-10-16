# This CMakeLists.txt project is for Ubuntu only
project(main)
cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 11)
link_libraries(
  /opt/github/redgpu/framework/libredgpu_f.so
  /opt/github/redgpu/framework/libcurl.so
)
add_executable(main main.cpp)
