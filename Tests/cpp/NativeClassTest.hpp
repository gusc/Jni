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
        // TODO: implement reference copies in C++
//        JString tmp = i;
//        i = val;
//        return static_cast<jstring>(tmp);
        return val;
    }

    jbooleanArray nativeBooleanArrayMethod(jbooleanArray val)
    {
//        JBooleanArray tmp = j;
//        j = val;
//        return static_cast<jbooleanArray>(tmp);
        return val;
    }

    jbyteArray nativeByteArrayMethod(jbyteArray val)
    {
//        JByteArray tmp = k;
//        k = val;
//        return static_cast<jbyteArray>(tmp);
        return val;
    }

    jcharArray nativeCharArrayMethod(jcharArray val)
    {
//        JCharArray tmp = l;
//        l = val;
//        return static_cast<jcharArray>(tmp);
        return val;
    }

    jshortArray nativeShortArrayMethod(jshortArray val)
    {
//        JShortArray tmp = m;
//        m = val;
//        return static_cast<jshortArray>(tmp);
        return val;
    }

    jintArray nativeIntArrayMethod(jintArray val)
    {
//        JIntArray tmp = n;
//        n = val;
//        return static_cast<jintArray>(tmp);
        return val;
    }

    jlongArray nativeLongArrayMethod(jlongArray val)
    {
//        JLongArray tmp = o;
//        o = val;
//        return static_cast<jlongArray>(tmp);
        return val;
    }

    jfloatArray nativeFloatArrayMethod(jfloatArray val)
    {
//        JFloatArray tmp = p;
//        p = val;
//        return static_cast<jfloatArray>(tmp);
        return val;
    }

    jdoubleArray nativeDoubleArrayMethod(jdoubleArray val)
    {
//        JDoubleArray tmp = q;
//        q = val;
//        return static_cast<jdoubleArray>(q);
        return val;
    }

    jobjectArray nativeObjectArrayMethod(jobjectArray val)
    {
//        JObjectArray tmp = q;
//        q = val;
//        return static_cast<jobjectArray>(q);
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
        cls.registerNativeMethod("nativeBooleanArrayMethod", &NativeClassTest::nativeBooleanArrayMethodJni);
        cls.registerNativeMethod("nativeByteArrayMethod", &NativeClassTest::nativeByteArrayMethodJni);
        cls.registerNativeMethod("nativeCharArrayMethod", &NativeClassTest::nativeCharArrayMethodJni);
        cls.registerNativeMethod("nativeShortArrayMethod", &NativeClassTest::nativeShortArrayMethodJni);
        cls.registerNativeMethod("nativeIntArrayMethod", &NativeClassTest::nativeIntArrayMethodJni);
        cls.registerNativeMethod("nativeLongArrayMethod", &NativeClassTest::nativeLongArrayMethodJni);
        cls.registerNativeMethod("nativeFloatArrayMethod", &NativeClassTest::nativeFloatArrayMethodJni);
        cls.registerNativeMethod("nativeDoubleArrayMethod", &NativeClassTest::nativeDoubleArrayMethodJni);
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
    JBooleanArray j { JBooleanArray::createFrom({ false, true, true, false }) };
    JByteArray k { JByteArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JCharArray l = { JCharArray::createFrom({ 'd', 'c', 'b', 'a' }) };
    JShortArray m = { JShortArray ::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JIntArray n = { JIntArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JLongArray o = { JLongArray::createFrom({ 1234567890, 1234567891, 1234567892, 1234567893 }) };
    JFloatArray p = { JFloatArray::createFrom({ 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f }) };
    JDoubleArray q = { JDoubleArray::createFrom({ 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 }) };

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
    static jbooleanArray nativeBooleanArrayMethodJni(JNIEnv*, jobject thiz, jbooleanArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeBooleanArrayMethod(val);
    }
    static jbyteArray nativeByteArrayMethodJni(JNIEnv*, jobject thiz, jbyteArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeByteArrayMethod(val);
    }
    static jcharArray nativeCharArrayMethodJni(JNIEnv*, jobject thiz, jcharArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeCharArrayMethod(val);
    }
    static jshortArray nativeShortArrayMethodJni(JNIEnv*, jobject thiz, jshortArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeShortArrayMethod(val);
    }
    static jintArray nativeIntArrayMethodJni(JNIEnv*, jobject thiz, jintArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeIntArrayMethod(val);
    }
    static jlongArray nativeLongArrayMethodJni(JNIEnv*, jobject thiz, jlongArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeLongArrayMethod(val);
    }
    static jfloatArray nativeFloatArrayMethodJni(JNIEnv*, jobject thiz, jfloatArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeFloatArrayMethod(val);
    }
    static jdoubleArray nativeDoubleArrayMethodJni(JNIEnv*, jobject thiz, jdoubleArray val)
    {
        auto ptr = toPtr<NativeClassTest>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeDoubleArrayMethod(val);
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
