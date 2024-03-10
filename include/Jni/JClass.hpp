#ifndef __GUSC_JCLASS_HPP
#define __GUSC_JCLASS_HPP 1

#include <jni.h>
#include "JEnv.hpp"
#include "JObject.hpp"
#include "JString.hpp"
#include "private/cast.hpp"
#include "private/strutils.hpp"
#include "private/signature.hpp"
#include <type_traits>

namespace gusc::Jni
{

class JClass
{
public:
    using JniType = jclass;

    JClass(jclass initClass)
    {
        copy(initClass);
    }
    JClass(const JEnv& /*initEnv*/, jclass initClass)
        : JClass { initClass }
    {}
    JClass(const JClass& other)
    {
        dispose();
        copy(other.jniClass);
    }
    inline operator jclass()
    {
        return jniClass;
    }
    JClass& operator=(const JClass& other)
    {
        dispose();
        copy(other.jniClass);
        return *this;
    }
    JClass(JClass&& other)
    {
        dispose();
        std::swap(jniClass, other.jniClass);
    }
    JClass& operator=(JClass&& other)
    {
        dispose();
        std::swap(jniClass, other.jniClass);
        return *this;
    }
    ~JClass()
    {
        dispose();
    }

    /// @brief Create a copy of this object with reference type of global ref
    JClass createGlobalRef() const
    {
        auto env = JVM::getEnv();
        return JClass { static_cast<jclass>(env->NewGlobalRef(jniClass)) };
    }

    /// @brief Create a copy of this object with reference type of weak global ref
    JClass createWeakGlobalRef() const
    {
        auto env = JVM::getEnv();
        return JClass { static_cast<jclass>(env->NewWeakGlobalRef(jniClass)) };
    }
    
    inline std::string getClassPath() const noexcept
    {
        auto env = JVM::getEnv();
        jmethodID getNameId = getMethodIdSign("getName", "()Ljava/lang/String;");
        std::string className = JString(static_cast<jstring>(env->CallObjectMethod(jniClass, getNameId)));
        std::size_t pos = 0;
        while ((pos = className.find('.', pos)) != std::string::npos)
        {
            className.replace(pos, 1, "/");
            ++pos;
        }
        return className;
    }
    
    inline jmethodID getStaticMethodIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        auto methodId = env->GetStaticMethodID(jniClass, name.c_str(), signature.c_str());
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find static method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    template<typename TReturn, typename... TArgs>
    inline jmethodID getStaticMethodId(const std::string& name) const
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodId(name, sign.str);
    }
    
