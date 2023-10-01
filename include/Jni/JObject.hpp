#ifndef __GUSC_JOBJECT_HPP
#define __GUSC_JOBJECT_HPP 1

#include "private/concat.hpp"
#include "private/signature.hpp"

namespace gusc::Jni
{

class JClass;
class JString;

// TODO:
// 1. make JArray and JString objects extend from JObject
// 2. drop current ownership in favor of always creating a reference copy
// 3. make it possible to copy objects taking into account their reference type (global, local, weak)
// note: maybe 2&3 can be somehow combined with some optimization for when creating object from raw
//       jtype we do not copy reference, but if we copy from JObject to another then we do
// 4. instead of JGlobalRef create method in JObject to create global or weak references that just copies the object
class JObject final
{
public:
    /// @brief create empty JNI object wrapper
    JObject() = default;
    explicit JObject(const jobject& initObject)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(initObject) == JNIGlobalRefType)
        {
            obj = env->NewLocalRef(initObject);
        }
        else if (env->GetObjectRefType(initObject) == JNIWeakGlobalRefType)
        {
            obj = env->NewWeakGlobalRef(initObject);
        }
        else
        {
            obj = env->NewGlobalRef(initObject);
        }
    }
    JObject(const JObject& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(other.obj) == JNIGlobalRefType)
        {
            obj = env->NewLocalRef(other.obj);
        }
        else if (env->GetObjectRefType(other.obj) == JNIWeakGlobalRefType)
        {
            obj = env->NewWeakGlobalRef(other.obj);
        }
        else
        {
            obj = env->NewGlobalRef(other.obj);
        }
    }
    JObject& operator=(const JObject& other)
    {
        auto env = JVM::getEnv();
        if (env->GetObjectRefType(other.obj) == JNIGlobalRefType)
        {
            obj = env->NewLocalRef(other.obj);
        }
        else if (env->GetObjectRefType(other.obj) == JNIWeakGlobalRefType)
        {
            obj = env->NewWeakGlobalRef(other.obj);
        }
        else
        {
            obj = env->NewGlobalRef(other.obj);
        }
        return *this;
    }
    JObject(JObject&& other)
        : obj(other.obj)
    {
        std::swap(obj, other.obj);
    }
    JObject& operator=(JObject&& other)
    {
        dispose();
        std::swap(obj, other.obj);
        return *this;
    }
    ~JObject()
    {
        dispose();
    }

    /// @brief Create a copy of this object with reference type of global ref
    JObject createGlobalRef() const
    {
        auto env = JVM::getEnv();
        JObject other;
        other.obj = env->NewGlobalRef(obj);
        return other;
    }

    /// @brief Create a copy of this object with reference type of weak global ref
    JObject createWeakGlobalRef() const
    {
        auto env = JVM::getEnv();
        JObject other;
        other.obj = env->NewWeakGlobalRef(obj);
        return other;
    }

    inline operator bool() const
    {
        return obj != nullptr;
    }

    inline operator jobject() const
    {
        return obj;
    }

    /// @brief Release the wrapped object.
    /// @note Use this method to return an object from native method without it being destroyed by RAII
    inline jobject release()
    {
        jobject tmp { nullptr };
        std::swap(obj, tmp);
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
    jobject obj { nullptr };

    void dispose()
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
        return static_cast<TReturn>(env->CallObjectMethod(obj, methodId, std::forward<const TArgs&>(args)...));
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
        return static_cast<T>(env->GetObjectField(obj, fieldId));
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
                                      std::is_same_v<T, JObject>,
                                      const T&
                              > value) noexcept
    {
        setFieldValue<jobject>(env, fieldId, static_cast<jobject>(value));
    }
};

}

#endif // __GUSC_JOBJECT_HPP
