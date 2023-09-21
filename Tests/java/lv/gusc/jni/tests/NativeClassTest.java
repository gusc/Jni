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
        boolean[] j = {false, true};
        byte[] k = {0, 1, 2, 3, 4, 5, 6, 7};
        char[] l = {'a', 'b', 'c', 'd'};
        short[] m = {0, 1, 2, 3, 4, 5, 6, 7};
        int[] n = {0, 1, 2, 3, 4, 5, 6, 7};
        long[] o = {0, 1, 2, 3, 4, 5, 6, 7};
        float[] p = {0, 1, 2, 3, 4, 5, 6, 7};
        double[] q = {0, 1, 2, 3, 4, 5, 6, 7};

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
        j = nativeBooleanArrayMethod(j);
        k = nativeByteArrayMethod(k);
        l = nativeCharArrayMethod(l);
        m = nativeShortArrayMethod(m);
        n = nativeIntArrayMethod(n);
        o = nativeLongArrayMethod(o);
        p = nativeFloatArrayMethod(p);
        q = nativeDoubleArrayMethod(q);
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
    native boolean[] nativeBooleanArrayMethod(boolean[] val);
    native byte[] nativeByteArrayMethod(byte[] val);
    native char[] nativeCharArrayMethod(char[] val);
    native short[] nativeShortArrayMethod(short[] val);
    native int[] nativeIntArrayMethod(int[] val);
    native long[] nativeLongArrayMethod(long[] val);
    native float[] nativeFloatArrayMethod(float[] val);
    native double[] nativeDoubleArrayMethod(double[] val);

}
