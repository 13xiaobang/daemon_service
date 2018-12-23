LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	IDaemonSubManagerService.cpp \
	DaemonSubManagerService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	liblog \
	libbinder 

LOCAL_MULTILIB := both
LOCAL_MODULE:= libdaemonsubmgr

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../include

#if android version > 6.0
#LOCAL_CFLAGS += -DCFLAGS_ANDROID_ABOVE_6

include $(BUILD_SHARED_LIBRARY)
