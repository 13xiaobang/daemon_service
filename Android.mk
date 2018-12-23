SUBDIR_MAKEFILES := $(call all-named-subdir-makefiles, framework libdaemonsubmgr test)
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp \
    main_process.cpp

LOCAL_SHARED_LIBRARIES := \
        libcutils \
        libutils \
        liblog \
        libbinder \
        libdaemonsubmgr_jni \
        libdaemonsubmgr

#LOCAL_CFLAGS := -DANDROID -DWITH_TLS
LOCAL_MODULE:= daemon_service
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/include

include $(BUILD_EXECUTABLE)
include $(SUBDIR_MAKEFILES)
