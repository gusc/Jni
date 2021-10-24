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
    JClass(JEnv& initEnv, jclass initClass)
        : jniEnv(initEnv)
        , cls(initClass)
    {}
    JClass(const JClass&) = delete;
    JClass& operator=(const JClass&) = delete;
    ~JClass()
    {
        if (cls)
        {
            jniEnv->DeleteLocalRef(cls);
        }
    }
    
    inline std::string getClassPath()
    {
        jmethodID getNameId = getMethodIdSign("getName", "()Ljava/lang/String;");
        std::string className = JString(static_cast<jstring>(jniEnv->CallObjectMethod(cls, getNameId)));
        std::size_t pos = 0;
        while ((pos = className.find('.', pos)) != std::string::npos)
        {
            className.replace(pos, 1, "/");
            ++pos;
        }
        return className;
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
            throw std::runtime_error(std::string("Failed to register native method ") + name + " with signature " + signature);
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
        auto obj = env->NewObject(cls, methodId, std::forward<const TArgs&>(args)...);
        if (!obj)
        {
            throw std::runtime_error(std::string("Failed to reate Java object "));
        }
        return JObject(obj, true);
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
        invokeMethodReturnVoid(env, methodId, std::forward<const TArgs&>(args)...);
        JEnv::checkException(env);
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
        invokeMethodJni<TReturn>(jniEnv, methodId, std::forward<const TArgs&>(args)...);
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
        auto res = invokeMethodReturn<TReturn>(env, methodId, std::forward<const TArgs&>(args)...);
        JEnv::checkException(env);
        return res;
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
    void setFieldJni(JEnv& env, jfieldID fieldId, const T& value)
    {
        setFieldValue<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setFieldSign(const char* name, const char* signature, const T& value)
    {
        const auto fieldId = getStaticFieldIdSign(name, signature);
        setFieldJni<T>(jniEnv, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setField(const char* name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }
    
private:
    JEnv& jniEnv;
    jclass cls { nullptr };

    inline void invokeMethodReturnVoid(JEnv& env, jmethodID methodId)
    {
        env->CallStaticVoidMethod(cls, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeMethodReturnVoid(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        env->CallStaticVoidMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jboolean>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticBooleanMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jchar>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticCharMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jbyte>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticByteMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jshort>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticShortMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jint>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticIntMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jlong>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticLongMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jfloat>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticFloatMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jdouble>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallStaticDoubleMethod(cls, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jstring>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<jstring>(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, JString>,
            TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JString(invokeMethodReturn<jstring>(env, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->CallStaticObjectMethod(cls, methodId, std::forward<const TArgs&>(args)...), true);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jboolean>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticBooleanField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jchar>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticCharField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jbyte>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticByteField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jshort>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticShortField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jint>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticIntField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jlong>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticLongField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticFloatField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticDoubleField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jstring>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<jstring>(env->GetStaticObjectField(cls, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JString>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return JString(getFieldValue<jstring>(env, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jobject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetStaticObjectField(cls, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return JObject(getFieldValue<jobject>(env, fieldId), true);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jboolean>,
                                  const T&
                              > value)
    {
        env->SetStaticBooleanField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jchar>,
                                  const T&
                              > value)
    {
        env->SetStaticCharField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jbyte>,
                                  const T&
                              > value)
    {
        env->SetStaticByteField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jshort>,
                                  const T&
                              > value)
    {
        env->SetStaticShortField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jint>,
                                  const T&
                              > value)
    {
        env->SetStaticIntField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jlong>,
                                  const T&
                              > value)
    {
        env->SetStaticLongField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jfloat>,
                                  const T&
                              > value)
    {
        env->SetStaticFloatField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jdouble>,
                                  const T&
                              > value)
    {
        env->SetStaticDoubleField(cls, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jstring> ||
                                  std::is_same_v<T, jobject> ||
                                  std::is_same_v<T, JObject>,
                                  const T&
                              > value)
    {
        env->SetStaticObjectField(cls, fieldId, static_cast<jobject>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, JString>,
                                  const T&
                              > value)
    {
        setFieldValue<jstring>(env, fieldId, static_cast<jstring>(value));
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

inline void JEnv::checkException(JEnv& env)
{
    if (env->ExceptionCheck() == JNI_TRUE)
    {
        auto ex = JObject(static_cast<jobject>(env->ExceptionOccurred()));
        auto message = ex.invokeMethod<JString>("getMessage");
        env->ExceptionClear();
        throw std::runtime_error(std::string("JNI Exception occured: ") + static_cast<std::string>(message));
    }
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
