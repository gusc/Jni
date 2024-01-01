//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;

void runInstanceTestMethodsAndFields(JObject& obj)
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

    a = obj.invokeMethod<jboolean>("getBoolean");
    b = obj.invokeMethod<jbyte>("getByte");
    c = obj.invokeMethod<jchar>("getChar");
    d = obj.invokeMethod<jshort>("getShort");
    e = obj.invokeMethod<jint>("getInt");
    f = obj.invokeMethod<jlong>("getLong");
    g = obj.invokeMethod<jfloat>("getFloat");
    h = obj.invokeMethod<jdouble>("getDouble");
    i = obj.invokeMethod<JString>("getString");
    j = obj.invokeMethod<jbooleanArray>("getBooleanArray");
    k = obj.invokeMethod<jbyteArray>("getByteArray");
    l = obj.invokeMethod<jcharArray>("getCharArray");
    m = obj.invokeMethod<jshortArray>("getShortArray");
    n = obj.invokeMethod<jintArray>("getIntArray");
    o = obj.invokeMethod<jlongArray>("getLongArray");
    p = obj.invokeMethod<jfloatArray>("getFloatArray");
    q = obj.invokeMethod<jdoubleArray>("getDoubleArray");

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

    a = obj.getField<jboolean>("booleanField");
    b = obj.getField<jbyte>("byteField");
    c = obj.getField<jchar>("charField");
    d = obj.getField<jshort>("shortField");
    e = obj.getField<jint>("intField");
    f = obj.getField<jlong>("longField");
    g = obj.getField<jfloat>("floatField");
    h = obj.getField<jdouble>("doubleField");
    i = obj.getField<JString>("stringField");
    j = obj.getField<jbooleanArray>("booleanArrayField");
    k = obj.getField<jbyteArray>("byteArrayField");
    l = obj.getField<jcharArray>("charArrayField");
    m = obj.getField<jshortArray>("shortArrayField");
    n = obj.getField<jintArray>("intArrayField");
    o = obj.getField<jlongArray>("longArrayField");
    p = obj.getField<jfloatArray>("floatArrayField");
    q = obj.getField<jdoubleArray>("doubleArrayField");

    obj.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));

}

TEST(InstanceTest, InstanceTest)
{
    auto cls = JVM::getEnv().getClass("lv/gusc/jni/tests/InstanceTest");

    {
        auto objNoArgs = cls.createObject();
        runInstanceTestMethodsAndFields(objNoArgs);
    }
    {
        jboolean a = false;
        jbyte b = 2;
        jchar c = 'b';
        jshort d = 16;
        jint e = 9876543;
        jlong f = 1234567890;
        jfloat g = 5.0f;
        jdouble h = 10.0;
        auto i = JString::createFrom("test");
        auto j = JBooleanArray::createFrom({ false, true, true, false });
        auto k = JByteArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
        auto l = JCharArray::createFrom({ 'd', 'c', 'b', 'a' });
        auto m = JShortArray ::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
        auto n = JIntArray::createFrom({ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
        auto o = JLongArray::createFrom({ 1234567890, 1234567891, 1234567892, 1234567893 });
        auto p = JFloatArray::createFrom({ 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f });
        auto q = JDoubleArray::createFrom({ 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 });

        auto obj = cls.createObject(a, b, c, d, e, f, g, h, static_cast<jstring>(i),
                                    static_cast<jbooleanArray>(j),
                                    static_cast<jbyteArray>(k),
                                    static_cast<jcharArray>(l),
                                    static_cast<jshortArray>(m),
                                    static_cast<jintArray>(n),
                                    static_cast<jlongArray>(o),
                                    static_cast<jfloatArray>(p),
                                    static_cast<jdoubleArray>(q)
                                    );
        runInstanceTestMethodsAndFields(obj);
    }

}