    inline jmethodID getMethodIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        auto methodId = env->GetMethodID(jniClass, name.c_str(), signature.c_str());
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find instance method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    template<typename TReturn, typename... TArgs>
    inline jmethodID getMethodId(const std::string& name) const
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        return getMethodIdSign(name, sign.str);
    }

    inline jfieldID getStaticFieldIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        auto fieldId = env->GetStaticFieldID(jniClass, name.c_str(), signature.c_str());
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find static field ") + name + " with signature " + signature);
        }
        return fieldId;
    }

    template<typename T>
    inline jfieldID getStaticFieldId(const std::string& name) const
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getStaticFieldIdSign(name, sign.str);
    }
    
    inline jfieldID getFieldIdSign(const std::string& name, const std::string& signature) const
    {
        auto env = JVM::getEnv();
        auto fieldId = env->GetFieldID(jniClass, name.c_str(), signature.c_str());
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find instance field ") + name + " with signature " + signature);
        }
        return fieldId;
    }

    template<typename T>
    inline jfieldID getFieldId(const std::string& name) const
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        return getFieldIdSign(name, sign.str);
    }

    template<typename TReturn, typename... TArgs>
    inline void registerNativeMethodSign(const std::string& name, const std::string& signature, TReturn(*fn)(JNIEnv*, jobject, TArgs...))
    {
        auto env = JVM::getEnv();
        const JNINativeMethod methodsArray[] = {
            {name.c_str(), signature.c_str(), Private::void_cast(fn)}
        };
        if (env->RegisterNatives(jniClass, methodsArray, sizeof(methodsArray) / sizeof(methodsArray[0])) < 0)
        {
            throw std::runtime_error(std::string("Failed to register native method ") + name + " with signature " + signature);
        }
    }

    template<typename TReturn, typename... TArgs>
    inline void registerNativeMethod(const std::string& name, TReturn(*fn)(JNIEnv*, jobject, TArgs...))
    {
        constexpr auto sign = Private::getMethodSignature<TReturn, TArgs...>();
        registerNativeMethodSign<TReturn>(name, sign.str, fn);
    }

    template<typename... TArgs>
    JObject createObjectJni(JEnv& env, jmethodID methodId, const TArgs&... args) const
    {
        auto obj = env->NewObject(jniClass, methodId, std::forward<const TArgs&>(args)...);
        if (!obj)
        {
            throw std::runtime_error(std::string("Failed to reate Java object "));
        }
        return static_cast<JObject>(obj);
    }

    template<typename... TArgs>
    JObject createObjectSign(const std::string& signature, const TArgs&... args) const
    {
        auto env = JVM::getEnv();
        const auto methodId = getMethodIdSign("<init>", signature);
        return createObjectJni(env, methodId, std::forward<const TArgs&>(args)...);
    }

    template<typename... TArgs>
    JObject createObject(const TArgs&... args) const
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
        const auto methodId = getStaticMethodIdSign(name, signature);
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
        const auto methodId = getStaticMethodIdSign(name, signature);
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
        const auto fieldId = getStaticFieldIdSign(name, signature);
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
        const auto fieldId = getStaticFieldIdSign(name, signature);
        setFieldJni<T>(env, fieldId, std::forward<const T&>(value));
    }

    template<typename T>
    void setField(const std::string& name, const T& value)
    {
        constexpr auto sign = Private::getJTypeSignature<T>();
        setFieldSign<T>(name, sign.str, std::forward<const T&>(value));
    }
    
protected:
    jclass jniClass {nullptr };

    void copy(jclass initClass)
    {
        if (initClass)
        {
            auto env = JVM::getEnv();
            if (env->GetObjectRefType(initClass) == JNIGlobalRefType)
            {
                jniClass = static_cast<jclass>(env->NewGlobalRef(initClass));
            }
            else if (env->GetObjectRefType(jniClass) == JNIWeakGlobalRefType)
            {
                jniClass = static_cast<jclass>(env->NewWeakGlobalRef(initClass));
            }
            else
            {
                jniClass = static_cast<jclass>(env->NewLocalRef(initClass));
            }
        }
    }

    void dispose()
    {
        if (jniClass)
        {
            auto env = JVM::getEnv();
            if (env->GetObjectRefType(jniClass) == JNIGlobalRefType)
            {
                env->DeleteGlobalRef(jniClass);
            }
            else if (env->GetObjectRefType(jniClass) == JNIWeakGlobalRefType)
            {
                env->DeleteWeakGlobalRef(jniClass);
            }
            else
            {
                env->DeleteLocalRef(jniClass);
            }
            jniClass = nullptr;
        }
    }

    inline void invokeMethodReturnVoid(JEnv& env, jmethodID methodId) const noexcept
    {
        env->CallStaticVoidMethod(jniClass, methodId);
    }

    template<typename... TArgs>
    inline
    void invokeMethodReturnVoid(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        env->CallStaticVoidMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jboolean>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticBooleanMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jchar>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticCharMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jbyte>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticByteMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jshort>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticShortMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jint>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticIntMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jlong>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticLongMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jfloat>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticFloatMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, jdouble>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return env->CallStaticDoubleMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...);
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
        return static_cast<TReturn>(env->CallStaticObjectMethod(jniClass, methodId, Private::to_jni(std::forward<const TArgs&>(args))...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
            std::is_same_v<TReturn, JString>,
            TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return JString(invokeMethodReturn<jstring>(env, methodId, Private::to_jni(std::forward<const TArgs&>(args))...));
    }

    template<typename TReturn, typename... TArgs>
    inline
    typename std::enable_if_t<
        std::is_same_v<TReturn, JObject>,
        TReturn
    >
    invokeMethodReturn(JEnv& env, jmethodID methodId, const TArgs&... args) const noexcept
    {
        return JObject(invokeMethodReturn<jobject>(env, methodId, Private::to_jni(std::forward<const TArgs&>(args))...), true);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jboolean>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticBooleanField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jchar>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticCharField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jbyte>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticByteField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jshort>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticShortField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jint>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticIntField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jlong>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticLongField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticFloatField(jniClass, fieldId);
    }

    template<typename T>
    inline
    typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept
    {
        return env->GetStaticDoubleField(jniClass, fieldId);
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
        return static_cast<T>(env->GetStaticObjectField(jniClass, fieldId));
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
        std::is_same_v<T, JObject>,
        T
    >
    getFieldValue(JEnv& env, jfieldID fieldId) const noexcept {
        return static_cast<JObject>(getFieldValue<jobject>(env, fieldId));
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jboolean>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticBooleanField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jchar>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticCharField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jbyte>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticByteField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jshort>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticShortField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jint>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticIntField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jlong>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticLongField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jfloat>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticFloatField(jniClass, fieldId, value);
    }

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, jdouble>,
                                  const T&
                              > value) noexcept
    {
        env->SetStaticDoubleField(jniClass, fieldId, value);
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
        env->SetStaticObjectField(jniClass, fieldId, static_cast<jobject>(value));
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

    template<typename T>
    inline void setFieldValue(JEnv& env, jfieldID fieldId,
                              typename std::enable_if_t<
                                  std::is_same_v<T, JString>,
                                  const T&
                              > value) noexcept
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
    return JClass(*this, cls).createGlobalRef();
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
        env->ExceptionClear();
        auto message = ex.invokeMethod<JString>("getMessage");
        throw std::runtime_error(std::string("JNI Exception occured: ") + static_cast<std::string>(message));
    }
}

