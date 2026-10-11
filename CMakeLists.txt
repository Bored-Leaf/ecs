cmake_minimum_required(VERSION 3.21.0)
project(JobScheduler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB source_files "src/*.cpp")

add_library(ecs STATIC ${source_files})

target_include_directories(ecs
    PUBLIC
    include
)

set_target_properties(ecs PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_features(ecs PUBLIC cxx_std_23)

option(ecs_build_tests "Build Jobscheduler tests" ${PROJECT_IS_TOP_LEVEL})
if (ecs_build_tests)
    add_subdirectory(tests)
endif()