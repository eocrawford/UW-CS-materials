import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * FileTableEntry class
 * 
 * Description:
 * Stores runtime information about a file being accessed by 1 or more threads.
 *
 * Assumptions and limitations:
 * None
 * 
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class FileTableEntry
{
    // Inode reference
    public final Inode inode;
    // Inumber of Inode
    public final int iNumber;
    // Access flags
    public final short flags;
    // File seek pointer
    public int seekPtr;
    // Refcount - # of threads using this entry
    public int count;
    // Reference to FileTable for callbacks
    private FileTable table;

    /**
    * FileTableEntry ctor
    * @param ft FileTable reference
    * @param i Inode
    * @param in iNumber
    * @param f Access flags
    */
    public FileTableEntry(FileTable ft, Inode i, int in, short f) 
    {
        table = ft;
        inode = i;
        iNumber = in;
        flags = f;
        count = 0;

        // if mode is append, seekPtr points to the end of file
        seekPtr = ((flags & FileSystem.ACCESS_APPEND) != 
            FileSystem.ACCESS_APPEND) ? 0 : inode.length;        
    }

    /**
     * Increment refcount
     * @return incremented refcount
     */
    public synchronized int addRef()
    {
        count++;
        return count;
    }

    /**
     * Decrement refcount
     * @return decremented refcount
     */
    public synchronized int release()
    {
        // Call back into the file table to do the 
        // right thing with this FTE.
        table.ffree(this);
        return count;
    }

    /**
     * get refcount
     * @return current refcount
     */
    public synchronized int getCount()
    {
        return count;
    }
}

