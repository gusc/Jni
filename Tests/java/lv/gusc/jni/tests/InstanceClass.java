package lv.gusc.jni.tests;

import androidx.annotation.Keep;

public class InstanceClass {

    public boolean booleanField = true;
    public byte byteField = 0x7F;
    public char charField = 'z';
    public short shortField = 0x7FFF;
    public int intField = 0x7FFFFFFF;
    public long longField = 0xFFFFFFFFFFFFFFFFL;
    public float floatField = 1.0f;
    public double doubleField = 2.0;
    public String stringField = "asdf";
    public boolean[] booleanArrayField = {true, true};
    public byte[] byteArrayField = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    public char[] charArrayField = {'a', 'b', 'c', 'd'};
    public short[] shortArrayField = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    public int[] intArrayField = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    public long[] longArrayField = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    public float[] floatArrayField = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
    public double[] doubleArrayField = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
    public TestClass testClassField = new TestClass();

    @Keep
    InstanceClass() {
    }

    @Keep
    InstanceClass(boolean a, byte b, char c, short d, int e, long f, float g, double h, String i,
                  boolean[] j, byte[] k, char[] l, short[] m, int[] n, long[] o, float[] p, double[] q) {
        booleanField = a;
        byteField = b;
        charField = c;
        shortField = d;
        intField = e;
        longField = f;
        floatField = g;
        doubleField = h;
        stringField = i;
        booleanArrayField = j;
        byteArrayField = k;
        charArrayField = l;
        shortArrayField = m;
        intArrayField = n;
        longArrayField = o;
        floatArrayField = p;
        doubleArrayField = q;
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
    boolean[] getBooleanArray() {
        return booleanArrayField;
    }

    @Keep
    byte[] getByteArray() {
        return byteArrayField;
    }

    @Keep
    char[] getCharArray() {
        return charArrayField;
    }

    @Keep
    short[] getShortArray() {
        return shortArrayField;
    }

    @Keep
    int[] getIntArray() {
        return intArrayField;
    }

    @Keep
    long[] getLongArray() {
        return longArrayField;
    }

    @Keep
    float[] getFloatArray() {
        return floatArrayField;
    }

    @Keep
    double[] getDoubleArray() {
        return doubleArrayField;
    }

    @Keep
    TestClass getTestClass() {
        return testClassField;
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

    @Keep
    void setBooleanArray(boolean[] val) {
        booleanArrayField = val;
    }

    @Keep
    void setByteArray(byte[] val) {
        byteArrayField = val;
    }

    @Keep
    void setCharArray(char[] val) {
        charArrayField = val;
    }

    @Keep
    void setShortArray(short[] val) {
        shortArrayField = val;
    }

    @Keep
    void setIntArray(int[] val) {
        intArrayField = val;
    }

    @Keep
    void setLongArray(long[] val) {
        longArrayField = val;
    }

    @Keep
    void setFloatArray(float[] val) {
        floatArrayField = val;
    }

    @Keep
    void setDoubleArray(double[] val) {
        doubleArrayField = val;
    }

    @Keep
    void setTestClass(TestClass val) {
        testClassField = val;
    }
}
