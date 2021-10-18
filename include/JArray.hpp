#ifndef __GUSC_JARRAY_HPP
#define __GUSC_JARRAY_HPP 1

#include <jni.h>
#include "JVM.hpp"
#include "JEnv.hpp"

namespace gusc::Jni
{

template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::vector<std::int8_t>>::type>
class JArray
{
public:
    JArray(JEnv& env, jbyteArray initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = env->GetByteArrayElements(array, &isCopy);
        }
    }
    JArray(const JArray<T>& other) = delete;
    JArray& operator = (const JArray<T>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            env->ReleaseByteArrayElements(array, dataPtr, JNI_ABORT);
        }
    }
    operator T()
    {
        if (dataPtr)
        {
            return T(dataPtr, dataPtr + length);
        }
        return {};
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jbyteArray array { nullptr };
    jbyte* dataPtr { nullptr };
};

template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::vector<std::int16_t>>::type>
class JArray
{
public:
    JArray(JEnv& env, jshortArray initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = env->GetShortArrayElements(array, &isCopy);
        }
    }
    JArray(const JArray<T>& other) = delete;
    JArray& operator = (const JArray<T>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            env->ReleaseShortArrayElements(array, dataPtr, JNI_ABORT);
        }
    }
    operator T()
    {
        if (dataPtr)
        {
            return T(dataPtr, dataPtr + length);
        }
        return {};
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jshortArray array { nullptr };
    jshort* dataPtr { nullptr };
};

template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::vector<std::int32_t>>::type>
class JArray
{
public:
    JArray(JEnv& env, jintArray initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = env->GetIntArrayElements(array, &isCopy);
        }
    }
    JArray(const JArray<T>& other) = delete;
    JArray& operator = (const JArray<T>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            env->ReleaseIntArrayElements(array, dataPtr, JNI_ABORT);
        }
    }
    operator T()
    {
        if (dataPtr)
        {
            return T(dataPtr, dataPtr + length);
        }
        return {};
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jintArray array { nullptr };
    jint* dataPtr { nullptr };
};

template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::vector<std::int64_t>>::type>
class JArray
{
public:
    JArray(JEnv& env, jlongArray initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = env->GetLongArrayElements(array, &isCopy);
        }
    }
    JArray(const JArray<T>& other) = delete;
    JArray& operator = (const JArray<T>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            env->ReleaseLongArrayElements(array, dataPtr, JNI_ABORT);
        }
    }
    operator T()
    {
        if (dataPtr)
        {
            return T(dataPtr, dataPtr + length);
        }
        return {};
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jlongArray array { nullptr };
    jlong* dataPtr { nullptr };
};

template<typename T, typename = typename std::enable_if<std::is_same_v<T, std::vector<float>>::type>
class JArray
{
public:
    JArray(JEnv& env, jfloatArray initArray)
        : array(initArray)
    {
        if (array)
        {
            length = env->GetArrayLength(array);
            dataPtr = env->GetFloatArrayElements(array, &isCopy);
        }
    }
    JArray(const JArray<T>& other) = delete;
    JArray& operator = (const JArray<T>& other) = delete;
    ~JArray()
    {
        const auto env = JVM::getEnv();
        if (array && isCopy == JNI_TRUE)
        {
            env->ReleaseFloatArrayElements(array, dataPtr, JNI_ABORT);
        }
    }
    operator T()
    {
        if (dataPtr)
        {
            return T(dataPtr, dataPtr + length);
        }
        return {};
    }
private:
    jboolean isCopy { JNI_FALSE };
    std::size_t length { 0 };
    jfloatArray array { nullptr };
    jfloat* dataPtr { nullptr };
};

}

#endif // __GUSC_JARRAY_HPP
