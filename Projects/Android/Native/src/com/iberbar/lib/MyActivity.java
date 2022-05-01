
package com.iberbar.lib;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

public class MyActivity extends Activity
{
    private final static String TAG = MyActivity.class.getSimpleName();
	private MyGLSurfaceView m_glSurfaceView = null;
    private FrameLayout mFrameLayout = null;
    private boolean m_focus = false;
    private boolean m_pause = false;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Log.d(TAG, "onCreate()");

        // 初始化FileUtil
        FileUtil.s_context = this;

        // 加载native so文件
        onLoadNativeLibraries();

        MyHelper.init( this );

        this.init();
    }

    protected void onLoadNativeLibraries() {
        try {
            ApplicationInfo ai = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            String libName = bundle.getString("android.app.lib_name");
            Log.i( TAG, "loadLibrary: " + libName );
            System.loadLibrary(libName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void init()
    {
                // FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                       ViewGroup.LayoutParams.MATCH_PARENT);

        mFrameLayout = new FrameLayout(this);

        mFrameLayout.setLayoutParams(framelayout_params);

                // Cocos2dxGLSurfaceView
        this.m_glSurfaceView = this.onCreateView();

        // ...add to FrameLayout
        mFrameLayout.addView(this.m_glSurfaceView);

        this.m_glSurfaceView.setMyRenderer(new MyRenderer());

        // Set framelayout as the content view
        setContentView(mFrameLayout);
    }

    public MyGLSurfaceView onCreateView() {
        MyGLSurfaceView glSurfaceView = new MyGLSurfaceView(this);
        //this line is need on some device if we specify an alpha bits
        // FIXME: is it needed? And it will cause afterimage.
        // if(this.mGLContextAttrs[3] > 0) glSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

        // use custom EGLConfigureChooser
        //Cocos2dxEGLConfigChooser chooser = new Cocos2dxEGLConfigChooser(this.mGLContextAttrs);
        //glSurfaceView.setEGLConfigChooser(chooser);

        return glSurfaceView;
    }

    @Override
    protected void onResume() {
    	Log.d(TAG, "onResume()");
        m_pause = false;
        super.onResume();
        //if(gainAudioFocus)
        //    Cocos2dxAudioFocusManager.registerAudioFocusListener(this);
       	resumeIfHasFocus();
    }
    
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
    	Log.d(TAG, "onWindowFocusChanged() hasFocus=" + hasFocus);
        super.onWindowFocusChanged(hasFocus);
        
        this.m_focus = hasFocus;
        resumeIfHasFocus();
    }
    
    private void resumeIfHasFocus() {
        //It is possible for the app to receive the onWindowsFocusChanged(true) event
        //even though it is locked or asleep
        //boolean readyToPlay = !isDeviceLocked() && !isDeviceAsleep();

        if(m_focus) {
            //Cocos2dxHelper.onResume();
        	m_glSurfaceView.onResume();
        }
    }

    @Override
    protected void onPause() {
    	Log.d(TAG, "onPause()");
        m_pause = true;
        super.onPause();
        //if(gainAudioFocus)
        //    Cocos2dxAudioFocusManager.unregisterAudioFocusListener(this);
        //Cocos2dxHelper.onPause();
        m_glSurfaceView.onPause();
    }
    
    @Override
    protected void onDestroy() {
        //if(gainAudioFocus)
        //    Cocos2dxAudioFocusManager.unregisterAudioFocusListener(this);
        Log.d( TAG, "onDestroy" );
        //m_glSurfaceView.onDestroy();
        super.onDestroy();
    }

    /*
    @Override
    public void runOnGLThread(final Runnable pRunnable) {
        this.m_glSurfaceView.queueEvent(pRunnable);
    }
    */
}
