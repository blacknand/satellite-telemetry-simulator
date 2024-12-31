cmake_minimum_required(VERSION 3.16)

project(satellit_sim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort Core)
qt_standard_project_setup()     # Will still link against Qt6::Core and Qt6::Gui since Widgets depends on them

qt_add_executable(satellite_sim
    mainwindow.cpp
    serial_port.cpp
    main.cpp
)

target_link_libraries(satellite_sim PRIVATE Qt6::Widgets Qt6::SerialPort Qt6::Core)

set_target_properties(satellite_sim PROPERTIES
    MACOSX_BUNDLE ON
)