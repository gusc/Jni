#ifndef __GUSC_JCLASS_HPP
#define __GUSC_JCLASS_HPP 1

#include <jni.h>
#include "JString.hpp"

namesapce gusc::Jni
{
    
class JClass
{
public:
    JClass(JEnv& initEnv, jclass initClass) :
        env(initEnv),
        cls(initClass){}
    
    inline std::string getClassName()
    {
        jmethodID getNameId = getMethodId("getName", "()Ljava/lang/String;");
        return JString(env, env->CallObjectMethod(cls, mid_getName));
    }
    
    inline jmethodID getStaticMethodId(const char* name, const char* signature)
    {
        auto methodId = env->GetStaticMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find static method ") + name + " with signature " + signature);
        }
        return methodId;
    }
    
    inline jmethodID getMethodId(const char* name, const char* signature)
    {
        auto methodId = env->GetMethodID(cls, name, signature);
        if (!methodId)
        {
            throw std::runtime_error(std::string("Can't find instance method ") + name + " with signature " + signature);
        }
        return methodId;
    }

    inline jfieldID getStaticFieldId(const char* name, const char* signature)
    {
        auto fieldId = env->GetStaticFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find static field ") + name + " with signature " + signature);
        }
        return fieldId;
    }
    
    inline jfieldID getFieldId(const char* name, const char* signature)
    {
        auto fieldId = env->GetFieldID(cls, name, signature);
        if (!fieldId)
        {
            throw std::runtime_error(std::string("Can't find instance field ") + name + " with signature " + signature);
        }
        return fieldId;
    }
    
private:
    JEnv& env;
    jclass cls { nullptr };
}
    
}

#endif // __GUSC_JCLASS_HPP
