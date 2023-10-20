#ifndef __GUSC_JENV_HPP
#define __GUSC_JENV_HPP 1

#include <memory>

namespace gusc::Jni
{

class JClass;

class JEnv
{
public:
    /// @brief Wrapped instance of JNIEnv object without thread attachment lifetime watch
    JEnv(JNIEnv* initEnv)
        : env(initEnv)
    {}

    /// @brief Wrapped instance of JNIEnv object with thread attachment lifetime watch
    /// This class hosts a shared pointer that get's passed between all copies of this object
    /// once the last copy is deleted the current thread is detached from JavaVM
    JEnv(JNIEnv* initEnv, JavaVM* initVm)
        : env(initEnv)
        , lifetimeHandle(std::make_shared<JEnvLifetimeHandle>(initVm))
    {}

    JEnv(const JEnv&) = default;
    JEnv& operator=(const JEnv&) = default;
    JEnv(JEnv&&) = default;
    JEnv& operator=(JEnv&&) = default;
    ~JEnv() = default;

    JClass getClass(const char* classPath);

    JClass getObjectClass(jobject jniObject);

    inline operator JNIEnv*()
    {
        return env;
    }

    inline JNIEnv* operator->()
    {
        return env;
    }

    static void checkException(JEnv& env);

private:
    struct JEnvLifetimeHandle
    {
        JEnvLifetimeHandle(JavaVM* initVm)
            : vm(initVm)
        {}
        ~JEnvLifetimeHandle()
        {
            if (vm)
            {
                vm->DetachCurrentThread();
                vm = nullptr;
            }
        }
        JavaVM* vm { nullptr };
    };

    JNIEnv* env { nullptr };
    std::shared_ptr<JEnvLifetimeHandle> lifetimeHandle;
};
    
}

#endif // __GUSC_JENV_HPP
