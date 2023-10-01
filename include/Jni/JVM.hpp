#ifndef __GUSC_JVM_HPP
#define __GUSC_JVM_HPP 1

#include <stdexcept>
#include <string>

namespace gusc::Jni
{

class JVM
{
public:
    static inline void init(JavaVM* initVm) noexcept
    {
        vm = initVm;

        auto env = getEnv();
        auto randomClass = env->FindClass("java/lang/String");
        auto classClass = env->GetObjectClass(randomClass);
        auto classLoaderClass = env->FindClass("java/lang/ClassLoader");
        auto getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader","()Ljava/lang/ClassLoader;");
        ClassLoader = env->NewGlobalRef(env->CallObjectMethod(classClass, getClassLoaderMethod));
        FindClassMethod = env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    }
    static inline JEnv getEnv()
    {
        if (vm)
        {
            JNIEnv *env { nullptr };
            auto res = vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            if (res == JNI_OK)
            {
                return JEnv(env);
            }
            else if (res == JNI_EDETACHED && vm->AttachCurrentThread(&env, nullptr) == JNI_OK)
            {
                return JEnv(env, vm);
            }
        }
        throw std::runtime_error("Java ENV cannot be created!");
    }
    static inline void detachThread()
    {
        if (vm)
        {
            if (vm->DetachCurrentThread() == JNI_OK)
            {
                return;
            }
        }
        throw std::runtime_error("Java can not detach from thread!");
    }

    static inline jclass loadAndFindClass(JEnv env, const std::string& classPath)
    {
        auto classDotted = classPath;
        // java.lang.ClassLoader.findClass takes in dotted syntax class name not class path
        std::size_t pos = 0;
        while((pos = classDotted.find("/", pos)) != std::string::npos) {
            classDotted.replace(pos, 1, ".");
            ++pos;
        }
        auto jniClassName = env->NewStringUTF(classDotted.c_str());
        return static_cast<jclass>(env->CallObjectMethod(ClassLoader, FindClassMethod, jniClassName));
    }
private:
    inline static JavaVM* vm { nullptr };
    inline static jobject ClassLoader { nullptr };
    inline static jmethodID FindClassMethod { nullptr };
};
    
}

#endif // __GUSC_JVM_HPP
