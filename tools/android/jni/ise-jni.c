#include <jni.h>
#include "../../../common/libise/include/isecore.h"
#define RGB888TO565(r,g,b) ((b>>3 & 0x1f) | ((g>>2 & 0x3f) << 5) | ((r>>3 & 0x1f) << 11))

JNIEXPORT jobject JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecureJpegBuffer(JNIEnv *env,jobject obj, jstring strpath, jstring strkey)
{
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
	const char *key = (*env)->GetStringUTFChars(env, strkey, 0);

	jpgx_decompress_container container = get_jpgx_container(path, (char *)key);

	/**
	 * convert to java class
	 */
	jclass jdc = (*env)->FindClass(env, "com/pigtools/isetool/container/JpgxDecompressContainer");
	jmethodID constructor = (*env)->GetMethodID(env, jdc, "<init>", "()V");
	jmethodID set_height = (*env)->GetMethodID(env, jdc, "setHeight", "(I)V");
	jmethodID set_width = (*env)->GetMethodID(env, jdc, "setWidth", "(I)V");
	jmethodID set_cs = (*env)->GetMethodID(env, jdc, "setColorSpace", "(I)V");
	jmethodID set_status = (*env)->GetMethodID(env, jdc, "setStatus", "(I)V");
	jmethodID set_bitmap = (*env)->GetMethodID(env, jdc, "setImage", "([B)V");

	jobject ret = (*env)->NewObject(env, jdc, constructor);

	int height = (int)container.jdcinfo.dcinfo.image_height;
	int width = (int)container.jdcinfo.dcinfo.image_width;
	int cs = (int)container.jdcinfo.dcinfo.jpeg_color_space;
	int status = (int)container.jdcinfo.status;
	unsigned char * bitmap = (unsigned char *)container.jdcinfo.image;

	(*env)->CallVoidMethod(env, ret, set_height, height);
	(*env)->CallVoidMethod(env, ret, set_width, width);
	(*env)->CallVoidMethod(env, ret, set_cs, cs);
	(*env)->CallVoidMethod(env, ret, set_status, status);

	/**
	 * 24bit bitmap is not supported in android
	 * convert rgb888 to rgb565
	 */

	int i = 0;
	for(i = 0 ; i < height*width ; i++)
	{
		unsigned char r = bitmap[i*3];
		unsigned char g = bitmap[i*3 + 1];
		unsigned char b = bitmap[i*3 + 2];

		short rgb565 = RGB888TO565(r,g,b);

		bitmap[i*2]= (rgb565 & 0xff);
		bitmap[i*2+1]= ((rgb565 >> 8) & 0xff);
	}

	/**
	 * copy to jbyteAray
	 */
	jbyteArray arr =(*env)->NewByteArray(env, height*width*2);
	(*env)->SetByteArrayRegion(env, arr, 0, height*width*2, (jbyte*)bitmap );
	(*env)->CallVoidMethod(env, ret, set_bitmap, arr);

	free(bitmap);

	return ret;
}

JNIEXPORT jstring JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecurePngBuffer(JNIEnv *env, jobject obj)
{
    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}

JNIEXPORT jstring JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeMakeJPGX(JNIEnv *env, jobject obj)
{

    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}

JNIEXPORT jstring JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeMakePNGX(JNIEnv *env, jobject obj)
{
    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}
