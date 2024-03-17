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

template<typename T, typename = std::void_t<>>
struct is_jni_primitive : std::false_type {};

template<typename T>
struct is_jni_primitive<T,
    typename std::enable_if_t<
        std::is_same_v<T, jboolean> ||
        std::is_same_v<T, jbyte> ||
        std::is_same_v<T, jchar> ||
        std::is_same_v<T, jshort> ||
        std::is_same_v<T, jint> ||
        std::is_same_v<T, jlong> ||
        std::is_same_v<T, jfloat> ||
        std::is_same_v<T, jdouble>,
        void
    >
> : std::true_type {};

template<typename T>
inline constexpr bool is_jni_primitive_v = is_jni_primitive<T>::value;

template<typename T, typename = std::void_t<>>
struct is_jni_array : std::false_type {};

template<typename T>
struct is_jni_array<T,
    typename std::enable_if_t<
        std::is_same_v<T, jbooleanArray> ||
        std::is_same_v<T, jbyteArray> ||
        std::is_same_v<T, jcharArray> ||
        std::is_same_v<T, jshortArray> ||
        std::is_same_v<T, jintArray> ||
        std::is_same_v<T, jlongArray> ||
        std::is_same_v<T, jfloatArray> ||
        std::is_same_v<T, jdoubleArray> ||
        std::is_same_v<T, jobjectArray>,
        void
    >
> : std::true_type {};

template<typename T>
inline constexpr bool is_jni_array_v = is_jni_array<T>::value;

template<typename T, typename = std::void_t<>>
struct is_jni_object : std::false_type {};

template<typename T>
struct is_jni_object<T,
    typename std::enable_if_t<
        std::is_same_v<T, jobject> ||
        std::is_same_v<T, jclass> ||
        std::is_same_v<T, jstring>,
        void
    >
> : std::true_type {};

template<typename T>
inline constexpr bool is_jni_object_v = is_jni_object<T>::value;

template<typename T, typename = std::void_t<>>
struct is_jni : std::false_type {};

template<typename T>
struct is_jni<T,
    typename std::enable_if_t<
        is_jni_primitive_v<T> ||
        is_jni_array_v<T> ||
        is_jni_object_v<T>,
        void
    >
> : std::true_type {};

template<typename T>
inline constexpr bool is_jni_v = is_jni<T>::value;

template<typename T>
inline
typename std::enable_if_t<
    is_jni_v<std::decay_t<T>>,
    T&&
> to_jni(T&& in)
{
    return std::forward<T>(in);
}

template<typename T>
inline
typename std::enable_if_t<
    is_jni_v<typename std::decay_t<T>::JniType>,
    typename std::decay_t<T>::JniType
> to_jni(T&& in)
{
    return static_cast<typename std::decay_t<T>::JniType>(in);
}

}

#endif //__GUSC_PRIVATE_CAST_HPP
