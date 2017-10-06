cmake_minimum_required(VERSION 3.1.2)

project(ape VERSION 0.1.0)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

include(GenerateExportHeader)

add_library(ape src/ape.c src/ape_tag.c src/ape_tag.h include/ape/ape.h)
target_include_directories(ape
    PUBLIC include ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(ape PROPERTIES
    DEFINE_SYMBOL APE_EXPORTS
    VERSION 0.1
    SOVERSION 0)
if(WIN32 AND BUILD_SHARED_LIBS)
    target_sources(ape PRIVATE src/ape.rc)
endif()

generate_export_header(ape)
