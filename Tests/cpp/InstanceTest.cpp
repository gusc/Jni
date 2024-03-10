//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;
using namespace ::testing;

constexpr const char lv_gusc_jni_tests_TestClass[] = "lv.gusc.jni.tests.TestClass";

class InstanceTest : public Test
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

    JClass cls { JVM::getEnv().getClass("lv/gusc/jni/tests/InstanceClass").createGlobalRef() };

    JClassS<lv_gusc_jni_tests_TestClass> testClass;
};

TEST_F(InstanceTest, ConstructNoArgs)
{
    auto obj = cls.createObject();
    auto a1 = obj.getField<jboolean>("booleanField");
    auto b1 = obj.getField<jbyte>("byteField");
    auto c1 = obj.getField<jchar>("charField");
    auto d1 = obj.getField<jshort>("shortField");
    auto e1 = obj.getField<jint>("intField");
    auto f1 = obj.getField<jlong>("longField");
    auto g1 = obj.getField<jfloat>("floatField");
    auto h1 = obj.getField<jdouble>("doubleField");
    auto i1 = obj.getField<JString>("stringField");
    auto j1 = obj.getField<jbooleanArray>("booleanArrayField");
    auto k1 = obj.getField<jbyteArray>("byteArrayField");
    auto l1 = obj.getField<jcharArray>("charArrayField");
    auto m1 = obj.getField<jshortArray>("shortArrayField");
    auto n1 = obj.getField<jintArray>("intArrayField");
    auto o1 = obj.getField<jlongArray>("longArrayField");
    auto p1 = obj.getField<jfloatArray>("floatArrayField");
    auto q1 = obj.getField<jdoubleArray>("doubleArrayField");
    EXPECT_NE(a, a1);
    EXPECT_NE(b, b1);
    EXPECT_NE(c, c1);
    EXPECT_NE(d, d1);
    EXPECT_NE(e, e1);
    EXPECT_NE(f, f1);
    EXPECT_NE(g, g1);
    EXPECT_NE(h, h1);
    EXPECT_FALSE(compare(i, i1));
    EXPECT_FALSE(compare(j, j1));
    EXPECT_FALSE(compare(k, k1));
    EXPECT_FALSE(compare(l, l1));
    EXPECT_FALSE(compare(m, m1));
    EXPECT_FALSE(compare(n, n1));
    EXPECT_FALSE(compare(o, o1));
    EXPECT_FALSE(compare(p, p1));
    EXPECT_FALSE(compare(q, q1));
}

TEST_F(InstanceTest, ConstructArgs)
{
    auto obj = cls.createObject(a, b, c, d, e, f, g, h,
                                static_cast<jstring>(i),
                                static_cast<jbooleanArray>(j),
                                static_cast<jbyteArray>(k),
                                static_cast<jcharArray>(l),
                                static_cast<jshortArray>(m),
                                static_cast<jintArray>(n),
                                static_cast<jlongArray>(o),
                                static_cast<jfloatArray>(p),
                                static_cast<jdoubleArray>(q)
                                );
    auto a2 = obj.getField<jboolean>("booleanField");
    auto b2 = obj.getField<jbyte>("byteField");
    auto c2 = obj.getField<jchar>("charField");
    auto d2 = obj.getField<jshort>("shortField");
    auto e2 = obj.getField<jint>("intField");
    auto f2 = obj.getField<jlong>("longField");
    auto g2 = obj.getField<jfloat>("floatField");
    auto h2 = obj.getField<jdouble>("doubleField");
    auto i2 = obj.getField<JString>("stringField");
    auto j2 = obj.getField<jbooleanArray>("booleanArrayField");
    auto k2 = obj.getField<jbyteArray>("byteArrayField");
    auto l2 = obj.getField<jcharArray>("charArrayField");
    auto m2 = obj.getField<jshortArray>("shortArrayField");
    auto n2 = obj.getField<jintArray>("intArrayField");
    auto o2 = obj.getField<jlongArray>("longArrayField");
    auto p2 = obj.getField<jfloatArray>("floatArrayField");
    auto q2 = obj.getField<jdoubleArray>("doubleArrayField");
    EXPECT_EQ(a, a2);
    EXPECT_EQ(b, b2);
    EXPECT_EQ(c, c2);
    EXPECT_EQ(d, d2);
    EXPECT_EQ(e, e2);
    EXPECT_EQ(f, f2);
    EXPECT_EQ(g, g2);
    EXPECT_EQ(h, h2);
    EXPECT_TRUE(compare(i, i2));
    EXPECT_TRUE(compare(j, j2));
    EXPECT_TRUE(compare(k, k2));
    EXPECT_TRUE(compare(l, l2));
    EXPECT_TRUE(compare(m, m2));
    EXPECT_TRUE(compare(n, n2));
    EXPECT_TRUE(compare(o, o2));
    EXPECT_TRUE(compare(p, p2));
    EXPECT_TRUE(compare(q, q2));
}

