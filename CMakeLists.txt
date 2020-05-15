cmake_minimum_required(VERSION 3.13)

project(libdisplay)

set(DISPLAY_SOURCE_FILES
    src/Display.cpp
    src/DisplayFactory.cpp
    src/GPIO.cpp
    src/I2C.cpp
    src/Port.cpp
    src/PortFactory.cpp
    src/sesp525.cpp
    src/SPI.cpp
)

set(MAIN_FILES
    test/main.cpp
)

include_directories(
    .
    internal
    /usr/local/include
)

add_library (display
    ${DISPLAY_SOURCE_FILES}
)

add_executable(lcd_test
    ${MAIN_FILES}
)

target_link_libraries(lcd_test
    pthread
    display
)