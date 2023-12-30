//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include "Jni/Jni.hpp"
#include "NativeClassMock.hpp"

class NativeClassTest : public Test
{
public:
    template<typename TJni, typename TJava>
    bool compare(TJni& jni, TJava& java)
    {
        TJni java2 { java };
        return static_cast<typename TJni::StlType>(jni) == static_cast<typename TJni::StlType>(java2);
    }

    jboolean a { false };
    jbyte b { 0 };
    jchar c { '\0' };
    jshort d { 0 };
    jint e { 0 };
    jlong f { 0 };
    jfloat g { 0.0f };
    jdouble h { 0.0 };
    JString i { JString::createFrom("") };
    JBooleanArray j { JBooleanArray::createFrom({ false, false }) };
    JByteArray k { JByteArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JCharArray l { JCharArray::createFrom({ 'd', 'c', 'b', 'a' }) };
    JShortArray m { JShortArray ::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JIntArray n { JIntArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JLongArray o { JLongArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }) };
    JFloatArray p { JFloatArray::createFrom({ 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f }) };
    JDoubleArray q { JDoubleArray::createFrom({ 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 }) };

    NativeClassMock mock;
};

TEST_F(NativeClassTest, NativeClassTest)
{
    // This test does a round-trip call C++ -> JNI -> Java -> JNI -> C++ and compares if values went throught
    EXPECT_CALL(mock, nativeVoidCallNoArgs).Times(1);
    EXPECT_CALL(mock, nativeVoidCall).Times(1);
    mock.voidMethodNoArgs();
    mock.voidMethod(a, b, c, d, e, f, g, h, static_cast<jstring>(i));
    auto a1 = mock.booleanMethod(a);
    auto b1 = mock.byteMethod(b);
    auto c1 = mock.charMethod(c);
    auto d1 = mock.shortMethod(d);
    auto e1 = mock.intMethod(e);
    auto f1 = mock.longMethod(f);
    auto g1 = mock.floatMethod(g);
    auto h1 = mock.doubleMethod(h);
    auto i1 = mock.stringMethod(i);
    auto j1 = mock.booleanArrayMethod(j);
    auto k1 = mock.byteArrayMethod(k);
    auto l1 = mock.charArrayMethod(l);
    auto m1 = mock.shortArrayMethod(m);
    auto n1 = mock.intArrayMethod(n);
    auto o1 = mock.longArrayMethod(o);
    auto p1 = mock.floatArrayMethod(p);
    auto q1 = mock.doubleArrayMethod(q);
    EXPECT_EQ(a, a1);
    EXPECT_EQ(b, b1);
    EXPECT_EQ(c, c1);
    EXPECT_EQ(d, d1);
    EXPECT_EQ(e, e1);
    EXPECT_EQ(f, f1);
    EXPECT_EQ(g, g1);
    EXPECT_EQ(h, h1);
    EXPECT_TRUE(compare(i, i1));
    EXPECT_TRUE(compare(j, j1));
    EXPECT_TRUE(compare(k, k1));
    EXPECT_TRUE(compare(l, l1));
    EXPECT_TRUE(compare(m, m1));
    EXPECT_TRUE(compare(n, n1));
    EXPECT_TRUE(compare(o, o1));
    EXPECT_TRUE(compare(p, p1));
    EXPECT_TRUE(compare(q, q1));
}

TEST_F(NativeClassTest, InstanceClass)
{
    auto instance = mock.getInstance();
    auto res = instance.invokeMethod<jboolean>("getBoolean");
    EXPECT_TRUE(res);
    instance.invokeMethod<void>("setBoolean", static_cast<jboolean>(false));
    auto instance2 = mock.instancePassthrough(instance);
    auto res2 = instance.invokeMethod<jboolean>("getBoolean");
    EXPECT_FALSE(res2);
}