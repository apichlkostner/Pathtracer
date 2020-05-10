cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(png++ REQUIRED)

# add the executable
add_executable(raytracing main.cpp sphere.cpp hittable_list.cpp vec3.cpp)

target_include_directories(raytracing PRIVATE ${PROJECT_SOURCE_DIR}/include
                                PUBLIC "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(raytracing png16)

# add the install targets
install(TARGETS raytracing DESTINATION bin)
