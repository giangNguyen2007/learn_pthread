cmake_minimum_required(VERSION 3.30)
project(learn_pthread_c C)

set(CMAKE_C_STANDARD 11)

add_executable(learn_pthread_c main.c
        )


add_executable(exe_simple_routine 1-simple_routie/main_simple.c)
add_executable(exe_bulletin_vote 2_exo_bulletin_vote/main_exo_bulletin_vote.c)
add_executable(exe_ressource_allocator 2_exo_ressource_allocateur/exo_ressource_allocator.c)
