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
    std::is_same_v<std::decay_t<T>, jobject>,
    T&&
> to_jni(T&& in)
{
    return std::forward<T>(in);
}

template<typename T>
inline
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, JClass> ||
    std::is_same_v<std::decay_t<T>, JString> ||
    std::is_same_v<std::decay_t<T>, JByteArray> ||
    std::is_same_v<std::decay_t<T>, JCharArray> ||
    std::is_same_v<std::decay_t<T>, JShortArray> ||
    std::is_same_v<std::decay_t<T>, JIntArray> ||
    std::is_same_v<std::decay_t<T>, JLongArray> ||
    std::is_same_v<std::decay_t<T>, JFloatArray> ||
    std::is_same_v<std::decay_t<T>, JDoubleArray> ||
    std::is_same_v<std::decay_t<T>, JBooleanArray>,
    typename T::JniType
> to_jni(T&& in)
{
    return static_cast<typename T::JniType>(in);
}

template<typename T>
inline
typename std::enable_if_t<
        !std::is_same_v<std::decay_t<T>, jboolean> &&
        !std::is_same_v<std::decay_t<T>, jbyte> &&
        !std::is_same_v<std::decay_t<T>, jchar> &&
        !std::is_same_v<std::decay_t<T>, jshort> &&
        !std::is_same_v<std::decay_t<T>, jint> &&
        !std::is_same_v<std::decay_t<T>, jlong> &&
        !std::is_same_v<std::decay_t<T>, jfloat> &&
        !std::is_same_v<std::decay_t<T>, jdouble> &&
        !std::is_same_v<std::decay_t<T>, jstring> &&
        !std::is_same_v<std::decay_t<T>, jbooleanArray> &&
        !std::is_same_v<std::decay_t<T>, jbyteArray> &&
        !std::is_same_v<std::decay_t<T>, jcharArray> &&
        !std::is_same_v<std::decay_t<T>, jshortArray> &&
        !std::is_same_v<std::decay_t<T>, jintArray> &&
        !std::is_same_v<std::decay_t<T>, jlongArray> &&
        !std::is_same_v<std::decay_t<T>, jfloatArray> &&
        !std::is_same_v<std::decay_t<T>, jdoubleArray> &&
        !std::is_same_v<std::decay_t<T>, jobjectArray> &&
        !std::is_same_v<std::decay_t<T>, jobject> &&
        !std::is_same_v<std::decay_t<T>, JClass> &&
        !std::is_same_v<std::decay_t<T>, JString> &&
        !std::is_same_v<std::decay_t<T>, JByteArray> &&
        !std::is_same_v<std::decay_t<T>, JCharArray> &&
        !std::is_same_v<std::decay_t<T>, JShortArray> &&
        !std::is_same_v<std::decay_t<T>, JIntArray> &&
        !std::is_same_v<std::decay_t<T>, JLongArray> &&
        !std::is_same_v<std::decay_t<T>, JFloatArray> &&
        !std::is_same_v<std::decay_t<T>, JDoubleArray> &&
        !std::is_same_v<std::decay_t<T>, JBooleanArray>,
        jobject
> to_jni(T&& in)
{
    return static_cast<jobject>(in);
}

}

#endif //__GUSC_PRIVATE_CAST_HPP
