LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_SHARED_LIBRARIES := \
#        libcutils \

#LOCAL_MODULE    := ise-lib

LOCAL_SRC_FILES := \
  ise-jni.cpp

LOCAL_CERTIFICATE := platform
include $(BUILD_SHARED_LIBRARY)
