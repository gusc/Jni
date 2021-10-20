#ifndef __GUSC_JCLASS_HPP
#define __GUSC_JCLASS_HPP 1

#include <jni.h>
#include "JEnv.hpp"
#include "JObject.hpp"
#include "JString.hpp"
#include "private/cast.hpp"
#include "private/concat.hpp"
#include "private/signature.hpp"

namespace gusc::Jni
{

class JClass final
{
public:
    JClass(JEnv& initEnv, jclass initClass) :
        jniEnv(initEnv),
        cls(initClass){}
    JClass(const JClass&) = delete;
    JClass& operator=(const JClass&) = delete;
    ~JClass()
    {
        if (cls)
        {
            jniEnv->DeleteLocalRef(cls);
        }
    }
    
    inline std::string getClassName()
    {
        jmethodID getNameId = getMethodIdSign("getName", "()Ljava/lang/String;");
        return JString(static_cast<jstring>(jniEnv->CallObjectMethod(cls, getNameId)));
    }
    
    inline jmethodID getStaticMethodIdSign(const char* name, const char* signature)
    {
        auto methodId = jniEnv->GetStaticMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find static method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    template<typename TReturn, typename... TArgs>
    inline jmethodID getStaticMethodId(const char* name)
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodId(name, sign.str);
    }
    
    inline jmethodID getMethodIdSign(const char* name, const char* signature)
    {
        auto methodId = jniEnv->GetMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find instance method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    template<typename TReturn, typename... TArgs>
    inline jmethodID getMethodId(const char* name)
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodIdSign(name, sign.str);
    }

    inline jfieldID getStaticFieldIdSign(const char* name, const char* signature)
    {
        auto fieldId = jniEnv->GetStaticFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find static field ") + name + " with signature " + signature);
        }
        return fieldId;
    }

    template<typename T>
    inline jfieldID getStaticFieldId(const char* name)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getStaticFieldIdSign(name, sign.str);
    }
    
    inline jfieldID getFieldIdSign(const char* name, const char* signature)
    {
        auto fieldId = jniEnv->GetFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find instance field ") + name + " with signature " + signature);
        }
        return fieldId;
    }

    template<typename T>
    inline jfieldID getFieldId(const char* name)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldIdSign(name, sign.str);
    }

    template<typename TReturn, typename... TArgs>
    inline void registerNativeMethodSign(const char* name, const char* signature, TReturn(*fn)(JNIEnv*, jobject, TArgs...))
    {
        const JNINativeMethod methodsArray[] = {
            {name, signature, Private::void_cast(fn)}
        };
        if (jniEnv->RegisterNatives(cls, methodsArray, sizeof(methodsArray) / sizeof(methodsArray[0])) < 0)
        {
            throw std::runtime_error(std::string("Failed to register native method ") + name);
        }
    }

    template<typename TReturn, typename... TArgs>
    inline void registerNativeMethod(const char* name, TReturn(*fn)(JNIEnv*, jobject, TArgs...))
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        registerNativeMethodSign<TReturn>(name, sign.str, fn);
    }

    template<typename... TArgs>
    JObject createObjectJni(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->NewObject(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename... TArgs>
    JObject createObjectSign(const char* signature, const TArgs&... args)
    {
        const auto methodId = getMethodIdSign("<init>", signature);
        return createObjectJni(jniEnv, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename... TArgs>
    JObject createObject(const TArgs&... args)
    {
        constexpr auto sign = Private::getMethodSignature<void, TArgs...>();
        return createObjectSign(sign.str, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethodJni(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        invokeVoidMethod(env, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        const auto methodId = getMethodIdSign(name, signature);
        invokeMethodJni(jniEnv, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        invokeMethodSign<TReturn>(name, sign.str, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethodJni(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return invokeMethodReturn<TReturn>(env, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        const auto methodId = getMethodIdSign(name, signature);
        return invokeMethodJni<TReturn>(jniEnv, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return invokeMethodSign<TReturn>(name, sign.str, std::forward<const TArgs&>(args)...);
    }

    template<typename T>
    T getFieldJni(JEnv& env, jfieldID fieldId)
    {
        return getFieldValue<T>(env, fieldId);
    }

    template<typename T>
    T getFieldSign(const char* name, const char* signature)
    {
        const auto fieldId = getStaticFieldIdSign(name, signature);
        return getFieldJni<T>(jniEnv, fieldId);
    }

    template<typename T>
    T getField(const char* name)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldSign<T>(name, sign.str);
    }

    template<typename T>
    T setFieldJni(JEnv& env, jfieldID fieldId, const T& value)
    {
        return setFieldValue<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    T setFieldSign(const char* name, const char* signature, const T& value)
    {
        const auto fieldId = getStaticFieldIdSign(name, signature);
        return setFieldJni<T>(jniEnv, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    T setField(const char* name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }
    
private:
    JEnv& jniEnv;
    jclass cls { nullptr };

    inline void invokeVoidMethod(JEnv& env, jmethodID methodId)
    {
        env->CallStaticVoidMethod(cls, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeVoidMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        env->CallStaticVoidMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, bool>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticBooleanMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, char>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticCharMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, std::int8_t>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticByteMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, short>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticShortMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, int>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticIntMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, long> ||
        std::is_same_v<TReturn, long long>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticLongMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, float>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<TReturn>(env->CallStaticFloatMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, double>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
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
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JString(static_cast<jstring>(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...)));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, bool>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticBooleanField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, char>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticCharField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, std::int8_t>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticByteField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, short>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticShortField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, int>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticIntField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, long> ||
        std::is_same_v<T, long long>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticLongField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, float>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticFloatField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, double>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetStaticDoubleField(cls, fieldId));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, bool>,
                                  const T&
                              > value)
    {
        env->SetBooleanField(cls, fieldId, static_cast<jboolean>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, char>,
                                  const T&
                              > value)
    {
        env->SetCharField(cls, fieldId, static_cast<jchar>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, std::int8_t>,
                                  const T&
                              > value)
    {
        env->SetByteField(cls, fieldId, static_cast<jbyte>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, short>,
                                  const T&
                              > value)
    {
        env->SetShortField(cls, fieldId, static_cast<jshort>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, int>,
                                  const T&
                              > value)
    {
        env->SetIntField(cls, fieldId, static_cast<jint>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, long> ||
                                  std::is_same_v<T, long long>,
                                  const T&
                              > value)
    {
        env->SetLongField(cls, fieldId, static_cast<jlong>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, float>,
                                  const T&
                              > value)
    {
        env->SetFloatField(cls, fieldId, static_cast<jfloat>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, double>,
                                  const T&
                              > value)
    {
        env->SetDoubleField(cls, fieldId, static_cast<jdouble>(value));
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

inline jmethodID JObject::getMethodIdJni(JEnv& env, const char* name, const char* signature)
{
    return getClass(env).getMethodIdSign(name, signature);
}

inline jfieldID JObject::getFieldIdJni(JEnv& env, const char* name, const char* signature)
{
    return getClass(env).getFieldIdSign(name, signature);
}

inline JClass JObject::getClass(JEnv& env)
{
    return env.getObjectClass(obj);
}

inline JClass JObject::getClass()
{
    auto env = JVM::getEnv();
    return getClass(env);
}

}

#endif // __GUSC_JCLASS_HPP
