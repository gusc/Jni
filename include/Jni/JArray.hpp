#ifndef __GUSC_JARRAY_HPP
#define __GUSC_JARRAY_HPP 1

#include <jni.h>
#include <type_traits>
#include <vector>

namespace gusc::Jni
{

template<typename TCpp, typename TJArray, typename TJArrayElement>
class JArray final : public JObject
{
    class JArrayData final
    {
    public:
        JArrayData(const TJArray& initArray)
            : jniArray(initArray)
        {
            if (jniArray)
            {
                auto env = JVM::getEnv();
                length = env->GetArrayLength(jniArray);
                dataPtr = getDataPtr(env);
            }
        }

        JArrayData(const JArrayData&) = delete;
        JArrayData& operator=(const JArrayData&) = delete;

        inline operator TCpp() const
        {
            return TCpp(dataPtr, dataPtr + length);
        }

        inline TJArrayElement* data() const
        {
            return dataPtr;
        }

        inline const TJArrayElement& operator[](int index) const
        {
            return *(data() + index);
        }

        ~JArrayData()
        {
            if (jniArray && dataPtr)
            {
                auto env = JVM::getEnv();
                freeDataPtr(env);
                dataPtr = nullptr;
                length = 0;
            }
        }
    private:
        TJArray jniArray { nullptr };
        std::size_t length { 0 };
        TJArrayElement* dataPtr { nullptr };

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jcharArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetCharArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jbyteArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetByteArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jshortArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetShortArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jintArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetIntArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jlongArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetLongArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jfloatArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetFloatArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetDoubleArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, TJArrayElement*>
        getDataPtr(JEnv& env)
        {
            return env->GetBooleanArrayElements(jniArray, nullptr);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jbyteArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseByteArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jcharArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseCharArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jshortArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseShortArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jintArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseIntArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jlongArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseLongArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jfloatArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseFloatArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseDoubleArrayElements(jniArray, dataPtr, JNI_ABORT);
        }

        template<typename T=TJArray>
        inline
        typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, void>
        freeDataPtr(JEnv& env)
        {
            env->ReleaseBooleanArrayElements(jniArray, dataPtr, JNI_ABORT);
        }
    };
public:
    using StlType = TCpp;
    using JniType = TJArray;

    JArray(const TJArray& initArray)
        : JObject(static_cast<jobject>(initArray))
    {}
    /// @deprecated
    JArray(const JEnv& /*env*/, const TJArray& initArray)
        : JArray(initArray)
    {}

    inline operator TCpp()
    {
        JArrayData data { static_cast<TJArray>(jniObject) };
        return static_cast<TCpp>(data);
    }

    inline operator TJArray() const
    {
        return static_cast<TJArray>(jniObject);
    }

    inline TJArrayElement operator[](int index) const
    {
        JArrayData data { static_cast<TJArray>(jniObject) };
        return data[index];
    }

    inline JArrayData getData() const
    {
        return JArrayData { static_cast<TJArray>(jniObject) };
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewByteArray(vector.size());
        env->SetByteArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewCharArray(vector.size());
        env->SetCharArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewShortArray(vector.size());
        env->SetShortArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewIntArray(vector.size());
        env->SetIntArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewLongArray(vector.size());
        env->SetLongArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewFloatArray(vector.size());
        env->SetFloatArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewDoubleArray(vector.size());
        env->SetDoubleArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, JArray<TCpp, T, TJArrayElement>>
    createFrom(JEnv env, const TCpp& vector)
    {
        TJArray a = env->NewBooleanArray(vector.size());
        env->SetBooleanArrayRegion(a, 0, vector.size(), reinterpret_cast<const TJArrayElement*>(vector.data()));
        return a;
    }

    static inline JArray<TCpp, TJArray, TJArrayElement> createFrom(const TCpp& vector)
    {
        return createFrom(JVM::getEnv(), vector);
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbyteArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewByteArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jcharArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewCharArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jshortArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewShortArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jintArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewIntArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jlongArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewLongArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jfloatArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewFloatArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jdoubleArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewDoubleArray(static_cast<jsize>(initSize));
    }

    template<typename T=TJArray>
    static inline
    typename std::enable_if_t<std::is_same_v<T, jbooleanArray>, JArray<TCpp, T, TJArrayElement>>
    createNew(JEnv env, std::size_t initSize)
    {
        return env->NewBooleanArray(static_cast<jsize>(initSize));
    }

    static inline JArray<TCpp, TJArray, TJArrayElement> createNew(std::size_t initSize)
    {
        return createNew(JVM::getEnv(), initSize);
    }

};

class JObjectArray : public JObject
{
public:
    using JniType = jobjectArray;

