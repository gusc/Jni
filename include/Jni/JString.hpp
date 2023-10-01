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
            : jniString(initString)
    {
        if (jniString)
        {
            length = env->GetStringUTFLength(jniString);
            dataPtr = env->GetStringUTFChars(jniString, nullptr);
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
        jniString = nullptr;
        std::swap(jniString, other.jniString);
        std::swap(length, other.length);
        std::swap(dataPtr, other.dataPtr);
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
        return jniString;
    }
    operator jstring() const
    {
        return jniString;
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
    std::size_t length { 0 };
    jstring jniString { nullptr };
    const char* dataPtr { nullptr };

    void dispose()
    {
        if (jniString && dataPtr)
        {
            auto env = JVM::getEnv();
            env->ReleaseStringUTFChars(jniString, dataPtr);
            dataPtr = nullptr;
            length = 0;
        }
    }
};

template<typename T>
inline
typename std::enable_if_t<
        std::is_same_v<T, JString>,
        T
>
JObject::getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
{
    return JString(getFieldValue<jstring>(env, fieldId));
}

template<typename TReturn, typename... TArgs>
inline
typename std::enable_if_t<
        std::is_same_v<TReturn, JString>,
        TReturn
>
JObject::invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
{
    return JString(invokeMethodReturn<jstring>(env, methodId, std::forward<const TArgs&>(args)...));
}

}

#endif // __GUSC_JSTRING_HPP
