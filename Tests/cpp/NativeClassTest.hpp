//
// Created by Gusts Kaksis on 23/10/2021.
//

#ifndef TESTS_NATIVECLASSTEST_HPP
#define TESTS_NATIVECLASSTEST_HPP

#include "Jni/Jni.hpp"

// C++20 will have this built-in:
#if __cplusplus <= 201703L
template<class To, class From>
typename std::enable_if_t<
        sizeof(To) == sizeof(From) &&
        std::is_trivially_copyable_v<From> &&
        std::is_trivially_copyable_v<To>,
        To>
bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>, "This implementation additionally requires destination type to be trivially constructible");
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}
#endif

namespace
{
template<typename T>
inline T* toPtr(jlong ptrVal)
{
    return bit_cast<T*>(static_cast<uintptr_t>(ptrVal));
}
template<typename T>
inline jlong fromPtr(T* ptr)
{
    return static_cast<jlong>(bit_cast<uintptr_t>(ptr));
}
}

class NativeClassTest
{
public:
    NativeClassTest()
        : objRef(JVM::getEnv().getClass("lv/gusc/jni/tests/NativeClassTest").createObject(fromPtr(this)))
    {}

    void runTest()
    {
        objRef->invokeMethod<void>("runTest");
    }

    void nativeVoidMethodNoArgs()
    {}

    void nativeVoidMethod(jboolean newA, jbyte newB, jchar newC, jshort newD, jint newE, jlong newF, jfloat newG, jdouble newH, jstring newI)
    {
        a = newA;
        b = newB;
        c = newC;
        d = newD;
        e = newE;
        f = newF;
        g = newG;
        h = newH;
        auto x = newI;
        (void)(x);
    }

    jboolean nativeBooleanMethod(jboolean val)
    {
        auto tmp = a;
        a = val;
        return tmp;
    }

    jbyte nativeByteMethod(jbyte val)
    {
        auto tmp = b;
        b = val;
        return tmp;
    }

    jchar nativeCharMethod(jchar val)
    {
        auto tmp = c;
        c = val;
        return tmp;
    }

    jshort nativeShortMethod(jshort val)
    {
        auto tmp = d;
        d = val;
        return tmp;
    }

    jint nativeIntMethod(jint val)
    {
        auto tmp = e;
        e = val;
        return tmp;
    }

    jlong nativeLongMethod(jlong val)
    {
        auto tmp = f;
        f = val;
        return tmp;
    }

    jfloat nativeFloatMethod(jfloat val)
    {
        auto tmp = g;
        g = val;
        return tmp;
    }

    jdouble nativeDoubleMethod(jdouble val)
    {
        auto tmp = h;
        h = val;
        return tmp;
    }

    jstring nativeStringMethod(jstring val)
    {
        //JString tmp = JString::createFrom(static_cast<std::string>(i));
        return val;
    }


    static void registerNativeMethods()
    {
        auto cls = JVM::getEnv().getClass("lv/gusc/jni/tests/NativeClassTest");
        cls.registerNativeMethod("nativeVoidMethodNoArgs", &NativeClassTest::nativeVoidMethodNoArgsJni);
        cls.registerNativeMethod("nativeVoidMethod", &NativeClassTest::nativeVoidMethodJni);
        cls.registerNativeMethod("nativeBooleanMethod", &NativeClassTest::nativeBooleanMethodJni);
        cls.registerNativeMethod("nativeByteMethod", &NativeClassTest::nativeByteMethodJni);
        cls.registerNativeMethod("nativeCharMethod", &NativeClassTest::nativeCharMethodJni);
        cls.registerNativeMethod("nativeShortMethod", &NativeClassTest::nativeShortMethodJni);
        cls.registerNativeMethod("nativeIntMethod", &NativeClassTest::nativeIntMethodJni);
        cls.registerNativeMethod("nativeLongMethod", &NativeClassTest::nativeLongMethodJni);
        cls.registerNativeMethod("nativeFloatMethod", &NativeClassTest::nativeFloatMethodJni);
        cls.registerNativeMethod("nativeDoubleMethod", &NativeClassTest::nativeDoubleMethodJni);
        cls.registerNativeMethod("nativeStringMethod", &NativeClassTest::nativeStringMethodJni);
    }
private:
    JGlobalRef objRef;
    jboolean a { false };
    jbyte b { 1 };
    jchar c { 'a' };
    jshort d { 0x1FFF };
    jint e { 0x1FFFFFFF };
    jlong f { 0xFFFFFFFF };
    jfloat g { 1.f };
    jdouble h { 2.0 };
    JString i { JString::createFrom("asdf") };

    static void nativeVoidMethodNoArgsJni(JNIEnv*, jobject thiz)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        ptr->nativeVoidMethodNoArgs();
    }
    static void nativeVoidMethodJni(JNIEnv*, jobject thiz, jboolean newA, jbyte newB, jchar newC, jshort newD, jint newE, jlong newF, jfloat newG, jdouble newH, jstring newI)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        ptr->nativeVoidMethod(newA, newB, newC, newD, newE, newF, newG, newH, newI);
    }
    static jboolean nativeBooleanMethodJni(JNIEnv*, jobject thiz, jboolean val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeBooleanMethod(val);
    }
    static jbyte nativeByteMethodJni(JNIEnv*, jobject thiz, jbyte val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeByteMethod(val);
    }
    static jchar nativeCharMethodJni(JNIEnv*, jobject thiz, jchar val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeCharMethod(val);
    }
    static jshort nativeShortMethodJni(JNIEnv*, jobject thiz, jshort val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeShortMethod(val);
    }
    static jint nativeIntMethodJni(JNIEnv*, jobject thiz, jint val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeIntMethod(val);
    }
    static jlong nativeLongMethodJni(JNIEnv*, jobject thiz, jlong val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeLongMethod(val);
    }
    static jfloat nativeFloatMethodJni(JNIEnv*, jobject thiz, jfloat val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeFloatMethod(val);
    }
    static jdouble nativeDoubleMethodJni(JNIEnv*, jobject thiz, jdouble val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeDoubleMethod(val);
    }
    static jstring nativeStringMethodJni(JNIEnv*, jobject thiz, jstring val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeStringMethod(val);
    }
};

void registerNativeMethods()
{
    NativeClassTest::registerNativeMethods();
}

void runNativeTest()
{
    NativeClassTest test;
    test.runTest();
}

#endif //TESTS_NATIVECLASSTEST_HPP
