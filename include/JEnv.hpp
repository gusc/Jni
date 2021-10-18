#ifndef __GUSC_JENV_HPP
#define __GUSC_JENV_HPP 1

#include <jni.h>
#include "JClass.hpp"

namesapce gusc::Jni
{

class JEnv
{
public:
    JEnv(JNIEnv* initEnv) :
        env(initEnv) {}
    
    inline JClass getClass(const char* classPath)
    {
        auto cls = env->FindClass(classPath);
        if (!cls)
        {
            throw std::runtime_error(std::string("Can't find ") + classPath + " Java class");
        }
        return JClass(env, cls);
    }
    
    inline JClass getObjectClass(jobject jniObject)
    {
        auto cls = env->GetObjectClass(jniObject);
        if (!cls)
        {
            throw std::runtime_error("Class not found");
        }
        return JClass(env, cls);
    }
    
    inline JNIEnv* operator->()
    {
        return env;
    }
    
private:
    JNIEnv* env { nullptr };
}
    
}

#endif // __GUSC_JENV_HPP
