cmake_minimum_required(VERSION 3.24)
project(EvoSnake C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

add_executable(EvoSnake
        main.c
        util.h
        evosnake.h evosnake.c)

target_link_libraries(EvoSnake ncurses)
