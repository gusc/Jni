#ifndef __GUSC_JSTRING_HPP
#define __GUSC_JSTRING_HPP 1

#include <jni.h>
#include "JVM.hpp"
#include "JEnv.hpp"

namespace gusc::Jni
{

class JString
{
public:
    JString(JEnv env, const jstring& initString)
            : string(initString)
    {
        if (string)
        {
            length = env->GetStringUTFLength(string);
            dataPtr = env->GetStringUTFChars(string, &isCopy);
        }
    }
    JString(const jstring& initString) : JString(JVM::getEnv(), initString)
    {}
    JString(const JString& other) = delete;
    JString& operator = (const JString& other) = delete;
    ~JString()
    {
        if (string && isCopy == JNI_TRUE)
        {
            auto env = JVM::getEnv();
            env->ReleaseStringUTFChars(string, dataPtr);
        }
    }
    operator std::string()
    {
        if (dataPtr)
        {
            return std::string(dataPtr, dataPtr + length);
        }
        return {};
    }
    operator jstring()
    {
        return string;
    }
    inline const char* data()
    {
        return dataPtr;
    }

    static JString createFrom(JEnv env, const std::string& str)
    {
        return JString(env, env->NewStringUTF(str.c_str()));
    }

    static JString createFrom(const std::string& str)
    {
        return createFrom(JVM::getEnv(), str);
    }

private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jstring string { nullptr };
    const char* dataPtr { nullptr };
};

}

#endif // __GUSC_JSTRING_HPP
