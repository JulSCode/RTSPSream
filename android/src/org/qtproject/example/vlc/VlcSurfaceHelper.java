package org.qtproject.example.vlc;

import android.app.Activity;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.os.Handler;
import android.os.Looper;
import android.net.Uri;
import android.util.Log;

import org.videolan.libvlc.LibVLC;
import org.videolan.libvlc.Media;
import org.videolan.libvlc.MediaPlayer;

import java.util.ArrayList;

public class VlcSurfaceHelper {

    private static final String TAG = "VlcSurfaceHelper";

    private static TextureView textureView;
    private static LibVLC libVLC;
    private static MediaPlayer mediaPlayer;

    public static void startPlaybackInSurface(Activity activity, String url, float x, float y, float width, float height) {
        new Handler(Looper.getMainLooper()).post(() -> {
            try {
                FrameLayout root = activity.findViewById(android.R.id.content);

                if (textureView == null) {
                    textureView = new TextureView(activity);
                    root.addView(textureView);
                }

                FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
                        (int) width, (int) height);
                params.leftMargin = (int) x;
                params.topMargin = (int) y;
                textureView.setLayoutParams(params);
                textureView.setVisibility(View.VISIBLE);

                if (libVLC == null) {
                    ArrayList<String> options = new ArrayList<>();
                    options.add("--no-drop-late-frames");
                    options.add("--rtsp-tcp");
                    options.add("--network-caching=150");
                    options.add("--live-caching=150");
                    libVLC = new LibVLC(activity, options);
                }

                if (mediaPlayer == null) {
                    mediaPlayer = new MediaPlayer(libVLC);
                } else {
                    mediaPlayer.stop();
                    mediaPlayer.getVLCVout().detachViews();
                }

                mediaPlayer.getVLCVout().setVideoView(textureView);
                
                mediaPlayer.getVLCVout().setWindowSize((int) width, (int) height);
                
                mediaPlayer.getVLCVout().attachViews();

                Media media = new Media(libVLC, Uri.parse(url));
                mediaPlayer.setMedia(media);
                media.release();
                mediaPlayer.play();

                Log.d(TAG, "Playback started at " + x + "," + y + " size " + width + "x" + height);

            } catch (Exception e) {
                Log.e(TAG, "Error starting playback", e);
            }
        });
    }

    public static void updateSurfacePosition(float x, float y, float width, float height) {
        new Handler(Looper.getMainLooper()).post(() -> {
            if (textureView != null) {
                FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) textureView.getLayoutParams();
                params.width = (int) width;
                params.height = (int) height;
                params.leftMargin = (int) x;
                params.topMargin = (int) y;
                textureView.setLayoutParams(params);
                
                if (mediaPlayer != null) {
                    mediaPlayer.getVLCVout().setWindowSize((int) width, (int) height);
                }
            }
        });
    }

    public static void pause() {
        new Handler(Looper.getMainLooper()).post(() -> {
            if (mediaPlayer != null && mediaPlayer.isPlaying()) {
                mediaPlayer.pause();
            }
        });
    }

    public static void stop() {
        new Handler(Looper.getMainLooper()).post(() -> {
            try {
                if (mediaPlayer != null) {
                    mediaPlayer.stop();
                    mediaPlayer.getVLCVout().detachViews();

                    new Handler(Looper.getMainLooper()).postDelayed(() -> {
                        if (mediaPlayer != null) {
                            mediaPlayer.release();
                            mediaPlayer = null;
                        }
                        if (libVLC != null) {
                            libVLC.release();
                            libVLC = null;
                        }
                        if (textureView != null) {
                            ViewGroup parent = (ViewGroup) textureView.getParent();
                            if (parent != null) {
                                parent.removeView(textureView);
                            }
                            textureView = null;
                        }
                    }, 150);
                }
            } catch (Exception e) {
                Log.e(TAG, "Error during stop", e);
            }
        });
    }
}
