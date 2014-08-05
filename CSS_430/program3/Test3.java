import java.util.Date;

/**
 * Test3 test class
 * Launches three TestThread3 instances
 * 
 * @author      Ethan Crawford
 * @date        November 13, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
class Test3 extends Thread
{

	public void run()
	{
		SysLib.exec(SysLib.stringToArgs("TestThread3 fs"));
		SysLib.exec(SysLib.stringToArgs("TestThread3 fs"));
		SysLib.exec(SysLib.stringToArgs("TestThread3 cpu"));
		for (int i = 0; i < 3; i++)
		{
			SysLib.join();
		}
		SysLib.cout( "Test3 finished\n" );
		SysLib.exit();
	}
}
