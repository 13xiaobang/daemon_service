#include "libdaemonsubmgr/IDaemonSubManagerService.h"
#include "libdaemonsubmgr/ICallback.h"
#include <binder/BinderService.h>
namespace android {
class Callback: public BnCallback {
    public:
      virtual int notifyCallback(int a);
};
}
