//
// Created by Gusts Kaksis on 16/03/2024.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"
#include "ObjectClassMock.hpp"
#include "ClassNames.hpp"

using namespace gusc::Jni;
using namespace ::testing;

class ObjectTest : public Test
{

};

TEST_F(ObjectTest, Static)
{
    // ObjectClass.intField = 567;
    // var val = ObjectClass.intField;
    Object<lv_gusc_jni_tests_ObjectClass>::setStatic("intField", 567);
    const auto intVal = Object<lv_gusc_jni_tests_ObjectClass>::getStatic<int>("intField");
    EXPECT_EQ(intVal, 567);

    // ObjectClass.stringField = "foobar";
    // var val = ObjectClass.stringField;
    Object<lv_gusc_jni_tests_ObjectClass>::setStatic("stringField", JString { "foobar" });
    const auto strVal = Object<lv_gusc_jni_tests_ObjectClass>::getStatic<JString>("stringField");
    EXPECT_EQ(static_cast<std::string>(strVal), "foobar");

    // ObjectClass.integerField = 1;
    // var val = ObjectClass.integerField.intValue();;
    Object<java_lang_Integer> integerValIn(1);
    Object<lv_gusc_jni_tests_ObjectClass>::setStatic("integerField", integerValIn);
    const auto integerValOut = Object<lv_gusc_jni_tests_ObjectClass>::getStatic<Object<java_lang_Integer>>("integerField");
    const auto numValInt = integerValOut.invoke<int>("intValue");
    EXPECT_EQ(numValInt, 1);

    // Number num = ObjectClass.testMethodStatic("123456");
    // var val = num.intValue();
    auto numVal = Object<lv_gusc_jni_tests_ObjectClass>::invokeStatic<Object<java_lang_Number>>("testMethodStatic", JString{"123456"});
    const auto numValInt2 = numVal.invoke<int>("intValue");
    EXPECT_EQ(numValInt2, 123456);

    // ObjectClass.testMethod2Static("asdf"); // sets stringField
    // var val = ObjectClass.stringField;
    Object<lv_gusc_jni_tests_ObjectClass>::invokeStatic<void>("testMethod2Static", JString{"asdf"});
    const auto str2Val = Object<lv_gusc_jni_tests_ObjectClass>::getStatic<JString>("stringField");
    EXPECT_EQ(static_cast<std::string>(str2Val), "asdf");
}

TEST_F(ObjectTest, Instance)
{
    // ObjectClass obj = new ObjectClass();
    Object<lv_gusc_jni_tests_ObjectClass> obj;

    // obj.intMember = 5;
    // var val = obj.intMember;
    obj.set("intMember", 5);
    const auto intVal = obj.get<int>("intMember");
    EXPECT_EQ(intVal, 5);

    // obj.stringMember = "root";
    // var val = obj.stringMember;
    obj.set("stringMember", JString{"root"});
    const auto stringVal = obj.get<JString>("stringMember");
    EXPECT_EQ(static_cast<std::string>(stringVal), "root");

    // obj.integerMember = 999;
    // var val = obj.integerMember.intValue();
    Object<java_lang_Integer> integerValIn(999);
    obj.set("integerMember", integerValIn);
    const auto integerValOut = obj.get<Object<java_lang_Integer>>("integerMember");
    const auto numValInt = integerValOut.invoke<int>("intValue");
    EXPECT_EQ(numValInt, 999);

    // Number num = obj.testMethod("777");
    // var val = num.intValue();
    auto numVal = obj.invoke<Object<java_lang_Number>>("testMethod", JString{"777"});
    const auto numValInt2 = numVal.invoke<int>("intValue");
    EXPECT_EQ(numValInt2, 777);

    // obj.testMethod2("asdf"); // sets stringMember
    // var val = obj.stringMember;
    obj.invoke<void>("testMethod2", JString{"asdf"});
    const auto str2Val = obj.get<JString>("stringMember");
    EXPECT_EQ(static_cast<std::string>(str2Val), "asdf");

    // ObjectClass obj2 = new ObjectClass(12, "as");
    Object<lv_gusc_jni_tests_ObjectClass> obj2 { 12, JString{"as"} };

    // var val = obj2.intMember;
    const auto intVal2 = obj2.get<int>("intMember");
    EXPECT_EQ(intVal2, 12);

    // var val = obj2.stringMember;
    const auto stringVal2 = obj2.get<JString>("stringMember");
    EXPECT_EQ(static_cast<std::string>(stringVal2), "as");
}

TEST_F(ObjectTest, Native)
{
    ObjectClassMock::instance = std::make_unique<ObjectClassMock>();
    Object<lv_gusc_jni_tests_ObjectClass>::registerNativeMethod("nativeMethod", &ObjectClassMock::nativeMethod);
    Object<lv_gusc_jni_tests_ObjectClass>::registerNativeMethod("nativeVoidMethod", &ObjectClassMock::nativeVoidMethod);

    // ObjectClass obj = new ObjectClass();
    Object<lv_gusc_jni_tests_ObjectClass> obj;

    // obj.callNativeVoid();
    EXPECT_CALL(*ObjectClassMock::instance.get(), nativeVoidMethodMock).Times(1);
    obj.invoke<void>("callNativeVoid");

    // obj.callNative();
    const auto res = obj.invoke<int>("callNative", 5);
    EXPECT_EQ(res, 5);
}