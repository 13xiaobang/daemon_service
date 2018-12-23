#define LOG_TAG "DAEMON_SUB_SERVICE"
#include <utils/Log.h>
#include <stdio.h>
#include <time.h>
#include <libdaemonsubmgr/DaemonSubManagerService.h>

#include <sys/types.h>
#include <binder/IServiceManager.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <cutils/properties.h>
#include <time.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <errno.h>
#include <math.h>
#define printf ALOGI
using namespace android;

const static sp<IDaemonSubManagerService> getDaemonSubManagerService()
{
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("daemonsub.manager"));

    return interface_cast<IDaemonSubManagerService>(binder);
}

void * StartThread(void * param)
{
    while(1)
        usleep(1000000);
    return (void*)0;
}

int main_process()
{
    ALOGI("main_process called!!!");

    pthread_t server_id = 0;
    int ret;
    ret=pthread_create(&server_id,NULL,StartThread,NULL);
    if(ret != 0) {
        ALOGE ("create StartThread error!\n");
        return 1;
    }

    printf("main_process quit\n");
    return 0;
}
