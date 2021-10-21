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
    JObject(const jobject& initObject, bool initIsOwned = false)
        : obj(initObject)
        , isOwned(initIsOwned)
    {}
    JObject(const JObject& other) = delete;
    JObject& operator=(const JObject& other) = delete;
    JObject(JObject&& other) = default;
    JObject& operator=(JObject&& other)
    {
        if (obj)
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
            else if (isOwned)
            {
                env->DeleteLocalRef(obj);
            }
        }
        obj = other.obj;
        isOwned = other.isOwned;
        other.obj = nullptr;
        other.isOwned = false;
        return *this;
    }
    ~JObject()
    {
        if (obj)
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
            else if (isOwned)
            {
                env->DeleteLocalRef(obj);
            }
        }
    }
    
    inline operator jobject() const
    {
        return obj;
    }

    jmethodID getMethodIdJni(JEnv& env, const char* name, const char* signature);
    inline jmethodID getMethodIdSign(const char* name, const char* signature)
    {
        auto env = JVM::getEnv();
        return getMethodIdJni(env, name, signature);
    }
    template<typename TReturn, typename... TArgs>
    inline jmethodID getMethodId(const char* name)
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodIdSign(name, sign.args);
    }
    jfieldID getFieldIdJni(JEnv& env, const char* name, const char* signature);
    inline jfieldID getFieldIdSign(const char* name, const char* signature)
    {
        auto env = JVM::getEnv();
        return getFieldIdJni(env, name, signature);
    }
    template<typename T>
    inline jfieldID getFieldId(const char* name)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldIdSign(name, sign.args);
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
        invokeMethodReturnVoid(env, methodId, std::forward<const TArgs&>(args)...);
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
        auto methodId = getMethodIdJni(env, name, signature);
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
        auto env = JVM::getEnv();
        const auto methodId = getMethodIdJni(env, name, signature);
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
        auto env = JVM::getEnv();
        const auto fieldId = getFieldIdJni(env, name, signature);
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
        const auto fieldId = getFieldIdJni(env, name, signature);
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
    bool isOwned { false };

    inline void invokeMethodReturnVoid(JEnv& env, jmethodID methodId)
    {
        env->CallVoidMethod(obj, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeMethodReturnVoid(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        env->CallVoidMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jboolean>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallBooleanMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jchar>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallCharMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jbyte>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallByteMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jshort>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallShortMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jint>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallIntMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jlong>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallLongMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jfloat>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallFloatMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jdouble>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return env->CallDoubleMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, jstring>,
            TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return static_cast<jstring>(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JString>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JString(invokeMethod<jstring>(env, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args)
    {
        return JObject(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jboolean>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetBooleanField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jchar>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetCharField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jbyte>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetByteField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jshort>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetShortField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jint>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetIntField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jlong>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetLongField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetFloatField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetDoubleField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jstring>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return static_cast<jstring>(env->GetObjectField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jobject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return env->GetObjectField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId)
    {
        return JObject(env->GetObjectField(obj, fieldId), true);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jboolean>,
                                  const T&
                              > value)
    {
        env->SetBooleanField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jchar>,
                                  const T&
                              > value)
    {
        env->SetCharField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jbyte>,
                                  const T&
                              > value)
    {
        env->SetByteField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jshort>,
                                  const T&
                              > value)
    {
        env->SetShortField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jint>,
                                  const T&
                              > value)
    {
        env->SetIntField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jlong>,
                                  const T&
                              > value)
    {
        env->SetLongField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jfloat>,
                                  const T&
                              > value)
    {
        env->SetFloatField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jdouble>,
                                  const T&
                              > value)
    {
        env->SetDoubleField(obj, fieldId, value);
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
        env->SetObjectField(obj, fieldId, static_cast<jobject>(value));
    }
};

}

#endif // __GUSC_JOBJECT_HPP
