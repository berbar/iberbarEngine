
package com.Test.Renderer;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import com.iberbar.lib.MyActivity;
import android.util.Log;
import android.os.Environment;
import android.content.Context;

public class TestRendererActivity extends MyActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.d( "file dir", getFilesPath( this ) );
    }

    public String getDiskCacheDir(Context context) {
        String cachePath = null;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState()) || !Environment.isExternalStorageRemovable()) {
            cachePath = context.getExternalCacheDir().getPath();
        } else {
            cachePath = context.getCacheDir().getPath();
        }
        return cachePath;
    }

    public String getFilesPath( Context context ){
        String filePath ;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState()) || !Environment.isExternalStorageRemovable()) {
            //外部存储可用
            filePath = context.getExternalFilesDir(null).getPath();
        }else {
            //外部存储不可用
            filePath = context.getFilesDir().getPath() ;
        }
        return filePath ;
    }

    public static native void nativeTest( final int w );
    private static native void nativeInit(final int width, final int height);
}
