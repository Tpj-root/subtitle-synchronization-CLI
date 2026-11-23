cmake_minimum_required(VERSION 3.16)

project(sub_synchronization
    VERSION 1.0
    LANGUAGES CXX
)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create executable
add_executable(sub_synchronization
    main.cpp
    SubtitleSynchronizer.cpp
)

# Include current directory for header files
target_include_directories(sub_synchronization
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)