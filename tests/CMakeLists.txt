
add_executable(ecs_tests tests_main.cpp src/user_type.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(ecs_tests PRIVATE -stdlib=libc++)
    target_link_options(ecs_tests PRIVATE -stdlib=libc++ -lc++abi)
endif()

target_include_directories(ecs_tests
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/tests_include"
)

target_link_libraries(ecs_tests PRIVATE ecs)