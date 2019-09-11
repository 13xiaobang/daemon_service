#ifndef __DAEMONSUB_MANAGER_SERVICE_H__
#define __DAEMONSUB_MANAGER_SERVICE_H__

#include <stdint.h>
#include <sys/types.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <libdaemonsubmgr/IDaemonSubManagerService.h>
#include "ICallback.h"
namespace android{
class DaemonSubManagerService: public BnDaemonSubService
{
public:
    static  void        instantiate();

    virtual status_t    setCmd(uint32_t mode);
    virtual status_t    getEvent(uint32_t *mode);

    // official interface:

    virtual status_t enquiry(void);
    virtual status_t open(void);
    virtual status_t close(void);
    virtual status_t read(char **str, int32_t *size);
    virtual status_t write(char **str, int32_t *size);
private:
                        DaemonSubManagerService();
    virtual             ~DaemonSubManagerService();
    virtual int setCallback(const sp<ICallback>& callback);
    sp<ICallback> mCallback;
};

};

#endif // __DAEMONSUB_MANAGER_SERVICE_H__