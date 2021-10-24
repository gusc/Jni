//
// Created by Gusts Kaksis on 23/10/2021.
//

#ifndef TESTS_STATICTEST_HPP
#define TESTS_STATICTEST_HPP

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

    a = cls.invokeMethod<jboolean>("getBoolean");
    b = cls.invokeMethod<jbyte>("getByte");
    c = cls.invokeMethod<jchar>("getChar");
    d = cls.invokeMethod<jshort>("getShort");
    e = cls.invokeMethod<jint>("getInt");
    f = cls.invokeMethod<jlong>("getLong");
    g = cls.invokeMethod<jfloat>("getFloat");
    h = cls.invokeMethod<jdouble>("getDouble");
    i = cls.invokeMethod<jstring>("getString");

    cls.setField("booleanField", a);
    cls.setField("byteField", b);
    cls.setField("charField", c);
    cls.setField("shortField", d);
    cls.setField("intField", e);
    cls.setField("longField", f);
    cls.setField("floatField", g);
    cls.setField("doubleField", h);
    cls.setField("stringField", static_cast<jstring>(i));

    a = cls.getField<jboolean>("booleanField");
    b = cls.getField<jbyte>("byteField");
    c = cls.getField<jchar>("charField");
    d = cls.getField<jshort>("shortField");
    e = cls.getField<jint>("intField");
    f = cls.getField<jlong>("longField");
    g = cls.getField<jfloat>("floatField");
    h = cls.getField<jdouble>("doubleField");
    i = cls.getField<jstring>("stringField");

    cls.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));

}

void runStaticTest()
{
    auto cls = JVM::getEnv().getClass("lv/gusc/jni/tests/StaticTest");
    runStaticTestMethodsAndFields(cls);
}

#endif //TESTS_STATICTEST_HPP
