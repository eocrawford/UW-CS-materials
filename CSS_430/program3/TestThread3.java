/**
 * TestThread3 test class
 * Implements code to exercise the SyncQueue thread synchronization class
 * Supports both CPU-intensive and filesystem-intensive modes.
 * 
 * @author      Ethan Crawford
 * @date        November 13, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */

import java.util.*;

class TestThread3 extends Thread
{
	private enum Mode
	{ 
		CPU,
		FileSystem
	}

	private Mode mode;
	private long submissionTime;
	private long responseTime;
	private long completionTime;

    public TestThread3(String args[])
    {
		submissionTime = new Date().getTime();
		if (args[0].equals("cpu"))
		{
//			System.out.printf("CPU-intensive\n");
			mode = Mode.CPU;
		}
		else
		{
//			System.out.printf("FS-intensive\n");
			mode = Mode.FileSystem;
		}
    }

    public void run()
    {
		responseTime = new Date().getTime();
		if (mode == Mode.CPU)
			field = stressCpu();
		else if (mode == Mode.FileSystem)
			stressFileSystem();

		completionTime = new Date().getTime();
		SysLib.cout("Thread[" + mode + "]:" +
				 " response time = " + (responseTime - submissionTime) +
				 " turnaround time = " + (completionTime - submissionTime) +
				 " execution time = " + (completionTime - responseTime) +
				 "\n");
		SysLib.exit();
    }

	/**
	 * Occupy the CPU
	 */
	private int stressCpu()
	{
		Random rng = new Random();
		int ret = rng.nextInt();
		final int repeat = 1;

		// Occupy the CPU
//		System.out.printf("Stressing the CPU: 0\n");
		for (int i = 0; i < repeat; i++)
		{
//			System.out.printf("Stressing the CPU: 1\n");
			for (int j = 0; j < 1000; j++)
			{
				for (int k = 0; k < 100000; k++)
				{
					ret /= (rng.nextInt() + rng.nextInt()) + rng.nextInt();
				}
			}
		}

		return ret;
	}

	/**
	 * Occupy the filesystem
	 */
	private void stressFileSystem()
	{
		final int blockCount = 200;
		final int blockSize = 512;
		final int repeat = 1;
		byte data[] = new byte[blockSize];

		for (int j = 0; j < blockSize; j++)
		{
			data[j] = (byte)j;
		}

//		System.out.printf("Stressing the FS: 0\n");
		for (int q = 0; q < repeat; q++)
		{
//			System.out.printf("Stressing the FS: %d\n", q);
			for (int i = 0; i < blockCount; i++)
			{
				SysLib.rawwrite(i, data);
			}

			for (int i = 0; i < blockCount; i++)
			{
				SysLib.rawread(i, data);
			}
		}
	}

	private int field;
}
