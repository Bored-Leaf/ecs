
add_executable(ecs_tests main.cpp)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(ecs_tests PRIVATE -stdlib=libc++)
    target_link_options(ecs_tests PRIVATE -stdlib=libc++ -lc++abi)
endif()

target_link_libraries(ecs_tests PRIVATE ecs)