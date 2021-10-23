package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class NativeClassTest {

    public long nativePtr = 0;

    @Keep
    NativeClassTest(long initNatvePtr)
    {
        nativePtr = initNatvePtr;
    }

    @Keep
    void runTest()
    {
        boolean a = true;
        byte b = 1;
        char c = 'a';
        short d = 512;
        int e = 0xFFFFFF;
        long f = 0xFFFFFFFFFFFFL;
        float g = 1.5f;
        double h = 2.3;
        String i = "asdf";

        nativeVoidMethodNoArgs();
        a = nativeBooleanMethod(a);
        b = nativeByteMethod(b);
        c = nativeCharMethod(c);
        d = nativeShortMethod(d);
        e = nativeIntMethod(e);
        f = nativeLongMethod(f);
        g = nativeFloatMethod(g);
        h = nativeDoubleMethod(h);
        i = nativeStringMethod(i);
        nativeVoidMethod(a, b, c, d, e, f, g, h , i);
    }

    native void nativeVoidMethodNoArgs();
    native void nativeVoidMethod(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i);
    native boolean nativeBooleanMethod(boolean val);
    native byte nativeByteMethod(byte val);
    native char nativeCharMethod(char val);
    native short nativeShortMethod(short val);
    native int nativeIntMethod(int val);
    native long nativeLongMethod(long val);
    native float nativeFloatMethod(float val);
    native double nativeDoubleMethod(double val);
    native String nativeStringMethod(String val);
}