    JObjectArray(const jobject& initArray)
            : JObject { initArray }
    {}
    JObjectArray(const jobjectArray& initArray)
            : JObjectArray { static_cast<jobject>(initArray) }
    {}

    inline operator std::vector<JObject>()
    {
        auto env = JVM::getEnv();
        std::vector<JObject> vector;
        for (int i = 0; i < env->GetArrayLength(static_cast<jarray>(jniObject)); ++i)
        {
            auto obj = env->GetObjectArrayElement(static_cast<jobjectArray>(jniObject), static_cast<jsize>(i));
            vector.emplace_back(obj);
        }
        return vector;
    }

    inline operator jobjectArray() const
    {
        return static_cast<jobjectArray>(jniObject);
    }

    inline JObject operator[](int index)
    {
        auto env = JVM::getEnv();
        return JObject{ env->GetObjectArrayElement(static_cast<jobjectArray>(jniObject), static_cast<jsize>(index)) };
    }

    static inline JObjectArray createFrom(const std::vector<JObject>& vector, const JClass& elementClass)
    {
        auto env = JVM::getEnv();
        auto arr = env->NewObjectArray(vector.size(), static_cast<jclass>(elementClass), nullptr);
        int i = 0;
        for (const auto& o : vector)
        {
            env->SetObjectArrayElement(arr, static_cast<jsize>(i), static_cast<jobject>(o));
            ++i;
        }
        return arr;
    }

    static inline JObjectArray createNew(std::size_t initSize, const JObject& initObject, const JClass& elementClass)
    {
        auto env = JVM::getEnv();
        return env->NewObjectArray(static_cast<jsize>(initSize), static_cast<jclass>(elementClass), static_cast<jobject>(initObject));
    }

};

template<const char ClassName[]>
class JObjectArrayS : public JObjectArray
{
public:

    JObjectArrayS(const jobject& initArray)
        : JObjectArray { initArray }
    {}
    JObjectArrayS(const jobjectArray& initArray)
        : JObjectArrayS { static_cast<jobject>(initArray) }
    {}

    inline operator std::vector<JObjectS<ClassName>>()
    {
        auto env = JVM::getEnv();
        std::vector<JObjectS<ClassName>> vector;
        for (int i = 0; i < env->GetArrayLength(static_cast<jarray>(jniObject)); ++i)
        {
            auto obj = env->GetObjectArrayElement(static_cast<jobjectArray>(jniObject), static_cast<jsize>(i));
            vector.emplace_back(obj);
        }
        return vector;
    }

    inline JObjectS<ClassName> operator[](int index)
    {
        auto env = JVM::getEnv();
        return JObjectS<ClassName>{ env->GetObjectArrayElement(static_cast<jobjectArray>(jniObject), static_cast<jsize>(index)) };
    }

    static inline JObjectArrayS<ClassName> createFromS(const std::vector<JObjectS<ClassName>>& vector)
    {
        auto env = JVM::getEnv();
        JClassS<ClassName> cls;
        auto arr = env->NewObjectArray(vector.size(), static_cast<jclass>(cls), nullptr);
        int i = 0;
        for (const auto& o : vector)
        {
            env->SetObjectArrayElement(arr, static_cast<jsize>(i), static_cast<jobject>(o));
            ++i;
        }
        return arr;
    }

    static inline JObjectArrayS<ClassName> createNewS(std::size_t initSize, const JObjectS<ClassName>& initObject)
    {
        auto env = JVM::getEnv();
        JClassS<ClassName> cls;
        return env->NewObjectArray(static_cast<jsize>(initSize), static_cast<jclass>(cls), static_cast<jobject>(initObject));
    }

    static constexpr const char* getElementClassName()
    {
        return ClassName;
    }

};

}

#endif // __GUSC_JARRAY_HPP
