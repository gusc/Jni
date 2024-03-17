package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class ObjectClass {

    // Static tests

    @Keep
    public static int intField = 123;

    @Keep
    public static String stringField = "asdf";

    @Keep
    public static Integer integerField = 1234;

    @Keep
    static Number testMethodStatic(String string) {
        return Integer.parseInt(string);
    }

    @Keep
    static void testMethod2Static(String string) {
        stringField = string;
    }

    // Instance tests

    ObjectClass() {
    }

    ObjectClass(int a, String b) {
        intMember = a;
        stringMember = b;
    }

    @Keep
    public int intMember = 987;

    @Keep
    public String stringMember = "qwerty";

    @Keep
    public Integer integerMember = 987654321;

    @Keep
    Number testMethod(String string) {
        return Integer.parseInt(string);
    }

    @Keep
    void testMethod2(String string) {
        stringMember = string;
    }

    // Native tests

    @Keep
    native void nativeVoidMethod();

    @Keep
    void callNativeVoid()
    {
        nativeVoidMethod();
    }

    @Keep
    native String nativeMethod(Integer value);

    @Keep
    int callNative(int in)
    {
        return Integer.parseInt(nativeMethod(in));
    }

}
