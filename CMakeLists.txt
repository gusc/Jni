cmake_minimum_required(VERSION 3.8)
project(Jni VERSION 1.0.0 LANGUAGES CXX)

option(Jni_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(Jni_AutoInit "Automatically init JNI via JNI_onLoad method" ON)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
	"include/Jni/JVM.hpp"
	"include/Jni/JArray.hpp"
	"include/Jni/JClass.hpp"
	"include/Jni/JEnv.hpp"
	"include/Jni/JGlobalRef.hpp"
	"include/Jni/Jni.hpp"
	"include/Jni/JObject.hpp"
	"include/Jni/JString.hpp"
	"include/Jni/private/signature.hpp"
	"include/Jni/private/concat.hpp"
	"include/Jni/private/cast.hpp"
)
	
if(Jni_AutoInit)
    list(APPEND SOURCES "Jni.cpp")
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    if (${CMAKE_VERSION} VERSION_GREATER "3.19.0")
        add_library(${PROJECT_NAME} INTERFACE ${SOURCES})
    else()
        add_library(${PROJECT_NAME} INTERFACE)
    endif()
endif()
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

include(CTest)
if(BUILD_TESTING AND Jni_BuildTests)
    # TODO: run gradlew as a custom build step
endif()