TEST_F(InstanceTest, Invoke)
{
    auto obj = cls.createObject();
    auto a1 = obj.invokeMethod<jboolean>("getBoolean");
    auto b1 = obj.invokeMethod<jbyte>("getByte");
    auto c1 = obj.invokeMethod<jchar>("getChar");
    auto d1 = obj.invokeMethod<jshort>("getShort");
    auto e1 = obj.invokeMethod<jint>("getInt");
    auto f1 = obj.invokeMethod<jlong>("getLong");
    auto g1 = obj.invokeMethod<jfloat>("getFloat");
    auto h1 = obj.invokeMethod<jdouble>("getDouble");
    auto i1 = obj.invokeMethod<JString>("getString");
    auto j1 = obj.invokeMethod<jbooleanArray>("getBooleanArray");
    auto k1 = obj.invokeMethod<jbyteArray>("getByteArray");
    auto l1 = obj.invokeMethod<jcharArray>("getCharArray");
    auto m1 = obj.invokeMethod<jshortArray>("getShortArray");
    auto n1 = obj.invokeMethod<jintArray>("getIntArray");
    auto o1 = obj.invokeMethod<jlongArray>("getLongArray");
    auto p1 = obj.invokeMethod<jfloatArray>("getFloatArray");
    auto q1 = obj.invokeMethod<jdoubleArray>("getDoubleArray");
    EXPECT_NE(a, a1);
    EXPECT_NE(b, b1);
    EXPECT_NE(c, c1);
    EXPECT_NE(d, d1);
    EXPECT_NE(e, e1);
    EXPECT_NE(f, f1);
    EXPECT_NE(g, g1);
    EXPECT_NE(h, h1);
    EXPECT_FALSE(compare(i, i1));
    EXPECT_FALSE(compare(j, j1));
    EXPECT_FALSE(compare(k, k1));
    EXPECT_FALSE(compare(l, l1));
    EXPECT_FALSE(compare(m, m1));
    EXPECT_FALSE(compare(n, n1));
    EXPECT_FALSE(compare(o, o1));
    EXPECT_FALSE(compare(p, p1));
    EXPECT_FALSE(compare(q, q1));
    obj.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));
    obj.invokeMethod<void>("voidMethodNoArgs");
    obj.invokeMethod<void>("setBoolean", a);
    obj.invokeMethod<void>("setByte", b);
    obj.invokeMethod<void>("setChar", c);
    obj.invokeMethod<void>("setShort", d);
    obj.invokeMethod<void>("setInt", e);
    obj.invokeMethod<void>("setLong", f);
    obj.invokeMethod<void>("setFloat", g);
    obj.invokeMethod<void>("setDouble", h);
    obj.invokeMethod<void>("setString", static_cast<jstring>(i));
    obj.invokeMethod<void>("setBooleanArray", static_cast<jbooleanArray>(j));
    obj.invokeMethod<void>("setByteArray", static_cast<jbyteArray>(k));
    obj.invokeMethod<void>("setCharArray", static_cast<jcharArray>(l));
    obj.invokeMethod<void>("setShortArray", static_cast<jshortArray>(m));
    obj.invokeMethod<void>("setIntArray", static_cast<jintArray>(n));
    obj.invokeMethod<void>("setLongArray", static_cast<jlongArray>(o));
    obj.invokeMethod<void>("setFloatArray", static_cast<jfloatArray>(p));
    obj.invokeMethod<void>("setDoubleArray", static_cast<jdoubleArray>(q));
    auto a2 = obj.invokeMethod<jboolean>("getBoolean");
    auto b2 = obj.invokeMethod<jbyte>("getByte");
    auto c2 = obj.invokeMethod<jchar>("getChar");
    auto d2 = obj.invokeMethod<jshort>("getShort");
    auto e2 = obj.invokeMethod<jint>("getInt");
    auto f2 = obj.invokeMethod<jlong>("getLong");
    auto g2 = obj.invokeMethod<jfloat>("getFloat");
    auto h2 = obj.invokeMethod<jdouble>("getDouble");
    auto i2 = obj.invokeMethod<JString>("getString");
    auto j2 = obj.invokeMethod<jbooleanArray>("getBooleanArray");
    auto k2 = obj.invokeMethod<jbyteArray>("getByteArray");
    auto l2 = obj.invokeMethod<jcharArray>("getCharArray");
    auto m2 = obj.invokeMethod<jshortArray>("getShortArray");
    auto n2 = obj.invokeMethod<jintArray>("getIntArray");
    auto o2 = obj.invokeMethod<jlongArray>("getLongArray");
    auto p2 = obj.invokeMethod<jfloatArray>("getFloatArray");
    auto q2 = obj.invokeMethod<jdoubleArray>("getDoubleArray");
    EXPECT_EQ(a, a2);
    EXPECT_EQ(b, b2);
    EXPECT_EQ(c, c2);
    EXPECT_EQ(d, d2);
    EXPECT_EQ(e, e2);
    EXPECT_EQ(f, f2);
    EXPECT_EQ(g, g2);
    EXPECT_EQ(h, h2);
    EXPECT_TRUE(compare(i, i2));
    EXPECT_TRUE(compare(j, j2));
    EXPECT_TRUE(compare(k, k2));
    EXPECT_TRUE(compare(l, l2));
    EXPECT_TRUE(compare(m, m2));
    EXPECT_TRUE(compare(n, n2));
    EXPECT_TRUE(compare(o, o2));
    EXPECT_TRUE(compare(p, p2));
    EXPECT_TRUE(compare(q, q2));

    auto test = obj.invokeMethod<JObjectS<lv_gusc_jni_tests_TestClass>>("getTestClass");
    EXPECT_EQ(static_cast<std::string>(test.invokeMethod<JString>("getString")), std::string{"ASDF"});
    auto testInstance = testClass.createObjectS().createGlobalRefS();
    obj.invokeMethod<void>("setTestClass", testInstance);

    auto testArray = obj.invokeMethod<JObjectArray<lv_gusc_jni_tests_TestClass>>("getTestClassArray");
    auto test2 = testArray[0];
    EXPECT_EQ(static_cast<std::string>(test2.invokeMethod<JString>("getString")), std::string{"ASDF"});
}

