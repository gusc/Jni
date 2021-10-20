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

class JClass;

class JObject final
{
public:
    struct JGlobal {};
    struct JWeak {};

    JObject(const jobject& initObject) :
        obj(initObject)
    {}
    JObject(const jobject& initObject, JGlobal) :
            obj(JVM::getEnv()->NewGlobalRef(initObject))
    {}
    JObject(const jobject& initObject, JWeak) :
            obj(JVM::getEnv()->NewWeakGlobalRef(initObject))
    {}
    JObject(const JObject& other) = delete;
    JObject& operator=(const JObject& other) = delete;
    JObject(JObject&& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
            env->DeleteLocalRef(other.obj);
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
        else
        {
            env->DeleteLocalRef(obj);
        }
        if (env->GetObjectRefType(other.obj) == JNILocalRefType)
        {
            obj = env->NewGlobalRef(other.obj);
            env->DeleteLocalRef(other.obj);
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
        else
        {
            env->DeleteLocalRef(obj);
        }
    }
    
    inline operator jobject() const
    {
        return obj;
    }

    jmethodID getMethodId(JEnv& env, const char* name, const char* signature);
    inline jmethodID getMethodId(const char* name, const char* signature)
    {
        auto env = JVM::getEnv();
        return getMethodId(env, name, signature);
    }
    jfieldID getFieldId(JEnv& env, const char* name, const char* signature);
    inline jfieldID getFieldId(const char* name, const char* signature)
    {
        auto env = JVM::getEnv();
        return getFieldId(env, name, signature);
    }
    JClass getClass(JEnv& env);
    JClass getClass();

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
        auto env = JVM::getEnv();
        auto methodId = getMethodId(env, name, signature);
        invokeMethodJni<TReturn>(env, methodId, std::forward<const TArgs&>(args)...);
    }
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto argSign = Private::getArgumentSignature<TArgs...>();
        constexpr auto retSign = Private::getJTypeSignature<TReturn>();
        constexpr auto sign = Private::concat("(", argSign.str, ")", retSign.str);
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
        auto env = JVM::getEnv();
        const auto methodId = getMethodId(env, name, signature);
        return invokeMethodJni<TReturn>(env, methodId, std::forward<const TArgs&>(args)...);
    }
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        !std::is_same_v<TReturn, void>,
        TReturn
    >
    invokeMethod(const char* name, const TArgs&... args)
    {
        constexpr auto argSign = Private::getArgumentSignature<TArgs...>();
        constexpr auto retSign = Private::getJTypeSignature<TReturn>();
        constexpr auto sign = Private::concat("(", argSign.str, ")", retSign.str);
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
        auto env = JVM::getEnv();
        const auto fieldId = getFieldId(env, name, signature);
        return getFieldJni<T>(env, fieldId);
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
        auto env = JVM::getEnv();
        const auto fieldId = getFieldId(env, name, signature);
        return setFieldJni<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    T setField(const char* name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
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
        std::is_same_v<TReturn, long> ||
        std::is_same_v<TReturn, long long>,
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
        std::is_same_v<TReturn, std::string> ||
        std::is_same_v<TReturn, JString>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JString(static_cast<jstring>(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...)));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethod(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, bool>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetBooleanField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, char>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetCharField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, std::int8_t>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetByteField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, short>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetShortField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, int>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetIntField(obj, fieldId));
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
        return static_cast<T>(env->GetLongField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, float>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetFloatField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, double>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<T>(env->GetDoubleField(obj, fieldId));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, bool>,
                                  const T&
                              > value)
    {
        env->SetBooleanField(obj, fieldId, static_cast<jboolean>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, char>,
                                  const T&
                              > value)
    {
        env->SetCharField(obj, fieldId, static_cast<jchar>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, std::int8_t>,
                                  const T&
                              > value)
    {
        env->SetByteField(obj, fieldId, static_cast<jbyte>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, short>,
                                  const T&
                              > value)
    {
        env->SetShortField(obj, fieldId, static_cast<jshort>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, int>,
                                  const T&
                              > value)
    {
        env->SetIntField(obj, fieldId, static_cast<jint>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, long> ||
                                  std::is_same_v<T, long long>,
                                  const T&
                              > value)
    {
        env->SetLongField(obj, fieldId, static_cast<jlong>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, float>,
                                  const T&
                              > value)
    {
        env->SetFloatField(obj, fieldId, static_cast<jfloat>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, double>,
                                  const T&
                              > value)
    {
        env->SetDoubleField(obj, fieldId, static_cast<jdouble>(value));
    }
};

}

#endif // __GUSC_JOBJECT_HPP
