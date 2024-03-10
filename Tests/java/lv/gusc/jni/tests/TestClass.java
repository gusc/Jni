package lv.gusc.jni.tests;

import androidx.annotation.Keep;
import java.lang.String;

public class TestClass {

    private String s = "ASDF";

    @Keep
    public TestClass() {
    }

    public TestClass(String init) {
        s = init;
    }

    @Keep
    public void callMe() {
    }

    @Keep
    public String getString() {
        return s;
    }
}
