#include <jni.h>
#include <android/log.h>
#include "../../../common/libise/include/isecore.h"
#define RGB888TO565(r,g,b) ((b>>3 & 0x1f) | ((g>>2 & 0x3f) << 5) | ((r>>3 & 0x1f) << 11))
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "isetool", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "isetool", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "isetool", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "isetool", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "isetool", __VA_ARGS__)

JNIEXPORT jobject JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecureJpegBuffer(JNIEnv *env,jobject obj, jstring strpath, jstring strkey)
{
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
	char *key = (*env)->GetStringUTFChars(env, strkey, 0);

	jpgx_decompress_container container = get_jpgx_container(path, (char *)key);

	/**
	 * convert to java class
	 */
	jclass jdc_class = (*env)->FindClass(env, "com/pigtools/isetool/service/container/JpgxDecompressContainer");
	jmethodID constructor = (*env)->GetMethodID(env, jdc_class, "<init>", "()V");
	jmethodID set_height = (*env)->GetMethodID(env, jdc_class, "setHeight", "(I)V");
	jmethodID set_width = (*env)->GetMethodID(env, jdc_class, "setWidth", "(I)V");
	jmethodID set_cs = (*env)->GetMethodID(env, jdc_class, "setColorSpace", "(I)V");
	jmethodID set_status = (*env)->GetMethodID(env, jdc_class, "setStatus", "(I)V");
	jmethodID set_bitmap = (*env)->GetMethodID(env, jdc_class, "setImage", "([B)V");

	jobject ret = (*env)->NewObject(env, jdc_class, constructor);

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

JNIEXPORT jobject JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecurePngBuffer(JNIEnv *env,jobject obj, jstring strpath, jstring strkey)
{
//	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
//	char *key = (*env)->GetStringUTFChars(env, strkey, 0);
//
//	pngx_decompress_container container = get_pngx_container(path, (char *)key);
//
//	/**
//	 * convert to java class
//	 */
//	jclass pdc_class = (*env)->FindClass(env, "com/pigtools/isetool/service/container/PngxDecompressContainer");
//	jmethodID constructor = (*env)->GetMethodID(env, pdc_class, "<init>", "()V");
//	jmethodID set_height = (*env)->GetMethodID(env, pdc_class, "setHeight", "(I)V");
//	jmethodID set_width = (*env)->GetMethodID(env, pdc_class, "setWidth", "(I)V");
//	jmethodID set_cs = (*env)->GetMethodID(env, pdc_class, "setColorSpace", "(I)V");
//	jmethodID set_status = (*env)->GetMethodID(env, pdc_class, "setStatus", "(I)V");
//	jmethodID set_bitmap = (*env)->GetMethodID(env, pdc_class, "setImage", "([B)V");
//
//	jobject ret = (*env)->NewObject(env, pdc_class, constructor);
//
//	int height = (int)container.jdcinfo.dcinfo.image_height;
//	int width = (int)container.jdcinfo.dcinfo.image_width;
//	int cs = (int)container.jdcinfo.dcinfo.jpeg_color_space;
//	int status = (int)container.jdcinfo.status;
//	unsigned char * bitmap = (unsigned char *)container.jdcinfo.image;
//
//	(*env)->CallVoidMethod(env, ret, set_height, height);
//	(*env)->CallVoidMethod(env, ret, set_width, width);
//	(*env)->CallVoidMethod(env, ret, set_cs, cs);
//	(*env)->CallVoidMethod(env, ret, set_status, status);
//
//	/**
//	 * 24bit bitmap is not supported in android
//	 * convert rgb888 to rgb565
//	 */
//
//	int i = 0;
//	for(i = 0 ; i < height*width ; i++)
//	{
//		unsigned char r = bitmap[i*3];
//		unsigned char g = bitmap[i*3 + 1];
//		unsigned char b = bitmap[i*3 + 2];
//
//		short rgb565 = RGB888TO565(r,g,b);
//
//		bitmap[i*2]= (rgb565 & 0xff);
//		bitmap[i*2+1]= ((rgb565 >> 8) & 0xff);
//	}
//
//	/**
//	 * copy to jbyteAray
//	 */
//	jbyteArray arr =(*env)->NewByteArray(env, height*width*2);
//	(*env)->SetByteArrayRegion(env, arr, 0, height*width*2, (jbyte*)bitmap );
//	(*env)->CallVoidMethod(env, ret, set_bitmap, arr);
//
//	free(bitmap);
//
//	return ret;
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
		char *key = (*env)->GetStringUTFChars(env, strkey, 0);

		jpgx_decompress_container container = get_jpgx_container(path, (char *)key);

		/**
		 * convert to java class
		 */
		jclass jdc_class = (*env)->FindClass(env, "com/pigtools/isetool/service/container/JpgxDecompressContainer");
		jmethodID constructor = (*env)->GetMethodID(env, jdc_class, "<init>", "()V");
		jmethodID set_height = (*env)->GetMethodID(env, jdc_class, "setHeight", "(I)V");
		jmethodID set_width = (*env)->GetMethodID(env, jdc_class, "setWidth", "(I)V");
		jmethodID set_cs = (*env)->GetMethodID(env, jdc_class, "setColorSpace", "(I)V");
		jmethodID set_status = (*env)->GetMethodID(env, jdc_class, "setStatus", "(I)V");
		jmethodID set_bitmap = (*env)->GetMethodID(env, jdc_class, "setImage", "([B)V");

		jobject ret = (*env)->NewObject(env, jdc_class, constructor);

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

JNIEXPORT jboolean JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeMakeJPGX(JNIEnv *env, jobject obj, jstring strpath, jobject list, jstring strkey)
{
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
	char *key = (*env)->GetStringUTFChars(env, strkey, 0);

	jclass al_class = (*env)->FindClass(env, "java/util/ArrayList");
	jmethodID get_obj = (*env)->GetMethodID(env, al_class, "get", "(I)Ljava/lang/Object;");
	jmethodID get_size =  (*env)->GetMethodID(env, al_class, "size", "()I");

	int sc_size = (int) (*env)->CallIntMethod(env, list, get_size);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*sc_size);

	jclass sc_class = (*env)->FindClass(env, "com/pigtools/isetool/service/container/SecureContainer");
	jmethodID get_height = (*env)->GetMethodID(env, sc_class, "getHeight", "()I");
	jmethodID get_width = (*env)->GetMethodID(env, sc_class, "getWidth", "()I");
	jmethodID get_x = (*env)->GetMethodID(env, sc_class, "getPosX", "()I");
	jmethodID get_y = (*env)->GetMethodID(env, sc_class, "getPosY", "()I");

	int i = 0;
	LOGE("path : %s ", path);
	LOGE("key  : %s ", key);
	LOGE("size : %d ", sc_size);
	for (i = 0; i < sc_size; i++)
	{
		jobject item = (*env)->CallObjectMethod(env, list, get_obj , i);

		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = (int) (*env)->CallIntMethod(env, item, get_height);
		sc_array[i]->width  = (int) (*env)->CallIntMethod(env, item, get_width);
		sc_array[i]->pos_x  = (int) (*env)->CallIntMethod(env, item, get_x);
		sc_array[i]->pos_y  = (int) (*env)->CallIntMethod(env, item, get_y);

		LOGE("x : %d / y : %d / width : %d / height : %d ", sc_array[i]->height, sc_array[i]->width, sc_array[i]->pos_x, sc_array[i]->pos_y);
	}

	jpgx_compress_container jcc = make_jpgx(path, sc_array, sc_size, key);

    return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_pigtools_isetool_service_IseProcessingService_nativeMakePNGX(JNIEnv *env, jobject obj, jstring strpath, jobject list, jstring strkey)
{
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
	char *key = (*env)->GetStringUTFChars(env, strkey, 0);

	jclass al_class = (*env)->FindClass(env, "java/util/ArrayList");
	jmethodID get_obj = (*env)->GetMethodID(env, al_class, "get", "(I)Ljava/lang/Object;");
	jmethodID get_size =  (*env)->GetMethodID(env, al_class, "size", "()I");

	int sc_size = (int) (*env)->CallIntMethod(env, list, get_size);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*sc_size);

	jclass sc_class = (*env)->FindClass(env, "com/pigtools/isetool/service/container/SecureContainer");
	jmethodID get_height = (*env)->GetMethodID(env, sc_class, "getHeight", "()I");
	jmethodID get_width = (*env)->GetMethodID(env, sc_class, "getWidth", "()I");
	jmethodID get_x = (*env)->GetMethodID(env, sc_class, "getPosX", "()I");
	jmethodID get_y = (*env)->GetMethodID(env, sc_class, "getPosY", "()I");

	int i = 0;
	for (i = 0; i < sc_size; i++)
	{
		jobject item = (*env)->CallObjectMethod(env, list, get_obj , i);

		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = (int) (*env)->CallIntMethod(env, item, get_height);;
		sc_array[i]->width  = (int) (*env)->CallIntMethod(env, item, get_width);;
		sc_array[i]->pos_x  = (int) (*env)->CallIntMethod(env, item, get_x);;
		sc_array[i]->pos_y  = (int) (*env)->CallIntMethod(env, item, get_y);;
	}

	pngx_compress_container jcc = make_pngx(path, sc_array, sc_size, key);

	return JNI_TRUE;
}
