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
    JArray(JEnv& env, TJni initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = getDataPtr<TJni>(env);
        }
    }
    JArray(const JArray<TCpp>& other) = delete;
    JArray& operator = (const JArray<TCpp>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            freeDataPtr<TJni>();
        }
    }
    inline operator TCpp()
    {
        if (dataPtr)
        {
            return TCpp(dataPtr, dataPtr + length);
        }
        return {};
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewByteArray(vector.size());
        env->SetByteArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewShortArray(vector.size());
        env->SetShortArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewIntArray(vector.size());
        env->SetIntArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewLongArray(vector.size());
        env->SetLongArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewFloatArray(vector.size());
        env->SetFloatArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewDoubleArray(vector.size());
        env->SetDoubleArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

    template<typename T=TJni>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, TJni>
    createFrom(const TCpp& vector)
    {
        auto env = JVM::getEnv();
        TJni a = env->NewBooleanArray(vector.size());
        env->SetBooleanArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJniEl*>(vector.data()));
        return a;
    }

private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    TJni array { nullptr };
    TJniEl* dataPtr { nullptr };

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetByteArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetShortArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetIntArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetLongArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetFloatArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetDoubleArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, TJniEl*>
    getDataPtr(JEnv& env)
    {
        return env->GetBooleanArrayElements(array, &isCopy);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseByteArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseShortArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseIntArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseLongArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseFloatArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseDoubleArrayElements(array, dataPtr, JNI_ABORT);
    }

    template<typename T>
    inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, void>
    freeDataPtr(JEnv& env)
    {
        env->ReleaseBooleanArrayElements(array, dataPtr, JNI_ABORT);
    }

};

using JByteArray = JArray<>;
using JShortArray = JArray<std::vector<std::int16_t>, jshortArray, jshort>;
using JIntArray = JArray<std::vector<std::int32_t>, jintArray, jint>;
using JLongArray = JArray<std::vector<std::int64_t>, jlongArray, jlong>;
using JFloatArray = JArray<std::vector<float>, jfloatArray, jfloat>;
using JDoubleArray = JArray<std::vector<double>, jdoubleArray, jdouble>;
using JBooleanArray = JArray<std::vector<bool>, jbooleanArray, jboolean>;

}

#endif // __GUSC_JARRAY_HPP
