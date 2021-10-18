#ifndef __GUSC_JOBJECT_HPP
#define __GUSC_JOBJECT_HPP 1

#include <jni.h>
#include "JVM.hpp"
#include "JEnv.hpp"
#include "JString.hpp"
#include "private/concat.hpp"
#include "private/signature.hpp"

namespace gusc::Jni
{

class JObject final
{
public:
    JObject(const jobject& initObject) :
        obj(initObject)
    {}
    JObject(const JObject& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
        }
        else
        {
            obj = other.obj;
        }
    }
    JObject& operator=(const JObject& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(obj) == JNIGlobalRefType)
        {
            env->DeleteGlobalRef(obj);
        }
        else if (env->GetObjectRefType(obj) == JNIWeakGlobalRefType)
        {
            env->DeleteWeakGlobalRef(obj);
        }
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
        }
        else
        {
            obj = other.obj;
        }
        return *this;
    }
    JObject(JObject&& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
        }
        else
        {
            obj = other.obj;
        }
        other.obj = nullptr;
    }
    JObject& operator=(JObject&& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(obj) == JNIGlobalRefType)
        {
            env->DeleteGlobalRef(obj);
        }
        else if (env->GetObjectRefType(obj) == JNIWeakGlobalRefType)
        {
            env->DeleteWeakGlobalRef(obj);
        }
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
        }
        else
        {
            obj = other.obj;
        }
        other.obj = nullptr;
        return *this;
    }
    ~JObject()
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(obj) == JNIGlobalRefType)
        {
            env->DeleteGlobalRef(obj);
        }
        else if (env->GetObjectRefType(obj) == JNIWeakGlobalRefType)
        {
            env->DeleteWeakGlobalRef(obj);
        }
    }
    
    inline operator jobject() const
    {
        return obj;
    }
    
    jmethodID getMethodId(const char* name, const char* signature);

    jfieldID getFieldId(const char* name, const char* signature);
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        auto env = JVM::getEnv();
        const auto methodId = getMethodId(name, signature);
        invokeVoidMethod(env, methodId, std::forward<const TArgs&>(args)...);
    }
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto argSign = Private::getArgumentSignature(std::forward<const TArgs&>(args)...);
        constexpr auto retSign = Private::getTypeSignature<TReturn>();
        constexpr auto sign = Private::concat("(", argSign.str, ")", retSign.str);
        invokeMethodSign<TReturn>(name, sign.str, std::forward<const TArgs&>(args)...);
    }
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        auto env = JVM::getEnv();
        const auto methodId = getMethodId(name, signature);
        return invokeMethodReturn<TReturn>(env, methodId, std::forward<const TArgs&>(args)...);
    }
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto argSign = Private::getArgumentSignature(std::forward<const TArgs&>(args)...);
        constexpr auto retSign = Private::getTypeSignature<TReturn>();
        constexpr auto sign = Private::concat("(", argSign.str, ")", retSign.str);
        return invokeMethodSign<TReturn>(name, sign.str, std::forward<const TArgs&>(args)...);
    }

protected:
    jobject obj { nullptr };

    inline void invokeVoidMethod(JEnv& env, jmethodID methodId)
    {
        env->CallVoidMethod(obj, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeVoidMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        env->CallVoidMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, bool>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallBooleanMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, char>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallCharMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, std::int8_t>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallByteMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, short>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallShortMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, int>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallIntMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, long>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallLongMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, float>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallFloatMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, double>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallDoubleMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, std::string>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JString(env, static_cast<jstring>(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...)));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env, env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }
};

}

#endif // __GUSC_JOBJECT_HPP
