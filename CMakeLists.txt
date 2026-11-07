cmake_minimum_required(VERSION 3.21)

project(MirzArchive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(MirzArchive
    src/main.cpp
    src/MainWindow.cpp
)

target_include_directories(MirzArchive PRIVATE include)

target_link_libraries(MirzArchive PRIVATE Qt6::Widgets)
