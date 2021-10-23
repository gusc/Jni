#include "include/Jni/Jni.hpp"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    gusc::Jni::JVM::init(vm);
    return JNI_VERSION_1_6;
}
