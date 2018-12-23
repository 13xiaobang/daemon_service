#define LOG_TAG "DAEMON_SUB_SERVICE"

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <utils/String8.h>

#include <utils/Errors.h>
#include <utils/Log.h>
#include <libdaemonsubmgr/IDaemonSubManagerService.h>
namespace android {

    enum {
        SET_CMD = IBinder::FIRST_CALL_TRANSACTION,
        GET_EVENT,
        ENQUIRY,
        OPEN,
        CLOSE,
        READ,
        WRITE
    };

    class BpDaemonSubManagerService: public BpInterface<IDaemonSubManagerService>
    {
        public:
            BpDaemonSubManagerService(const sp<IBinder>& impl)
                : BpInterface<IDaemonSubManagerService>(impl)
            {
            }

            virtual status_t    setCmd(uint32_t mode)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                data.writeInt32((int32_t)mode);
                remote()->transact(SET_CMD, data, &reply);
                return reply.readInt32();
            }

            virtual status_t    getEvent(uint32_t *mode)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                remote()->transact(GET_EVENT, data, &reply);
                status_t status =  reply.readInt32();
                *mode = (uint32_t)reply.readInt32();
                return status;
            }
            virtual status_t	enquiry(void)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                remote()->transact(ENQUIRY, data, &reply);
                status_t status =  reply.readInt32();
                return status;
            }

            virtual status_t	open(void)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                remote()->transact(OPEN, data, &reply);
                status_t status =  reply.readInt32();
                return status;
            }

            virtual status_t	close(void)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                remote()->transact(CLOSE, data, &reply);
                status_t status =  reply.readInt32();
                return status;
            }

            virtual status_t	read(char **str, int32_t *size)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());

                data.writeInt32(*size);
                remote()->transact(READ, data, &reply);
                status_t status =  reply.readInt32();
                *size =  reply.readInt32();
                Parcel::ReadableBlob blob;
                reply.readBlob(*size, &blob);
                memcpy(*str, blob.data(), *size);
                blob.release();
                return status;
            }
            virtual status_t	write(char **str, int32_t *size)
            {
                Parcel data, reply;
                data.writeInterfaceToken(IDaemonSubManagerService::getInterfaceDescriptor());
                data.writeInt32(*size);
                Parcel::WritableBlob blob;
                data.writeBlob(*size, &blob);
                memcpy(blob.data(), *str, *size);
                status_t status = reply.writeBlob(*size, &blob);
                remote()->transact(WRITE, data, &reply);

                blob.release();
                return status;
            }
            virtual void register_cmd_callback(void (*cmd_cb)(int cmd)) {};
            virtual void sendEvent(int event) {};

    };

    IMPLEMENT_META_INTERFACE(DaemonSubManagerService, "android.hardware.IDaemonSubManagerService");

    // ----------------------------------------------------------------------

    status_t BnDaemonSubService::onTransact(
            uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
    {
        switch (code) {

            case SET_CMD: {
                              ALOGI("IDaemonSubManagerService :  SET_CMD");
                              CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                              uint32_t mode = 0;
                              mode = (uint32_t)data.readInt32();
                              reply->writeInt32(setCmd(mode));
                              return NO_ERROR;
                          } break;
            case GET_EVENT: {
                                ALOGI("IDaemonSubManagerService :  GET_EVENT");
                                CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                                uint32_t mode = 0;
                                reply->writeInt32(getEvent(&mode));
                                reply->writeInt32(mode);
                                return NO_ERROR;
                            } break;
            case ENQUIRY: {
                              ALOGI("IDaemonSubManagerService :  ENQUIRY");
                              CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                              reply->writeInt32(enquiry());
                              return NO_ERROR;
                          } break;
            case OPEN: {
                           ALOGI("IDaemonSubManagerService :  OPEN");
                           CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                           reply->writeInt32(open());
                           return NO_ERROR;
                       } break;
            case CLOSE: {
                            ALOGI("IDaemonSubManagerService :  CLOSE");
                            CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                            reply->writeInt32(close());
                            return NO_ERROR;
                        } break;
            case READ: {
                           ALOGI("IDaemonSubManagerService :  READ");
                           CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                           int32_t size;
                           char *str = (char *)malloc(100);

                           int require_size = data.readInt32();
                           reply->writeInt32(read(&str, &require_size));
                           reply->writeInt32(require_size);
                           Parcel::WritableBlob blob;
#ifdef CFLAGS_ANDROID_ABOVE_6
                           reply->writeBlob(require_size, false, &blob);
#else
                           reply->writeBlob(require_size, &blob);
#endif
                           memcpy(blob.data(), str, require_size);
                           blob.release();
                           free(str);
                           return NO_ERROR;
                       } break;
            case WRITE: {
                            ALOGI("IDaemonSubManagerService :  WRITE");
                            CHECK_INTERFACE(BpDaemonSubManagerService, data, reply);
                            int32_t size;		

                            int write_size = data.readInt32();
                            Parcel::ReadableBlob blob;
#ifdef CFLAGS_ANDROID_ABOVE_6
                            data.readBlob(write_size, false, &blob);
#else
                            data.readBlob(write_size, &blob);
#endif
                            char *str =(char *)malloc(write_size+1);
                            memcpy(str, blob.data(), write_size);
                            str[write_size] = '\0';
                            blob.release();
                            reply->writeInt32(write(&str, &write_size));
                            free(str);
                            return NO_ERROR;
                        }
            default:
                        ALOGV("UNKNOWN_CODE");
                        return BBinder::onTransact(code, data, reply, flags);
        }
        return NO_ERROR;
    }

}; // namespace android
