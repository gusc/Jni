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
}
