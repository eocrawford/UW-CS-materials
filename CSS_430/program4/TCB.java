/**
 * TCB class
 * 
 * Description:
 * Stores information used for thread scheduling and file descriptor sharing
 *
 * Assumptions and limitations:
 * 32 file descriptors total are available. 3 of them are reserved 
 * for cin, cout, and cerr.
 * 
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class TCB
{
    private final static int startingFd = 3;
    private final static int maxFd = 32;
    private Thread thread = null;
    private int tid = 0;
    private int pid = 0;
    private boolean terminated = false;
    private int sleepTime = 0;
    // file descriptor table - maps between descriptors and FTEs
    private FileTableEntry[] fileDescriptorTable = null;

    public TCB(Thread newThread, int myTid, TCB parentTcb)
    {
        thread = newThread;
        tid = myTid;
        pid = (parentTcb != null) ? parentTcb.getTid() : -1;
        terminated = false;

        fileDescriptorTable = new FileTableEntry[maxFd];

        for (int i = 0; i < maxFd; i++)
        {
            // In order to support parent/child sharing of file descriptors,
            // copy the parent fileDescriptorTable and increment the
            // FileTableEntry refcounts.
            if (parentTcb != null)
            {
                fileDescriptorTable[i] = parentTcb.fileDescriptorTable[i];

                // Increment the refcount
                if (fileDescriptorTable[i] != null)
                {
                    fileDescriptorTable[i].addRef();
                }
            }
            // No parent; set table addresses to null
            else
            {
                fileDescriptorTable[i] = null;
            }
        }

        System.err.println( "threadOS: a new thread (thread=" + thread + 
                    " tid=" + tid + 
                    " pid=" + pid + ")");
    }

    public synchronized Thread getThread( ) {
    return thread;
    }

    public synchronized int getTid( ) {
    return tid;
    }

    public synchronized int getPid( ) {
    return pid;
    }

    public synchronized boolean setTerminated( )
    {
        terminated = true;

        for (int i = startingFd; i < maxFd; i++)
        {
            // Decrement file descriptor refcounts on thread exit
            // This prevents orphaned file descriptors.
            if (fileDescriptorTable[i] != null)
            {
                fileDescriptorTable[i].release();
            }
        }

        return terminated;
    }

    public synchronized boolean getTerminated( ) {
    return terminated;
    }

    /**
     * Check if the thread has any open file descriptor table entries
     *  
     * @return true if entries are available; false otherwise.
     */
    public synchronized boolean isFdAvailable()
    {
        for (int i = startingFd; i < maxFd; i++) 
        {
            if (fileDescriptorTable[i] == null)
            {
                return true;
            }
        }

        return false;
    }

    /**
     * Add a FileTableEntry to the internal array, returning the associated 
     * file descriptor
     *  
     * @return file descriptor on success, -1 on failure 
     */
    public synchronized int addFTE(FileTableEntry entry)
    {
        if (entry == null)
        {
            return -1;
        }
        
        for (int i = startingFd; i < maxFd; i++) 
        {
            if (fileDescriptorTable[i] == null)
            {
                fileDescriptorTable[i] = entry;
                return i;
            }
        }
        
        return -1;
    }

    /**
     * Returns the FileTableEntry associated with the provided file descriptor
     * and removes the reference from the internal array.
     * 
     * @param fd File descriptor
     * @return valid FTE on success, null FTE on failure
     */
    public synchronized FileTableEntry removeFTE(int fd)
    {
        return getFTE(fd, true);
    }

    /**
     * Returns the FileTableEntry associated with the provided file descriptor
     * 
     * @param fd File descriptor
     * @return valid FTE on success, null FTE on failure
     */
    public synchronized FileTableEntry peekFTE(int fd)
    {
        return getFTE(fd, false);
    }
    
    /**
     * Gets the FileTableEntry associated with the provided file descriptor,
     * optionally removing it from the file descriptor table
     * 
     * @param fd File descriptor
     * @param erase If true, removes entry from file descriptor table
     * @return valid FTE on success, null FTE on failure
     */
    private FileTableEntry getFTE(int fd, boolean erase)
    {
        FileTableEntry ret = null;

        if (fd >= startingFd && fd < maxFd 
                && fileDescriptorTable[fd] != null)
        {
            ret = fileDescriptorTable[fd];
            
            if (erase)
                fileDescriptorTable[fd] = null;
        }

        return ret;
    }
}
