//
// Created by Gusts Kaksis on 02/01/2024.
//

#include <gtest/gtest.h>
#include <jni.h>
#include "Jni/Jni.hpp"
#include "Jni/private/signature.hpp"

using namespace gusc::Jni;
using namespace ::testing;

constexpr const char customClassName[] = "some.custom.class";

class PrivateTest : public Test
{
public:
    bool compare(const char* a, const char* b)
    {
        return strcmp(a, b) == 0;
    }
};

TEST_F(PrivateTest, ArgumentSignature)
{
    auto a = Private::getArgumentSignature<jboolean>();
    EXPECT_TRUE(compare(a.str, "Z"));
    auto b = Private::getArgumentSignature<jbyte>();
    EXPECT_TRUE(compare(b.str, "B"));
    auto c = Private::getArgumentSignature<jchar>();
    EXPECT_TRUE(compare(c.str, "C"));
    auto d = Private::getArgumentSignature<jshort>();
    EXPECT_TRUE(compare(d.str, "S"));
    auto e = Private::getArgumentSignature<jint>();
    EXPECT_TRUE(compare(e.str, "I"));
    auto f = Private::getArgumentSignature<jlong>();
    EXPECT_TRUE(compare(f.str, "J"));
    auto g = Private::getArgumentSignature<jfloat>();
    EXPECT_TRUE(compare(g.str, "F"));
    auto h = Private::getArgumentSignature<jdouble>();
    EXPECT_TRUE(compare(h.str, "D"));
    auto i = Private::getArgumentSignature<jstring>();
    EXPECT_TRUE(compare(i.str, "Ljava/lang/String;"));
    auto j = Private::getArgumentSignature<jbooleanArray>();
    EXPECT_TRUE(compare(j.str, "[Z"));
    auto k = Private::getArgumentSignature<jbyteArray>();
    EXPECT_TRUE(compare(k.str, "[B"));
    auto l = Private::getArgumentSignature<jcharArray>();
    EXPECT_TRUE(compare(l.str, "[C"));
    auto m = Private::getArgumentSignature<jshortArray>();
    EXPECT_TRUE(compare(m.str, "[S"));
    auto n = Private::getArgumentSignature<jintArray>();
    EXPECT_TRUE(compare(n.str, "[I"));
    auto o = Private::getArgumentSignature<jlongArray>();
    EXPECT_TRUE(compare(o.str, "[J"));
    auto p = Private::getArgumentSignature<jfloatArray>();
    EXPECT_TRUE(compare(p.str, "[F"));
    auto q = Private::getArgumentSignature<jdoubleArray>();
    EXPECT_TRUE(compare(q.str, "[D"));

    auto z = Private::getArgumentSignature<JObjectS<customClassName>>();
    EXPECT_TRUE(compare(z.str, "Lsome/custom/class;"));
}

TEST_F(PrivateTest, MethodSignature)
{
    auto a = Private::getMethodSignature<jboolean, jboolean>();
    EXPECT_TRUE(compare(a.str, "(Z)Z"));
    auto b = Private::getMethodSignature<jbyte, jbyte>();
    EXPECT_TRUE(compare(b.str, "(B)B"));
    auto c = Private::getMethodSignature<jchar, jchar>();
    EXPECT_TRUE(compare(c.str, "(C)C"));
    auto d = Private::getMethodSignature<jshort, jshort>();
    EXPECT_TRUE(compare(d.str, "(S)S"));
    auto e = Private::getMethodSignature<jint, jint>();
    EXPECT_TRUE(compare(e.str, "(I)I"));
    auto f = Private::getMethodSignature<jlong, jlong>();
    EXPECT_TRUE(compare(f.str, "(J)J"));
    auto g = Private::getMethodSignature<jfloat, jfloat>();
    EXPECT_TRUE(compare(g.str, "(F)F"));
    auto h = Private::getMethodSignature<jdouble, jdouble>();
    EXPECT_TRUE(compare(h.str, "(D)D"));
    auto i = Private::getMethodSignature<jstring, jstring>();
    EXPECT_TRUE(compare(i.str, "(Ljava/lang/String;)Ljava/lang/String;"));
    auto j = Private::getMethodSignature<jbooleanArray, jbooleanArray>();
    EXPECT_TRUE(compare(j.str, "([Z)[Z"));
    auto k = Private::getMethodSignature<jbyteArray, jbyteArray>();
    EXPECT_TRUE(compare(k.str, "([B)[B"));
    auto l = Private::getMethodSignature<jcharArray, jcharArray>();
    EXPECT_TRUE(compare(l.str, "([C)[C"));
    auto m = Private::getMethodSignature<jshortArray, jshortArray>();
    EXPECT_TRUE(compare(m.str, "([S)[S"));
    auto n = Private::getMethodSignature<jintArray, jintArray>();
    EXPECT_TRUE(compare(n.str, "([I)[I"));
    auto o = Private::getMethodSignature<jlongArray, jlongArray>();
    EXPECT_TRUE(compare(o.str, "([J)[J"));
    auto p = Private::getMethodSignature<jfloatArray, jfloatArray>();
    EXPECT_TRUE(compare(p.str, "([F)[F"));
    auto q = Private::getMethodSignature<jdoubleArray, jdoubleArray>();
    EXPECT_TRUE(compare(q.str, "([D)[D"));

    auto z = Private::getMethodSignature<JObjectS<customClassName>, JObjectS<customClassName>>();
    EXPECT_TRUE(compare(z.str, "(Lsome/custom/class;)Lsome/custom/class;"));
    auto zz = Private::getMethodSignature<JObjectS<customClassName>, jboolean, jchar, jint, jfloat, JObjectS<customClassName>>();
    EXPECT_TRUE(compare(zz.str, "(ZCIFLsome/custom/class;)Lsome/custom/class;"));
}
