#ifndef __GUSC_JARRAY_HPP
#define __GUSC_JARRAY_HPP 1

#include <jni.h>
#include <type_traits>

namespace gusc::Jni
{

template<typename TCpp, typename TJArray, typename TJArrayElement>
class JArray final : public JObject
{
    class JArrayData final
    {
    public:
        JArrayData(JEnv env, const TJArray& initArray)
            : jniArray(initArray)
        {
            if (jniArray)
            {
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

    JArray(JEnv env, TJArray initArray)
        : JObject(env, static_cast<jobject>(initArray))
    {}
    JArray(TJArray initArray)
        : JArray(JVM::getEnv(), initArray)
    {}

    inline operator TCpp()
    {
        JArrayData data(JVM::getEnv(), static_cast<TJArray>(jniObject));
        return static_cast<TCpp>(data);
    }

    inline operator TJArray() const
    {
        return static_cast<TJArray>(jniObject);
    }

    inline JArrayData getData() const
    {
        return JArrayData(JVM::getEnv(), static_cast<TJArray>(jniObject));
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

}

#endif // __GUSC_JARRAY_HPP
