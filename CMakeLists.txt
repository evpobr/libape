cmake_minimum_required(VERSION 3.1.2)

project(ape VERSION 0.1.0)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CheckIncludeFile)

check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(stdint.h HAVE_STDINT_H)

configure_file(src/config.h.cmake src/config.h)

set(libape_PUBLIC_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/ape/ape.h
	${CMAKE_CURRENT_BINARY_DIR}/ape_export.h)
set(libape_SOURCES
	src/ape.c
	src/ape_tag.c)

add_library(ape ${libape_SOURCES})
target_include_directories(ape
    PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)
set_target_properties(ape PROPERTIES
    DEFINE_SYMBOL APE_EXPORTS
    VERSION 0.1
    SOVERSION 0)
if(WIN32 AND BUILD_SHARED_LIBS)
    target_sources(ape PRIVATE src/ape.rc)
endif()
if(MSVC)
	target_compile_definitions(ape PRIVATE _CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE)
endif()

generate_export_header(ape)

target_sources(ape PUBLIC ${libape_PUBLIC_HEADERS})
set_target_properties(ape PROPERTIES
	PUBLIC_HEADER "${libape_PUBLIC_HEADERS}")

include(CTest)

add_executable(mac-399-id3v2 tests/mac-399-id3v2.c)
target_link_libraries(mac-399-id3v2 ape)
if(MSVC)
	target_compile_definitions(mac-399-id3v2 PRIVATE _CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE)
endif()
add_test(NAME mac-399-id3v2 COMMAND mac-399-id3v2 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

install(TARGETS ape DESTINATION
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ape
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
