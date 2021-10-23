cmake_minimum_required(VERSION 3.8)
project(JniTests VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
	"cpp/main.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../include/)
