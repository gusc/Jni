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
    /// @brief create empty global reference to JNI object
    JGlobalRef() = default;
    /// @brief create global reference to JNI object and wrap it
    /// @note this automatically assumes the ownership of the global reference object
    JGlobalRef(jobject initObj)
        : obj(static_cast<JObject>(initObj).createGlobalRef())
    {}
    JGlobalRef(const JGlobalRef&) = delete;
    JGlobalRef& operator=(const JGlobalRef&) = delete;
    JGlobalRef(JGlobalRef&&) = default;
    JGlobalRef& operator=(JGlobalRef&&) = default;
    ~JGlobalRef() = default;

    inline operator bool() const
    {
        return obj;
    }
    inline operator jobject()
    {
        return static_cast<jobject>(obj);
    }
    inline operator const JObject&() const
    {
        return obj;
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
