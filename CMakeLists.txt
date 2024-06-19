cmake_minimum_required(VERSION 3.12)

set(SERVER-EXECUTABLE "check-servers")
set(SYMLINK-EXECUTABLE "findsym")


add_executable(${SERVER-EXECUTABLE}
        check-servers.c
        )

add_executable(${SYMLINK-EXECUTABLE}
        findsym.c)

# Enable threads for server executable
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${SERVER-EXECUTABLE} PRIVATE Threads::Threads)

add_compile_options(-Wall
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )


