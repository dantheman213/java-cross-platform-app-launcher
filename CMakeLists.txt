cmake_minimum_required(VERSION 3.10)
project(Launcher)
set(CMAKE_CXX_STANDARD 17)

set(EXTRA_ARGS "")
if(WIN32)
    set(EXTRA_ARGS "WIN32")
endif ()

add_executable(Launcher ${EXTRA_ARGS}
        main.cpp
        JavaLauncher.cpp
        JavaLauncher.h
        MessageBox.cpp
        MessageBox.h

)

target_link_libraries(Launcher)
