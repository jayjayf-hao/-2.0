cmake_minimum_required(VERSION 3.10)
project(snakeII)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    core/GameLoop.cpp
    core/StateMachine.cpp
    systems/InputSystem.cpp
    systems/RenderSystem.cpp
    systems/TimeSystem.cpp
    gameplay/Snake.cpp
    gameplay/Food.cpp
    persistence/SaveSystem.cpp
)

add_executable(snakeII ${SOURCES})

target_include_directories(snakeII PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
