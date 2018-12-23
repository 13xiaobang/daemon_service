#include <jni.h>

#ifndef DAEMON_SUB_DAEMON_SUBMANAGER_H
#define DAEMON_SUB_DAEMON_SUBMANAGER_H
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

static jint native_setCmd(JNIEnv *env, jobject thiz,jint mode);
static jint native_getEvent(JNIEnv *env, jobject thiz);


static jint native_enquiry(JNIEnv *env, jobject thiz);
static jint native_open(JNIEnv *env, jobject thiz);
static jint native_close(JNIEnv *env, jobject thiz);
//static jbyteArray native_read(JNIEnv *env, jobject);
static jint native_read(JNIEnv *env, jobject, jbyteArray javaAudioData, jint sizeInBytes);
static jint native_write(JNIEnv *env, jobject, jbyteArray javaAudioData, jint sizeInBytes);
static jint native_block(JNIEnv *env, jobject thiz);

static JNINativeMethod daemon_sub_manager_method_table[] = {

    {"setCmd", "(I)I", (void*)native_setCmd},
    {"getEvent", "()I", (void*)native_getEvent},

//official interfaces:
    {"enquiry", "()I", (void*)native_enquiry},
    {"open", "()I", (void*)native_open},
    {"close", "()I", (void*)native_close},
    {"read", "([BI)I", (void*)native_read},
    {"write", "([BI)I", (void*)native_write},
    {"block", "()I", (void*)native_block},
};

#ifdef __cplusplus
}
#endif
#endif
