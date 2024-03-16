//
// Created by Gusts Kaksis on 23/10/2021.
//

#include <gtest/gtest.h>
#include <vector>
#include "Jni/Jni.hpp"

using namespace gusc::Jni;
using namespace ::testing;

constexpr const char lv_gusc_jni_tests_TestClass[] = "lv.gusc.jni.tests.TestClass";

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

}

TEST_F(ObjectTest, Instance)
{
    
}