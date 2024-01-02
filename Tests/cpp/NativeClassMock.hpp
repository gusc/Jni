//
// Created by Gusts Kaksis on 23/10/2021.
//

#ifndef TESTS_NATIVECLASSMOCK_HPP
#define TESTS_NATIVECLASSMOCK_HPP

#include "Jni/Jni.hpp"
#include <gmock/gmock.h>

using namespace gusc::Jni;
using namespace ::testing;

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

class NativeClassMock
{
public:
    NativeClassMock()
        : objRef(JVM::getEnv().getClass("lv/gusc/jni/tests/NativeClass").createObject(fromPtr(this)))
    {}

    void voidMethodNoArgs()
    {
        objRef->invokeMethod<void>("voidMethodNoArgs");
    }

    void voidMethod(jboolean newA, jbyte newB, jchar newC, jshort newD, jint newE, jlong newF, jfloat newG, jdouble newH, jstring newI)
    {
        objRef->invokeMethod<void>("voidMethod", newA, newB, newC, newD, newE, newF, newG, newH, newI);
    }

    jboolean booleanMethod(jboolean val)
    {
        return objRef->invokeMethod<jboolean>("booleanMethod", val);
    }

    jbyte byteMethod(jbyte val)
    {
        return objRef->invokeMethod<jbyte>("byteMethod", val);
    }

    jchar charMethod(jchar val)
    {
        return objRef->invokeMethod<jchar>("charMethod", val);
    }

    jshort shortMethod(jshort val)
    {
        return objRef->invokeMethod<jshort>("shortMethod", val);
    }

    jint intMethod(jint val)
    {
        return objRef->invokeMethod<jint>("intMethod", val);
    }

    jlong longMethod(jlong val)
    {
        return objRef->invokeMethod<jlong>("longMethod", val);
    }

    jfloat floatMethod(jfloat val)
    {
        return objRef->invokeMethod<jfloat>("floatMethod", val);
    }

    jdouble doubleMethod(jdouble val)
    {
        return objRef->invokeMethod<jdouble>("doubleMethod", val);
    }

    jstring stringMethod(jstring val)
    {
        return objRef->invokeMethod<jstring>("stringMethod", val);
    }

    jbooleanArray booleanArrayMethod(jbooleanArray val)
    {
        return objRef->invokeMethod<jbooleanArray>("booleanArrayMethod", val);
    }

    jbyteArray byteArrayMethod(jbyteArray val)
    {
        return objRef->invokeMethod<jbyteArray>("byteArrayMethod", val);
    }

    jcharArray charArrayMethod(jcharArray val)
    {
        return objRef->invokeMethod<jcharArray>("charArrayMethod", val);
    }

    jshortArray shortArrayMethod(jshortArray val)
    {
        return objRef->invokeMethod<jshortArray>("shortArrayMethod", val);
    }

    jintArray intArrayMethod(jintArray val)
    {
        return objRef->invokeMethod<jintArray>("intArrayMethod", val);
    }

    jlongArray longArrayMethod(jlongArray val)
    {
        return objRef->invokeMethod<jlongArray>("longArrayMethod", val);
    }

    jfloatArray floatArrayMethod(jfloatArray val)
    {
        return objRef->invokeMethod<jfloatArray>("floatArrayMethod", val);
    }

    jdoubleArray doubleArrayMethod(jdoubleArray val)
    {
        return objRef->invokeMethod<jdoubleArray>("doubleArrayMethod", val);
    }

    MOCK_METHOD(void, nativeVoidCallNoArgs, ());
    MOCK_METHOD(void, nativeVoidCall, ());

    static void registerNativeMethods()
    {
        auto cls = JVM::getEnv().getClass("lv/gusc/jni/tests/NativeClass");
        cls.registerNativeMethod("nativeVoidMethodNoArgs", &NativeClassMock::nativeVoidMethodNoArgsJni);
        cls.registerNativeMethod("nativeVoidMethod", &NativeClassMock::nativeVoidMethodJni);
        cls.registerNativeMethod("nativeBooleanMethod", &NativeClassMock::nativeBooleanMethodJni);
        cls.registerNativeMethod("nativeByteMethod", &NativeClassMock::nativeByteMethodJni);
        cls.registerNativeMethod("nativeCharMethod", &NativeClassMock::nativeCharMethodJni);
        cls.registerNativeMethod("nativeShortMethod", &NativeClassMock::nativeShortMethodJni);
        cls.registerNativeMethod("nativeIntMethod", &NativeClassMock::nativeIntMethodJni);
        cls.registerNativeMethod("nativeLongMethod", &NativeClassMock::nativeLongMethodJni);
        cls.registerNativeMethod("nativeFloatMethod", &NativeClassMock::nativeFloatMethodJni);
        cls.registerNativeMethod("nativeDoubleMethod", &NativeClassMock::nativeDoubleMethodJni);
        cls.registerNativeMethod("nativeStringMethod", &NativeClassMock::nativeStringMethodJni);
        cls.registerNativeMethod("nativeBooleanArrayMethod", &NativeClassMock::nativeBooleanArrayMethodJni);
        cls.registerNativeMethod("nativeByteArrayMethod", &NativeClassMock::nativeByteArrayMethodJni);
        cls.registerNativeMethod("nativeCharArrayMethod", &NativeClassMock::nativeCharArrayMethodJni);
        cls.registerNativeMethod("nativeShortArrayMethod", &NativeClassMock::nativeShortArrayMethodJni);
        cls.registerNativeMethod("nativeIntArrayMethod", &NativeClassMock::nativeIntArrayMethodJni);
        cls.registerNativeMethod("nativeLongArrayMethod", &NativeClassMock::nativeLongArrayMethodJni);
        cls.registerNativeMethod("nativeFloatArrayMethod", &NativeClassMock::nativeFloatArrayMethodJni);
        cls.registerNativeMethod("nativeDoubleArrayMethod", &NativeClassMock::nativeDoubleArrayMethodJni);
    }
private:
    JGlobalRef objRef;

