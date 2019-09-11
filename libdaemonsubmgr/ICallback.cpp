#define LOG_TAG "DAEMON_SUB_SERVICE"
#include <utils/Log.h>
#include <binder/Parcel.h>
#include "libdaemonsubmgr/IDaemonSubManagerService.h"
#include <binder/IInterface.h>


#define LOG_NDEBUG 0

namespace android {

enum {
    NOTIFY_CALLBACK,
};

//////////////////client
class BpCallback : public BpInterface<ICallback> {
public:
    BpCallback(const sp<IBinder>& impl) : BpInterface<ICallback>(impl) {
    }

    virtual int notifyCallback(int a) {
        ALOGD("BpCallback::notifyCallback, a = %d", a);
        Parcel data,reply;
        data.writeInt32(a);
        remote()->transact(NOTIFY_CALLBACK,data,&reply);
        return reply.readInt32();
    }
};

IMPLEMENT_META_INTERFACE(Callback, "android.daemonservice.ICallback");


////////////////server
status_t BnCallback::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    switch (code) {
        case NOTIFY_CALLBACK: {
            ALOGD("BnCallback::onTransact>>NOTIFY_CALLBACK\n");
            reply->writeInt32(notifyCallback((int) data.readInt32()) );
            return NO_ERROR;
        } break;
    }

    return BBinder::onTransact(code, data, reply, flags);
}
}
