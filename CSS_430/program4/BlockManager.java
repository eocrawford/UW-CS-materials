import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * BlockManager class
 * 
 * Description:
 * Handles allocating and freeing disk blocks and updating the Superblock.
 * 
 * Assumptions and limitations:
 * Only sizeof(signed short) (2^15) blocks are available for addressing
 *
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class BlockManager
{
    public BlockManager(Superblock sb, int tb)
    {
        superBlock = sb;
        totalBlocks = tb;
    }
    
    /**
     * Formats data blocks as a linked list
     * @return 0 on success, -1 on failure
     */
    public synchronized int format()
    {
        int nextBlock = superBlock.freeList;
        
        for (int i = superBlock.freeList; i < totalBlocks; i++)
        {
            byte b[] = new byte[Disk.blockSize];
            
            if (i == (totalBlocks - 1))
                nextBlock = -1;
            else
                nextBlock++;

            SysLib.int2bytes(nextBlock, b, 0);
            SysLib.rawwrite(i, b);
        }
        
        return 0;
    }

    /**
     * Allocates a new disk block, returning the block number
     * @return The block number on success, -1 on failure
     */
    public synchronized short allocateBlock()
    {
        // Get the first available block
        short ret = (short)superBlock.freeList;

        // Read the first available block for the next available block
        // (-1 means out of storage)
        if (ret != -1)
        {
            byte b[] = new byte[Disk.blockSize];

            SysLib.rawread(ret, b);

            // Set the next available block in the Superblock
            superBlock.freeList = SysLib.bytes2int(b, 0);
        }

        // Flush everything
        superBlock.flush();

        // Return the next available block
        return ret;
    }

    /**
     * Frees the provided disk block
     * @return 0 on success, -1 on failure
     */
    public synchronized int freeBlock(short block)
    {
        // Block 0 is reserved for the superblock
        // Block should be no larger than the largest block
        // Block should not already be on the free list
        if (block < 1 
            || block > (superBlock.totalBlocks - 1) 
            || block == superBlock.freeList)
        {
            return -1;
        }

        byte b[] = new byte[Disk.blockSize];

        // Store the previous free block on this block
        SysLib.int2bytes(superBlock.freeList, b, 0);
        SysLib.rawwrite(block, b);

        // Set the next available block in the Superblock
        superBlock.freeList = block;

        // Flush everything
        superBlock.flush();

        return 0;
    }

    private Superblock superBlock;
    private int totalBlocks;
}

