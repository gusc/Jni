#include "include/Jni.hpp"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    gusc::Jni::InitJni(vm);
    return JNI_VERSION_1_6;
}