TEST_F(InstanceTest, Fields)
{
    auto obj = cls.createObject();
    auto a1 = obj.getField<jboolean>("booleanField");
    auto b1 = obj.getField<jbyte>("byteField");
    auto c1 = obj.getField<jchar>("charField");
    auto d1 = obj.getField<jshort>("shortField");
    auto e1 = obj.getField<jint>("intField");
    auto f1 = obj.getField<jlong>("longField");
    auto g1 = obj.getField<jfloat>("floatField");
    auto h1 = obj.getField<jdouble>("doubleField");
    auto i1 = obj.getField<JString>("stringField");
    auto j1 = obj.getField<jbooleanArray>("booleanArrayField");
    auto k1 = obj.getField<jbyteArray>("byteArrayField");
    auto l1 = obj.getField<jcharArray>("charArrayField");
    auto m1 = obj.getField<jshortArray>("shortArrayField");
    auto n1 = obj.getField<jintArray>("intArrayField");
    auto o1 = obj.getField<jlongArray>("longArrayField");
    auto p1 = obj.getField<jfloatArray>("floatArrayField");
    auto q1 = obj.getField<jdoubleArray>("doubleArrayField");
    EXPECT_NE(a, a1);
    EXPECT_NE(b, b1);
    EXPECT_NE(c, c1);
    EXPECT_NE(d, d1);
    EXPECT_NE(e, e1);
    EXPECT_NE(f, f1);
    EXPECT_NE(g, g1);
    EXPECT_NE(h, h1);
    EXPECT_FALSE(compare(i, i1));
    EXPECT_FALSE(compare(j, j1));
    EXPECT_FALSE(compare(k, k1));
    EXPECT_FALSE(compare(l, l1));
    EXPECT_FALSE(compare(m, m1));
    EXPECT_FALSE(compare(n, n1));
    EXPECT_FALSE(compare(o, o1));
    EXPECT_FALSE(compare(p, p1));
    EXPECT_FALSE(compare(q, q1));
    obj.setField("booleanField", a);
    obj.setField("byteField", b);
    obj.setField("charField", c);
    obj.setField("shortField", d);
    obj.setField("intField", e);
    obj.setField("longField", f);
    obj.setField("floatField", g);
    obj.setField("doubleField", h);
    obj.setField("stringField", i);
    obj.setField("booleanArrayField", static_cast<jbooleanArray>(j));
    obj.setField("byteArrayField", static_cast<jbyteArray>(k));
    obj.setField("charArrayField", static_cast<jcharArray>(l));
    obj.setField("shortArrayField", static_cast<jshortArray>(m));
    obj.setField("intArrayField", static_cast<jintArray>(n));
    obj.setField("longArrayField", static_cast<jlongArray>(o));
    obj.setField("floatArrayField", static_cast<jfloatArray>(p));
    obj.setField("doubleArrayField", static_cast<jdoubleArray>(q));
    auto a2 = obj.getField<jboolean>("booleanField");
    auto b2 = obj.getField<jbyte>("byteField");
    auto c2 = obj.getField<jchar>("charField");
    auto d2 = obj.getField<jshort>("shortField");
    auto e2 = obj.getField<jint>("intField");
    auto f2 = obj.getField<jlong>("longField");
    auto g2 = obj.getField<jfloat>("floatField");
    auto h2 = obj.getField<jdouble>("doubleField");
    auto i2 = obj.getField<JString>("stringField");
    auto j2 = obj.getField<jbooleanArray>("booleanArrayField");
    auto k2 = obj.getField<jbyteArray>("byteArrayField");
    auto l2 = obj.getField<jcharArray>("charArrayField");
    auto m2 = obj.getField<jshortArray>("shortArrayField");
    auto n2 = obj.getField<jintArray>("intArrayField");
    auto o2 = obj.getField<jlongArray>("longArrayField");
    auto p2 = obj.getField<jfloatArray>("floatArrayField");
    auto q2 = obj.getField<jdoubleArray>("doubleArrayField");
    EXPECT_EQ(a, a2);
    EXPECT_EQ(b, b2);
    EXPECT_EQ(c, c2);
    EXPECT_EQ(d, d2);
    EXPECT_EQ(e, e2);
    EXPECT_EQ(f, f2);
    EXPECT_EQ(g, g2);
    EXPECT_EQ(h, h2);
    EXPECT_TRUE(compare(i, i2));
    EXPECT_TRUE(compare(j, j2));
    EXPECT_TRUE(compare(k, k2));
    EXPECT_TRUE(compare(l, l2));
    EXPECT_TRUE(compare(m, m2));
    EXPECT_TRUE(compare(n, n2));
    EXPECT_TRUE(compare(o, o2));
    EXPECT_TRUE(compare(p, p2));
    EXPECT_TRUE(compare(q, q2));

    auto test = obj.getField<JObjectS<lv_gusc_jni_tests_TestClass>>("testClassField");
    EXPECT_EQ(static_cast<std::string>(test.invokeMethod<JString>("getString")), std::string{"ASDF"});
    auto testInstance = testClass.createObjectS().createGlobalRefS();
    obj.setField("testClassField", testInstance);
}