inline jmethodID JObject::getMethodIdJni(JEnv& env, const std::string& name, const std::string& signature) const
{
    return getClass(env).getMethodIdSign(name, signature);
}

inline jfieldID JObject::getFieldIdJni(JEnv& env, const std::string& name, const std::string& signature) const
{
    return getClass(env).getFieldIdSign(name, signature);
}

inline JClass JObject::getClass(JEnv& env) const noexcept
{
    return env.getObjectClass(jniObject);
}

inline JClass JObject::getClass() const noexcept
{
    auto env = JVM::getEnv();
    return getClass(env);
}

/// @brief JClass with class name signature literal, use it with constexpr const char[] signature strings:
/// @code
///   constexpr const char java_lang_String[] = "java.lang.String";
///   JClassS<java_lang_String> myStringClass;
template<const char ClassName[]>
struct JClassS : public JClass
{
    /// @brief create empty JNI class wrapper
    JClassS()
        : JClass(JVM::getEnv().getClass(Private::get_class_path<JClassS<ClassName>>().str).createGlobalRef())
    {}
    /// @brief wrap around an existing JNI object
    JClassS(JEnv env, const jclass& initClass)
        : JClass(env, initClass)
    {}
    JClassS(const JClassS& other) = delete;
    JClassS& operator=(const JClassS& other) = delete;

    template<typename... TArgs>
    JObjectS<ClassName> createObjectS(const TArgs&... args) const
    {
        return JObjectS<ClassName> { createObject(std::forward<const TArgs&>(args)...) };
    }

    /// @brief Create a copy of this object with reference type of global ref
    JClassS<ClassName> createGlobalRefS() const
    {
        auto env = JVM::getEnv();
        return JClassS<ClassName> { createGlobalRef() };
    }

    /// @brief Create a copy of this object with reference type of weak global ref
    JClassS<ClassName> createWeakGlobalRefS() const
    {
        auto env = JVM::getEnv();
        return JClassS<ClassName> { createWeakGlobalRef() };
    }

    static constexpr const char* getClassName()
    {
        return ClassName;
    }
};

}

#endif // __GUSC_JCLASS_HPP
