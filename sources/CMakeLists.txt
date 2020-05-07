cmake_minimum_required(VERSION 3.5)

# add the executable
add_executable(raytracing main.cpp)

target_include_directories(raytracing PRIVATE ${PROJECT_SOURCE_DIR}/include
                                PUBLIC "${PROJECT_BINARY_DIR}"
                           )

# add the install targets
install(TARGETS raytracing DESTINATION bin)
