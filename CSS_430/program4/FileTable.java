import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * FileTable class
 * 
 * Description:
 * Enforces file access policy, gets Inodes and maintains a table 
 * of open files.
 *
 * Assumptions and limitations:
 * Invalid access flags result in an error 
 *
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class FileTable
{
    /**
    * FileTable ctor
    * @param dir Directory object
    */
    public FileTable(Directory dir)
    {
        // instantiate a file (structure) table
        table = new Vector<FileTableEntry>();
        
        // Store a Directory instance (passed by the filesystem)
        directory = dir;
    }
    
    /**
    * Check file access policy, map the file name to an Inode, and
    * return a FileTableEntry object containing Inode reference and seek
    * pointer. 
    *
    * Remarks:
    * This FileTableObject is maintained inside FileTable
    * and a reference to it is stored (by the FileSystem) on the calling
    * thread's TCB. This provides support for file descriptor duplication
    * on fork().
    *
    * @param fileName provided filename
    * @param flags File mode
    * @return FileTableEntry
    */
    public synchronized FileTableEntry falloc(String fileName, short flags) 
    {
        FileTableEntry fte = null;
        Inode inode = null;
        boolean fCreate;
        int iNumber;

        // Invalid parameters
        if (flags == FileSystem.ACCESS_NONE)
        {
            return null;
        }

        // Create a new file if opened for writing and one does not exist
        fCreate = ((flags & FileSystem.ACCESS_WRITE) == 
            FileSystem.ACCESS_WRITE);

        // Get the iNumber corresponding to this file
        iNumber = directory.getiNumber(fileName, fCreate);
        if (iNumber < 0)
        {
            return null;
        }

        // Load the Inode
        inode = new Inode(iNumber);
        if (null == inode)
        {
            return null;
        }

        // Check requested access mode and enforce policy.
        if (inode.getCount() > 0)
        {
            // Only one thread may write to a file.
            if ((inode.flags & FileSystem.ACCESS_WRITE) == 
                FileSystem.ACCESS_WRITE)
            {
                return null;
            }

            // Files opened for reading but not writing may not be written.
            if (((inode.flags & FileSystem.ACCESS_READ) == 
                FileSystem.ACCESS_READ) &&
                ((flags & FileSystem.ACCESS_WRITE) == 
                FileSystem.ACCESS_WRITE))
            {
                return null;
            }
        }

        // Increment Inode refcount and store interesting information
        inode.addRef();
        inode.flags = flags;
        inode.flush();

        // Free all existing Inode blocks, if this is a pure write
        if (((inode.flags & FileSystem.ACCESS_APPEND) != 
            FileSystem.ACCESS_APPEND) &&
            ((inode.flags & FileSystem.ACCESS_READWRITE) !=
            FileSystem.ACCESS_READWRITE) &&
            ((inode.flags & FileSystem.ACCESS_WRITE) == 
            FileSystem.ACCESS_WRITE))
        {
            inode.clear();
        }

        // Create a new FTE and increment the refcount
        // NOTE: we never return an existing entry, meaning that a thread can
        // have the same file open for reading multiple times.
        // If a thread creates a child, its file descriptor table will be
        // automatically duplicated during the SysLib.exec() call.
        fte = new FileTableEntry(this, inode, iNumber, flags);
        fte.addRef();

        // Add the FTE to the internal table
        table.add(fte);

        return fte;
    }
    
    /**
    * Free the provided FileTableEntry, saving the corresponding inode 
    * to the disk and freeing the FileTableEntry.
    *
    * @param threadFte Provided FileTableEntry
    * @return 0 on success; -1 on failure
    */
    public synchronized int ffree(FileTableEntry threadFte) 
    {
        FileTableEntry fte = null;
        int fteIndex = -1;

        // Get the FTE from the Vector
        for (int i = 0; i < table.size(); i++)
        {
            fte = table.get(i);

            if (threadFte == fte)
            {
                fteIndex = i;
                break;
            }
        }

        // FTE not found
        if (fteIndex < 0)
        {
                return -1;
        }

        // Decrement the FTE refcount
        // If FTE refcount is 0, delete the FTE from the Vector
        // and clear the Inode access flags
        fte.count--;
        if (fte.count <= 0)
        {
            table.remove(fteIndex);
            fte.inode.flags = 0;
        }
        
        // Decrement the Inode refcount
        // If the file is marked for unlinking, free all
        // allocated disk blocks.
        if (fte.inode.release() <= 0)
        {
            if (((fte.inode.flags & FileSystem.ACCESS_UNLINKED) == 
                FileSystem.ACCESS_UNLINKED))
            {
                fte.inode.clear();
            }
        }

        // Write state changes back to disk
        fte.inode.flush();
        
        return 0;
    }
    
    /**
    * Returns true if the FileTable is empty
    *
    * @return True if the FileTable is empty
    */
    public synchronized boolean fempty() 
    {
        return table.isEmpty();
    }
    
     // the actual entity of this file table
    private Vector<FileTableEntry> table;
    
    // the root directory
    private Directory directory;
}

