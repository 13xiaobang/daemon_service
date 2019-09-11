package daemon.sub;

public class DaemonSubManager{
	private static final String TAG = "DaemonSubManager";
	private static volatile DaemonSubManager defaultInstance;
    static {
		System.loadLibrary("daemonsubmgr_jni");
	}
	public DaemonSubManager() {}
	public static DaemonSubManager getInstance() {
        if (defaultInstance == null) {
            synchronized (DaemonSubManager.class) {
                if (defaultInstance == null) {
                    defaultInstance = new DaemonSubManager();
                }
            }
        }
        return defaultInstance;
    }
	public native int setCmd(int mode);
	public native int getEvent();
	/*
	//public api:
	public native String getStr();
	public native byte[] getPcm();//temp use string to test blob r/w
	public native int pollEvent();
	*/

	// official interface:
	public native int enquiry(); //  return max size
	public native int open();
	public native int read(byte[] javaAudioData, int sizeInBytes); // return read size.
	public native int close();
	public native int write(byte[] javaAudioData, int sizeInBytes);
    public native int block();
    public native int reg_callback();
    public int onProgressCallBack(byte[] bytes, int len) {
        String str = new String(bytes);
        System.out.println("str:"+str);
        System.out.println("len:"+len);
        return 0;
    }

    public void onServiceCallback(int event) {
        System.out.println("onServiceCallback: "+ event);
    }
}
