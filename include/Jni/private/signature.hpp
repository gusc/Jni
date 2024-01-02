#ifndef __GUSC_PRIVATE_SIGNATURE_HPP
#define __GUSC_PRIVATE_SIGNATURE_HPP 1

#include "concat.hpp"
#include <type_traits>

namespace gusc::Jni
{
class JString;
}

namespace gusc::Jni::Private
{

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, void>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("V");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jboolean>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("Z");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jchar>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("C");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jbyte>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("B");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jshort>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("S");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jint>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("I");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jlong>,
    char_string<2>
>
getJTypeSignature()
{
    return concat("J");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<T, jfloat>,
        char_string<2>
>
getJTypeSignature()
{
    return concat("F");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
        std::is_same_v<T, jdouble>,
        char_string<2>
>
getJTypeSignature()
{
    return concat("D");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<T, jstring> ||
    std::is_same_v<T, JString>,
    char_string<19>
>
getJTypeSignature()
{
    return concat("Ljava/lang/String;");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jbooleanArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[Z");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jcharArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[C");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jbyteArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[B");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jshortArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[S");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jintArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[I");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jlongArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[J");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jfloatArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[F");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::decay_t<jdoubleArray>>,
    char_string<3>
>
getJTypeSignature()
{
    return concat("[D");
}

template<typename... TArgs>
inline constexpr auto getArgumentSignature()
{
    return concat(getJTypeSignature<TArgs>().str...);
}

template<>
inline constexpr auto getArgumentSignature()
{
    return concat("");
}

template<typename TReturn, typename... TArgs>
inline constexpr auto getMethodSignature()
{
    constexpr const auto argSign = Private::getArgumentSignature<TArgs...>();
    constexpr const auto retSign = Private::getJTypeSignature<TReturn>();
    return Private::concat("(", argSign.str, ")", retSign.str);
}

}

#endif // __GUSC_PRIVATE_SIGNATURE_HPP
