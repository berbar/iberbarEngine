
package com.iberbar.lib;

import android.content.Context;
import android.util.Log;

public class FileUtil
{
	public static Context s_context = null;

	public static String GetFilesDir()
	{
		String str = s_context.getFilesDir().getPath();
		Log.d("FileUtil", str );
		return str;
	}

	public static String GetExternalFilesDir()
	{
		String str = s_context.getExternalFilesDir(null).getPath();
		Log.d("FileUtil", str );
		return str;
	}
}
