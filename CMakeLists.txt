cmake_minimum_required(VERSION 3.12)

add_executable(check-servers
        test.c
        )

add_executable(interrupt
        interrupt.c
        )

add_compile_options(-Wall
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )


