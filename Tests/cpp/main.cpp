//
//  main.cpp
//  Jni
//
//  Created by Gusts Kaksis on 18/10/2021.
//  Copyright © 2021 Gusts Kaksis. All rights reserved.
//

#include <gmock/gmock.h>
#include <jni.h>
#include "Utilities.hpp"
#include "NativeClassMock.hpp"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    // Initialize JNI lib
    gusc::Jni::JVM::init(vm);

    // Initialize any native classes
    NativeClassMock::registerNativeMethods();

    return JNI_VERSION_1_6;
}

int _argc;
char** _argv;
std::vector<std::vector<char>> argvStorage;
std::vector<char*> argvPtrs;

extern "C"
JNIEXPORT jint JNICALL
Java_lv_gusc_jni_tests_Main_main(JNIEnv *env, jclass clazz, jobjectArray jargv)
{
    copyArgs(env, jargv, argvStorage, argvPtrs);
    LogCapture logCapture;
    int argc = argvPtrs.size();
    testing::InitGoogleTest(&argc, argvPtrs.data());
    testing::InitGoogleMock(&argc, argvPtrs.data());
    _argc = argc;
    _argv = argvPtrs.data();
    return RUN_ALL_TESTS();
}

extern "C"
JNIEXPORT void JNICALL
Java_lv_gusc_jni_tests_Main_exit(JNIEnv *env, jclass clazz, jint result)
{
    exit(result);
}
