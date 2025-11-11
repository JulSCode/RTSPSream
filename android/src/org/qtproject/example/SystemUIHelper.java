// D:\QtProgects\RTSPStream\android\src\org\qtproject\example\SystemUIHelper.java
package org.qtproject.example;

import android.app.Activity;
import android.view.View;
import android.os.Vibrator;
import android.content.Context;

public class SystemUIHelper {
    public static void hideSystemUI(Activity activity) {
        if (activity == null) return;
        
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                View decorView = activity.getWindow().getDecorView();
                int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                              | View.SYSTEM_UI_FLAG_FULLSCREEN
                              | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
                decorView.setSystemUiVisibility(uiOptions);
            }
        });
    }
    
    public static void showSystemUI(Activity activity) {
        if (activity == null) return;
        
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                View decorView = activity.getWindow().getDecorView();
                decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_VISIBLE);
            }
        });
    }
    
    public static void vibrate(Activity activity, int milliseconds) {
        if (activity == null) return;
        
        Vibrator vibrator = (Vibrator) activity.getSystemService(Context.VIBRATOR_SERVICE);
        if (vibrator != null) {
            vibrator.vibrate(milliseconds);
        }
    }
}
