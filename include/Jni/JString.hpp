#ifndef __GUSC_JSTRING_HPP
#define __GUSC_JSTRING_HPP 1

#include <string>

namespace gusc::Jni
{

class JString final: public JObject
{
    class JStringData final
    {
    public:
        JStringData(const jstring& initString)
            : jniString(initString)
        {
            if (jniString)
            {
                auto env = JVM::getEnv();
                length = env->GetStringUTFLength(jniString);
                dataPtr = env->GetStringUTFChars(jniString, nullptr);
            }
        }
        JStringData(const JStringData&) = delete;
        JStringData& operator=(const JStringData&) = delete;

        inline operator std::string() const
        {
            return std::string(dataPtr, dataPtr + length);
        }

        inline const char* data() const
        {
            return dataPtr;
        }

        ~JStringData()
        {
            if (jniString && dataPtr)
            {
                auto env = JVM::getEnv();
                env->ReleaseStringUTFChars(jniString, dataPtr);
                dataPtr = nullptr;
                length = 0;
            }
        }
    private:
        jstring jniString { nullptr };
        std::size_t length { 0 };
        const char* dataPtr { nullptr };
    };
public:
    /// @brief wrap around existing JNI jstring object
    JString(JEnv env, const jstring& initString)
        : JObject(env,static_cast<jobject>(initString))
    {}
    JString(const jstring& initString)
        : JString(JVM::getEnv(), initString)
    {}

    inline operator std::string() const
    {
        JStringData data(static_cast<jstring>(jniObject));
        return static_cast<std::string>(data);
    }

    inline operator jstring() const
    {
        return static_cast<jstring>(jniObject);
    }

    inline JStringData getData() const
    {
        return static_cast<jstring>(jniObject);
    }

    inline static JString createFrom(JEnv env, const std::string& str)
    {
        return JString(env->NewStringUTF(str.c_str()));
    }

    inline static JString createFrom(const std::string& str)
    {
        return createFrom(JVM::getEnv(), str);
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
