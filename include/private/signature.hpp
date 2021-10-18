#ifndef __GUSC_PRIVATE_SIGNATURE_HPP
#define __GUSC_PRIVATE_SIGNATURE_HPP 1

#include "concat.hpp"

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
    std::is_same_v<std::decay_t<T>, bool>,
    char_string<2>
>
getTypeSignature()
{
    return concat("Z");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, char>,
    char_string<2>
>
getTypeSignature()
{
    return concat("C");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::int8_t> ||
    std::is_same_v<std::decay_t<T>, std::uint8_t>,
    char_string<2>
>
getTypeSignature()
{
    return concat("B");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::int16_t> ||
    std::is_same_v<std::decay_t<T>, std::uint16_t>,
    char_string<2>
>
getTypeSignature()
{
    return concat("S");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::int32_t> ||
    std::is_same_v<std::decay_t<T>, std::uint32_t>,
    char_string<2>
>
getTypeSignature()
{
    return concat("I");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::int64_t> ||
    std::is_same_v<std::decay_t<T>, std::uint64_t> ||
    std::is_same_v<std::decay_t<T>, long> ||
    std::is_same_v<std::decay_t<T>, unsigned long> ||
    std::is_same_v<std::decay_t<T>, long long> ||
    std::is_same_v<std::decay_t<T>, unsigned long long>,
    char_string<2>
>
getTypeSignature()
{
    return concat("J");
}

template<typename T>
inline constexpr
typename std::enable_if_t<
    std::is_same_v<std::decay_t<T>, std::string> ||
    std::is_same_v<std::decay_t<T>, char*> ||
    std::is_same_v<std::decay_t<T>, const char*>,
    char_string<19>
>
getTypeSignature()
{
    return concat("Ljava/lang/String;");
}

inline constexpr auto getArgumentSignature()
{
    return concat("");
}

template<typename T, typename... TArgs>
constexpr auto getArgumentSignature(const T&, const TArgs&... args)
{
    return concat(getTypeSignature<T>().str, getArgumentSignature(std::forward<const TArgs&>(args)...).str);
}

}

#endif // __GUSC_PRIVATE_SIGNATURE_HPP
