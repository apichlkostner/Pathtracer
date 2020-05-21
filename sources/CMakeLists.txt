cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(png++)

if (png++_FOUND)
     add_definitions(-DUSE_PNG)
     message(INFO " Using png++")
else()
     message(INFO " Using P3")
endif()

# add the executable
add_executable(raytracing main.cpp sphere.cpp hittable_list.cpp vec3.cpp rtweekend.cpp)

target_include_directories(raytracing PRIVATE ${PROJECT_SOURCE_DIR}/include
                                PUBLIC "${PROJECT_BINARY_DIR}"
                           )
# amd math library
set(AMD_MATHLIB "/usr/local/lib/libamdlibm.so")
if(EXISTS ${AMD_MATHLIB})
     set(ENV{LD_PRELOAD} ${AMD_MATHLIB})
     target_link_libraries(raytracing amdlibm)
endif()

if (png++_FOUND)
     target_link_libraries(raytracing png16)
endif()

target_link_libraries(raytracing pthread)

# add the install targets
install(TARGETS raytracing DESTINATION bin)
