cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(png++)
find_package (Eigen3 3.3 NO_MODULE)
find_package(OpenMP)

# add the executable
add_executable(raytracing main.cpp sphere.cpp hittable_list.cpp vec3.cpp rtweekend.cpp random_world.cpp material.cpp)

target_include_directories(raytracing PRIVATE ${PROJECT_SOURCE_DIR}/include
                                PUBLIC "${PROJECT_BINARY_DIR}"
                           )
# amd math library
set(AMD_MATHLIB "/usr/local/lib/libamdlibm.so")
if(EXISTS ${AMD_MATHLIB})
     set(ENV{LD_PRELOAD} ${AMD_MATHLIB})
     target_link_libraries(raytracing amdlibm)
endif()

if (Eigen3_FOUND)
     add_definitions(-DUSE_EIGEN)
     target_link_libraries(raytracing Eigen3::Eigen)
     message(INFO " Using Eigen library")
endif()

if (png++_FOUND)
     add_definitions(-DUSE_PNG)
     target_link_libraries(raytracing ${png++_LIBRARIES})
     message(INFO " Using png++")
else()
     message(INFO " Using P3")
endif()

if(OpenMP_CXX_FOUND)
    target_link_libraries(raytracing OpenMP::OpenMP_CXX)
endif()

target_link_libraries(raytracing pthread)

target_compile_options(raytracing PRIVATE -Wall -Wextra -Wpedantic -march=native -ffast-math)
#target_compile_options(raytracing PRIVATE $<$<CXX_COMPILER_ID:GNU>:-ffast-math>)

# add the install targets
install(TARGETS raytracing DESTINATION bin)
