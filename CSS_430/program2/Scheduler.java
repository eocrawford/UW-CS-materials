import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * Thread scheduler implemented using a multi-level feedback queue.
 * @author      Ethan Crawford
 * @date        October 23, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class Scheduler extends Thread
{
	/**
	 * Null-parameter constructor
	 */
	public Scheduler()
	{
		this(DEFAULT_TIME_SLICE, DEFAULT_MAX_THREADS);
	}

	/**
	 * One-parameter constructor
	 */
	public Scheduler(int quantum)
	{
		this(quantum, DEFAULT_MAX_THREADS);
	}

	/**
	 * Two-parameter constructor
	 */
	public Scheduler(int quantum, int maxThreads)
	{
		timeSlice = quantum;
		initTid(maxThreads);
		jobQueues.add(new JobQueue(DEFAULT_TIME_SLICE));
		jobQueues.add(new JobQueue(DEFAULT_TIME_SLICE * 2));
		jobQueues.add(new JobQueue(DEFAULT_TIME_SLICE * 4));
//		jobQueues.add(new JobQueue(INFINITE)); // FCFS
	}

	/**
	 * Run the scheduler
	 */
	public void run()
	{
		int jobIndex = -1;
		Job job = null;
		Thread t = null;

		// Run forever
		while (true)
		{
			// Check for jobs in priority order
			for (int i = 0; i < jobQueues.size(); i++)
			{
				// A job exists; run it
				if ((job = jobQueues.get(i).peek()) != null)
				{
					t = job.getThread();
					if (t != null)
					{
//						System.out.printf(
//						"Running job %d in queue %d\n", job.getTid(), i);

						if (!t.isAlive() && 
							t.getState() != Thread.State.TERMINATED)
							t.start();
						else
							t.resume();

						// Decrement the job's remaining quantum 
						// by the sleep interval.
						if (job.remainingQuantum != INFINITE)
							job.remainingQuantum -= DEFAULT_TIME_SLICE;

						jobIndex = i;

						break;
					}
				}
			}

			// Sleep to let other jobs do some real work.
			// Add a "fudge factor" to account for JRE overhead. This 
			// enables the scheduler to behave more or less 
			// as-expected when job CPU bursts are precise multiples
			// of job queue quantums.
			schedulerSleep(DEFAULT_TIME_SLICE + FUDGE_FACTOR);

			// Free completed or reschedule uncompleted jobs
			if (job != null)
			{
				t = job.getThread();

				// Dequeue and free completed jobs
				if (job.getTerminated())
				{
					freeTid(
						jobQueues.get(jobIndex).dequeue().getTid());
				}
				// Demote jobs that have outlived their allocated quantum.
				// Jobs with 'INFINITE' are never rescheduled, resulting
				// in FCFS behavior.
				else if (t != null && t.isAlive() 
					&& (job.remainingQuantum <= 0 
					&& job.remainingQuantum != INFINITE))
				{
					int i;

					t.suspend();

					// If the job is not already in the last queue, demote
					// it one queue. Otherwise, requeue it in the 
					// lowest-priority queue.
					i = (jobIndex >= jobQueues.size() - 1) ?
						jobIndex : jobIndex + 1;

//					if (i > jobIndex)
//					{
//						System.out.printf(
//							"Demoting job %d from %d to %d\n", 
//							job.getTid(), jobIndex, i);
//					}

					jobQueues.get(i).enqueue(
						jobQueues.get(jobIndex).dequeue());
				}

				job = null;
				t = null;
				jobIndex = -1;
			}
		}
	}

	/**
	 * Gets the TCB of the current thread
	 */
	public TCB getMyTcb()
	{
		Thread myThread = Thread.currentThread();

		try
		{
			for (JobQueue jq : jobQueues)
				for (Job j : jq)
					if (j.getThread() == myThread) 
						return j;
		}
		catch (NoSuchElementException ex) {}

		return null;
	}

	/**
	 * Returns the maximum number of available threads
	 */
	public int getMaxThreads()
	{
		return tids.length;
	}

	/**
	 * Schedule a new thread
	 */
	public TCB addThread(Thread t)
	{
		TCB parentTcb = getMyTcb(); // get my Job and find my TID
		int pid = (parentTcb != null) ? parentTcb.getTid() : -1;
		int tid = getNewTid(); // get a new TID
		if (tid == -1)
			return null;
		Job job = new Job(t, tid, pid); // create a new job
		jobQueues.get(0).enqueue(job);
		return job;
	}

	/**
	 * Remove a thread
	 */
	public boolean deleteThread()
	{
		TCB tcb = getMyTcb();
		if (tcb != null)
			return tcb.setTerminated();
		else
			return false;
	}

	/**
	 * Sleep a thread for some number of milliseconds
	 */
	public void sleepThread(int milliseconds)
	{
		try { sleep(milliseconds); }
		catch (InterruptedException e) { }
	}

	/**
	 * Sleep the scheduler for ms milliseconds
	 */
	private void schedulerSleep(int ms)
	{
		try { Thread.sleep(ms); }
		catch (InterruptedException ex) { }
	}

	/**
	 * Initializes the thread ID (tid) list
	 */
	private void initTid(int maxThreads)
	{
		tids = new boolean[maxThreads];
		for (int i = 0; i < maxThreads; i++)
			tids[i] = false;
	}

	/**
	 * Gets a tid for a new thread
	 */
	private int getNewTid()
	{
		for (int i = nextId; i < tids.length; i++)
		{
			if (tids[i] == false)
			{
				tids[i] = true;
				nextId = (i + 1) % tids.length;
				return i;
			}
		}

		return -1;
	}

	/**
	 * Removes the provided tid from the allocated list
	 */
	private void freeTid(int tid)
	{
		if (tid >= 0 && tid < tids.length && tids[tid] == true)
		{
			tids[tid] = false;
		}
	}

	private static final int DEFAULT_TIME_SLICE = 500;
	private static final int DEFAULT_MAX_THREADS = 10000;
	private static final int FUDGE_FACTOR = 15;
	private static final int INFINITE = -1;

	private boolean[] tids;
	private int nextId = 0;
	private int timeSlice = DEFAULT_TIME_SLICE;
	private List<JobQueue> jobQueues = new ArrayList<JobQueue>();

	/**
	 * Class derived from TCB, used to store the remaining execution quantum
	 * allocated to the job.
	 */
	class Job extends TCB
	{
		public Job(Thread newThread, int myTid, int parentTid)
		{
			super(newThread, myTid, parentTid);
		}

		public int remainingQuantum;
	}

	/**
	 * Class that wraps Queue<T> functionality, providing the max quantum to
	 * contained jobs. Implements Iterable and Iterator to enable foreach 
	 * support.
	 */
	class JobQueue implements Iterable<Job>, Iterator<Job>
	{
		public JobQueue(int q)
		{
			quantum = q;
		}

		public void enqueue(Job j)
		{
			j.remainingQuantum = quantum;
			q.add(j);
		}

		public Job dequeue()
		{
			return q.poll();
		}

		public Job peek()
		{
			return q.peek();
		}

		public Iterator<Job> iterator()
		{
			return (Iterator<Job>)this;
		}

		public boolean hasNext()
		{
			return q.iterator().hasNext();
		}

		public Job next()
		{
			return q.iterator().next();
		}

		public void remove()
		{
			q.iterator().remove();
		}

		private Queue<Job> q = new LinkedBlockingQueue<Job>();
		private int quantum;
	}
}
