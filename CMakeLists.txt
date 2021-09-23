project(main)
set(CMAKE_CXX_STANDARD 11)
link_libraries(
  "${CMAKE_SOURCE_DIR}/../framework/libredgpu_f.so"
  "${CMAKE_SOURCE_DIR}/../framework/libcurl.so"
)
add_executable(main main.cpp)
