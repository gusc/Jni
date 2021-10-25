//
//  main.cpp
//  Jni
//
//  Created by Gusts Kaksis on 18/10/2021.
//  Copyright © 2021 Gusts Kaksis. All rights reserved.
//

#include "Jni/Jni.hpp"
#include "InstanceTest.hpp"
#include "StaticTest.hpp"
#include "NativeClassTest.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_lv_gusc_jni_tests_Test_runTest(JNIEnv *env, jclass clazz)
{
    runInstanceTest();
    runStaticTest();
    runNativeTest();
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    gusc::Jni::JVM::init(vm);

    registerNativeMethods();

    return JNI_VERSION_1_6;
}
