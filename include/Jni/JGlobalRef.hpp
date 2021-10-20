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
    JGlobalRef(jobject initObj)
        : obj(JVM::getEnv()->NewGlobalRef(initObj))
    {}
    JGlobalRef(const JGlobalRef&) = delete;
    JGlobalRef& operator=(const JGlobalRef&) = delete;
    ~JGlobalRef() = default;

    inline operator jobject()
    {
        return static_cast<jobject>(obj);
    }
    inline JObject* operator->()
    {
        return &obj;
    }
private:
    JObject obj;
};

}

#endif //__GUSCJGLOBALREF_HPP
