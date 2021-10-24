//
// Created by Gusts Kaksis on 23/10/2021.
//

#ifndef TESTS_INSTANCETEST_HPP
#define TESTS_INSTANCETEST_HPP

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

    a = obj.invokeMethod<jboolean>("getBoolean");
    b = obj.invokeMethod<jbyte>("getByte");
    c = obj.invokeMethod<jchar>("getChar");
    d = obj.invokeMethod<jshort>("getShort");
    e = obj.invokeMethod<jint>("getInt");
    f = obj.invokeMethod<jlong>("getLong");
    g = obj.invokeMethod<jfloat>("getFloat");
    h = obj.invokeMethod<jdouble>("getDouble");
    i = std::move(obj.invokeMethod<jstring>("getString"));

    obj.setField("booleanField", a);
    obj.setField("byteField", b);
    obj.setField("charField", c);
    obj.setField("shortField", d);
    obj.setField("intField", e);
    obj.setField("longField", f);
    obj.setField("floatField", g);
    obj.setField("doubleField", h);
    obj.setField("stringField", static_cast<jstring>(i));

    a = obj.getField<jboolean>("booleanField");
    b = obj.getField<jbyte>("byteField");
    c = obj.getField<jchar>("charField");
    d = obj.getField<jshort>("shortField");
    e = obj.getField<jint>("intField");
    f = obj.getField<jlong>("longField");
    g = obj.getField<jfloat>("floatField");
    h = obj.getField<jdouble>("doubleField");
    i = std::move(obj.getField<jstring>("stringField"));

    obj.invokeMethod<void>("voidMethod", a, b, c, d, e, f, g, h, static_cast<jstring>(i));

}

void runInstanceTest()
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
        auto obj = cls.createObject(a, b, c, d, e, f, g, h, static_cast<jstring>(i));
        runInstanceTestMethodsAndFields(obj);
    }

}


#endif //TESTS_INSTANCETEST_HPP
