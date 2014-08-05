import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * FileSystem class
 * 
 * Description:
 * Encapsulates the functionality of a filesystem, providing thread 
 * synchronization and all the methods appropriate to a stream-driven
 * FS. Internal implementation is indexed, in simplified UFS-style.
 * 
 * Assumptions and limitations:
 * Max files is bounded by the maximum space available to store Inodes
 *
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class FileSystem
{
    public final static short ACCESS_NONE = 0x0;
    public final static short ACCESS_READ = 0x1;
    public final static short ACCESS_WRITE = 0x2;
    public final static short ACCESS_READWRITE = ACCESS_READ | ACCESS_WRITE;
    public final static short ACCESS_UNLINKED = 0x4;
    public final static short ACCESS_APPEND = 0x8 | ACCESS_WRITE;
    public static int totalBlocks;
    public static int maxFiles;
    public static BlockManager blockManager = null;

    /**
     * FileSystem ctor - init total blocks and determine
     * current formatted state.
     */
    public FileSystem(int tb)
    {
        totalBlocks = tb;
        superBlock = new Superblock();
        blockManager = new BlockManager(superBlock, totalBlocks);
        
        if (superBlock.load() < 0)
        {
            isFormatted = false;
        }
        else
        {
            isFormatted = true;
            maxFiles = superBlock.totalInodes;
            totalBlocks = superBlock.totalBlocks;
            load();
        }
    }

    /**
     * Returns the current formatted state of the FileSystem
     *
     * @return true if FS is formatted; false otherwise
     */
    public boolean formatted()
    {
        return isFormatted;
    }

    /**
     * Formats the disk.
     * This consists of writing Superblock and empty Inodes and 
     * formatting free disk blocks in a linked list.
     *
     * @param mf The maximum number of files to be created
     * @return 0 on success, -1 on failure
     */
    public synchronized int format(int mf)
    {
        int r = -1;

        // Check for out-of-range maxFiles
        // Max files is bounded by the storage space required for inodes. 
        if (mf < 1 || (Inode.iNodeSize * mf) >= 
            (Disk.blockSize * (totalBlocks - 1)))
        {
            return r;
        }

        // Check for open files - can't format in that case.
        if (fileTable != null && !fileTable.fempty())
        {
            return r;
        }

        maxFiles = mf;

        // Init Superblock
        superBlock.totalBlocks = totalBlocks;
        superBlock.totalInodes = maxFiles;
        superBlock.freeList = ((maxFiles * Inode.iNodeSize) / 
            Disk.blockSize) + 
            (((Disk.blockSize % maxFiles) != 0) ? 1 : 0) + 1;

        r = superBlock.flush();
        if (r < 0)
        {
            return r;
        }
        
        // Write empty Inodes to disk.
        Inode inode = new Inode();
        for (int i = 0; i < maxFiles; i++)
        {
            inode.iNumber = i;

            r = inode.flush();
            if (r < 0)
            {
                return r;
            }
        }
        
        // Format free disk blocks as a linked list
        r = blockManager.format();
        if (r < 0)
        {
            return r;
        }
                
        r = load();

        return r;
    }
    
    /**
     * Set up objects that require knowlege of maxFiles
     * @return 0 on success, -1 on failure
    */
    public synchronized int load()
    {
        
        // Inode 0 is reserved for the directory contents
        directory = new Directory(new Inode(0), maxFiles);
        fileTable = new FileTable(directory);

        return 0;
    }

    /**
     * Opens the specified file in the given mode
     * ("r" = read-only, "w" = write only, "w+" = read/write,"a" = append),
     * and returns the new file descriptor.
     *
     * Remarks:
     * The FileTable does most of the heavy lifting required by this method.
     * 
     * @param tcb TCB provided by the kernel
     * @param fileName Specified file
     * @param mode Specified mode (r/w/w+/a)
     * @return new file descriptor on success, -1 on failure.
     */
    public synchronized int open(TCB tcb, String fileName, String mode)
    {
        int fd = -1;
        short flags;
        FileTableEntry fte;

        // Invalid params
        if (tcb == null || fileName == null || mode == null)
        {
            return -1;
        }

        // Convert string to flags
        flags = stringToFlags(mode);
        if (flags == ACCESS_NONE)
        {
            return -1;
        }

        // Ensure that the TCB has file descriptor space
        if (! tcb.isFdAvailable())
        {
            return -1;
        }

        // Get a new FileTableEntry for this thread.
        fte = fileTable.falloc(fileName, flags);
        if (null == fte)
        {
            return -1;
        }

        // Add the new FileTableEntry to the provided TCB
        fd = tcb.addFTE(fte);

        return fd;
    }
    
    /**
     * Closes the file corresponding to fd and frees the fd from the TCB
     *  
     * @param tcb TCB provided by the kernel
     * @param fd File descriptor
     * @return 0 on success, -1 on failure
     */
    public synchronized int close(TCB tcb, int fd)
    {
        // Check for invalid or reserved file descriptor
        if (verifyFileDescriptor(fd) < 0)
        {
            return -1;
        }

        // Remove the FTE from the calling thread's TCB
        FileTableEntry fte = tcb.removeFTE(fd);
        if (null == fte)
        {
            return -1;
        }

        // Remove the FTE from the FileTable
        if (fileTable.ffree(fte) < 0)
        {
            return -1;
        }

        return 0;
    }

    /**
     * Returns the size in bytes of the file indicated by fd.
     *  
     * @param tcb TCB provided by the kernel
     * @param fd File descriptor
     * @return file size in bytes on success or -1 on failure
     */
    public synchronized int fsize(TCB tcb, int fd)
    {
        int r = -1;
        Inode inode = null;
        FileTableEntry fte = null;
        
        // Check for invalid or reserved file descriptor
        if (verifyFileDescriptor(fd) < 0)
        {
            return -1;
        }

        // Get FileTableEntry associated with file descriptor
        fte = tcb.peekFTE(fd);
        if (null == fte)
        {
            return -1;
        }

        // Get Inode associated with FileTableEntry
        inode = fte.inode;
        if (null == inode)
        {
            return -1;
        }

        return inode.getFileSize();
    }
    
    /**
     * Reads either data.length or file.length bytes (whichever is smaller)
     * from the file indicated by fd, starting at the current seek pointer
     * position. The seek pointer is incremented by the number of bytes read.
     * 
     * @param tcb TCB provided by the kernel
     * @param fd File descriptor
     * @param data Buffer in which to store the bytes read
     * @return number of bytes read on success, -1 on failure
     */
    public synchronized int read(TCB tcb, int fd, byte data[])
    {
        int ret = -1;
        Inode inode = null;
        FileTableEntry fte = null;

        // Check for invalid or reserved file descriptors 
        if (verifyFileDescriptor(fd) < 0)
        {
            return -1;
        }

        // Invalid parameters
        if (data == null)
        {
            return -1;
        }

        // Check for invalid buffer length. Handle this as a no-op.
        if (data.length <= 0)
        {
            return 0;
        }

        // Get FileTableEntry associated with file descriptor
        fte = tcb.peekFTE(fd);
        if (null == fte)
        {
            return -1;
        }
        
        // Get Inode associated with FileTableEntry
        inode = fte.inode;
        if (null == inode)
        {
            return -1;
        }

        // Read
        inode.iNumber = fte.iNumber;
        if ((ret = inode.read(fte.seekPtr, data)) < 0)
        {
            return -1;
        }

        // Seek ahead
        seek(fte, ret, SysLib.SEEK_CUR);

        return ret;
    }
    
    /**
     * Writes data.length bytes to the file indicated by fd, starting at 
     * the current seek pointer position. The seek pointer is incremented by 
     * the number of bytes written. If necessary, the file size is increased.
     * 
     * @param tcb TCB provided by the kernel
     * @param fd File descriptor
     * @param data Buffer containing the bytes to be written
     * @return number of bytes written on success, -1 on failure
     */
    public synchronized int write(TCB tcb, int fd, byte data[])
    {
        int ret = -1;
        Inode inode = null;
        FileTableEntry fte = null;

        // Check for invalid or reserved file descriptors 
        if (verifyFileDescriptor(fd) < 0)
        {
            return -1;
        }

        // Invalid parameters
        if (data == null)
        {
            return -1;
        }

        // Check for invalid buffer length. Handle this as a no-op.
        if (data.length <= 0)
        {
            return 0;
        }

        // Get FileTableEntry associated with file descriptor
        fte = tcb.peekFTE(fd);
        if (null == fte)
        {
            return -1;
        }
        
        // Get Inode associated with FileTableEntry
        inode = fte.inode;
        if (null == inode)
        {
            return -1;
        }

        // Write
        inode.iNumber = fte.iNumber;
        if ((ret = inode.write(fte.seekPtr, data)) < 0)
        {
            return -1;
        }

        // Seek ahead
        seek(fte, ret, SysLib.SEEK_CUR);

        return ret;
    }

    /**
     * Updates the seek pointer corresponding to fd as follows:
     * If w is SEEK_SET (= 0), the file's seek pointer is set to offset 
     * bytes from the beginning of the file
     * If w is SEEK_CUR (= 1), the file's seek pointer is set to its 
     * current value plus the offset. The offset can be positive or negative.
     * If w is SEEK_END (= 2), the file's seek pointer is set to the size 
     * of the file plus the offset. The offset can be positive or negative.
     * 
     * NOTE: Seeks are bounded by filesize - seeks beyond bof reset to bof;
     * seeks beyond eof reset to eof. Both are success cases.
     * 
     * @param tcb TCB provided by the kernel
     * @param fd File descriptor
     * @param o Offset within file
     * @param w Flag that determines offset handling and seek direction
     * @return current value of the seek pointer on success, -1 on failure
     */
    public synchronized int seek(TCB tcb, int fd, int o, int w)
    {
        FileTableEntry fte = null;

        // Check for invalid or reserved file descriptors 
        if (verifyFileDescriptor(fd) < 0)
        {
            return -1;
        }

        // Get FileTableEntry associated with file descriptor
        fte = tcb.peekFTE(fd);
        if (null == fte)
        {
            return -1;
        }

        return seek(fte, o, w);
    }

    /**
     * Implements seek as described above.
     * 
     * @param fte FileTableEntry containing the seekPtr
     * @param o Offset within file
     * @param w Flag that determines offset handling and seek direction
     * @return current value of the seek pointer on success, -1 on failure
     */
    private int seek(FileTableEntry fte, int o, int w)
    {
        // Use the file length as eof marker
        int eof = fte.inode.length;

        // Check for invalid seek flag
        if (! (w == SysLib.SEEK_SET || 
               w == SysLib.SEEK_CUR || 
               w == SysLib.SEEK_END))
        {
            return -1;
        }

        // Check for invalid offset/flag combos
        if (w == SysLib.SEEK_SET && o < 0)
        {
            return -1;
        }

        // Adjust the seek pointer
        switch (w)
        {
            case SysLib.SEEK_SET:
                fte.seekPtr = o;
                break;

            case SysLib.SEEK_CUR:
                fte.seekPtr += o;
                break;

            case SysLib.SEEK_END:
                fte.seekPtr = eof + o;
                break;
        }

        // Seek pointer has been moved past eof - reset to eof
        if (fte.seekPtr > eof)
        {
            fte.seekPtr = eof;
        }
        // Seek pointer has been moved before bof - reset to bof
        else if (fte.seekPtr < 0)
        {
            fte.seekPtr = 0;
        }
        
        return fte.seekPtr;
    }

    /**
     * Deletes the file specified by fileName. If the file is currently open,
     * it is not deleted until the last open on it is closed, but new 
     * attempts to open it will fail.
     * 
     * @param fileName File to be deleted
     * @return 0 on success, -1 on failure
     */
    public synchronized int delete(String fileName)
    {
        int iNumber;
        Inode inode;
        
        // Get the iNumber corresponding to this file
        // This will fail if the file has been marked for deletion
        iNumber = directory.getiNumber(fileName, false);
        if (iNumber < 0)
        {
            return -1;
        }

        // Load the Inode
        inode = new Inode(iNumber);
        if (null == inode)
        {
            return -1;
        }
        
        // If refcount is 0, clear the Inode
        if (inode.getCount() <= 0)
        {
            inode.clear();
        }
        else
        {
            // Otherwise, mark the file for deletion to prevent 
            // further access to it.
            inode.flags |= FileSystem.ACCESS_UNLINKED;
        }

        // Write Inode state changes to disk
        inode.flush();

        // Remove the file from the directory listing
        return directory.deleteEntry(fileName);
    }
    
    private int verifyFileDescriptor(int fd)
    {
        if (fd > 2 && fd < maxFiles)
        {
            return 0;
        }
        return -1;
    }

    private static short stringToFlags(String s)
    {
        short flags = ACCESS_NONE;

        if (s.equals("r"))
            flags = ACCESS_READ;
        else if (s.equals("w"))
            flags = ACCESS_WRITE;
        else if (s.equals("w+"))
            flags = ACCESS_READWRITE;
        else if (s.equals("a"))
            flags = ACCESS_APPEND;
        
        return flags;
    }

    private boolean isFormatted;
    private Superblock superBlock = null;
    private FileTable fileTable = null;
    private Directory directory = null;
}

