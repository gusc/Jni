//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;

void runStaticTestMethodsAndFields(JClass& cls)
{
    jboolean a = true;
    jbyte b = 3;
    jchar c = 'c';
    jshort d = 32;
    jint e = 10000000;
    jlong f = 10987654321;
    jfloat g = 6.3f;
    jdouble h = 20.2;
    auto i = JString::createFrom("test 2");
    auto j = JBooleanArray::createFrom({ false, true, true, false });
    auto k = JByteArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
    auto l = JCharArray::createFrom({ 'd', 'c', 'b', 'a' });
    auto m = JShortArray ::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
    auto n = JIntArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
    auto o = JLongArray::createFrom({ 1234567890, 1234567891, 1234567892, 1234567893 });
    auto p = JFloatArray::createFrom({ 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f });
    auto q = JDoubleArray::createFrom({ 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 });

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

    a = cls.invokeMethod<jboolean>("getBoolean");
    b = cls.invokeMethod<jbyte>("getByte");
    c = cls.invokeMethod<jchar>("getChar");
    d = cls.invokeMethod<jshort>("getShort");
    e = cls.invokeMethod<jint>("getInt");
    f = cls.invokeMethod<jlong>("getLong");
    g = cls.invokeMethod<jfloat>("getFloat");
    h = cls.invokeMethod<jdouble>("getDouble");
    i = cls.invokeMethod<jstring>("getString");
    j = cls.invokeMethod<jbooleanArray>("getBooleanArray");
    k = cls.invokeMethod<jbyteArray>("getByteArray");
    l = cls.invokeMethod<jcharArray>("getCharArray");
    m = cls.invokeMethod<jshortArray>("getShortArray");
    n = cls.invokeMethod<jintArray>("getIntArray");
    o = cls.invokeMethod<jlongArray>("getLongArray");
    p = cls.invokeMethod<jfloatArray>("getFloatArray");
    q = cls.invokeMethod<jdoubleArray>("getDoubleArray");

    cls.setField("booleanField", a);
    cls.setField("byteField", b);
    cls.setField("charField", c);
    cls.setField("shortField", d);
    cls.setField("intField", e);
    cls.setField("longField", f);
    cls.setField("floatField", g);
    cls.setField("doubleField", h);
    cls.setField("stringField", i);
    cls.setField("booleanArrayField", static_cast<jbooleanArray>(j));
    cls.setField("byteArrayField", static_cast<jbyteArray>(k));
    cls.setField("charArrayField", static_cast<jcharArray>(l));
    cls.setField("shortArrayField", static_cast<jshortArray>(m));
    cls.setField("intArrayField", static_cast<jintArray>(n));
    cls.setField("longArrayField", static_cast<jlongArray>(o));
    cls.setField("floatArrayField", static_cast<jfloatArray>(p));
    cls.setField("doubleArrayField", static_cast<jdoubleArray>(q));

    a = cls.getField<jboolean>("booleanField");
    b = cls.getField<jbyte>("byteField");
    c = cls.getField<jchar>("charField");
    d = cls.getField<jshort>("shortField");
    e = cls.getField<jint>("intField");
    f = cls.getField<jlong>("longField");
    g = cls.getField<jfloat>("floatField");
    h = cls.getField<jdouble>("doubleField");
    i = cls.getField<jstring>("stringField");
    j = cls.getField<jbooleanArray>("booleanArrayField");
    k = cls.getField<jbyteArray>("byteArrayField");
    l = cls.getField<jcharArray>("charArrayField");
    m = cls.getField<jshortArray>("shortArrayField");
    n = cls.getField<jintArray>("intArrayField");
    o = cls.getField<jlongArray>("longArrayField");
    p = cls.getField<jfloatArray>("floatArrayField");
    q = cls.getField<jdoubleArray>("doubleArrayField");

    cls.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));

}

TEST(StaticTest, StaticTest)
{
    auto cls = JVM::getEnv().getClass("lv/gusc/jni/tests/StaticTest");
    runStaticTestMethodsAndFields(cls);
}
