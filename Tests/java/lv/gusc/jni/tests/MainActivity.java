package lv.gusc.jni.tests;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("JniTests");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // To pass CLI arguments, pass them like this:
        //Main.main(new String[]{"--gtest_filter=StaticTest*", "--gtest_repeat=10"});
        int result = Main.main(new String[]{});
        Log.d("MainActivity", "Result: " + result);
        Main.exit(result);
    }
}