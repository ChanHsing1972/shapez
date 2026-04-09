cmake_minimum_required(VERSION 3.10)
project(Shapez VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia REQUIRED)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE UI_FILES "ui/*.ui")

add_executable(Shapez 
    ${SRC_FILES} 
    ${HEADERS} 
    ${UI_FILES}
    assets.qrc
)

target_include_directories(Shapez PRIVATE src)

target_link_libraries(Shapez Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia)
