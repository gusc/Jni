//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include "Jni/Jni.hpp"
#include "NativeClassMock.hpp"

TEST(NativeClassTest, NativeClassTest)
{
    NativeClassMock test;
    test.runTest();
}
