
package com.iberbar.lib;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;

public class MyHelper
{
	private static AssetManager sAssetManager;
	private static String sPackageName;
    private static Activity sActivity = null;

	public static void init( final Activity activity )
	{
		sAssetManager = activity.getAssets();
        nativeSetContext((Context)activity, sAssetManager);
	}

	public static void tryQuit()
	{
		nativeOnDestroy();
		System.exit(0);
	}

	private static native void nativeSetContext( Context context, AssetManager assetManager );
	private static native void nativeOnDestroy();
}
