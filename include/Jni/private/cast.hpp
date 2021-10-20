#ifndef __GUSC_PRIVATE_CAST_HPP
#define __GUSC_PRIVATE_CAST_HPP

namespace gusc::Jni::Private
{

template<typename T>
inline void* void_cast(T& Value)
{
    return reinterpret_cast<void*>(Value);
}

}

#endif //__GUSC_PRIVATE_CAST_HPP
