#define LOG_TAG "DAEMON_SUB_SERVICE"

#include <stdint.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/String16.h>
#include <unistd.h>
#include <binder/IServiceManager.h>
#include <libdaemonsubmgr/DaemonSubManagerService.h>

namespace android {

    void DaemonSubManagerService::instantiate()
    {
        defaultServiceManager()->addService(
                String16("daemonsub.manager"), new DaemonSubManagerService());
    }

    DaemonSubManagerService::DaemonSubManagerService()
    {
        ALOGV("DaemonSubManagerService created");
    }

    DaemonSubManagerService::~DaemonSubManagerService()
    {
        ALOGV("DaemonSubManagerService destroyed");
    }

    static int global_cmd = 0;
    status_t DaemonSubManagerService::setCmd(uint32_t mode)
    {
        ALOGI("setCmd: %d", mode);
        global_cmd = mode;
        if(mCallback != NULL) {
            mCallback->notifyCallback(mode+1);
        }
            else
            ALOGI("noting to callback");
        return 0;
    }

    status_t DaemonSubManagerService::getEvent(uint32_t *event)
    {
        *event  = global_cmd;
        return 0;
    }

    status_t DaemonSubManagerService::enquiry()
    {
        ALOGI("DaemonSubManagerService::enquiry called.");
        return 0;
    }

    status_t DaemonSubManagerService::open()
    {
        ALOGI("DaemonSubManagerService::open called.");
        return 0;
    }

    status_t DaemonSubManagerService::close()
    {
        ALOGI("DaemonSubManagerService::close called.");
        return 0;
    }

    char read_buf[] =  "this is daemon sub read buf!";
    status_t DaemonSubManagerService::read(char **str, int32_t *size)
    {
        ALOGI("DaemonSubManagerService::read called.1");
        memcpy(*str, read_buf, strlen(read_buf));
        ALOGI("DaemonSubManagerService::read called.1.1");
        (*str)[strlen(read_buf)] = '\0';
        *size = strlen(read_buf) + 1;
        ALOGI("DaemonSubManagerService::read called ++done2");
        return 0;
    }

    status_t DaemonSubManagerService::write(char **str, int32_t *size)
    {
        ALOGI("DaemonSubManagerService::write called");
        ALOGI("DaemonSubManagerService::write %s, len = %d", *str, *size );
        return 0;
    }

    status_t DaemonSubManagerService::setCallback(const sp<ICallback>& callback)
    {
        ALOGD("DaemonSubManagerService::setCallback\n");
        mCallback = callback;
        return 0;
    }

}; // namespace android
