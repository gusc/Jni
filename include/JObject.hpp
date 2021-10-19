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
    
    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args)
    {
        auto env = JVM::getEnv();
        const auto methodId = getMethodId(env, name, signature);
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
        const auto methodId = getMethodId(env, name, signature);
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

    template<typename T>
    T getFieldSign(const char* name, const char* signature)
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldId(env, name, signature);
        return getFieldValue<T>(env, fieldId);
    }

    template<typename T>
    T getField(const char* name)
    {
        constexpr auto sign = Private::getTypeSignature<T>();
        return getFieldSign<T>(name, sign.str);
    }

    template<typename T>
    T setFieldSign(const char* name, const char* signature, const T& value)
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldId(env, name, signature);
        return setFieldValue<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    T setField(const char* name, const T& value)
    {
        constexpr auto sign = Private::getTypeSignature<T>();
        return setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }

protected:
    jobject obj { nullptr };

    jmethodID getMethodId(JEnv& env, const char* name, const char* signature);

    jfieldID getFieldId(JEnv& env, const char* name, const char* signature);

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
        std::is_same_v<T, long>,
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
                                  std::is_same_v<T, long>,
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
