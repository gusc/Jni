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
    /// @brief create new JNI object wrapper
    /// @param initObject - the jobject to wrap
    /// @param initIsOwned - whether we own this object (objects created by you, must be owned, so that they are freed on destruction)
    JObject(const jobject& initObject, bool initIsOwned = false)
        : obj(initObject)
        , isOwned(initIsOwned)
    {}
    JObject(const JObject& other) = delete;
    JObject& operator=(const JObject& other) = delete;
    JObject(JObject&& other)
        : obj(other.obj)
        , isOwned(other.isOwned)
    {
        other.obj = nullptr;
        other.isOwned = false;
    }
    JObject& operator=(JObject&& other)
    {
        dispose();
        obj = other.obj;
        isOwned = other.isOwned;
        other.obj = nullptr;
        other.isOwned = false;
        return *this;
    }
    ~JObject()
    {
        dispose();
    }

    inline operator jobject() const
    {
        return obj;
    }

    /// @brief Release the wrapped object. If it was owned, it's ownership is also released.
    /// @note Use this method to return an object from native method
    inline jobject release()
    {
        auto tmp = obj;
        obj = nullptr;
        isOwned = false;
        return tmp;
    }

    jmethodID getMethodIdJni(JEnv& env, const char* name, const char* signature) const;
    inline jmethodID getMethodIdSign(const char* name, const char* signature) const
    {
        auto env = JVM::getEnv();
        return getMethodIdJni(env, name, signature);
    }
    template<typename TReturn, typename... TArgs>
    inline jmethodID getMethodId(const char* name) const noexcept
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodIdSign(name, sign.args);
    }
    jfieldID getFieldIdJni(JEnv& env, const char* name, const char* signature) const;
    inline jfieldID getFieldIdSign(const char* name, const char* signature) const
    {
        auto env = JVM::getEnv();
        return getFieldIdJni(env, name, signature);
    }
    template<typename T>
    inline jfieldID getFieldId(const char* name) const noexcept
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldIdSign(name, sign.args);
    }
    JClass getClass(JEnv& env) const noexcept;
    JClass getClass() const noexcept;

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, void>,
        void
    >
    invokeMethodJni(JEnv& env, jmethodID methodId, const TArgs&... args) const
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
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args) const
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
    invokeMethod(const char* name, const TArgs&... args) const
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
    invokeMethodJni(JEnv& env, jmethodID methodId, const TArgs&... args) const
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
    invokeMethodSign(const char* name, const char* signature, const TArgs&... args) const
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
    invokeMethod(const char* name, const TArgs&... args) const
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return invokeMethodSign<TReturn>(name, sign.str, std::forward<const TArgs&>(args)...);
    }

    template<typename T>
    T getFieldJni(JEnv& env, jfieldID fieldId) const noexcept
    {
        return getFieldValue<T>(env, fieldId);
    }

    template<typename T>
    T getFieldSign(const char* name, const char* signature) const
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldIdJni(env, name, signature);
        return getFieldJni<T>(env, fieldId);
    }

    template<typename T>
    T getField(const char* name) const
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldSign<T>(name, sign.str);
    }

    template<typename T>
    void setFieldJni(JEnv& env, jfieldID fieldId, const T& value) noexcept
    {
        setFieldValue<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setFieldSign(const char* name, const char* signature, const T& value)
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldIdJni(env, name, signature);
        setFieldJni<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setField(const char* name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }

protected:
    jobject obj { nullptr };
    bool isOwned { false };

    void dispose()
    {
        if (obj && isOwned)
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
            obj = nullptr;
        }
    }

    inline void invokeMethodReturnVoid(JEnv& env, jmethodID methodId) const noexcept
    {
        env->CallVoidMethod(obj, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeMethodReturnVoid(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        env->CallVoidMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jboolean>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallBooleanMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jchar>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallCharMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jbyte>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallByteMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jshort>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallShortMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jint>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallIntMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jlong>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallLongMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jfloat>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallFloatMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jdouble>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallDoubleMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, jstring>,
            TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return static_cast<jstring>(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JString>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return JString(invokeMethodReturn<jstring>(env, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jobject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return JObject(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jboolean>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetBooleanField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jchar>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetCharField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jbyte>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetByteField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jshort>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetShortField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jint>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetIntField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jlong>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetLongField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetFloatField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetDoubleField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jstring>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return static_cast<jstring>(env->GetObjectField(obj, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JString>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return JString(getFieldValue<jstring>(env, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jobject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetObjectField(obj, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return JObject(getFieldValue<jobject>(env, fieldId), true);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
            std::is_same_v<T, jbyteArray> ||
            std::is_same_v<T, jcharArray> ||
            std::is_same_v<T, jshortArray> ||
            std::is_same_v<T, jintArray> ||
            std::is_same_v<T, jlongArray> ||
            std::is_same_v<T, jfloatArray> ||
            std::is_same_v<T, jdoubleArray> ||
            std::is_same_v<T, jobjectArray>,
            T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return static_cast<T>(getFieldValue<jobject>(env, fieldId));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jboolean>,
                                  const T&
                              > value) noexcept
    {
        env->SetBooleanField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jchar>,
                                  const T&
                              > value) noexcept
    {
        env->SetCharField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jbyte>,
                                  const T&
                              > value) noexcept
    {
        env->SetByteField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jshort>,
                                  const T&
                              > value) noexcept
    {
        env->SetShortField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jint>,
                                  const T&
                              > value) noexcept
    {
        env->SetIntField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jlong>,
                                  const T&
                              > value) noexcept
    {
        env->SetLongField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jfloat>,
                                  const T&
                              > value) noexcept
    {
        env->SetFloatField(obj, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jdouble>,
                                  const T&
                              > value) noexcept
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
                              > value) noexcept
    {
        env->SetObjectField(obj, fieldId, static_cast<jobject>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, JString>,
                                  const T&
                              > value) noexcept
    {
        setFieldValue<jstring>(env, fieldId, static_cast<jstring>(value));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                      std::is_same_v<T, jbyteArray> ||
                                      std::is_same_v<T, jcharArray> ||
                                      std::is_same_v<T, jshortArray> ||
                                      std::is_same_v<T, jintArray> ||
                                      std::is_same_v<T, jlongArray> ||
                                      std::is_same_v<T, jfloatArray> ||
                                      std::is_same_v<T, jdoubleArray> ||
                                      std::is_same_v<T, jobjectArray>,
                                      const T&
                              > value) noexcept
    {
        setFieldValue<jobject>(env, fieldId, static_cast<jobject>(value));
    }
};

}

#endif // __GUSC_JOBJECT_HPP
