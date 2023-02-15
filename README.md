# Jni wrapper library

This C++17 header-only library provides handy wrapper classes for JNI objects.

The library provides:

* Safe way to manage `jobject` lifetimes.
* Safe way to get `JniEnv` pointer that is associated with current thread.
* Simpler method invocation with automatic signature generation for simple JNI types.
* Simpler field getters and setters with automatic signature generation for simple JNI types.
* Java exception marshalling.

Simple usage:

```c++
JObject strObj = JVM::getEnv().getClass("java/lang/String").createObject();
const auto strLen = strObj.invokeMethod<jlong>("length");
```

Register native method:

```c++
void myNativeMethod(JNIEnv* /*env*/, jobject thiz, jlong val)
{
    JObject obj(thiz);
    const auto fieldVal = obj.getField<jlong>("someLongField");
    obj.invokeMethod<void>("callBack", val + fieldVal);
}

void registerMethod()
{
    JVM::getEnv().getClass("my/java/Class").registerNativeMethod<void>("myNativeMethod", &myNativeMethod);
}
```

## Build options

* `Jni_BuildTests` - build unittests
* `Jni_AutoInit` - automatically initialize JVM static object

## Classes

### JVM

Java virtual machine pointer wrapper.

Methods:

* `static void init(JavaVM*)` - initialize JVM static object manually. This must be called before any other class can be used! This is automatically called when `Jni_AutoInit` build option is enabled.
* `static JEnv getEnv()` - get JNI environment wrapper for current thread.

### JEnv

JNI environment wrapper.

Constructors:

* `JEnv(JNIEnv*)` - the wrapper constructor

Methods:

* `JClass getClass(const char* classPath)` - get a JClass wrapper object for a class path (i.e. `java/lang/String`).
* `JClass getObjectClass(jobject)` - get a JClass wrapper object for a JNI object.

Internal JNI helper methods (you might not need to use them):

* `JNIEnv* operator->()` - access JNIEnv pointer

### JClass

Java class object wrapper.

The wrapper manages the wrapped jobject lifetime and deletes reference when wrapper get's destroyed.

Constructors:

* `JClass(JEnv&, jclass)` - the wrapper constructor

Methods:

* `std::string getClassPath()` - get the class path of current JClass object (handy for signature generation)
* `void registerNativeMethodSign<TReturn, TArgs...>(const char* name, const char* signature, TReturn(*fn)(JNIEnv*, jobject, TArgs...))` - register a native method that matches the name and signature
* `void registerNativeMethod<TReturn, TArgs...>(const char* name, TReturn(*fn)(JNIEnv*, jobject, TArgs...))` - register a native method that matches the name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `JObject createObjectSign<TArgs...>(const char* signature, const TArgs&... args)` - construct a new object by calling a constructor that matches the signature
* `JObject createObject<TArgs...>(const TArgs&... args)` - construct a new object (the signature is automatically generated at compile time from TArgs... types)
* `TReturn invokeMethodSign<TReturn, TArgs...>(const char* name, const char* signature, const TArgs&... args)` - invoke static method by name and signature
* `TReturn invokeMethod<TReturn, TArgs...>(const char* name, const TArgs&... args)` - invoke static method by name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `T getFieldSign<T>(const char* name, const char* signature)` - get static field value by name and signature
* `T getField<T>(const char* name)` - get static field value by name (the signature is automatically generated at compile time from T type)
* `void setFieldSign<T>(const char* name, const char* signature, const T& value)` - set static field value by name and signature
* `void setField<T>(const char* name, const T& value)` - set static field value by name (the signature is automatically generated at compile time from T type)

Internal JNI helper methods (you might not need to use them):

* `operator jclass()` - access `jclass` object
* `jmethodID getStaticMethodIdSign(const char* name, const char* signature)` - get a static method ID from name and signature strings
* `jmethodID getStaticMethodId<TReturn, TArgs...>(const char* name)` - get a static method ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `jmethodID getMethodIdSign(const char* name, const char* signature)` - get an instance method ID from name and signature strings
* `jmethodID getMethodId<TReturn, TArgs...>(const char* name)` - get an instance method ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `jmethodID getStaticFieldIdSign(const char* name, const char* signature)` - get a static field ID from name and signature strings
* `jmethodID getStaticFieldId<TReturn, TArgs...>(const char* name)` - get a static field ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `jmethodID getFieldIdSign(const char* name, const char* signature)` - get an instance field ID from name and signature strings
* `jmethodID getFieldId(const char* name)` - get an instance field ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)

