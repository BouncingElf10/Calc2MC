cmake_minimum_required(VERSION 3.20)
project(ti84busman C)

# Point CLion at CEdev headers for code intelligence only
# This is never used to actually build anything
set(CEDEV "C:/CEdev")

add_executable(ti84busman src/main.c
        src/content.c
        src/content.h
        src/search.c
        src/search.h
)

target_include_directories(ti84busman PRIVATE
        ${CEDEV}/include
        ${CEDEV}/include/ce
)

# Suppress CLion's compiler errors for TI-specific attributes
target_compile_options(ti84busman PRIVATE
        -w
)