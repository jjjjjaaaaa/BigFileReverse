cmake_minimum_required(VERSION 3.10)

project(BigFileReverse
    VERSION 1.0
    DESCRIPTION "A library for walking through large files"
    LANGUAGES CXX
)

add_library(BigFileReverseLib
    DataProvider/MmapDataProvider.cpp
    DataProvider/StreamDataProvider.cpp
    EncodingFixer/Big5EncodingFixer.cpp
    FileReverser/FileReverser.cpp
    FileReverser/Big5FileReverser.cpp
    )


target_include_directories(BigFileReverseLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/DataProvider
    ${CMAKE_SOURCE_DIR}/EncodingFixer
    ${CMAKE_SOURCE_DIR}/FileReverser)

add_executable(BigFileReverseTest main.cpp)
target_include_directories(BigFileReverseTest PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(BigFileReverseTest PRIVATE BigFileReverseLib)

add_custom_command(TARGET BigFileReverseTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/input.txt
            ${CMAKE_SOURCE_DIR}/input_golden.txt
            $<TARGET_FILE_DIR:BigFileReverseTest>
)
