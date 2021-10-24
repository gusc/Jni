#ifndef __GUSC_JSTRING_HPP
#define __GUSC_JSTRING_HPP 1

#include <jni.h>
#include "JVM.hpp"
#include "JEnv.hpp"

#include <string>

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
    JString& operator=(const JString& other) = delete;
    JString(JString&& other) = default;
    JString& operator=(JString&& other)
    {
        dispose();
        isCopy = other.isCopy;
        length = other.length;
        string = other.string;
        dataPtr = other.dataPtr;
        other.isCopy = JNI_FALSE;
        other.length = 0;
        other.string = nullptr;
        other.dataPtr = nullptr;
        return *this;
    }
    ~JString()
    {
        dispose();
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
    operator jstring() const
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

    void dispose()
    {
        if (string && isCopy == JNI_TRUE)
        {
            auto env = JVM::getEnv();
            env->ReleaseStringUTFChars(string, dataPtr);
        }
    }
};

}

#endif // __GUSC_JSTRING_HPP
