package test.daemon.sub.commands;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import com.android.internal.os.BaseCommand;
import java.io.PrintStream;
import daemon.sub.DaemonSubManager;

public class TestDaemonSubCmd extends BaseCommand {
	DaemonSubManager TSM = DaemonSubManager.getInstance();
	public static void main(String[] args) {
		(new TestDaemonSubCmd()).run(args);
		return;
	}
	public void onRun() throws Exception {
		String op = nextArgRequired();

		if (op.equals("setcmd"))
		{
			String id = nextArg();
			if (id == null)
			{
				onShowUsage(System.err);
				return;
			}
			DaemonSubManager.getInstance().reg_callback();
			DaemonSubManager.getInstance().setCmd(Integer.parseInt(id));
			System.out.println("TestDaemonSubCmd: setCmd: " + Integer.parseInt(id));
		}
		else if(op.equals("getevent"))
		{
			int  mode  = DaemonSubManager.getInstance().getEvent();
			System.out.println("TestDaemonSubCmd: getEvent: " + mode);
		}
		else if(op.equals("read"))
		{
			byte[] str_byte = new byte[100];
			DaemonSubManager.getInstance().read(str_byte, 100);
			System.out.println("TestDaemonSubCmd: getStr: " + new String(str_byte));
		}
		else if(op.equals("write"))
		{
			byte[] str_byte = "write something".getBytes();
			DaemonSubManager.getInstance().write(str_byte, 100);
		}
		else if(op.equals("open"))
		{
			TSM.open();
			TSM.block();
		}
		else {
			showError("Error: unknow command '" + op + "'");
		}
		return;
	}


	public void onShowUsage(PrintStream out) {
		out.println(
				"help:\n" +
				"setmode    [0/1]\n" +
				"getmode\n"
			   );
	}
}
