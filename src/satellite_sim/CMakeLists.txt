cmake_minimum_required(VERSION 3.16)

project(satellit_sim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort Core)
qt_standard_project_setup()     # Will still link against Qt6::Core and Qt6::Gui since Widgets depends on them

# Download nlohmann::json fresh content from source 
include(FetchContent)

# TODO: Fix the warning that is generated when the json library is downloaded
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# GoogleTest
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)

qt_add_executable(satellite_sim
    mainwindow.cpp
    serial_port.cpp
    main.cpp
    q_satellite_factory.cpp
    q_threads.cpp
    ../common/utc_data.cpp
    ../data_preprocessing/json_conversion.cpp
    ../satellite-telemetry-simulator/pico_satellite_factory.cpp
)

target_include_directories(satellite_sim 
                            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/../common
                            ${CMAKE_CURRENT_SOURCE_DIR}/../data_preprocessing
                            ${CMAKE_CURRENT_SOURCE_DIR}/../satellite-telemetry-simulator)

target_link_libraries(satellite_sim 
                        PRIVATE Qt6::Widgets 
                        Qt6::SerialPort 
                        Qt6::Core 
                        PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(satellite_sim PROPERTIES
    MACOSX_BUNDLE ON 
)   