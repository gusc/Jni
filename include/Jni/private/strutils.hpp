#ifndef __GUSC_PRIVATE_STRUTILS_HPP
#define __GUSC_PRIVATE_STRUTILS_HPP 1

#include <utility>

namespace gusc::Jni::Private
{

template<std::size_t N>
struct char_string
{
    char str[N];
};

template<std::size_t ...Len>
constexpr auto concat(const char (&...strings)[Len])
{
    constexpr std::size_t N = (... + Len) - sizeof...(Len);
    char_string<N + 1> result = {};
    result.str[N] = '\0';
    char *dst = result.str;
    for (const char *src : {strings...})
    {
        for (; *src != '\0'; src++, dst++)
        {
            *dst = *src;
        }
    }
    return result;
}

constexpr std::size_t str_len(const char* str)
{
    return *str ? 1 + str_len(str + 1) : 0;
}

template<typename T>
constexpr auto get_class_path()
{
    constexpr auto str = std::decay_t<T>::getClassName();
    constexpr std::size_t N = str_len(str);
    char_string<N + 1> result = {};
    result.str[N] = '\0';
    char *dst = result.str;
    const char *src = str;
    for (; *src != '\0'; src++, dst++)
    {
        if (*src == '.')
        {
            *dst = '/';
        }
        else
        {
            *dst = *src;
        }
    }
    return result;
}

template<typename T>
constexpr auto get_element_class_path()
{
    constexpr auto str = std::decay_t<T>::getElementClassName();
    constexpr std::size_t N = str_len(str);
    char_string<N + 1> result = {};
    result.str[N] = '\0';
    char *dst = result.str;
    const char *src = str;
    for (; *src != '\0'; src++, dst++)
    {
        if (*src == '.')
        {
            *dst = '/';
        }
        else
        {
            *dst = *src;
        }
    }
    return result;
}

}

#endif // __GUSC_PRIVATE_STRUTILS_HPP
