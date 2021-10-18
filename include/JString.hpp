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
    JString(JEnv& initEnv, const jstring& initString)
            : string(initString)
    {
        if (string)
        {
            length = env->GetStringUTFLength(string);
            dataPtr = env->GetStringUTFChars(string, &isCopy);
        }
    }
    JString(const JString& other) = delete;
    JString& operator = (const JString& other) = delete;
    ~JString()
    {
        if (string && isCopy == JNI_TRUE)
        {
            const auto env = JVM::getEnv();
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
    inline const char* data()
    {
        return dataPtr;
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jstring string { nullptr };
    const char* dataPtr { nullptr };
};

}

#endif // __GUSC_JSTRING_HPP
