cmake_minimum_required(VERSION 3.10)
project(AES C)

# Set C Standard
set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(include)

add_executable(a 
    src/main.c
    lib/utils/fileio.c
)

install(TARGETS a DESTINATION bin)