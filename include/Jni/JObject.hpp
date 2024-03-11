#ifndef __GUSC_JOBJECT_HPP
#define __GUSC_JOBJECT_HPP 1

#include "private/strutils.hpp"
#include "private/signature.hpp"
#include "private/cast.hpp"
#include <type_traits>

namespace gusc::Jni
{

class JClass;
class JString;

class JObject
{
public:
    using JniType = jobject;

    /// @brief create empty JNI object wrapper
    JObject() = default;
    /// @brief wrap around an existing JNI object
    JObject(const jobject& initObject)
    {
        copy(initObject);
    }
    /// @deprecated
    JObject(const JEnv& /*env*/, const jobject& initObject)
        : JObject { initObject }
    {}
    JObject(const JObject& other)
    {
        dispose();
        copy(other.jniObject);
    }
    JObject& operator=(const JObject& other)
    {
        dispose();
        copy(other.jniObject);
        return *this;
    }
    JObject(JObject&& other)
    {
        dispose();
        std::swap(jniObject, other.jniObject);
    }
    JObject& operator=(JObject&& other)
    {
        dispose();
        std::swap(jniObject, other.jniObject);
        return *this;
    }
    virtual ~JObject()
    {
        dispose();
    }

    /// @brief Create a copy of this object with reference type of global ref
    JObject createGlobalRef() const
    {
        auto env = JVM::getEnv();
        JObject other;
        other.jniObject = env->NewGlobalRef(jniObject);
        return other;
    }

    /// @brief Create a copy of this object with reference type of weak global ref
    JObject createWeakGlobalRef() const
    {
        auto env = JVM::getEnv();
        JObject other;
        other.jniObject = env->NewWeakGlobalRef(jniObject);
        return other;
    }

    inline operator bool() const
    {
        return jniObject != nullptr;
    }

    inline operator jobject() const
    {
        return jniObject;
    }

    /// @brief Release the wrapped object.
    /// @note Use this method to return an object from native method without it being destroyed by RAII
    inline jobject release()
    {
        jobject tmp { nullptr };
        std::swap(jniObject, tmp);
        return tmp;
    }

