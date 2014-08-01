LOCAL_PATH := $(call my-dir)

###PREBUILT SHARD LIBRARY
include $(CLEAR_VARS)
LOCAL_MODULE := libise
LOCAL_SRC_FILES := ../../../library/lib-android/libs/armeabi/libise.so
LOCAL_EXPORT_C_INCLUDES := \
	../../common/libpng/include/ \
	../../common/libjpeg/include/ \
	../../common/mxml/include/ \
	../../common/polarssl/include/ \
	../../common/zlib/include/ \
	../../common/libise/include/ \

include $(PREBUILT_SHARED_LIBRARY)

###REFERENCING THE PREBUILD LIBRARY IN OTHER MODULES
include $(CLEAR_VARS)
LOCAL_MODULE := libise-jni
LOCAL_SRC_FILES := ise-jni.c \
	libise.so
LOCAL_SHARED_LIBRARIES := libise
LOCAL_CERTIFICATE := platform
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)