### JObject

Java object instance wrapper.

The wrapper manages the wrapped jobject lifetime and deletes reference when wrapper get's destroyed.

Constructors:

* `JObject(jobject)` - the wrapper constructor

Methods:

* `TReturn invokeMethodSign<TReturn, TArgs...>(const char* name, const char* signature, const TArgs&... args)` - invoke instance method by name and signature
* `TReturn invokeMethod<TReturn, TArgs...>(const char* name, const TArgs&... args)` - invoke instance method by name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `T getFieldSign<T>(const char* name, const char* signature)` - get instance field value by name and signature
* `T getField<T>(const char* name)` - get instance field value by name (the signature is automatically generated at compile time from T type)
* `void setFieldSign<T>(const char* name, const char* signature, const T& value)` - set instance field value by name and signature
* `void setField<T>(const char* name, const T& value)` - set instance field value by name (the signature is automatically generated at compile time from T type)

Internal JNI helper methods (you might not need to use them):

* `operator jobject()` - access `jobject` object
* `jmethodID getMethodIdSign(const char* name, const char* signature)` - get an instance method ID from name and signature strings
* `jmethodID getMethodId<TReturn, TArgs...>(const char* name)` - get an instance method ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)
* `jmethodID getFieldIdSign(const char* name, const char* signature)` - get an instance field ID from name and signature strings
* `jmethodID getFieldId(const char* name)` - get an instance field ID from name (the signature is automatically generated at compile time from TReturn and TArgs... types)

### JGlobalRef

Global reference container - use this class to take global reference ownership of the Java object.

Constructors:

* `JGlobalRef(jobject)`

Methods:

* `JObject* operator->()` - access the underlying `JObject`

Internal JNI helper methods (you might not need to use them):

* `operator jobject()` - access `jobject` object

### JString

Wrapper for `jstring` object that gives RAII safe approach to convert it into `std::string` and back.

Constructors:

* `JString(JEnv, jstring)` - the wrapper constructor

Methods:

* `static JString createFrom(JEnv, const std::string&)` - construct a new `JString` from `std::string`
* `operator std::string()` - construct new `std::string` from `jstring`
* `operator jstring()` - access internal `jstring` object

### JArray

Wrapper for `j*Array` objects that gives RAII safe approach to convert it into `std::vector<T>` and back.

Predefined types:

* `JBooleanArray` - converts `jbooleanArray` to `std::vector<bool>` and back
* `JByteArray` - converts `jbyteArray` to `std::vector<std::int8_t>` and back
* `JShortArray` - converts `jshortArray` to `std::vector<std::int16_t>` and back
* `JIntArray` - converts `jintArray` to `std::vector<std::int32_t>` and back
* `JLongArray` - converts `jlongArray` to `std::vector<std::int64_t>` and back
* `JFloatArray` - converts `jfloatArray` to `std::vector<float>` and back
* `JDoubleArray` - converts `jdoubleArray` to `std::vector<double>` and back

Methods:

Constructors:

* `JArray<TCpp, TJni, TJniEl>(JEnv, TJni)` - the wrapper constructor (wrap `TJni` - i.e. `j*Array` object)

Methods:

* `static TJni createFrom(JEnv, const std::vector<TCpp>&)` - construct a new `TJni` from `std::vector<TCpp>`
* `operator std::vector<TCpp>()` - construct new `std::vecotr<TCpp>` from `TJni`
* `operator TJni()` - access internal `TJni` object

## TODO

1. Proper container (JString, JArray) tests
2. Global references for containers (rethink the global references in general and define stricter rules)
3. Signature generator for JObject, maybe non-type template parameters can be used to achieve the constexpr generation or be able to gracefully fall back to runtime generation when JObject is passed
4. Also be able to forward JObject, JString etc. objects as arguments with internal cast to jobject and jstring respectively (currently all of them are just forwarded which won't work)
5. Exception marshalling from C++ to Java
