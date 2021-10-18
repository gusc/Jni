#ifndef __GUSC_JCLASS_HPP
#define __GUSC_JCLASS_HPP 1

#include <jni.h>
#include "JEnv.hpp"
#include "JObject.hpp"
#include "JString.hpp"
#include "private/concat.hpp"
#include "private/signature.hpp"

namespace gusc::Jni
{

class JClass final
{
public:
    JClass(JEnv& initEnv, jclass initClass) :
        env(initEnv),
        cls(initClass){}
    
    inline std::string getClassName()
    {
        jmethodID getNameId = getMethodId("getName", "()Ljava/lang/String;");
        return JString(static_cast<jstring>(env->CallObjectMethod(cls, getNameId)));
    }
    
    inline jmethodID getStaticMethodId(const char* name, const char* signature)
    {
        auto methodId = env->GetStaticMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find static method ") + name + " with signature " + signature);
        }
        return methodId;
    }
    
    inline jmethodID getMethodId(const char* name, const char* signature)
    {
        auto methodId = env->GetMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find instance method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    inline jfieldID getStaticFieldId(const char* name, const char* signature)
    {
        auto fieldId = env->GetStaticFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find static field ") + name + " with signature " + signature);
        }
        return fieldId;
    }
    
    inline jfieldID getFieldId(const char* name, const char* signature)
    {
        auto fieldId = env->GetFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find instance field ") + name + " with signature " + signature);
        }
        return fieldId;
    }

    template<typename... TArgs>
    JObject createObjectSign(const char* signature, const TArgs&... args)
    {
        const auto methodId = getMethodId("<init>", signature);
        return JObject(env->NewObject(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename... TArgs>
    JObject createObject(const TArgs&... args)
    {
        constexpr auto sign = Private::getArgumentSignature(std::forward<const TArgs&>(args)...);
        return createObjectSign(sign.str, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, void>,
            void
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        const auto methodId = getMethodId(name, signature);
        invokeVoidMethod(methodId, std::forward<const TArgs&>(args)...);
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
        const auto methodId = getMethodId(name, signature);
        return invokeMethodReturn<TReturn>(methodId, std::forward<const TArgs&>(args)...);
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
    
private:
    JEnv& env;
    jclass cls { nullptr };

    inline void invokeVoidMethod(jmethodID methodId)
    {
        env->CallStaticVoidMethod(cls, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeVoidMethod(jmethodID methodId, const TArgs&... args)
    {
        env->CallStaticVoidMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, bool>,
            TReturn
    >
    invokeMethodReturn(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticBooleanMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, char>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticCharMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, std::int8_t>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticByteMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, short>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticShortMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, int>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticIntMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, long>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticLongMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, float>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticFloatMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, double>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticDoubleMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, std::string> ||
            std::is_same_v<TReturn, JString>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return JString(static_cast<jstring>(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...)));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, JObject>,
            TReturn
    >
    invokeMethod(jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }
};

inline JClass JEnv::getClass(const char* classPath)
{
    auto cls = env->FindClass(classPath);
    if (!cls)
    {
        throw std::runtime_error(std::string("Can't find ") + classPath + " Java class");
    }
    return JClass(*this, cls);
}

inline JClass JEnv::getObjectClass(jobject jniObject)
{
    auto cls = env->GetObjectClass(jniObject);
    if (!cls)
    {
        throw std::runtime_error("Class not found");
    }
    return JClass(*this, cls);
}

inline jmethodID JObject::getMethodId(const char* name, const char* signature)
{
    auto env = JVM::getEnv();
    auto cls = env.getObjectClass(obj);
    return cls.getMethodId(name, signature);
}

inline jfieldID JObject::getFieldId(const char* name, const char* signature)
{
    auto env = JVM::getEnv();
    auto cls = env.getObjectClass(obj);
    return cls.getFieldId(name, signature);
}

}

#endif // __GUSC_JCLASS_HPP
