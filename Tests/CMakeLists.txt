cmake_minimum_required(VERSION 3.8)
project(JniTests VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
	"cpp/main.cpp"
	"cpp/InstanceTest.hpp"
	"cpp/NativeClassTest.hpp"
	"cpp/StaticTest.hpp"
	# Adding these to ease development in Android Studio
	"../include/Jni/JClass.hpp"
	"../include/Jni/JEnv.hpp"
	"../include/Jni/JGlobalRef.hpp"
	"../include/Jni/Jni.hpp"
	"../include/Jni/JObject.hpp"
	"../include/Jni/JVM.hpp"
	"../include/Jni/JArray.hpp"
	"../include/Jni/JString.hpp"
	"../include/Jni/private/cast.hpp"
	"../include/Jni/private/concat.hpp"
	"../include/Jni/private/signature.hpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../include/)
