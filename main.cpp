#define LOG_TAG "DAEMON_SUB_SERVICE"
#include <utils/Log.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <cutils/properties.h>
#include <libdaemonsubmgr/DaemonSubManagerService.h>
using namespace android;
extern int main_process();
int main(int argc, char** argv)
{
    (char**) argv;
    if (argc == 1) {
        ALOGI("daemon sub Manager Server init.\n");
        sp<ProcessState> proc(ProcessState::self());
        sp<IServiceManager> sm = defaultServiceManager();
        ALOGI("daemon sub Manager: %p", sm.get());

        if (sm->checkService(String16("daemonsub.manager")) != NULL) {
            ALOGW("daemon sub Manager has already started!, return.\n");
            return 0;
        }
        DaemonSubManagerService::instantiate();
        ProcessState::self()->startThreadPool();
        main_process();
        IPCThreadState::self()->joinThreadPool();
    } else {
        ALOGI("daemon sub Manager Test Client.\n");
    }
    ALOGI("daemon sub Manager close, should never come here.\n");
    return 0;
}
