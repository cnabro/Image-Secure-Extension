#include <jni.h>
#include "../../../common/libise/include/isecore.h"

JNIEXPORT jstring JNICALL Java_com_cnabro_isetool_MainActivity_getSecureJpegBuffer(JNIEnv *env,  jobject obj, jstring strpath, jstring strkey)
{
	char *path = (*env)->GetStringUTFChars(env, strpath, 0);
	const char *key = (*env)->GetStringUTFChars(env, strkey, 0);

	jpgx_decompress_container container = get_jpgx_container(path, (const char *)key);
	//jpgx_decompress_container container = get_jpgx_container("/storage/emulated/0/test.jpgx", "1234");

    return strpath;
}

JNIEXPORT jstring JNICALL Java_com_cnabro_isetool_MainActivity_getSecurePngBuffer(JNIEnv *env, jobject obj)
{
    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}

JNIEXPORT jstring JNICALL Java_com_cnabro_isetool_MainActivity_makeJPGX(JNIEnv *env, jobject obj)
{

    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}

JNIEXPORT jstring JNICALL Java_com_cnabro_isetool_MainActivity_makePNGX(JNIEnv *env, jobject obj)
{
    return (*env)->NewStringUTF(env, "Hello JNI!!!!!");
}
