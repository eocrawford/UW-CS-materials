import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * Inode class
 * 
 * Description:
 * Stores file metadata and provides access to file data, through a 
 * stream-style read and write interface.
 *
 * Assumptions and limitations:
 * Maximum file size is limited by the amount of total block pointer
 * storage space.
 *
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */
public class Inode 
{
    // Count of direct pointers
    private final static int directSize   = 11;

    // Count of indirect pointers
    private final static int indirectSize = (Disk.blockSize / SysLib.CBSHORT);

    public static final int iNodeSize     = 32;       // fix to 32 bytes
    public static final int maxFileSize   = 
        Disk.blockSize * (directSize + indirectSize);
    
    // # file-table entries pointing to this
    private short count;

    // File access flags
    public short flags;

    // The iNumber associated with this Inode.
    public int iNumber;

    // file size in bytes
    public int length;

    // Starting Inode disk block
    private final static int startingBlock = 1;

    // direct pointers
    private short direct[] = null;

    // a indirect pointer
    private short indirect;

    /**
     * Inode ctor - create an empty Inode
     */
    Inode()
    {
        this(-1);
    }

    /**
     * Inode ctor - load Inode at iNumber.
     * @param in iNumber to load into this instance
     */
    Inode(int in)
    {
        iNumber = in;

        length = 0;
        count = 0;
        flags = FileSystem.ACCESS_NONE;

        direct = new short[directSize];
        for (int i = 0; i < directSize; i++)
        {
            direct[i] = -1;
        }
        
        indirect = -1;
        iNumber = in;

        if (iNumber > -1)
        {
            load();
        }
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
        count--;
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

    /**
     * Read data.length bytes into provided buffer at the provided offset
     * @param fileOffset Offset into file
     * @param data out buffer to receive read bytes
     * @return number of bytes read on success, -1 on failure
     */
    public synchronized int read(int fileOffset, byte data[])
    {
        int cbRead = -1;
        int dataLen = data.length;

        // Uninitialized object (no iNumber to read)
        if (iNumber < 0)
        {
            return -1;
        }

        // File does not have read permissions at this time.
        if ((flags & FileSystem.ACCESS_READ) != FileSystem.ACCESS_READ)
        {
            return -1;
        }

        // Validate inputs - fileOffset cannot be greater than 
        // the current file length and should not be negative.
        if (fileOffset > length || fileOffset < 0)
        {
            return -1;
        }

        // Read as many bytes as possible, but don't read past eof
        int dstOffset = 0;
        int cbRemaining = cbRead = (dataLen < (length - fileOffset)) 
            ? dataLen : (length - fileOffset);

        // Read block-by-block until remaining is 0
        while (cbRemaining > 0)
        {
            byte b[] = new byte[Disk.blockSize];
            int indexEntry = fileOffset / Disk.blockSize;
            int cbCopied;
            int srcOffset;
            short block;

            // Get the block ptr for the current index entry
            block = getBlockPtr(indexEntry);
            if (block < 0)
            {
                return -1;
            }

            // Calculate the starting read byte within the given block
            srcOffset = (fileOffset % Disk.blockSize);

            // Calculate how much data should be copied from this block.
            // Copy either to the end of the block or to the end of the
            // buffer.
            cbCopied = (Disk.blockSize - srcOffset) < cbRemaining ? 
                (Disk.blockSize - srcOffset) : cbRemaining;

            // Read the data from the block and copy it into the buffer
            SysLib.rawread(block, b);
            System.arraycopy(b, srcOffset, data, dstOffset, cbCopied);

            // Decrement the bytes remaining to be copied
            cbRemaining -= cbCopied;

            // Increment the file offset to account for bytes already copied
            fileOffset += cbCopied;

            // Increment the dst buffer offset to account for bytes already 
            // copied
            dstOffset += cbCopied;
        }

        return cbRead;
    }

    /**
     * Writes provided data at the provided offset to the provided inode
     * @param startingOffset Offset into file
     * @param data in buffer providing bytes written
     * @return number of bytes written on success, -1 on failure
     */
    public synchronized int write(int startingOffset, byte data[])
    {
        int cbWritten = data.length;
        int fileOffset = startingOffset;

        // Uninitialized object (no iNumber to read)
        if (iNumber < 0)
        {
            return -1;
        }

        // File does not have write permissions at this time.
        if ((flags & FileSystem.ACCESS_WRITE) != FileSystem.ACCESS_WRITE)
        {
            return -1;
        }
        
        // Validate inputs - fileOffset cannot be greater than 
        // the current file length and should not be negative.
        if (fileOffset > length || fileOffset < 0)
        {
            return -1;
        }

        // Do nothing if there is no data to write.
        if (cbWritten <= 0)
        {
            return 0;
        }

        // Reserve sufficient space
        if (ensureCapacity(fileOffset + cbWritten) < 0)
        {
            return -1;
        }

        int srcOffset = 0;
        int cbRemaining = cbWritten;

        // Write the data buffer to disk
        while (cbRemaining > 0)
        {
            byte b[] = new byte[Disk.blockSize];
            int indexEntry = fileOffset / Disk.blockSize;
            int dstOffset;
            int cbCopied;
            short block;

            // Get the block ptr for the current index entry
            block = getBlockPtr(indexEntry);
            if (block < 0)
            {
                return -1;
            }

            // Calculate the starting write byte within the given block
            dstOffset = (fileOffset % Disk.blockSize);

            // Calculate how much data should be copied into this block.
            // Copy either to the end of the block or to the end of the
            // buffer.
            cbCopied = (Disk.blockSize - dstOffset) < cbRemaining ?
                (Disk.blockSize - dstOffset) : cbRemaining;

            // Write the data to the block
            SysLib.rawread(block, b);
            System.arraycopy(data, srcOffset, b, dstOffset, cbCopied);
            SysLib.rawwrite(block, b);

            // Decrement the bytes remaining to be copied
            cbRemaining -= cbCopied;

            // Increment the file offset to account for bytes already copied
            fileOffset += cbCopied;

            // Increment the source buffer offset to account for bytes 
            // already copied
            srcOffset += cbCopied;
        }

        // Update the current file length, if it grew
        int endingOffset = startingOffset + cbWritten;
        if (endingOffset > length)
        {
            length = endingOffset;
        }

        // Save the updated Inode
        if (flush() < 0)
        {
            return -1;
        }

        return cbWritten;
    }

    /**
     * Clear all blocks and block ptrs and reset the filesize to 0.
     */
    public void clear()
    {
        // Cannot empty an empty object
        if (iNumber < 0)
        {
            return;
        }

        // Free direct blocks and reset ptrs
        for (int i = 0; i < directSize; i++)
        {
            if (direct[i] > -1)
            {
                FileSystem.blockManager.freeBlock(direct[i]);
                direct[i] = -1;
            }
        }

        // Free indirect blocks and reset ptrs
        if (indirect != -1)
        {
            byte b[] = new byte[Disk.blockSize];
            int offset = 0;
            short indirectBlock = -1;

            SysLib.rawread(indirect, b);

            // Free indirect blocks
            for (int i = 0; i < indirectSize; i++)
            {
                indirectBlock = SysLib.bytes2short(b, offset);

                if (indirectBlock > -1)
                {
                    FileSystem.blockManager.freeBlock(indirectBlock);
                    offset += SysLib.CBSHORT;
                }
            }

            // Free the indirect ptr block
            FileSystem.blockManager.freeBlock(indirect);
            indirect = -1;
        }

        length = 0;

        flush();
    }

    /**
     * Get the file size
     * @return current file size
     */
    public int getFileSize()
    {
        return length;
    }

    /**
     * Get the block pointer corresponding to a direct or indirect index entry
     * 
     * NOTE: This implementation assumes that the block has been previously
     * allocated.
     *
     * @param index Block index (0 to 266)
     * @return Block pointer
     */
    private short getBlockPtr(int index)
    {
        short ret = (short)-1;

        // Check for array out-of range - this implementation only supports
        // one disk block-worth of indirect pointers + 11 direct pointers
        if (index > (directSize + indirectSize - 1) 
            || index < 0)
        {
            return -1;
        }

        if (index < directSize)
        {
            ret = direct[index];
        }
        else
        {
            // Read the indirect pointer block for the desired pointer.
            // Note that directSize is subtracted from index, since we are 
            // indexing into a new array.
            // $PERF: We could keep a copy of the indirect pointer array
            // in memory to avoid this disk read.
            byte b[] = new byte[Disk.blockSize];
            SysLib.rawread(indirect, b);
            ret = SysLib.bytes2short(b, (index - directSize)
                * SysLib.CBSHORT);
        }

        return ret;
    }

    /**
     * Calculate the number of total blocks required by cb bytes
     * 
     * @param cb Count of bytes
     * @return # of total required blocks on success; -1 on failure
     */
    private int calcBlocks(int cb)
    {
        // To store cb bytes, we require 1 block per blockSize bytes,
        // 1 block for any overflow, and 1 block for a set of indirect 
        // pointers, if the file is big enough.
        return ((cb / Disk.blockSize) + 
            ((cb % Disk.blockSize != 0) ? 1 : 0) + 
            (((cb / directSize > 0) && indirect < 0) ? 1 : 0));
    }

    /**
     * Reserve enough blocks to store cb bytes of data
     * 
     * @param cb Count of bytes to reserve
     * @return 0 on success; -1 on failure
     */
    private int ensureCapacity(int cb)
    {
        int indirectOffset = 0;
        int requiredBlocks = calcBlocks(cb);
        byte indirectBuffer[] = null;
        boolean flushIndirectPtrBlock = false;

        // Check if we already have enough space reserved
        if (cb <= length)
        {
            return 0;
        }

        // Check for max filesize
        if (cb > maxFileSize)
        {
            return -1;        
        }

        // Allocate all the required blocks before assigning them to the 
        // Inode. If there are not enough, we should determine that before
        // modifying the Inode.
        boolean enoughDisk = true;
        // There is no need to allocate blocks we already own, just new ones.
        int newBlocks = requiredBlocks - calcBlocks(length);
        short committedBlocks[] = new short[newBlocks];

        // Initialize the committed block array
        for (int i = 0; i < newBlocks; i++)
        {
            committedBlocks[i] = -1;
        }

        // Allocate as many blocks as possible
        for (int i = 0; i < newBlocks; i++)
        {
            committedBlocks[i] = FileSystem.blockManager.allocateBlock();

            // Insufficient storage
            if (committedBlocks[i] < 0)
            {
                enoughDisk = false;
                break;
            }
        }

        // If insufficient storage, free committed blocks and return an error
        if (!enoughDisk)
        {
            for (int i = 0; i < newBlocks; i++)
            {
                if (committedBlocks[i] > -1)
                {
                    FileSystem.blockManager.freeBlock(committedBlocks[i]);
                }
            }
            
            return -1;
        }

        int newBlockCount = 0;

        // Assign committed blocks to the correct pointers
        for (int i = 0; i < requiredBlocks; i++)
        {
            // Assign a direct block
            if (i < directSize)
            {

                if (direct[i] < 0)
                {
                    direct[i] = committedBlocks[newBlockCount];
                    newBlockCount++;
                }
            }
            // Assign an indirect block
            else
            {
                // Assign and initialize a block of indirect pointers,
                // if one does not already exist.
                if (indirect < 0)
                {
                    indirect = committedBlocks[newBlockCount];
                    newBlockCount++;
                    // Increment the requiredBlocks counter for this
                    // special case.
                    i++;

                    // Initialize the pointer array to -1
                    byte b[] = new byte[Disk.blockSize];

                    SysLib.rawread(indirect, b);
        
                    for (int j = 0; j < indirectSize; j++)
                    {
                        SysLib.short2bytes((short)-1, b, j * SysLib.CBSHORT);
                    }

                    SysLib.rawwrite(indirect, b);
                }

                // Load the indirect pointer block
                if (indirectBuffer == null)
                {
                    indirectBuffer = new byte[Disk.blockSize];
                    SysLib.rawread(indirect, indirectBuffer);
                }

                // Check if the pointer at the required location already has a 
                // block
                if (SysLib.bytes2short(indirectBuffer, indirectOffset) < 0)
                {
                    // If not, allocate one and update the indirect block with 
                    // the new ptr
                    SysLib.short2bytes(committedBlocks[newBlockCount], 
                        indirectBuffer, indirectOffset);
                    newBlockCount++;

                    // Flush the pointer block only once, to minimize I/O
                    flushIndirectPtrBlock = true;
                }
                
                indirectOffset += SysLib.CBSHORT;
            }
        }

        // Only perform I/O on the indirect ptr block if necessary
        if (flushIndirectPtrBlock)
        {
            SysLib.rawwrite(indirect, indirectBuffer);
        }

        return 0;
    }

    /**
     * Load an iNode at the provided index
     * @return 0 on success; -1 on failure
     */
    public int load()
    {
        // Check for iNumber out of range
        if (iNumber < 0 || iNumber > (FileSystem.maxFiles - 1))
        {
            return -1;
        }

        // Load class members from disk
        byte data[] = new byte[Disk.blockSize];
        int offset;
        BlockAndOffset bao = iNumberToBlockAndOffset(iNumber);
        
        offset = bao.offset;
        
        // read the entire block and initialize member variables based
        // on an offset within the block.
        SysLib.rawread(bao.block, data);
        
        length = SysLib.bytes2int(data, offset);
        offset += SysLib.CBINT;
        
        count = SysLib.bytes2short(data, offset);
        offset += SysLib.CBSHORT;
        
        flags = SysLib.bytes2short(data, offset);
        offset += SysLib.CBSHORT;
        
        for (int i = 0; i < directSize; i++)
        {
            direct[i] = SysLib.bytes2short(data, offset);
            offset += SysLib.CBSHORT;
        }
        
        indirect = SysLib.bytes2short(data, offset);

        return 0;
    }

    /**
     * Save Inode to disk in the iNumber position
     * @return 0 on success; -1 on failure
     */
    public int flush() 
    {
        int ret = -1;

        // Check for iNumber out of range
        if (iNumber < 0)
        {
            return ret;
        }

        // Marshal the data
        byte data[] = new byte[Disk.blockSize];
        int offset;
        
        BlockAndOffset bao = iNumberToBlockAndOffset(iNumber);
        
        offset = bao.offset;
        
        SysLib.rawread(bao.block, data);
        
        SysLib.int2bytes(length, data, offset);
        offset += SysLib.CBINT;
        
        SysLib.short2bytes(count, data, offset);
        offset += SysLib.CBSHORT;
        
        SysLib.short2bytes(flags, data, offset);
        offset += SysLib.CBSHORT;
        
        for (int i = 0; i < directSize; i++)
        {
            SysLib.short2bytes(direct[i], data, offset);
            offset += SysLib.CBSHORT;
        }
        
        SysLib.short2bytes(indirect, data, offset);
        
        if (SysLib.rawwrite(bao.block, data) >= 0)
        {
            ret = 0;
        }

        return ret;
    }

    /**
     * Return the disk block and byte offset of the provided iNumber 
     * @return disk block and byte offset
     */
    private BlockAndOffset iNumberToBlockAndOffset(int inum)
    {
        BlockAndOffset ret = new BlockAndOffset();
        ret.block = ((inum * iNodeSize) / Disk.blockSize) + startingBlock;
        ret.offset = ((inum % (Disk.blockSize / iNodeSize)) * iNodeSize);
        
        return ret;
    }
    
    /**
     * Return the iNumber at the provided disk block and byte offset
     * @return iNumber
     */
    private int blockAndOffsetToiNumber(BlockAndOffset bao)
    {
        int ret = bao.block * (Disk.blockSize / iNodeSize) + 
            (bao.offset / iNodeSize); 
        
        return ret;
    }
    
    private class BlockAndOffset
    {
        public int block;
        public int offset;
    }
}

