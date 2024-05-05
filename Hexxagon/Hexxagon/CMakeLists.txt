cmake_minimum_required(VERSION 3.24)
project(Hexxagon)
set(BUILD_SHARED_LIBS FALSE)
include(FetchContent)

FETCHCONTENT_DECLARE(SFML
        GIT_REPOSITORY
        https://github.com/SFML/SFML.git)

FETCHCONTENT_MAKEAVAILABLE(SFML)
add_executable(Hexxagon main.cpp)
add_executable(Hexxagon1 pomoc.cpp)
add_executable(Hexxagon2 pomoc2.cpp)
target_link_libraries(Hexxagon
        sfml-system sfml-window sfml-graphics)
target_link_libraries(Hexxagon1
        sfml-system sfml-window sfml-graphics)
target_link_libraries(Hexxagon2
        sfml-system sfml-window sfml-graphics)