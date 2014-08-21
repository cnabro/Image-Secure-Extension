LOCAL_PATH := ./

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	../../common/libpng/include/ \
	../../common/libjpeg/include/ \
	../../common/zlib/include/ \
	../../common/polarssl/include/ \
	../../common/mxml/include/ \
	../../common/libise/include/ \
	
	

FILE_LIST := \
	$(wildcard $(LOCAL_PATH)/../../common/libjpeg/source/*.c) \
	$(wildcard $(LOCAL_PATH)/../../common/zlib/source/*.c) \
	$(wildcard $(LOCAL_PATH)/../../common/libpng/source/*.c) \
	$(wildcard $(LOCAL_PATH)/../../common/polarssl/source/*.c) \
	$(wildcard $(LOCAL_PATH)/../../common/mxml/source/*.c) \
	$(wildcard $(LOCAL_PATH)/../../common/libise/source/*.c) \
	
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true	

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE    := libise

LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)