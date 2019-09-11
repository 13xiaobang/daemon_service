# daemon_service
a daemon service for native to be called by android up layer( Java apk/services)

how to make:
copy the project in android project and then mm -B

build out:
/system/framerowk/testdaemonsubcmd.jar  
/system/framework/daemon.sub.jar  
/system/lib/libdaemonsubmgr.so  
/system/lib/libdaemonsubmgr_jni.so  
/system/bin/test_daemon_sub_cmd  
/system/bin/daemon_service  

how to use：
1. logcat -v time | grep DAEMON_SUB_SERVICE
2. run daemon_service &
3. run test_daemon_sub_cmd setcmd 1234 (onServiceCallback: 1235 will call back from server service)  
   run test_daemon_sub_cmd getevent  
   run test_daemon_sub_cmd write something  
   run test_daemon_sub_cmd read  
   run test_daemon_sub_cmd open (will show "call daemon_callback: str=this is callback" every 2s in logcat)  
  