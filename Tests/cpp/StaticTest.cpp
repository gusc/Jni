//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;
using namespace ::testing;

constexpr const char lv_gusc_jni_tests_TestClass[] = "lv.gusc.jni.tests.TestClass";

class StaticTest : public Test
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

    JClassS<lv_gusc_jni_tests_TestClass> testClass;

    // Static class has global state, so after invoke calls we need to use inverted values for field access
    jboolean aa { true };
    jbyte bb { 0x7F };
    jchar cc { 'z' };
    jshort dd { 0x7FFF };
    jint ee { 0x7FFFFFFF };
    jlong ff { 0x7FFFFFFFFFFFFFFF };
    jfloat gg { 1.0f };
    jdouble hh { 2.0 };
    JString ii { JString::createFrom("asdf") };
    JBooleanArray jj { JBooleanArray::createFrom({ true, true }) };
    JByteArray kk { JByteArray::createFrom({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) };
    JCharArray ll { JCharArray::createFrom({ 'a', 'b', 'c', 'd' }) };
    JShortArray mm { JShortArray ::createFrom({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) };
    JIntArray nn { JIntArray::createFrom({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) };
    JLongArray oo { JLongArray::createFrom({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) };
    JFloatArray pp { JFloatArray::createFrom({ 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f }) };
    JDoubleArray qq { JDoubleArray::createFrom({ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 }) };
};

TEST_F(StaticTest, Invoke)
{
    JClass cls { JVM::getEnv().getClass("lv/gusc/jni/tests/StaticClass") };
    auto a1 = cls.invokeMethod<jboolean>("getBoolean");
    auto b1 = cls.invokeMethod<jbyte>("getByte");
    auto c1 = cls.invokeMethod<jchar>("getChar");
    auto d1 = cls.invokeMethod<jshort>("getShort");
    auto e1 = cls.invokeMethod<jint>("getInt");
    auto f1 = cls.invokeMethod<jlong>("getLong");
    auto g1 = cls.invokeMethod<jfloat>("getFloat");
    auto h1 = cls.invokeMethod<jdouble>("getDouble");
    auto i1 = cls.invokeMethod<JString>("getString");
    auto j1 = cls.invokeMethod<jbooleanArray>("getBooleanArray");
    auto k1 = cls.invokeMethod<jbyteArray>("getByteArray");
    auto l1 = cls.invokeMethod<jcharArray>("getCharArray");
    auto m1 = cls.invokeMethod<jshortArray>("getShortArray");
    auto n1 = cls.invokeMethod<jintArray>("getIntArray");
    auto o1 = cls.invokeMethod<jlongArray>("getLongArray");
    auto p1 = cls.invokeMethod<jfloatArray>("getFloatArray");
    auto q1 = cls.invokeMethod<jdoubleArray>("getDoubleArray");
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
    cls.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));
    cls.invokeMethod<void>("voidMethodNoArgs");
    cls.invokeMethod<void>("setBoolean", a);
    cls.invokeMethod<void>("setByte", b);
    cls.invokeMethod<void>("setChar", c);
    cls.invokeMethod<void>("setShort", d);
    cls.invokeMethod<void>("setInt", e);
    cls.invokeMethod<void>("setLong", f);
    cls.invokeMethod<void>("setFloat", g);
    cls.invokeMethod<void>("setDouble", h);
    cls.invokeMethod<void>("setString", static_cast<jstring>(i));
    cls.invokeMethod<void>("setBooleanArray", static_cast<jbooleanArray>(j));
    cls.invokeMethod<void>("setByteArray", static_cast<jbyteArray>(k));
    cls.invokeMethod<void>("setCharArray", static_cast<jcharArray>(l));
    cls.invokeMethod<void>("setShortArray", static_cast<jshortArray>(m));
    cls.invokeMethod<void>("setIntArray", static_cast<jintArray>(n));
    cls.invokeMethod<void>("setLongArray", static_cast<jlongArray>(o));
    cls.invokeMethod<void>("setFloatArray", static_cast<jfloatArray>(p));
    cls.invokeMethod<void>("setDoubleArray", static_cast<jdoubleArray>(q));
    auto a2 = cls.invokeMethod<jboolean>("getBoolean");
    auto b2 = cls.invokeMethod<jbyte>("getByte");
    auto c2 = cls.invokeMethod<jchar>("getChar");
    auto d2 = cls.invokeMethod<jshort>("getShort");
    auto e2 = cls.invokeMethod<jint>("getInt");
    auto f2 = cls.invokeMethod<jlong>("getLong");
    auto g2 = cls.invokeMethod<jfloat>("getFloat");
    auto h2 = cls.invokeMethod<jdouble>("getDouble");
    auto i2 = cls.invokeMethod<JString>("getString");
    auto j2 = cls.invokeMethod<jbooleanArray>("getBooleanArray");
    auto k2 = cls.invokeMethod<jbyteArray>("getByteArray");
    auto l2 = cls.invokeMethod<jcharArray>("getCharArray");
    auto m2 = cls.invokeMethod<jshortArray>("getShortArray");
    auto n2 = cls.invokeMethod<jintArray>("getIntArray");
    auto o2 = cls.invokeMethod<jlongArray>("getLongArray");
    auto p2 = cls.invokeMethod<jfloatArray>("getFloatArray");
    auto q2 = cls.invokeMethod<jdoubleArray>("getDoubleArray");
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

    auto test = cls.invokeMethod<JObjectS<lv_gusc_jni_tests_TestClass>>("getTestClass");
    EXPECT_EQ(static_cast<std::string>(test.invokeMethod<JString>("getString")), std::string{"ASDF"});
    auto testInstance = testClass.createObjectS().createGlobalRefS();
    cls.invokeMethod<void>("setTestClass", testInstance);
}

TEST_F(StaticTest, Fields)
{
    JClass cls { JVM::getEnv().getClass("lv/gusc/jni/tests/StaticClass") };
    auto a1 = cls.getField<jboolean>("booleanField");
    auto b1 = cls.getField<jbyte>("byteField");
    auto c1 = cls.getField<jchar>("charField");
    auto d1 = cls.getField<jshort>("shortField");
    auto e1 = cls.getField<jint>("intField");
    auto f1 = cls.getField<jlong>("longField");
    auto g1 = cls.getField<jfloat>("floatField");
    auto h1 = cls.getField<jdouble>("doubleField");
    auto i1 = cls.getField<JString>("stringField");
    auto j1 = cls.getField<jbooleanArray>("booleanArrayField");
    auto k1 = cls.getField<jbyteArray>("byteArrayField");
    auto l1 = cls.getField<jcharArray>("charArrayField");
    auto m1 = cls.getField<jshortArray>("shortArrayField");
    auto n1 = cls.getField<jintArray>("intArrayField");
    auto o1 = cls.getField<jlongArray>("longArrayField");
    auto p1 = cls.getField<jfloatArray>("floatArrayField");
    auto q1 = cls.getField<jdoubleArray>("doubleArrayField");
    EXPECT_NE(aa, a1);
    EXPECT_NE(bb, b1);
    EXPECT_NE(cc, c1);
    EXPECT_NE(dd, d1);
    EXPECT_NE(ee, e1);
    EXPECT_NE(ff, f1);
    EXPECT_NE(gg, g1);
    EXPECT_NE(hh, h1);
    EXPECT_FALSE(compare(ii, i1));
    EXPECT_FALSE(compare(jj, j1));
    EXPECT_FALSE(compare(kk, k1));
    EXPECT_FALSE(compare(ll, l1));
    EXPECT_FALSE(compare(mm, m1));
    EXPECT_FALSE(compare(nn, n1));
    EXPECT_FALSE(compare(oo, o1));
    EXPECT_FALSE(compare(pp, p1));
    EXPECT_FALSE(compare(qq, q1));
    cls.setField("booleanField", aa);
    cls.setField("byteField", bb);
    cls.setField("charField", cc);
    cls.setField("shortField", dd);
    cls.setField("intField", ee);
    cls.setField("longField", ff);
    cls.setField("floatField", gg);
    cls.setField("doubleField", hh);
    cls.setField("stringField", ii);
    cls.setField("booleanArrayField", static_cast<jbooleanArray>(jj));
    cls.setField("byteArrayField", static_cast<jbyteArray>(kk));
    cls.setField("charArrayField", static_cast<jcharArray>(ll));
    cls.setField("shortArrayField", static_cast<jshortArray>(mm));
    cls.setField("intArrayField", static_cast<jintArray>(nn));
    cls.setField("longArrayField", static_cast<jlongArray>(oo));
    cls.setField("floatArrayField", static_cast<jfloatArray>(pp));
    cls.setField("doubleArrayField", static_cast<jdoubleArray>(qq));
    auto a2 = cls.getField<jboolean>("booleanField");
    auto b2 = cls.getField<jbyte>("byteField");
    auto c2 = cls.getField<jchar>("charField");
    auto d2 = cls.getField<jshort>("shortField");
    auto e2 = cls.getField<jint>("intField");
    auto f2 = cls.getField<jlong>("longField");
    auto g2 = cls.getField<jfloat>("floatField");
    auto h2 = cls.getField<jdouble>("doubleField");
    auto i2 = cls.getField<JString>("stringField");
    auto j2 = cls.getField<jbooleanArray>("booleanArrayField");
    auto k2 = cls.getField<jbyteArray>("byteArrayField");
    auto l2 = cls.getField<jcharArray>("charArrayField");
    auto m2 = cls.getField<jshortArray>("shortArrayField");
    auto n2 = cls.getField<jintArray>("intArrayField");
    auto o2 = cls.getField<jlongArray>("longArrayField");
    auto p2 = cls.getField<jfloatArray>("floatArrayField");
    auto q2 = cls.getField<jdoubleArray>("doubleArrayField");
    EXPECT_EQ(aa, a2);
    EXPECT_EQ(bb, b2);
    EXPECT_EQ(cc, c2);
    EXPECT_EQ(dd, d2);
    EXPECT_EQ(ee, e2);
    EXPECT_EQ(ff, f2);
    EXPECT_EQ(gg, g2);
    EXPECT_EQ(hh, h2);
    EXPECT_TRUE(compare(ii, i2));
    EXPECT_TRUE(compare(jj, j2));
    EXPECT_TRUE(compare(kk, k2));
    EXPECT_TRUE(compare(ll, l2));
    EXPECT_TRUE(compare(mm, m2));
    EXPECT_TRUE(compare(nn, n2));
    EXPECT_TRUE(compare(oo, o2));
    EXPECT_TRUE(compare(pp, p2));
    EXPECT_TRUE(compare(qq, q2));

    auto test = cls.getField<JObjectS<lv_gusc_jni_tests_TestClass>>("testClassField");
    EXPECT_EQ(static_cast<std::string>(test.invokeMethod<JString>("getString")), std::string{"ASDF"});
    auto testInstance = testClass.createObjectS().createGlobalRefS();
    cls.setField("testClassField", testInstance);
}