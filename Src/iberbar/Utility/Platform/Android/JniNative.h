#pragma once

#include <jni.h>

//private static native void nativeTouchesBegin( final int id, final float x, final float y );
//private static native void nativeTouchesEnd( final int id, final float x, final float y );
//private static native void nativeTouchesMove( final int[] ids, final float[] xs, final float[] ys );
//private static native void nativeTouchesCancel( final int[] ids, final float[] xs, final float[] ys );
//private static native boolean nativeKeyEvent( final int keyCode, boolean isPressed );
//private static native void nativeRender();
//private static native void nativeInit( final int width, final int height );
//private static native void nativeOnSurfaceChanged( final int width, final int height );
//private static native void nativeOnPause();
//private static native void nativeOnResume();

extern "C"
{
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeRender( JNIEnv* env, jobject obj );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeInit( JNIEnv* env, jobject obj, jint width, jint height );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeOnSurfaceChanged( JNIEnv* env, jobject obj, jint width, jint height );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeOnPause( JNIEnv* env, jobject obj );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeOnResume( JNIEnv* env, jobject obj );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeOnDestroy( JNIEnv* env, jobject obj );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeTouchesBegin( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeTouchesEnd( JNIEnv* env, jobject obj, jint id, jfloat x, jfloat y );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeTouchesMove( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys );
	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyRenderer_nativeTouchesCancel( JNIEnv* env, jobject obj, jintArray ids, jfloatArray xs, jfloatArray ys );
	JNIEXPORT jboolean JNICALL Java_com_iberbar_lib_MyRenderer_nativeKeyEvent( JNIEnv* env, jobject obj, jint keyCode, jboolean isPressed );

	JNIEXPORT void JNICALL Java_com_iberbar_lib_MyHelper_nativeSetContext( JNIEnv* env, jobject obj, jobject context, jobject assetManager );
}


//#define JNISigType_Array( s ) "["s
//#define JNISigType_Int "I"
//#define JNISigType_FLOAT "F";
//#define JNISigType_Object "Ljava/lang/Object"
//#define JNISigType_String "Ljava/lang/String"
//
//#define JNISigArg( s ) s";"
//#define JNISigMethod( returnSig, ... ) "("__VA_ARGS__")"returnSig

