
cmake_minimum_required(VERSION 2.8)

set(CPACK_PACKAGE_NAME "libhls")
set(CPACK_PACKAGE_VENDOR "libhls-vendor")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libhls library")
set(CPACK_PACKAGE_VERSION "1.2.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_SYSROOT ${CMAKE_SYSROOT})
set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})



###############################################################################

project("libhls")

# Enable ExternalProject CMake module
include(ExternalProject)

# Enable c++11
set (CMAKE_CXX_STANDARD 11)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-unused-function -Wno-unused-variable -std=c++11")

# Build with symbols
set(CMAKE_BUILD_TYPE Debug)

###############################################################################
# Include CURL as external project

ExternalProject_Add(
    curl_external_project
    PREFIX curl
    URL https://curl.haxx.se/download/curl-7.58.0.tar.gz
    CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/curl/src/curl_external_project//configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/curl --host=mips-linux-gnu CC=${CMAKE_C_COMPILER}
)

add_library(curl SHARED IMPORTED)

set_target_properties(curl PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/curl/lib/libcurl.so
)

add_dependencies(curl curl_external_project)

ExternalProject_Get_Property(curl_external_project source_dir binary_dir)

include_directories("${source_dir}/include")

###############################################################################
# Include Google test as external project

# We need thread support
find_package(Threads REQUIRED)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    CMAKE_ARGS
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}
        -DCMAKE_SYSROOT=${CMAKE_SYSROOT}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

###############################################################################

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

###############################################################################

set(LIB_ROOT "libhls")

set(EXTERNAL_ROOT "external")

set(SOURCE_DIR "${LIB_ROOT}/source")

# With -fPIC
IF(UNIX AND NOT WIN32)
  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
  IF(CMAKE_UNAME)
    EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
    SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL
"processor type (i386 and x86_64)")
    IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
      ADD_DEFINITIONS(-fPIC)
    ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

find_package (Threads)

################################################################################
# libiwu library
########################################

# IWU
include(ExternalProject)

ExternalProject_Add(libiwu_external_project
    PREFIX libiwu
    URL file://${CMAKE_CURRENT_SOURCE_DIR}/../libiwu
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libiwu
    INSTALL_COMMAND ""
    DOWNLOAD_COMMAND ""
    CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
)

add_library(libiwu SHARED IMPORTED)

set_target_properties(libiwu PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libiwu/src/libiwu_external_project-build/lib/libiwu.so
)

add_dependencies(libiwu libiwu_external_project)

include_directories(
    ../libiwu/libiwu/include/
)

add_dependencies(libiwu libiwu_external_project)

################################################################################
# Common
########################################

include_directories(
    "${LIB_ROOT}/m3u8/include"
)

################################################################################
# M3U8 library
########################################

set(M3U8_SOURCE_FILES
    ${LIB_ROOT}/m3u8/source/Parser.cpp
)

# NOTE: To keep libraries isolated should only link against libiwu
SET(MPD_REQUIRED_LIBRARIES
    pthread
    m
    libiwu
)

########################################
# Static
########################################
add_library(
    hlsM3u8_static STATIC
    ${M3U8_SOURCE_FILES}
)

TARGET_LINK_LIBRARIES(hlsM3u8_static
    ${M3U8_REQUIRED_LIBRARIES}
)

########################################
# Shared
########################################
add_library(
    hlsM3u8 SHARED
    ${M3U8_SOURCE_FILES}
)

add_executable(
    hls
    ${M3U8_SOURCE_FILES}
)


TARGET_LINK_LIBRARIES(hls
    pthread
    m
    libiwu
)

#############################################################################
# Test

set(HLS_TEST_SOURCES
    # Test
    ${LIB_ROOT}/test/source/main.cpp
    ${LIB_ROOT}/test/source/ATest.cpp
    ${LIB_ROOT}/test/source/TestParser.cpp
)

include_directories("${LIB_ROOT}/test/include")

add_executable(
    hls_test
    ${HLS_TEST_SOURCES}
)

TARGET_LINK_LIBRARIES(hls_test
    pthread
    m
    libgtest
    libgmock
    libiwu
)