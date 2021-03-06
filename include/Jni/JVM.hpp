#ifndef __GUSC_JVM_HPP
#define __GUSC_JVM_HPP 1

#include <jni.h>

#include <stdexcept>

#include "JEnv.hpp"

namespace gusc::Jni
{

class JVM
{
public:
    static inline void init(JavaVM* initVm) noexcept
    {
        vm = initVm;
    }
    static inline JEnv getEnv()
    {
        if (vm)
        {
            JNIEnv *env { nullptr };
            auto res = vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            if (res == JNI_OK)
            {
                return JEnv(env);
            }
            else if (res == JNI_EDETACHED && vm->AttachCurrentThread(&env, nullptr) == JNI_OK)
            {
                return JEnv(env);
            }
        }
        throw std::runtime_error("Java ENV cannot be created!");
    }
private:
    inline static JavaVM* vm { nullptr };
};
    
}

#endif // __GUSC_JVM_HPP