    void nativeVoidMethodNoArgs()
    {
        nativeVoidCallNoArgs();
    }

    void nativeVoidMethod(jboolean newA, jbyte newB, jchar newC, jshort newD, jint newE, jlong newF, jfloat newG, jdouble newH, jstring newI)
    {
        (void)newA;
        (void)newB;
        (void)newC;
        (void)newD;
        (void)newE;
        (void)newF;
        (void)newG;
        (void)newH;
        (void)newI;
        nativeVoidCall();
    }

    jboolean nativeBooleanMethod(jboolean val)
    {
        return val;
    }

    jbyte nativeByteMethod(jbyte val)
    {
        return val;
    }

    jchar nativeCharMethod(jchar val)
    {
        return val;
    }

    jshort nativeShortMethod(jshort val)
    {
        return val;
    }

    jint nativeIntMethod(jint val)
    {
        return val;
    }

    jlong nativeLongMethod(jlong val)
    {
        return val;
    }

    jfloat nativeFloatMethod(jfloat val)
    {
        return val;
    }

    jdouble nativeDoubleMethod(jdouble val)
    {
        return val;
    }

    jstring nativeStringMethod(jstring val)
    {
        return val;
    }

    jbooleanArray nativeBooleanArrayMethod(jbooleanArray val)
    {
        return val;
    }

    jbyteArray nativeByteArrayMethod(jbyteArray val)
    {
        return val;
    }

    jcharArray nativeCharArrayMethod(jcharArray val)
    {
        return val;
    }

    jshortArray nativeShortArrayMethod(jshortArray val)
    {
        return val;
    }

    jintArray nativeIntArrayMethod(jintArray val)
    {
        return val;
    }

    jlongArray nativeLongArrayMethod(jlongArray val)
    {
        return val;
    }

    jfloatArray nativeFloatArrayMethod(jfloatArray val)
    {
        return val;
    }

    jdoubleArray nativeDoubleArrayMethod(jdoubleArray val)
    {
        return val;
    }

    static void nativeVoidMethodNoArgsJni(JNIEnv*, jobject thiz)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        ptr->nativeVoidMethodNoArgs();
    }
    static void nativeVoidMethodJni(JNIEnv*, jobject thiz, jboolean newA, jbyte newB, jchar newC, jshort newD, jint newE, jlong newF, jfloat newG, jdouble newH, jstring newI)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        ptr->nativeVoidMethod(newA, newB, newC, newD, newE, newF, newG, newH, newI);
    }
    static jboolean nativeBooleanMethodJni(JNIEnv*, jobject thiz, jboolean val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeBooleanMethod(val);
    }
    static jbyte nativeByteMethodJni(JNIEnv*, jobject thiz, jbyte val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeByteMethod(val);
    }
    static jchar nativeCharMethodJni(JNIEnv*, jobject thiz, jchar val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeCharMethod(val);
    }
    static jshort nativeShortMethodJni(JNIEnv*, jobject thiz, jshort val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeShortMethod(val);
    }
    static jint nativeIntMethodJni(JNIEnv*, jobject thiz, jint val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeIntMethod(val);
    }
    static jlong nativeLongMethodJni(JNIEnv*, jobject thiz, jlong val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeLongMethod(val);
    }
    static jfloat nativeFloatMethodJni(JNIEnv*, jobject thiz, jfloat val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeFloatMethod(val);
    }
    static jdouble nativeDoubleMethodJni(JNIEnv*, jobject thiz, jdouble val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeDoubleMethod(val);
    }
    static jstring nativeStringMethodJni(JNIEnv*, jobject thiz, jstring val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeStringMethod(val);
    }
    static jbooleanArray nativeBooleanArrayMethodJni(JNIEnv*, jobject thiz, jbooleanArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeBooleanArrayMethod(val);
    }
    static jbyteArray nativeByteArrayMethodJni(JNIEnv*, jobject thiz, jbyteArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeByteArrayMethod(val);
    }
    static jcharArray nativeCharArrayMethodJni(JNIEnv*, jobject thiz, jcharArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeCharArrayMethod(val);
    }
    static jshortArray nativeShortArrayMethodJni(JNIEnv*, jobject thiz, jshortArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeShortArrayMethod(val);
    }
    static jintArray nativeIntArrayMethodJni(JNIEnv*, jobject thiz, jintArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeIntArrayMethod(val);
    }
    static jlongArray nativeLongArrayMethodJni(JNIEnv*, jobject thiz, jlongArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeLongArrayMethod(val);
    }
    static jfloatArray nativeFloatArrayMethodJni(JNIEnv*, jobject thiz, jfloatArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeFloatArrayMethod(val);
    }
    static jdoubleArray nativeDoubleArrayMethodJni(JNIEnv*, jobject thiz, jdoubleArray val)
    {
        auto ptr = toPtr<NativeClassMock>(JObject(thiz).getField<jlong>("nativePtr"));
        return ptr->nativeDoubleArrayMethod(val);
    }
};

#endif //TESTS_NATIVECLASSMOCK_HPP
