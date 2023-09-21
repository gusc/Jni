package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class StaticTest {

    public static boolean booleanField = true;
    public static byte byteField = 1;
    public static char charField = 'a';
    public static short shortField = 512;
    public static int intField = 0xFFFFFF;
    public static long longField = 0xFFFFFFFFFFFFL;
    public static float floatField = 1.5f;
    public static double doubleField = 2.3;
    public static String stringField = "asdf";
    public static boolean[] booleanArrayField = {false, true};
    public static byte[] byteArrayField = {0, 1, 2, 3, 4, 5, 6, 7};
    public static char[] charArrayField = {'a', 'b', 'c', 'd'};
    public static short[] shortArrayField = {0, 1, 2, 3, 4, 5, 6, 7};
    public static int[] intArrayField = {0, 1, 2, 3, 4, 5, 6, 7};
    public static long[] longArrayField = {0, 1, 2, 3, 4, 5, 6, 7};
    public static float[] floatArrayField = {0, 1, 2, 3, 4, 5, 6, 7};
    public static double[] doubleArrayField = {0, 1, 2, 3, 4, 5, 6, 7};

    @Keep
    static void voidMethodNoArgs() {
    }

    @Keep
    static void voidMethod(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i) {
        booleanField = a;
        byteField = b;
        charField = c;
        shortField = d;
        intField = e;
        longField = f;
        floatField = g;
        doubleField = h;
        stringField = i;
    }

    @Keep
    static boolean getBoolean() {
        return booleanField;
    }

    @Keep
    static byte getByte() {
        return byteField;
    }

    @Keep
    static char getChar() {
        return charField;
    }

    @Keep
    static short getShort() {
        return shortField;
    }

    @Keep
    static int getInt() {
        return intField;
    }

    @Keep
    static long getLong() {
        return longField;
    }

    @Keep
    static float getFloat() {
        return floatField;
    }

    @Keep
    static double getDouble() {
        return doubleField;
    }

    @Keep
    static String getString() {
        return stringField;
    }


    @Keep
    static boolean[] getBooleanArray() {
        return booleanArrayField;
    }

    @Keep
    static byte[] getByteArray() {
        return byteArrayField;
    }

    @Keep
    static char[] getCharArray() {
        return charArrayField;
    }

    @Keep
    static short[] getShortArray() {
        return shortArrayField;
    }

    @Keep
    static int[] getIntArray() {
        return intArrayField;
    }

    @Keep
    static long[] getLongArray() {
        return longArrayField;
    }

    @Keep
    static float[] getFloatArray() {
        return floatArrayField;
    }

    @Keep
    static double[] getDoubleArray() {
        return doubleArrayField;
    }

    @Keep
    static void setBoolean(boolean val) {
        booleanField = val;
    }

    @Keep
    static void setByte(byte val) {
        byteField = val;
    }

    @Keep
    static void setChar(char val) {
        charField = val;
    }

    @Keep
    static void setShort(short val) {
        shortField = val;
    }

    @Keep
    static void setInt(int val) {
        intField = val;
    }

    @Keep
    static void setLong(long val) {
        longField = val;
    }

    @Keep
    static void setFloat(float val) {
        floatField = val;
    }

    @Keep
    static void setDouble(double val) {
        doubleField = val;
    }

    @Keep
    static void setString(String val) {
        stringField = val;
    }

    @Keep
    static void setBooleanArray(boolean[] val) {
        booleanArrayField = val;
    }

    @Keep
    static void setByteArray(byte[] val) {
        byteArrayField = val;
    }

    @Keep
    static void setCharArray(char[] val) {
        charArrayField = val;
    }

    @Keep
    static void setShortArray(short[] val) {
        shortArrayField = val;
    }
    @Keep
    static void setIntArray(int[] val) {
        intArrayField = val;
    }
    @Keep
    static void setLongArray(long[] val) {
        longArrayField = val;
    }
    @Keep
    static void setFloatArray(float[] val) {
        floatArrayField = val;
    }
    @Keep
    static void setDoubleArray(double[] val) {
        doubleArrayField = val;
    }
}
