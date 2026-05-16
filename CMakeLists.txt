cmake_minimum_required(VERSION 3.20)
project(ti84controller C)

# Point CLion at CEdev headers for code intelligence only
# This is never used to actually build anything
set(CEDEV "C:/CEdev")

add_executable(ti84controller src/main.c
        src/util.c
        src/util.h)

target_include_directories(ti84controller PRIVATE
        ${CEDEV}/include
        ${CEDEV}/include/ce
)

# Suppress CLion's compiler errors for TI-specific attributes
target_compile_options(ti84controller PRIVATE
        -w
)