cmake_minimum_required(VERSION 3.10)
project(PO_1)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        main.cpp
        World.cpp
        Organism.cpp
        Animal.cpp
        Sheep.cpp
        Turtle.cpp
        Wolf.cpp
        Fox.cpp
        Antelope.cpp
        Human.cpp
        Guarana.cpp
        Dandelion.cpp
        Grass.cpp
        Hogweed.cpp
        Nightshade.cpp
        Plant.cpp
        load_config.cpp
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(PO_1 ${SOURCE_FILES})

target_link_libraries(PO_1 ${CURSES_LIBRARIES})