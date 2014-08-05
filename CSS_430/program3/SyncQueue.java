import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * SyncQueue synchronization object
 * Implements an array of queues that synchnronize threads based on a 
 * user-provided set of events. Each event is represented by a
 * ThreadSyncNode, and each ThreadSyncNode maintains a queue of threads 
 * to synchronize.
 * 
 * @author      Ethan Crawford
 * @date        November 13, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class SyncQueue
{
	/**
	 * Zero-parameter constructor
	 */
	public SyncQueue()
	{
		this(10);
	}

	/**
	 * One-parameter constructor - takes the number of events to support
	 * Note that this is currently a no-op; this implementation adds events
	 * dynamically when enqueueAndSleep() is called.
	 */
	public SyncQueue(int eventMax)
    {
		events = new ThreadSyncNode[eventMax];
    }

	/**
	 * Enqueues and sleeps the calling thread until event is satisfied. 
	 * Returns the ID of a child thread that woke the calling thread, or 
	 * INVALID_TID if no thread satisfies the event.
	 */
	public int enqueueAndSleep(int event)
    {
//		System.out.printf("enqueueAndSleep(event %d)\n", event);
		ensureExists(event);
		return events[event]._wait();
    }

	/**
	 * Dequeues and wakes up the first thread waiting for a given event. 
	 */
	public void dequeueAndWakeup(int event)
	{
//		System.out.printf("dequeueAndWakeup(event %d)\n", event);
		dequeueAndWakeup(event, INVALID_TID);
	}

	/**
	 * Dequeues and wakes up the first thread waiting for a given event. 
	 * Tid will be passed to the thread that has been woken up from enqueueAndSleep.
	 */
	public void dequeueAndWakeup(int event, int tid)
    {
//		System.out.printf("dequeueAndWakeup(event %d, tid %d)\n", 
//			event, tid);
		ensureExists(event);
		events[event]._notify(tid);
    }

	/**
	 * Make sure the specified event exists in the array
	 */
	private void ensureExists(int event)
	{
		if (events[event] == null)
		{
			events[event] = new ThreadSyncNode();
		}
	}

	private ThreadSyncNode[] events = null;
	private static final int INVALID_TID = 0;

	/**
	 * ThreadSyncNode class: used to synchronize a list of threads
	 * associated with a single event.
	 */
	private class ThreadSyncNode
	{
		/**
		 * Wait for queue activity, processing one thread at a time.
		 * After being awakened, return the integer in the tidQueue
		 */
		public synchronized int _wait()
		{
			int ret;

			if (tidQueue.size() <= 0)
			{
//				System.out.printf(
//					"_wait(): waiting for an item to be added to the queue\n");
				try { wait(); }
				catch(InterruptedException ex) {}
			}

			ret = tidQueue.poll().intValue();
//			System.out.printf("_wait(): waking up and returning %d\n", ret);
			return ret;
		}

		/**
		 * Wake up one thread in the queue, storing the tid
		 */
		public synchronized void _notify(int tid)
		{
//			System.out.printf("_notify(tid %d): adding an item to the queue\n", 
//				tid);
			tidQueue.add(new Integer(tid));
			notify();
		}

		private Queue<Integer> tidQueue = new LinkedBlockingQueue<Integer>();
	}
}
