#ifndef __GUSC_PRIVATE_FORWARD_HPP
#define __GUSC_PRIVATE_FORWARD_HPP

namespace gusc::Jni
{
    class JClass;
    class JObject;
    template<const char ClassName[]>
    struct JObjectS;
    class JString;
    template<typename TCpp=std::vector<std::int8_t>, typename TJArray=jbyteArray, typename TJArrayElement=jbyte>
    class JArray;
    using JByteArray = JArray<>;
    using JCharArray = JArray<std::vector<char>, jcharArray, jchar>;
    using JShortArray = JArray<std::vector<std::int16_t>, jshortArray, jshort>;
    using JIntArray = JArray<std::vector<std::int32_t>, jintArray, jint>;
    using JLongArray = JArray<std::vector<std::int64_t>, jlongArray, jlong>;
    using JFloatArray = JArray<std::vector<float>, jfloatArray, jfloat>;
    using JDoubleArray = JArray<std::vector<double>, jdoubleArray, jdouble>;
    using JBooleanArray = JArray<std::vector<char>, jbooleanArray, jboolean>;
}

#endif //__GUSC_PRIVATE_FORWARD_HPP
