#ifnedf __GUSC_PRIVATE_CONCAT_HPP
#define __GUSC_PRIVATE_CONCAT_HPP 1

#include <utility>

template <std::size_t N>
struct char_string {
    char str[N];
};

template<std::size_t ...Len>
constexpr auto concat(const char (&...strings)[Len]) {
    constexpr std::size_t N = (... + Len) - sizeof...(Len);
    char_string<N + 1> result = {};
    result.str[N] = '\0';
    char* dst = result.str;
    for (const char* src : {strings...}) {
        for (; *src != '\0'; src++, dst++) {
            *dst = *src;
        }
    }
    return result;
}

#endif // __GUSC_PRIVATE_CONCAT_HPP
