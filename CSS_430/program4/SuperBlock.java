import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * SuperBlock class
 * 
 * Description
 * Disk block 0 is called a superblock and used to describe (1) the number of 
 * disk blocks, (2) the number of inodes, and (3) the block number of the 
 * head block of the free list. This is the OS-managed block. No other 
 * information must be recorded in and no user threads must be able to get 
 * access to the superblock. 
 * 
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
class Superblock 
{
    // The number of disk blocks
    public int totalBlocks = 0;
    
    // The number of inodes
    public int totalInodes = 0;
    
    // The block number of the free list's head
    public int freeList = 0;

    /**
     * Load Superblock data from disk
     * Note that the Superblock location is hard-coded to 0.
     * 
     * Fields less than 0 indicate a corrupt or missing filesystem
     * @return 0 on success or -1 on failure
     */
    public synchronized int load()
    {
        int ret = -1;
        int offset = 0;
        byte[] data = new byte[Disk.blockSize];

        int tb, ti, fl;

        SysLib.rawread(0, data);
        tb = SysLib.bytes2int(data, offset);
        offset += SysLib.CBINT;
        ti = SysLib.bytes2int(data, offset);
        offset += SysLib.CBINT;
        fl = SysLib.bytes2int(data, offset);
            
        // freeList can be -1 if we are out of disk space.
        // Otherwise, we assume that there is no valid filesystem
        if (tb > 0 && ti > 0 && fl >= -1)
        {
            totalBlocks = tb;
            totalInodes = ti;
            freeList = fl;
            ret = 0;
        }

        return ret;
    }
    
    /**
     * Flush Superblock data to disk
     * @return 0 on success or -1 on failure
     */
    public synchronized int flush()
    {
        int ret = -1;
        byte[] data = new byte[Disk.blockSize];
        
        // freeList can be -1 if we are out of disk space
        if (totalBlocks > 0 && totalInodes > 0 && freeList >= -1)
        {
            int offset = 0;
            
            SysLib.int2bytes(totalBlocks, data, offset);
            offset += SysLib.CBINT;
            SysLib.int2bytes(totalInodes, data, offset);
            offset += SysLib.CBINT;
            SysLib.int2bytes(freeList, data, offset);
            
            if (SysLib.rawwrite(0, data) == 0)
            {
                ret = 0;
            }
        }

        return ret;
    }
}

