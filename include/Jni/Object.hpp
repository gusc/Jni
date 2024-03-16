#ifndef __GUSC_JNI_OBJECT_HPP
#define __GUSC_JNI_OBJECT_HPP 1

#include "private/strutils.hpp"
#include "private/signature.hpp"
#include "private/cast.hpp"
#include <type_traits>
#include <string>
#include <utility>

namespace gusc::Jni
{

/// @brief a JNI class/object base class
/// @code
///   constexpr const char java_lang_Number[] = "java.lang.Number";
///   Jni::Object<java_lang_Number> myNumber;
template<const char ClassName[]>
class Object
{
public:
    using JniType = jobject;

    /// @brief wrap around existing JNI object
    explicit Object(jobject obj);
    /// @brief construct a new object
    template<typename... TArgs>
    Object(TArgs&&... args);
    Object(const Object& other);
    Object& operator=(const Object& other);
    Object(Object&& other);
    Object& operator=(Object&& other);
    ~Object();

    /// Get member value
    template<typename T>
    T get(const std::string& memberName) const;

    /// Set member value
    template<typename T>
    void set(const std::string& memberName, T&& memberValue);

    /// Invoke instance method
    template<typename TReturn, typename... TArgs>
    TReturn invoke(const std::string& methodName, TArgs&&... args) const;

    /// Get static member value
    template<typename T>
    static T getStatic(const std::string& memberName);

    /// Set static member value
    template<typename T>
    static void setStatic(const std::string& memberName, T&& memberValue);

    /// Invoke static method
    template<typename TReturn, typename... TArgs>
    static TReturn invokeStatic(const std::string& methodName, TArgs&&... args);

    inline operator jobject()
    {
        return instance;
    }

    inline static constexpr const char* getClassName()
    {
        return ClassName;
    }
private:
    JObjectS<ClassName> instance;
};

template<const char ClassName[]>
Object<ClassName>::Object(jobject obj)
    : instance { obj }
{}

template<const char ClassName[]>
template<typename... TArgs>
Object<ClassName>::Object(TArgs&&... args)
{
    JClassS<ClassName> cls;
    instance = cls.createObjectS(std::forward<TArgs>(args)...);
}

template<const char ClassName[]>
Object<ClassName>::Object(const Object& other)
    : instance { other.instance }
{}

template<const char ClassName[]>
Object<ClassName>& Object<ClassName>::operator=(const Object& other)
{
    instance = other.instance;
    return *this;
}

template<const char ClassName[]>
Object<ClassName>::Object(Object&& other)
    : instance { std::move(other.instance) }
{}

template<const char ClassName[]>
Object<ClassName>& Object<ClassName>::operator=(Object&& other)
{
    instance = std::move(other.instance);
    return *this;
}

template<const char ClassName[]>
Object<ClassName>::~Object()
{}

template<const char ClassName[]>
template<typename T>
T Object<ClassName>::get(const std::string& memberName) const
{
    return instance.template getField<T>(memberName.c_str());
}

template<const char ClassName[]>
template<typename T>
void Object<ClassName>::set(const std::string& memberName, T&& memberValue)
{
    instance.template setField<T>(memberName.c_str(), std::forward<T>(memberValue));
}

template<const char ClassName[]>
template<typename TReturn, typename... TArgs>
TReturn Object<ClassName>::invoke(const std::string& methodName, TArgs&&... args) const
{
    return instance.template invokeMethod<TReturn>(methodName.c_str(), std::forward<TArgs>(args)...);
}

template<const char ClassName[]>
template<typename T>
T Object<ClassName>::getStatic(const std::string& memberName)
{
    JClassS<ClassName> cls;
    return cls.template getField<T>(memberName.c_str());
}

template<const char ClassName[]>
template<typename T>
void Object<ClassName>::setStatic(const std::string& memberName, T&& memberValue)
{
    JClassS<ClassName> cls;
    cls.template setField<T>(memberName.c_str(), std::forward<T>(memberValue));
}

template<const char ClassName[]>
template<typename TReturn, typename... TArgs>
TReturn Object<ClassName>::invokeStatic(const std::string& methodName, TArgs&&... args)
{
    JClassS<ClassName> cls;
    return cls.template invokeMethod<TReturn>(methodName.c_str(), std::forward<TArgs>(args)...);
}

}

#endif // __GUSC_JNI_OBJECT_HPP
