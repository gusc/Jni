#ifndef __GUSC_JGLOBALREF_HPP
#define __GUSC_JGLOBALREF_HPP

#include <jni.h>
#include "JVM.hpp"
#include "JObject.hpp"

namespace gusc::Jni
{

class JGlobalRef final
{
public:
    /// @brief create global reference to JNI object and wrap it
    /// @note this automatically assumes the ownership of the global reference object
    JGlobalRef(jobject initObj)
        : obj(JVM::getEnv()->NewGlobalRef(initObj), true)
    {}
    JGlobalRef(const JGlobalRef&) = delete;
    JGlobalRef& operator=(const JGlobalRef&) = delete;
    JGlobalRef(JGlobalRef&&) = default;
    JGlobalRef& operator=(JGlobalRef&&) = default;
    ~JGlobalRef() = default;

    inline operator jobject()
    {
        return static_cast<jobject>(obj);
    }
    inline JObject* operator->()
    {
        return &obj;
    }
    inline JObject& operator*()
    {
        return obj;
    }
    inline const JObject* operator->() const
    {
        return &obj;
    }
private:
    JObject obj;
};

}

#endif //__GUSCJGLOBALREF_HPP
