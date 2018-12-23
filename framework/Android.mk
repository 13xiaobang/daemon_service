LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdaemonsubmgr_jni
LOCAL_SRC_FILES := jni/daemon_sub_DaemonSubManager.cpp

LOCAL_SHARED_LIBRARIES := libdaemonsubmgr \
                        libcutils \
                        libutils \
                        liblog \
                        libbinder \
                        libnativehelper

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_MULTILIB := both
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
include $(CLEAR_VARS)
include $(BUILD_MULTI_PREBUILT)
include $(CLEAR_VAR)
#LOCAL_JACK_ENABLED := disabled
LOCAL_MODULE := daemon.sub
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under, java)
include $(BUILD_JAVA_LIBRARY)
