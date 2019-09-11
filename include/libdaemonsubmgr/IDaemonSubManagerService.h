#ifndef __IDAEMONSUB_MANAGER_SERVICE_H__
#define __IDAEMONSUB_MANAGER_SERVICE_H__

#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include "ICallback.h"
namespace android {
class IDaemonSubManagerService: public IInterface
{
public:
    DECLARE_META_INTERFACE(DaemonSubManagerService);

    virtual status_t    setCmd(uint32_t mode) = 0;
    virtual status_t    getEvent(uint32_t *mode) = 0;

    //official interface:
    virtual status_t    enquiry() = 0;
    virtual status_t    open() = 0;
    virtual status_t    close() = 0;
    virtual status_t    read(char **str, int32_t *size) = 0;
    virtual status_t    write(char **str, int32_t *size) = 0;
    virtual int setCallback(const sp<ICallback>& callback)=0;
};

// ----------------------------------------------------------------------------

class BnDaemonSubService: public BnInterface<IDaemonSubManagerService>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}; // namespace android

#endif // __IDAEMONSUB_MANAGER_SERVICE_H__