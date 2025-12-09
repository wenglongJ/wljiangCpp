cmake_minimum_required(VERSION 3.10) #CMake version
project(demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #compile_commands.json
set(CMAKE_CXX_STANDARD 17) #c++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 
option(DISABLE_GNU_SOURCE "Disable _GNU_SOURCE macro" ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)

# Find Protobuf library
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

# Manual path specification if not found
if(NOT Protobuf_FOUND)
    set(Protobuf_INCLUDE_DIRS "/usr/local/include")
    set(Protobuf_LIBRARIES "/usr/local/lib/libprotobuf.so")
endif()

# Include directories


set(COM_INCLUDE_PATH
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/json

)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PROTOBUF_INCLUDE_DIRS}
    ${COM_INCLUDE_PATH}
) #whihe .h

#mian push bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

#link main
file(GLOB SRC_LIST "src/*.cc")
file(GLOB JSON_LIST "src/jsoncpp/*.cc")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${JSON_LIST})
# Link libraries

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${PROTOBUF_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
target_link_options(${PROJECT_NAME} PRIVATE -pthread)
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)
