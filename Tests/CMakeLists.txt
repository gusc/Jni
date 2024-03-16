cmake_minimum_required(VERSION 3.8)
project(JniTests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
if(Jni_GoogleTest_DIR)
	get_filename_component(Jni_GoogleTest_DIR "${Jni_GoogleTest_DIR}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../")
else()
	get_filename_component(Jni_GoogleTest_DIR "${CMAKE_CURRENT_LIST_DIR}/../../googletest" ABSOLUTE "${CMAKE_CURRENT_LIST_DIR}/../../")
endif()

add_subdirectory(../ Jni)
if(NOT TARGET gtest)
	add_subdirectory(${Jni_GoogleTest_DIR} googletest)
endif()

set(SOURCES
	"cpp/main.cpp"
	"cpp/InstanceTest.cpp"
	"cpp/NativeClassMock.hpp"
	"cpp/NativeClassTest.cpp"
    "cpp/ObjectTest.cpp"
    "cpp/PrivateTest.cpp"
	"cpp/StaticTest.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../include/)
target_link_libraries(${PROJECT_NAME}
	log
	Jni
	gtest
	gmock
)
