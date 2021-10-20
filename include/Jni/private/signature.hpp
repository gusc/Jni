#ifndef __GUSC_PRIVATE_SIGNATURE_HPP
#define __GUSC_PRIVATE_SIGNATURE_HPP 1

#include "concat.hpp"
#include <jni.h>

namespace gusc::Jni::Private
{

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, void>,
    char_string<2>
>
getTypeSignature()
{
    return concat("V");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jboolean>,
    char_string<2>
>
getTypeSignature()
{
    return concat("Z");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jchar>,
    char_string<2>
>
getTypeSignature()
{
    return concat("C");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jbyte>,
    char_string<2>
>
getTypeSignature()
{
    return concat("B");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jshort>,
    char_string<2>
>
getTypeSignature()
{
    return concat("S");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jint>,
    char_string<2>
>
getTypeSignature()
{
    return concat("I");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jlong>,
    char_string<2>
>
getTypeSignature()
{
    return concat("J");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, jstring>,
    char_string<19>
>
getTypeSignature()
{
    return concat("Ljava/lang/String;");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jbooleanArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[Z");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jcharArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[C");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jbyteArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[B");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jshortArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[S");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jintArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[I");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jlongArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[J");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jfloatArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[F");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<std::decay_t<T>, jdoubleArray>,
        char_string<19>
>
getTypeSignature()
{
    return concat("[D");
}

inline constexpr auto getArgumentSignature()
{
    return concat("");
}

template<typename T, typename... TArgs>
inline constexpr auto getArgumentSignature(const T&, const TArgs&... args)
{
    return concat(getTypeSignature<T>().str, getArgumentSignature(std::forward<const TArgs&>(args)...).str);
}

}

#endif // __GUSC_PRIVATE_SIGNATURE_HPP
