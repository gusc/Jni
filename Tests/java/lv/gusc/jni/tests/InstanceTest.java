package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class InstanceTest {

    public boolean booleanField = true;
    public byte byteField = 1;
    public char charField = 'a';
    public short shortField = 512;
    public int intField = 0xFFFFFF;
    public long longField = 0xFFFFFFFFFFFFL;
    public float floatField = 1.5f;
    public double doubleField = 2.3;
    public String stringField = "asdf";

    @Keep
    InstanceTest() {
    }

    @Keep
    InstanceTest(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i) {
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
    void voidMethodNoArgs() {
    }

    @Keep
    void voidMethod(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i) {
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
    boolean getBoolean() {
        return booleanField;
    }

    @Keep
    byte getByte() {
        return byteField;
    }

    @Keep
    char getChar() {
        return charField;
    }

    @Keep
    short getShort() {
        return shortField;
    }

    @Keep
    int getInt() {
        return intField;
    }

    @Keep
    long getLong() {
        return longField;
    }

    @Keep
    float getFloat() {
        return floatField;
    }

    @Keep
    double getDouble() {
        return doubleField;
    }

    @Keep
    String getString() {
        return stringField;
    }

    @Keep
    void setBoolean(boolean val) {
        booleanField = val;
    }

    @Keep
    void setByte(byte val) {
        byteField = val;
    }

    @Keep
    void setChar(char val) {
        charField = val;
    }

    @Keep
    void setShort(short val) {
        shortField = val;
    }

    @Keep
    void setInt(int val) {
        intField = val;
    }

    @Keep
    void setLong(long val) {
        longField = val;
    }

    @Keep
    void setFloat(float val) {
        floatField = val;
    }

    @Keep
    void setDouble(double val) {
        doubleField = val;
    }

    @Keep
    void setString(String val) {
        stringField = val;
    }
}
