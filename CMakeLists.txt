
cmake_minimum_required(VERSION 3.10)
project(SimpleGuiFramework VERSION 1.0.0 LANGUAGES CXX)

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML (version 2.5 or higher); look for `graphics`, `window` and `system` modules
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Create settings
set(PROJECT_STATIC sgf-s)
set(PROJECT_SHARED sgf)
set(PROJECT_SOURCES
    src/SGF/Button.cpp
    src/SGF/Canvas.cpp
    src/SGF/Context.cpp
    src/SGF/FunctionGrapher.cpp
    src/SGF/FunctionParser.cpp
    src/SGF/FunctionProperties.cpp
    src/SGF/InputParser.cpp
    src/SGF/Rectangle.cpp
    src/SGF/ScrollView.cpp
    src/SGF/TextInput.cpp
    src/SGF/TextProperties.cpp
    src/SGF/VList.cpp
    src/SGF/VSlider.cpp)
    
# Make static library
add_library               (${PROJECT_STATIC} STATIC  ${PROJECT_SOURCES})
target_include_directories(${PROJECT_STATIC} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries     (${PROJECT_STATIC} sfml-graphics sfml-window sfml-system)

# Make shared library
add_library               (${PROJECT_SHARED} SHARED  ${PROJECT_SOURCES})
target_include_directories(${PROJECT_SHARED} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries     (${PROJECT_SHARED} sfml-graphics sfml-window sfml-system)
