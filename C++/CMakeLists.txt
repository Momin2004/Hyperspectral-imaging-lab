cmake_minimum_required(VERSION 3.10)

# Project name
project(HyperspectralImaging)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set paths for HALCON includes and libraries
set(HALCON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(HALCON_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/x64-win64")

# Include HALCON headers
include_directories(${HALCON_INCLUDE_DIR})

# Link HALCON libraries
link_directories(${HALCON_LIB_DIR})

# Add the executable and specify the source file
add_executable(easy easy_image.cpp) 
add_executable(medium medium_image.cpp)
add_executable(startup startup.cpp)
# add_executable(hard hard.cpp)

# Link HALCON libraries to the executable
target_link_libraries(easy halconcpp halcon)
target_link_libraries(medium halconcpp halcon)
# target_link_libraries(hard halconcpp halcon)
target_link_libraries(startup halconcpp halcon)