cmake_minimum_required(VERSION 3.8)
project(Jni VERSION 1.0.0 LANGUAGES CXX)

option(Jni_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(Jni_AutoInit "Automatically init JNI via JNI_onLoad method" ON)

set(SOURCES
	"include/Jni.hpp")
	
if(Jni_AutoInit)
    list(APPEND SOURCES "Jni.cpp")
endif()

add_library(${PROJECT_NAME} INTERFACE ${SOURCES})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

include(CTest)
if(BUILD_TESTING AND Jni_BuildTests)
    add_subdirectory(Tests)
endif()