    jmethodID getMethodIdJni(JEnv& env, const std::string& name, const std::string& signature) const;
    inline jmethodID getMethodIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        return getMethodIdJni(env, name, signature);
    }
    template<typename TReturn, typename... TArgs>
    inline jmethodID getMethodId(const std::string& name) const noexcept
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodIdSign(name, sign.args);
    }
    jfieldID getFieldIdJni(JEnv& env, const std::string& name, const std::string& signature) const;
    inline jfieldID getFieldIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        return getFieldIdJni(env, name, signature);
    }
    template<typename T>
    inline jfieldID getFieldId(const std::string& name) const noexcept
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
    invokeMethodSign(const std::string& name, const std::string& signature, const TArgs&... args) const
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
    invokeMethod(const std::string& name, const TArgs&... args) const
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
    invokeMethodSign(const std::string& name, const std::string& signature, const TArgs&... args) const
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
    invokeMethod(const std::string& name, const TArgs&... args) const
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
    T getFieldSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldIdJni(env, name, signature);
        return getFieldJni<T>(env, fieldId);
    }

    template<typename T>
    T getField(const std::string& name) const
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
    void setFieldSign(const std::string& name, const std::string& signature, const T& value)
    {
        auto env = JVM::getEnv();
        const auto fieldId = getFieldIdJni(env, name, signature);
        setFieldJni<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setField(const std::string& name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }

protected:
    jobject jniObject { nullptr };

    void copy(jobject initObject)
    {
        if (!initObject)
        {
            return;
        }
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(initObject) == JNIGlobalRefType)
        {
            jniObject = env->NewGlobalRef(initObject);
        }
        else if (env->GetObjectRefType(initObject) == JNIWeakGlobalRefType)
        {
            jniObject = env->NewWeakGlobalRef(initObject);
        }
        else
        {
            jniObject = env->NewLocalRef(initObject);
        }
    }
    void dispose()
    {
        if (!jniObject)
        {
            return;
        }
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(jniObject) == JNIGlobalRefType)
        {
            env->DeleteGlobalRef(jniObject);
        }
        else if (env->GetObjectRefType(jniObject) == JNIWeakGlobalRefType)
        {
            env->DeleteWeakGlobalRef(jniObject);
        }
        else
        {
            env->DeleteLocalRef(jniObject);
        }
        jniObject = nullptr;
    }

    inline void invokeMethodReturnVoid(JEnv& env, jmethodID methodId) const noexcept
    {
        env->CallVoidMethod(jniObject, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeMethodReturnVoid(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        env->CallVoidMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jboolean>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallBooleanMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jchar>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallCharMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jbyte>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallByteMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jshort>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallShortMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jint>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallIntMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jlong>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallLongMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jfloat>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallFloatMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jdouble>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallDoubleMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            !std::is_same_v<TReturn, jboolean> &&
            !std::is_same_v<TReturn, jbyte> &&
            !std::is_same_v<TReturn, jchar> &&
            !std::is_same_v<TReturn, jshort> &&
            !std::is_same_v<TReturn, jint> &&
            !std::is_same_v<TReturn, jlong> &&
            !std::is_same_v<TReturn, jfloat> &&
            !std::is_same_v<TReturn, jdouble> &&
            !std::is_same_v<TReturn, JString> &&
            !std::is_same_v<TReturn, JObject>,
            TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return static_cast<TReturn>(env->CallObjectMethod(jniObject, methodId, Private::to_jni(std::forward<const TArgs&>(args))...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JString>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept;

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return JObject(invokeMethodReturn<jobject>(env, methodId, std::forward<const TArgs&>(args)...));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jboolean>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetBooleanField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jchar>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetCharField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jbyte>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetByteField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jshort>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetShortField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jint>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetIntField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jlong>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetLongField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetFloatField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetDoubleField(jniObject, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        !std::is_same_v<T, jboolean> &&
        !std::is_same_v<T, jbyte> &&
        !std::is_same_v<T, jchar> &&
        !std::is_same_v<T, jshort> &&
        !std::is_same_v<T, jint> &&
        !std::is_same_v<T, jlong> &&
        !std::is_same_v<T, jfloat> &&
        !std::is_same_v<T, jdouble> &&
        !std::is_same_v<T, JString> &&
        !std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return static_cast<T>(env->GetObjectField(jniObject, fieldId));
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JString>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept;

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return static_cast<JObject>(getFieldValue<jobject>(env, fieldId));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jboolean>,
                                  const T&
                              > value) noexcept
    {
        env->SetBooleanField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jchar>,
                                  const T&
                              > value) noexcept
    {
        env->SetCharField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jbyte>,
                                  const T&
                              > value) noexcept
    {
        env->SetByteField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jshort>,
                                  const T&
                              > value) noexcept
    {
        env->SetShortField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jint>,
                                  const T&
                              > value) noexcept
    {
        env->SetIntField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jlong>,
                                  const T&
                              > value) noexcept
    {
        env->SetLongField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jfloat>,
                                  const T&
                              > value) noexcept
    {
        env->SetFloatField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jdouble>,
                                  const T&
                              > value) noexcept
    {
        env->SetDoubleField(jniObject, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  !std::is_same_v<T, jboolean> &&
                                  !std::is_same_v<T, jbyte> &&
                                  !std::is_same_v<T, jchar> &&
                                  !std::is_same_v<T, jshort> &&
                                  !std::is_same_v<T, jint> &&
                                  !std::is_same_v<T, jlong> &&
                                  !std::is_same_v<T, jfloat> &&
                                  !std::is_same_v<T, jdouble> &&
                                  !std::is_same_v<T, JObject> &&
                                  !std::is_same_v<T, JString>,
                                  const T&
                              > value) noexcept
    {
        env->SetObjectField(jniObject, fieldId, static_cast<jobject>(value));
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
                                      std::is_same_v<T, JObject>,
                                      const T&
                              > value) noexcept
    {
        setFieldValue<jobject>(env, fieldId, static_cast<jobject>(value));
    }
};

/// @brief JObject with class name signature literal, use it with constexpr const char[] signature strings:
/// @code
///   constexpr const char java_lang_String[] = "java.lang.String";
///   JObjectS<java_lang_String> myString;
template<const char ClassName[]>
struct JObjectS : public JObject
{
    using JniType = jobject;

    /// @brief create empty JNI object wrapper
    JObjectS() = default;
    /// @brief wrap around an existing JNI object
    JObjectS(const jobject& initObject)
        : JObject(initObject)
    {}
    JObjectS(const JObjectS& other)
        : JObject(other)
    {}
    JObjectS& operator=(const JObjectS& other)
    {
        JObject::operator=(other);
        return *this;
    }
    JObjectS(JObjectS&& other)
        : JObject(std::move(other))
    {}
    JObjectS& operator=(JObjectS&& other)
    {
        JObject::operator=(std::move(other));
        return *this;
    }
    JObjectS(JObject&& other)
        : JObject(std::move(other))
    {}

    /// @brief Create a copy of this object with reference type of global ref
    JObjectS<ClassName> createGlobalRefS() const
    {
        auto env = JVM::getEnv();
        return JObjectS<ClassName> { createGlobalRef() };
    }

    /// @brief Create a copy of this object with reference type of weak global ref
    JObjectS<ClassName> createWeakGlobalRefS() const
    {
        auto env = JVM::getEnv();
        return JObjectS<ClassName> { createWeakGlobalRef() };
    }

    static constexpr const char* getClassName()
    {
        return ClassName;
    }
};

}

#endif // __GUSC_JOBJECT_HPP
