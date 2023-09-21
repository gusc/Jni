#ifndef __GUSC_JARRAY_HPP
#define __GUSC_JARRAY_HPP 1

#include <jni.h>
#include "JVM.hpp"
#include "JEnv.hpp"

namespace gusc::Jni
{

template<typename TCpp=std::vector<std::int8_t>, typename TJni=jbyteArray, typename TJniEl=jbyte>
class JArray
{
public:
    JArray(JEnv env, TJni initArray)
        : jniArray(initArray)
    {
        init(env);
    }
    JArray(TJni initArray)
        : JArray(JVM::getEnv(), initArray)
    {}
    JArray(JEnv env, std::size_t initSize)
    {
        create<TJni>(env, initSize);
        init(env);
    }
    JArray(std::size_t initSize)
        : JArray(JVM::getEnv(), initSize)
    {}
    JArray(const JArray<TCpp>& other) = delete;
    JArray& operator = (const JArray<TCpp>& other) = delete;
    JArray(JArray&& other) = default;
    JArray& operator=(JArray&& other)
    {
        dispose();
        jniArray = nullptr;
        std::swap(jniArray, other.jniArray);
        std::swap(length, other.length);
        std::swap(dataPtr, other.dataPtr);
        return *this;
    }
    ~JArray()
    {
        dispose();
    }
    inline operator TCpp()
    {
        if (dataPtr)
        {
            return TCpp(dataPtr, dataPtr + length);
        }
        return {};
    }
    inline operator TJni()
    {
        return jniArray;
    }
    inline operator TJni() const
    {
        return jniArray;
    }
    inline TJniEl* data()
    {
        return dataPtr;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewByteArray(vector.size());
        env->SetByteArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewCharArray(vector.size());
        env->SetCharArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewShortArray(vector.size());
        env->SetShortArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewIntArray(vector.size());
        env->SetIntArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewLongArray(vector.size());
        env->SetLongArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewFloatArray(vector.size());
        env->SetFloatArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewDoubleArray(vector.size());
        env->SetDoubleArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, TJni>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJni a = env->NewBooleanArray(vector.size());
        env->SetBooleanArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    static inline auto createFrom(const TCpp& vector)
    {
        return createFrom(JVM::getEnv(), vector);
    }

private:
    std::size_t length { 0 };
    TJni jniArray { nullptr };
    TJniEl* dataPtr { nullptr };

    void init(JEnv env)
    {
        if (jniArray)
        {
            length = env->GetArrayLength(jniArray);
            dataPtr = getDataPtr<TJni>(env);
        }
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewByteArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewCharArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewShortArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewIntArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewLongArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewFloatArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewDoubleArray(static_cast<jsize>(initSize));
    }
    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, void>
    create(JEnv env, std::size_t initSize)
    {
        jniArray = env->NewBooleanArray(static_cast<jsize>(initSize));
    }

    void dispose()
    {
        if (jniArray && dataPtr)
        {
            auto env = JVM::getEnv();
            freeDataPtr<TJni>(env);
            length = 0;
            dataPtr = nullptr;
        }
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetCharArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetByteArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetShortArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetIntArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetLongArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetFloatArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetDoubleArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetBooleanArrayElements(jniArray, nullptr);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseByteArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseCharArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseShortArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseIntArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseLongArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseFloatArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseDoubleArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseBooleanArrayElements(jniArray, dataPtr, JNI_ABORT);
    }

};

using JByteArray = JArray<>;
using JCharArray = JArray<std::vector<char>, jcharArray, jchar>;
using JShortArray = JArray<std::vector<std::int16_t>, jshortArray, jshort>;
using JIntArray = JArray<std::vector<std::int32_t>, jintArray, jint>;
using JLongArray = JArray<std::vector<std::int64_t>, jlongArray, jlong>;
using JFloatArray = JArray<std::vector<float>, jfloatArray, jfloat>;
using JDoubleArray = JArray<std::vector<double>, jdoubleArray, jdouble>;
// Can't use std::vector<bool> as it's a bitset not an array
using JBooleanArray = JArray<std::vector<char>, jbooleanArray, jboolean>;
// JObjectArray will probably need it's own class, because it depends on the element class
//using JObjectArray = JArray<std::vector<JObject>, jobjectArray , jobject>;

}

#endif // __GUSC_JARRAY_HPP
