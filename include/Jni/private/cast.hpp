#ifndef __GUSC_PRIVATE_CAST_HPP
#define __GUSC_PRIVATE_CAST_HPP

#include "forward.hpp"

namespace gusc::Jni::Private
{

template<typename T>
inline void* void_cast(T& Value)
{
    return reinterpret_cast<void*>(Value);
}

template<typename T>
inline
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jboolean> ||
    std::is_same_v<std::decay_t<T>, jbyte> ||
    std::is_same_v<std::decay_t<T>, jchar> ||
    std::is_same_v<std::decay_t<T>, jshort> ||
    std::is_same_v<std::decay_t<T>, jint> ||
    std::is_same_v<std::decay_t<T>, jlong> ||
    std::is_same_v<std::decay_t<T>, jfloat> ||
    std::is_same_v<std::decay_t<T>, jdouble> ||
    std::is_same_v<std::decay_t<T>, jstring> ||
    std::is_same_v<std::decay_t<T>, jbooleanArray> ||
    std::is_same_v<std::decay_t<T>, jbyteArray> ||
    std::is_same_v<std::decay_t<T>, jcharArray> ||
    std::is_same_v<std::decay_t<T>, jshortArray> ||
    std::is_same_v<std::decay_t<T>, jintArray> ||
    std::is_same_v<std::decay_t<T>, jlongArray> ||
    std::is_same_v<std::decay_t<T>, jfloatArray> ||
    std::is_same_v<std::decay_t<T>, jdoubleArray> ||
    std::is_same_v<std::decay_t<T>, jobjectArray> ||
    std::is_same_v<std::decay_t<T>, jobject> ||
    std::is_same_v<std::decay_t<T>, jclass>,
    T&&
> to_jni(T&& in)
{
    return std::forward<T>(in);
}

template<typename T>
inline
typename std::enable_if_t<
    std::is_same_v<typename std::decay_t<T>::JniType, jboolean> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jbyte> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jchar> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jshort> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jint> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jlong> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jfloat> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jdouble> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jstring> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jbooleanArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jbyteArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jcharArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jshortArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jintArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jlongArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jfloatArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jdoubleArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jobjectArray> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jobject> ||
    std::is_same_v<typename std::decay_t<T>::JniType, jclass>,
    typename std::decay_t<T>::JniType
> to_jni(T&& in)
{
    return static_cast<typename std::decay_t<T>::JniType>(in);
}

}

#endif //__GUSC_PRIVATE_CAST_HPP
