package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class NativeClass {

    public long nativePtr = 0;

    @Keep
    NativeClass(long initNatvePtr)
    {
        nativePtr = initNatvePtr;
    }

    @Keep
    boolean booleanMethod(boolean val)
    {
        return nativeBooleanMethod(val);
    }

    @Keep
    byte byteMethod(byte val)
    {
        return nativeByteMethod(val);
    }

    @Keep
    char charMethod(char val)
    {
        return nativeCharMethod(val);
    }

    @Keep
    short shortMethod(short val)
    {
        return nativeShortMethod(val);
    }

    @Keep
    int intMethod(int val)
    {
        return nativeIntMethod(val);
    }

    @Keep
    long longMethod(long val)
    {
        return nativeLongMethod(val);
    }

    @Keep
    float floatMethod(float val)
    {
        return nativeFloatMethod(val);
    }

    @Keep
    double doubleMethod(double val)
    {
        return nativeDoubleMethod(val);
    }

    @Keep
    String stringMethod(String val)
    {
        return nativeStringMethod(val);
    }

    @Keep
    boolean[] booleanArrayMethod(boolean val[])
    {
        return nativeBooleanArrayMethod(val);
    }

    @Keep
    byte[] byteArrayMethod(byte val[])
    {
        return nativeByteArrayMethod(val);
    }

    @Keep
    char[] charArrayMethod(char val[])
    {
        return nativeCharArrayMethod(val);
    }

    @Keep
    short[] shortArrayMethod(short val[])
    {
        return nativeShortArrayMethod(val);
    }

    @Keep
    int[] intArrayMethod(int val[])
    {
        return nativeIntArrayMethod(val);
    }

    @Keep
    long[] longArrayMethod(long val[])
    {
        return nativeLongArrayMethod(val);
    }

    @Keep
    float[] floatArrayMethod(float val[])
    {
        return nativeFloatArrayMethod(val);
    }

    @Keep
    double[] doubleArrayMethod(double val[])
    {
        return nativeDoubleArrayMethod(val);
    }

    @Keep
    void voidMethodNoArgs()
    {
        nativeVoidMethodNoArgs();
    }

    @Keep
    void voidMethod(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i)
    {
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
