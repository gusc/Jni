# Jni wrapper library

This C++17 header-only library provides handy wrapper classes for JNI objects.

The library provides:

* Safe way to manage `jobject` lifetimes.
* Safe way to get `JniEnv` pointer that is associated with current thread.
* Simpler method invocation with automatic signature generation for simple JNI types.
* Simpler field getters and setters with automatic signature generation for simple JNI types.
* Java exception marshalling.
* A more advanced class name to signature generation using constexpr character strings as non-type template parameters

Simple usage:

```c++
JObject strObj = JVM::getEnv().getClass("java/lang/String").createObject();
const auto strLen = strObj.invokeMethod<jlong>("length");
```

More advanced use case:

```java
package my.package;

class MyClass
{
    private Number x = 0;
    MyClass() {
    }
    MyClass(Number n) {
        x = n;
    }
    Number getX() {
        return x;
    }
    void setX(Number n) {
        x = n;
    }
}
```

```c++
constexpr const char java_lang_Number[] = "java.lang.Number";
constexpr const char my_package_MyClass[] = "my.package.MyClass";

JClassS<java_lang_Number> ncls;
JObjectS<java_lang_Number> n = ncls.createObjectS();
JString str = n.invokeMethod<JString>("toString");

JClassS<my_package_MyClass> mycls;
JObjectS<my_package_MyClass> my = mycls.createObjectS(n);
JObjectS<java_lang_Number> n2 = my.invokeMethod<JObjectS<java_lang_Number>>("getX");
my.invokeMethod<JObjectS<java_lang_Number>>("setX", n);
my.invokeMethod<JObjectS<java_lang_Number>>("setX", n2);
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

* `JClass(const jclass&)` - the wrapper constructor

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

### JClassS<ClassName>

Template jclass wrapepr with Java class name passed via non-type template parameter (`constexpr const char ClassName[]`). This class extends JClass so it has all the same methods as above with additional methods listed bellow.

Constructors:

* `JClassS()` - create new jclass wrapper
* `JClassS(const jclass&)` - wrap around an existing JNI object

Methods:

* `JObjectS<ClassName> createObjectS(const TArgs&... args)`- create new object and return `JObjectS<ClassName>`
* `JClassS<ClassName> createGlobalRefS()`- create new global reference to jclass object and wrap it in `JClassS<ClassName>`
* `JClassS<ClassName> createWeakGlobalRefS()`- create new weak global reference to jclass object and wrap it in `JClassS<ClassName>`
* `static constexpr const char* getClassName()`- get class name passed via non-type template parameter

### JObject

Java object instance wrapper.

The wrapper manages the wrapped jobject lifetime and deletes reference when wrapper get's destroyed.

Constructors:

* `JObject()` - construct an empty JNI object
* `JObject(const jobject&)` - the wrapper constructor

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

### JObjectS<ClassName>

Template jobject wrapepr with Java class name passed via non-type template parameter (`constexpr const char ClassName[]`). This class extends JObject so it has all the same methods as above, with additional methods listed bellow.

Constructors:

* `JObjectS()` - create empty JNI object wrapper
* `JObjectS(const jobject&)` - wrap around an existing JNI object

Methods:

* `JObjectS<ClassName> createGlobalRefS()`- create new global reference to jclass object and wrap it in `JObjectS<ClassName>`
* `JObjectS<ClassName> createWeakGlobalRefS()`- create new weak global reference to jclass object and wrap it in `JObjectS<ClassName>`
* `static constexpr const char* getClassName()`- get class name passed via non-type template parameter

### JGlobalRef (deprecated)

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

* `JString(const jstring&)` - the wrapper constructor

Methods:

* `static JString createFrom(JEnv, const std::string&)` - construct a new `JString` from `std::string`
* `operator std::string()` - construct new `std::string` from `jstring`
* `operator jstring()` - access internal `jstring` object

### JArray

Wrapper for `j*Array` objects that gives RAII safe approach to convert it into `std::vector<T>` and back.

Predefined types:

* `JBooleanArray` - converts `jbooleanArray` to `std::vector<bool>` and back
* `JByteArray` - converts `jbyteArray` to `std::vector<std::int8_t>` and back
* `JCharArray` - converts `jcharArray` to `std::vector<char>` and back
* `JShortArray` - converts `jshortArray` to `std::vector<std::int16_t>` and back
* `JIntArray` - converts `jintArray` to `std::vector<std::int32_t>` and back
* `JLongArray` - converts `jlongArray` to `std::vector<std::int64_t>` and back
* `JFloatArray` - converts `jfloatArray` to `std::vector<float>` and back
* `JDoubleArray` - converts `jdoubleArray` to `std::vector<double>` and back

Methods:

Constructors:

* `JArray<TCpp, TJArray, TJArrayElement>(TJArray)` - the wrapper constructor (wrap `TJArray` - i.e. `j*Array` object)

Methods:

* `static TJni createFrom(JEnv, const std::vector<TCpp>&)` - construct a new `TJni` from `std::vector<TCpp>`
* `operator std::vector<TCpp>()` - construct new `std::vecotr<TCpp>` from `TJni`
* `operator TJni()` - access internal `TJni` object

### JObjectArray

JNI object arrays are handled differently from other array types so it has it's own specialization of the array wrapper.

Constructors:

* `JObjectArray(const jobjectArray&)`

Methods:

* `operator std::vector<JObject>()` - construct new `std::vecotr<JObject>` from `jobjectArray` (wraps each element in `JObject` class)
* `operator jobjectArray()` - access internal `jobjectArray` object
* `JObject operator[](int index)` - access each element of the array directly
* `static JObjectArray createFrom(const std::vector<JObject>& cppArray, const JClass& elementClass)` - create a new `jobjectArray` wrapper from `std::vector` of `JObject` elements
* `static JObjectArray createNew(std::size_t initSize, const JObject& initObject, const JClass& elementClass)` - create a new `jobjectArray` wrapper

### JObjectArrayS<ElementClassName>

Template jobject wrapepr with Java class name passed via non-type template parameter (`constexpr const char ClassName[]`). This class extends JObject so it has all the same methods as above, with additional methods listed bellow.

Constructors:

* `JObjectArrayS(const jobjectArray&)`

Methods:

* `operator std::vector<JObjectS<ElementClassName>>()` - construct new `std::vecotr<JObjectS<ElementClassName>>` from `jobjectArray` (wraps each element in `JObjectS` class)
* `JObjectS<ElementClassName> operator[](int index)` - access each element of the array directly
* `static JObjectArrayS<ElementClassName> createFromS(const std::vector<JObjectS<ClassName>>& vector)` - create a new `jobjectArray` wrapper from `std::vector` of `JObjectS` elements
* `static JObjectArrayS<ElementClassName> createNewS(std::size_t initSize, const JObjectS<ClassName>& initObject)` - create a new `jobjectArray` wrapper
* `static constexpr const char* getClassName()`- get class name passed via non-type template parameter

## TODO

1. Global references for arrays (maybe drop JGlobalRef)
2. Exception marshalling from C++ to Java
3. Introduce new abstraction with just JNI::Object which would act like a wrapper around both JObjectS and JClassS and act like dirrect mirror to Java class with option to register native methods, call static methods via `static invoke` method and instance methods via instance `invoke` method and be constructable via C++ constructor.

### Idea dump

```c++

// it would be nice to have something like this:

constexpr const char java_lang_StringBuffer[] = "java.lang.StringBuffer";
constexpr const char java_lang_Thread[] = "java.lang.Thread";
constexpr const char my_package_MyClass[] = "my.package.MyClass";

                                                        // Java equivalents:
// Construct new JNI object                                                         
JNI::Object<java_lang_StringBuffer> buffer { 1024 };    // StringBuffer buffer = new StringBuffer(1024);

// Invoke instance method
auto c = buffer.invoke<char>("charAt", 123);            // char c = buffer.charAt(123);

// Invoke static method
auto currentThread = JNI::Object<java_lang_Thread>::invoke<JNI::Object<java_lang_Thread>>("currentThread"); 
                                                        // Thread currentThread = Thread.currentThread();

// Get static field and immediatelly cast it to STL type
auto x = JNI::Object<my_package_MyClass>::getField<std::string>("staticStringField");
// Set static field from STL/C++ type
JNI::Object<my_package_MyClass>::setField("staticStringField", "some string");

JNI::Object<my_package_MyClass> my; 

// Get instance field
auto y = my.getField<std::string>("instanceStringField");
// Set instance field
my.setField("instanceStringField", "my string");

```


