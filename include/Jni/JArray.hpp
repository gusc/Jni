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
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = getDataPtr<TJni>(env);
        }
    }
    JArray(TJni initArray) : JArray(JVM::getEnv(), initArray)
    {}
    JArray(const JArray<TCpp>& other) = delete;
    JArray& operator = (const JArray<TCpp>& other) = delete;
    JArray(JArray&& other) = default;
    JArray& operator=(JArray&& other)
    {
        dispose();
        isCopy = other.isCopy;
        length = other.length;
        array = other.array;
        dataPtr = other.dataPtr;
        other.isCopy = JNI_FALSE;
        other.length = 0;
        other.array = nullptr;
        other.dataPtr = nullptr;
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
        return array;
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
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    TJni array { nullptr };
    TJniEl* dataPtr { nullptr };

    void dispose()
    {
        if (array && isCopy == JNI_TRUE)
        {
            auto env = JVM::getEnv();
            freeDataPtr<TJni>(env);
        }
    }

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
