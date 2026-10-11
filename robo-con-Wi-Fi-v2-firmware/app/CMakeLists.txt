cmake_minimum_required(VERSION 3.22)

enable_language(C CXX ASM)

add_library(robo-con STATIC
    src/robo-con.cpp
    src/printf.c
)

target_include_directories(robo-con PUBLIC
    include
)

target_compile_features(robo-con PUBLIC cxx_std_17)

target_link_libraries(robo-con PRIVATE stm32cubemx)
target_link_libraries(robo-con PRIVATE ThreadX)
target_link_libraries(robo-con PRIVATE NetXDuo)
target_link_libraries(robo-con PRIVATE Connectivity)
