//
// Created by Gusts Kaksis on 17/03/2024.
//

#ifndef TESTS_OBJECTCLASSMOCK_HPP
#define TESTS_OBJECTCLASSMOCK_HPP

#include "Jni/Jni.hpp"
#include <gmock/gmock.h>
#include "ClassNames.hpp"

using namespace gusc::Jni;
using namespace ::testing;

class ObjectClassMock
{
public:
    MOCK_METHOD(void, nativeVoidMethodMock, ());

    static std::unique_ptr<ObjectClassMock> instance;

    static void nativeVoidMethod(JNIEnv*, jobject /* thiz */)
    {
        instance->nativeVoidMethodMock();
    }

    static JString nativeMethod(JNIEnv*, jobject /* thiz */, Object<java_lang_Integer> val)
    {
        const auto intVal = val.invoke<int>("intValue");
        return JString{ std::to_string(intVal) };
    }

};

std::unique_ptr<ObjectClassMock> ObjectClassMock::instance;

#endif //TESTS_OBJECTCLASSMOCK_HPP
