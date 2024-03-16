//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;
using namespace ::testing;

constexpr const char lv_gusc_jni_tests_ObjectClass[] = "lv.gusc.jni.tests.ObjectClass";
constexpr const char java_lang_Number[] = "java.lang.Number";
constexpr const char java_lang_Integer[] = "java.lang.Integer";

class ObjectTest : public Test
{
public:
    template<typename TJni, typename TJava>
    bool compare(TJni& jni, TJava& java)
    {
        TJni java2 { java };
        return static_cast<typename TJni::StlType>(jni) == static_cast<typename TJni::StlType>(java2);
    }
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

}