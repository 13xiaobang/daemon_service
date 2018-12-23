#define LOG_TAG "DAEMON_SUB_SERVICE"

#include "jni.h"
#include "JNIHelp.h"

#include <utils/misc.h>
#include <utils/Log.h>

#include <stdio.h>
#include <utils/threads.h>
#include <unistd.h>
#include <sys/types.h>
#include <binder/IServiceManager.h>
#include "daemon_sub_DaemonSubManager.h"
using namespace android;

#include <libdaemonsubmgr/DaemonSubManagerService.h>
const sp<IDaemonSubManagerService> getDaemonSubManagerService()
{
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("daemonsub.manager"));

    return interface_cast<IDaemonSubManagerService>(binder);
}

static jint native_setCmd(JNIEnv *, jobject,jint mode)
{
    sp<IDaemonSubManagerService> sp = getDaemonSubManagerService();
    if(sp == NULL) {
        ALOGE("getDaemonSubManagerService failed\n");
        return -1;
    }
    else
        return sp->setCmd(mode);
}

static jint native_getEvent(JNIEnv *, jobject)
{
    jint ret;
    sp<IDaemonSubManagerService> sp = getDaemonSubManagerService();
    if(sp == NULL) {
        ALOGE("getDaemonSubManagerService failed\n");
        usleep(10000000);//delay 10s for next get event.
        return -1;
    }
    jint status =  sp->getEvent((unsigned int*)&ret);
    if(status == NO_ERROR)
        return ret;
    return status;
}

static jint native_enquiry(JNIEnv *, jobject)
{
    jint status =  getDaemonSubManagerService()->enquiry();
    return status;
}
JavaVM *g_VM;
jobject g_obj;

int mNeedDetach = JNI_FALSE;
jmethodID javaCallbackId;
JNIEnv *g_env;

#define JNIREG_CLASS "daemon/sub/DaemonSubManager"

void daemon_callback(char* str)
{
    ALOGI("call daemon_callback: str=%s", str);

    mNeedDetach = JNI_FALSE;
    int getEnvStat = g_VM->GetEnv((void **) &g_env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        if ((*g_VM).AttachCurrentThread(&g_env, NULL) != 0) {
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass javaClass = (*g_env).GetObjectClass(g_obj);
    if (javaClass == 0) {
        ALOGE("Unable to find class");
        (*g_VM).DetachCurrentThread();
        return;
    }

    javaCallbackId = (*g_env).GetMethodID(javaClass,
            "onProgressCallBack", "([BI)I");
    if (javaCallbackId == NULL) {
        ALOGE("Unable to find method:onProgressCallBack");
        return;
    }
    int sizeInBytes = strlen(str);
    jbyte *by = (jbyte*)str;
    jbyteArray jarray = g_env->NewByteArray(sizeInBytes+1);
    g_env->SetByteArrayRegion(jarray, 0, sizeInBytes, by);
    (*g_env).CallIntMethod(g_obj, javaCallbackId,jarray,sizeInBytes);
    g_env->DeleteLocalRef(jarray);
    if(mNeedDetach) {
        (*g_VM).DetachCurrentThread();
    }
    g_env = NULL;
}

void * DaemonThread(void* param)
{
    ALOGI("start DaemonThread");
    while(1) {
        usleep(2000000);
        daemon_callback("this is callback");
    }
    return (void*)NULL;
}
static jint native_open(JNIEnv * env, jobject thiz)
{
    jint status =  getDaemonSubManagerService()->open();

    env->GetJavaVM(&g_VM);
    g_obj = env->NewGlobalRef(thiz);

    pthread_t id = 0;
    int ret;
    ret=pthread_create(&id ,NULL,DaemonThread,NULL);
    if(ret != 0) {
        ALOGE ("create DaemonThread error!\n");
        return 1;
    }
    return 0;
}

static jint native_close(JNIEnv *, jobject)
{
    jint status =  getDaemonSubManagerService()->close();
    return status;
}

static jint native_block(JNIEnv *, jobject)
{
    ALOGI("block now");
    while(1)
        usleep(100000000);
    return 0;
}

static jint native_read(JNIEnv *env, jobject, jbyteArray javaAudioData, jint sizeInBytes)
{
    //int size;
    char *temp;
    jbyte* recordBuff = (jbyte*)env->GetByteArrayElements(javaAudioData, NULL);

    if (recordBuff == NULL || sizeInBytes < 0)
    {
        ALOGE("Error retrieving destination for recorded audio data, can't record");
        return 0;
    }
    temp = (char*)recordBuff;
    jint status =  getDaemonSubManagerService()->read(&temp, &sizeInBytes);
    if(status == NO_ERROR)
    {
        env->ReleaseByteArrayElements(javaAudioData, recordBuff, 0);
        return sizeInBytes;
    }
    env->ReleaseByteArrayElements(javaAudioData, recordBuff, 0);
    return 0;
}

static jint native_write(JNIEnv *env, jobject, jbyteArray javaAudioData, jint sizeInBytes)
{
    //int size;
    char *temp;
    jbyte* recordBuff = (jbyte*)env->GetByteArrayElements(javaAudioData, NULL);

    if (recordBuff == NULL || sizeInBytes < 0)
    {
        ALOGE("Error retrieving destination for recorded audio data, can't record");
        return 0;
    }
    temp = (char*)recordBuff;
    jint status =  getDaemonSubManagerService()->write(&temp, &sizeInBytes);
    if(status == NO_ERROR)
    {
        env->ReleaseByteArrayElements(javaAudioData, recordBuff, 0);
        return sizeInBytes;
    }
    env->ReleaseByteArrayElements(javaAudioData, recordBuff, 0);
    return 0;
}

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = (env)->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if ((env)->RegisterNatives( clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}


/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, JNIREG_CLASS, daemon_sub_manager_method_table,
                sizeof(daemon_sub_manager_method_table) / sizeof(daemon_sub_manager_method_table[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*)
{
    JNIEnv* env = NULL;
    jint result = -1;
    ALOGD("JNI_OnLoad begin");
    if ((vm)->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);
    if (!registerNatives(env)) {
        return -1;
    }
    /* success -- return valid version number */
    result = JNI_VERSION_1_6;
    ALOGD("JNI_OnLoad successfully");
    return result;
}

