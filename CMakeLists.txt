cmake_minimum_required(VERSION 3.12)

set(PROG_NAME "check-servers")

add_executable(${PROG_NAME}
        check.c
        )

# Enable threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROG_NAME} PRIVATE Threads::Threads)

add_compile_options(-Wall
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )


