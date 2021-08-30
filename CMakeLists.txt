CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(h2test)

# Generate compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ standard: c++11 (not gnu++11)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_EXTENSIONS OFF)

# C standard: gnu99 (not c99)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_EXTENSIONS ON)

# External dependency modules - nghttp2, curl
ADD_SUBDIRECTORY(externals)

# Application
ADD_SUBDIRECTORY(src)
