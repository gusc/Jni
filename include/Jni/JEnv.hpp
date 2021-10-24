#ifndef __GUSC_JENV_HPP
#define __GUSC_JENV_HPP 1

#include <jni.h>

namespace gusc::Jni
{

class JClass;

class JEnv
{
public:
    JEnv(JNIEnv* initEnv) :
        env(initEnv) {}
    
    JClass getClass(const char* classPath);

    JClass getObjectClass(jobject jniObject);

    inline operator JNIEnv*()
    {
        return env;
    }

    inline JNIEnv* operator->()
    {
        return env;
    }

    static void checkException(JEnv& env);

private:
    JNIEnv* env { nullptr };
};
    
}

#endif // __GUSC_JENV_HPP
