/**
 * MyTests class
 * 
 * Description:
 * Contains FileSystem tests
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
class MyTests extends Thread 
{
    int fd;
    final byte[] buf16 = new byte[16];
    final byte[] buf32 = new byte[32];
    final byte[] buf24 = new byte[24];
    final byte[] buf6656 = new byte[6656];
    final String testFileName   = new String("TestFile");
    final String maxLenFileName = new String("123456789012345678901234567890");
    final String longFileName   = new String("1234567890123456789012345678901");
    final String emptyFileName  = new String("");
    boolean allTestsPass = true;

    int size;

    public void run( ) 
    {
        SysLib.cout("MyTests.run(): entering\n");
        
        if (formatTests())
            SysLib.cout("Correct behavior of format......................1\n");
        if (openTests())
            SysLib.cout("Correct behavior of open........................1\n");
        if (readTests())
            SysLib.cout("Correct behavior of read........................1\n");
        if (writeTests())
            SysLib.cout("Correct behavior of write.......................1\n");
        if (seekTests())
            SysLib.cout("Correct behavior of seek........................1\n");
        if (closeTests())
            SysLib.cout("Correct behavior of close.......................1\n");
        if (fsizeTests())
            SysLib.cout("Correct behavior of fsize.......................1\n");
        if (deleteTests())
            SysLib.cout("Correct behavior of delete......................1\n");

        if (allTestsPass)
        {
            SysLib.cout("-------------------------------------------------\n");
            SysLib.cout("All Tests Pass..................................8\n");
        }
        
        SysLib.cout("MyTests.run(): exiting\n");
        SysLib.exit( );
    }

    private boolean formatTests()
    {
        if (format(33) &&
            reformat(47, 99) &&
            formatWithOpenFiles() &&
            invalidFormat())
        {
            return true;
        }

        SysLib.cout("Wrong behavior of format************************0\n");
        SysLib.cout("Format tests (wrong)\n");
        allTestsPass = false;
        return false;
    }

    private boolean openTests()
    {
        SysLib.cout("openTests\n");

        if (!(doOpenTest("Open for read (no file)", testFileName, "r", 
                false)  &&
            doOpenTest("Create for write", testFileName, "w", true)  &&
            doOpenTest("Open for read", testFileName, "r", true)  &&
            doOpenTest("Open for read-write", testFileName, "w+", true)  &&
            doOpenTest("Open for append", testFileName, "a", true)  &&
            doOpenTest("Open max fn", maxLenFileName, "w", true)  &&
            doOpenTest("Open invalid mode", testFileName, "Q", false) &&
            doOpenTest("Open upper-case mode", testFileName, "R", false) &&
            doOpenTest("Open null fn", null, "a", false) &&
            doOpenTest("Open null mode", testFileName, null, false) &&
            doOpenTest("Open null strings", null, null, false) &&
            doOpenTest("Open 0-length fn", emptyFileName, "a",  false) &&
            doOpenTest("Open > max fn", longFileName,  "r",  false) &&
            openMaxFiles() &&
            openConflictingModes()))
        {
            SysLib.cout("Wrong behavior of open**************************0\n");
            allTestsPass = false;
            return false;
        }
            
        SysLib.delete(testFileName);
        SysLib.delete(maxLenFileName);

        return true;
    }

    private boolean readTests()
    {
        SysLib.cout("readTests\n");

        if (! (roundTripTests() &&
            invalidReadTests()))               
        {
            SysLib.cout("Wrong behavior of read**************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean writeTests()
    {
        SysLib.cout("writeTests\n");
        if (! (roundTripTests() &&
            invalidWriteTests()))
        {
            SysLib.cout("Wrong behavior of write*************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean seekTests()
    {
        SysLib.cout("seekTests\n");

        if (!(
            doSeekTest("SEEK_SET positive", 38, 3, 1,  SysLib.SEEK_SET,
                1)   &&
            doSeekTest("SEEK_CUR positive", 43, 3, 4,  SysLib.SEEK_CUR,
                4)   &&
            doSeekTest("SEEK_END positive", 511, 3, 1,  SysLib.SEEK_END,
                511) &&
            doSeekTest("SEEK_SET zero", 300, 3, 0,  SysLib.SEEK_SET,
                0)   &&
            doSeekTest("SEEK_CUR zero", 1, 3, 0,  SysLib.SEEK_CUR,
                0)   &&
            doSeekTest("SEEK_END zero", 0, 3, 0,  SysLib.SEEK_END,
                0)   &&
            doSeekTest("SEEK_CUR negative", 38, 3, -1, SysLib.SEEK_CUR,
                0)   &&
            doSeekTest("SEEK_END negative", 38, 3, -1, SysLib.SEEK_END, 
                37)  &&
            doSeekTest("Seek past eof", 38, 3, 39, SysLib.SEEK_SET,
                 38) &&
            doSeekTest("Seek before eof", 38, 3, -39, SysLib.SEEK_CUR,
                 0)  &&
            doSeekTest("Reserved fd", 38, 0, 1, SysLib.SEEK_SET,
                 -1)  &&
            doSeekTest("Invalid fd", 38, -1, 1, SysLib.SEEK_SET,
                 -1) &&
            doSeekTest("SEEK_SET negative", 38, 3, -1, SysLib.SEEK_SET,
                 -1) &&
            doSeekTest("Invalid seek flag", 38, 3, 1, -1,
                 -1)))
        {
            SysLib.cout("Wrong behavior of seek**************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean closeTests()
    {
        SysLib.cout("closeTests\n");

        if (!(
            invalidCloseTests()
            ))
        {
            SysLib.cout("Wrong behavior of close*************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean fsizeTests()
    {
        SysLib.cout("fsizeTests\n");

        if (!(
            invalidFSizeTests()
            ))
        {
            SysLib.cout("Wrong behavior of fsize*************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean deleteTests()
    {
        SysLib.cout("deleteTests\n");

        if (!(
            invalidDeleteTests()
            ))
        {
            SysLib.cout("Wrong behavior of delete************************0\n");
            allTestsPass = false;
            return false;
        }
            
        return true;
    }

    private boolean roundTripTests()
    {
        SysLib.cout("roundTripTests\n");

        if (!(
            doRTTest("Null input", null, 23) && 
            doRTTest("Zero bytes", new byte[0], 17) && 
            doRTTest("One byte from 1-byte file", new byte[1], 1) && 
            doRTTest("One byte from 0-byte file", new byte[1], 0) && 
            doRTTest("511 bytes from 512 byte file", new byte[511], 512) && 
            doRTTest("512 bytes from 511 byte file", new byte[512], 511) && 
            doRTTest("513 bytes from 513 byte file", new byte[513], 513) && 
            doRTTest("513 bytes from 512 byte file", new byte[513], 512) && 
            doRTTest("512 * 12 bytes", new byte[512 * 12], 512 * 12 + 5) && 
            doRTTest("Inode.maxFileSize - 1 bytes", 
                new byte[Inode.maxFileSize - 1], 
                Inode.maxFileSize - 1) && 
            doRTTest("Inode.maxFileSize bytes", 
                new byte[Inode.maxFileSize], 
                Inode.maxFileSize) && 
            doRTTest("Inode.maxFileSize + 1 bytes", 
                new byte[Inode.maxFileSize + 1], 
                Inode.maxFileSize)))
        {
            SysLib.cout("Round-trip tests (wrong)\n");
            return false;
        }
            
        return true;
    }

    private boolean invalidDeleteTests()
    {
        int ret, fd, fd2;
        int cb = 512 * 18;
        byte file[] = new byte[cb];
        
        // Delete non-existent file
        ret = SysLib.delete("null");
        if (ret > -1)
        {
            SysLib.cout("Delete succeeded on non-existent file\n");
            return false;
        }
        
        for (int i = 0; i < cb; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }

        // Delete open file
        ret = SysLib.delete(testFileName);
        if (ret < 0)
        {
            SysLib.cout("Delete failed on open file\n");
            return false;
        }

        // Verify that the fd for the deleted file still works
        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write to deleted file\n");
            return false;
        }

        // Open a new file with the same name
        fd2 = SysLib.open(testFileName, "w");
        if (fd2 < 0)
        {
            SysLib.cout("Could not open a new file " + 
                "with the deleted filename\n");
            return false;
        }

        // fsize on a new file
        ret = SysLib.fsize(fd2);
        if (ret != 0)
        {
            SysLib.cout("Invalid fsize on new file\n");
            return false;
        }
        
        // fsize on deleted file
        ret = SysLib.fsize(fd);
        if (ret != (file.length * 2))
        {
            SysLib.cout("Invalid fsize on deleted file\n");
            return false;
        }
        
        // Close new file
        ret = SysLib.close(fd2);
        if (ret < 0)
        {
            SysLib.cout("Could not close new file\n");
            return false;
        }

        // Close deleted file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }

        // Delete new file
        ret = SysLib.delete(testFileName);
        if (ret < 0)
        {
            SysLib.cout("Could not delete new file\n");
            return false;
        }

        // Delete already deleted file
        ret = SysLib.delete(testFileName);
        if (ret > -1)
        {
            SysLib.cout("Deleted already deleted file\n");
            return false;
        }

        return true;
    }

    private boolean invalidFSizeTests()
    {
        int ret, fd;
        int cb = Inode.maxFileSize;
        byte file[] = new byte[cb];
        
        for (int i = 0; i < cb; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        // fsize on a new file
        ret = SysLib.fsize(fd);
        if (ret != 0)
        {
            SysLib.cout("Invalid fsize on new file\n");
            return false;
        }
        
        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }
        
        // fsize on a written file
        ret = SysLib.fsize(fd);
        if (ret != cb)
        {
            SysLib.cout("Invalid fsize on written file\n");
            return false;
        }
        
        ret = SysLib.write(fd, file);
        if (ret > -1)
        {
            SysLib.cout("Wrote more than maxFileSize to a file\n");
            return false;
        }
        
        // fsize after failed write
        ret = SysLib.fsize(fd);
        if (ret != cb)
        {
            SysLib.cout("fsize is bigger than actual filesize\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }

        // fsize on a closed fd
        ret = SysLib.fsize(fd);
        if (ret > -1)
        {
            SysLib.cout("fsize on a closed fd\n");
            return false;
        }

        // fsize on an invalid fd
        ret = SysLib.fsize(0);
        if (ret > -1)
        {
            SysLib.cout("fsize on a reserved fd\n");
            return false;
        }

        SysLib.delete(testFileName);
        
        return true;
    }

    private boolean invalidCloseTests()
    {
        int ret, fd;
        int cb = 512 * 12 - 1;
        byte file[] = new byte[cb];
        
        // Write file of cb
        for (int i = 0; i < cb; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }

        // fd is usable after closing
        ret = SysLib.write(fd, file);
        if (ret > -1)
        {
            SysLib.cout("File writable after closing\n");
            return false;
        }
        
        // Close an already closed file
        ret = SysLib.close(fd);
        if (ret > -1)
        {
            SysLib.cout("already closed fd succeeded in closing again\n");
            return false;
        }

        // Close invalid fd
        ret = SysLib.close(0);
        if (ret > -1)
        {
            SysLib.cout("Closed reserved fd\n");
            return false;
        }

        SysLib.delete(testFileName);

        return true;
    }

    private boolean invalidWriteTests()
    {
        SysLib.cout("invalidWriteTests\n");

        int ret, fd;
        int cb = 513;
        byte file[] = new byte[cb];
        
        // Write file of cb
        for (int i = 0; i < cb; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }
        
        // Open the file for read
        fd = SysLib.open(testFileName, "r");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read\n");
            return false;
        }

        // fd opened for read only
        ret = SysLib.write(fd, file);
        if (ret > -1)
        {
            SysLib.cout("Wrote to a read-only fd\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file\n");
            return false;
        }
        
        // Try to write to the closed fd
        ret = SysLib.write(fd, file);
        if (ret > -1)
        {
            SysLib.cout("Write to closed fd succeeded\n");
            return false;
        }

        // Try to write to reserved fd
        ret = SysLib.write(0, file);
        if (ret > -1)
        {
            SysLib.cout("Write to reserved fd\n");
            return false;
        }

        // Try to read from negative fd
        ret = SysLib.write(-1, file);
        if (ret > -1)
        {
            SysLib.cout("Write to negative fd\n");
            return false;
        }

        // Delete file
        ret = SysLib.delete(testFileName);
        if (ret < 0)
        {
            SysLib.cout("Error deleting file\n");
            return false;
        }

        return true;
    }

    private boolean doSeekTest(String testName, int fileLen, 
        int fd, int offset, int flag, int cbExpected)
    {
        SysLib.cout(testName + "\n");
        boolean failureExpected = ((flag == SysLib.SEEK_SET) && (offset < 0)) 
            || (flag < 0) || (fd < 3);

        int ret;
        int fd2;
        byte file[] = new byte[fileLen];
        
        // Write file of length fileLen
        for (int i = 0; i < fileLen; i++)
            file[i] = (byte)(i % 256);

        fd2 = SysLib.open(testFileName, "w");
        if (fd2 < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd2, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }

        // Close file
        ret = SysLib.close(fd2);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }
        
        if (fd > 2)
        {
            // Open the file for read/write
            fd = SysLib.open(testFileName, "w+");
            if (fd < 0)
            {
                SysLib.cout("Could not open " + testFileName + " for read\n");
                return false;
            }
        }

        ret = SysLib.seek(fd, offset, flag);
        if (ret != cbExpected)
        {
            SysLib.cout("Actual seek " + ret + 
            " did not match expected seek " + cbExpected + "\n");
            SysLib.close(fd);
            return false;
        }

        // Close file
        if (fd > 2)
        {
            ret = SysLib.close(fd);
            if (ret < 0)
            {
                SysLib.cout("Could not close file after seeking\n");
                return false;
            }
        }
        
        return true;
    }

    private boolean doRTTest(String testName, byte data[], int fileLen)
    {
        SysLib.cout(testName + "\n");

        int ret, fd;
        byte file[] = new byte[fileLen];
        int cbExpected;
        
        if (data != null)
        {
            cbExpected = ((data.length > fileLen) ? fileLen : data.length);
        }
        else
        {
            cbExpected = -1;
        }
        
        // Write file of fileLen bytes
        for (int i = 0; i < fileLen; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }

        // Verify file size
        ret = SysLib.fsize(fd);
        if (ret != file.length)
        {
            SysLib.cout("Incorrect filesize\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }
        
        // Open file for read
        fd = SysLib.open(testFileName, "r");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read\n");
            return false;
        }

        // Read data.length bytes
        ret = SysLib.read(fd, data);
        if (ret != cbExpected)
        {
            SysLib.cout("Expected read " + cbExpected  + 
                " does not match actual " + ret + "\n");
            return false;
        }

        // Verify seek return value
        if (data == null) 
            cbExpected = 0;

        ret = SysLib.seek(fd, 0, SysLib.SEEK_CUR);
        if (ret != cbExpected)
        {
            SysLib.cout("Expected seek pos " + cbExpected  + 
                " does not match actual " + ret + "\n");
                return false;
        }

        // Verify content
        if (data != null)
        {
            for (int i = 0; i < data.length; i++)
            {
                if (file.length <= i)
                    break;

                if (data[i] != file[i])
                {
                    SysLib.cout("Expected " + file[i]  + 
                        " does not match actual " + data[i] + "\n");
                    return false;
                }
            }
        }

        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after reading\n");
            return false;
        }
        
        // Delete file
        ret = SysLib.delete(testFileName);
        if (ret < 0)
        {
            SysLib.cout("Error deleting file\n");
            return false;
        }

        return true;
    }

    private boolean invalidReadTests()
    {
        SysLib.cout("invalidReadTests\n");

        int ret, fd;
        int cb = 13;
        byte file[] = new byte[cb];
        byte data[] = new byte[cb];
        
        // Write file of cb
        for (int i = 0; i < cb; i++)
            file[i] = (byte)(i % 256);

        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.write(fd, file);
        if (ret < 0)
        {
            SysLib.cout("Could not write file\n");
            return false;
        }
        
        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after writing\n");
            return false;
        }
        
        // Open the file for read
        fd = SysLib.open(testFileName, "r");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read\n");
            return false;
        }

        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file\n");
            return false;
        }
        
        // Try to read from the closed fd
        ret = SysLib.read(fd, data);
        if (ret > -1)
        {
            SysLib.cout("Read from closed fd succeeded\n");
            return false;
        }

        // Open file for write
        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        // Try to read from the fd
        ret = SysLib.read(fd, data);
        if (ret > -1)
        {
            SysLib.cout("Read from fd open for write\n");
            return false;
        }

        // Close file
        ret = SysLib.close(fd);
        if (ret < 0)
        {
            SysLib.cout("Could not close file after reading\n");
            return false;
        }
        
        // Try to read from reserved fd
        ret = SysLib.read(0, data);
        if (ret > -1)
        {
            SysLib.cout("Read from reserved fd\n");
            return false;
        }

        // Try to read from negative fd
        ret = SysLib.read(-1, data);
        if (ret > -1)
        {
            SysLib.cout("Read from negative fd\n");
            return false;
        }

        // Delete file
        ret = SysLib.delete(testFileName);
        if (ret < 0)
        {
            SysLib.cout("Error deleting file\n");
            return false;
        }

        return true;
    }

    private boolean doSeekTests()
    {
        // Seek < 0 and > inode.length before reading
        return false;
    }
    
    private boolean format(int maxFiles)
    {
        SysLib.cout("format\n");
        SysLib.format(maxFiles);
        return verifySBElements(maxFiles);
    }

    private boolean reformat(int firstMax, int nextMax)
    {
        SysLib.cout("reformat\n");
        SysLib.format(firstMax);
        SysLib.format(nextMax);
        return verifySBElements(nextMax);
    }

    private boolean formatWithOpenFiles()
    {
        SysLib.cout("formatWithOpenFiles\n");
        int ret, fd;
        
        fd = SysLib.open(testFileName, "w");
        if (fd < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for write\n");
            return false;
        }

        ret = SysLib.format(48);
        if (ret > -1)
        {
            SysLib.cout("Format succeeded with open files\n");
            SysLib.close(fd);
            SysLib.delete(testFileName);
            return false;
        }
        
        SysLib.close(fd);
        SysLib.delete(testFileName);
        return true;
    }

    private boolean invalidFormat()
    {
        SysLib.cout("invalidFormat\n");
        int ret = SysLib.format(0);
        
        if (ret > -1)
        {
            SysLib.cout("Format succeeded with 0 files\n");
            return false;
        }

        ret = SysLib.format(-1);
        if (ret > -1)
        {
            SysLib.cout("Format succeeded with -1 files\n");
            return false;
        }

        ret = SysLib.format(Inode.iNodeSize * Disk.blockSize * 1000);
        if (ret > -1)
        {
            SysLib.cout("Format succeeded with too many files\n");
            return false;
        }

        return true;
    }

    private boolean verifySBElements(int formatFiles)
    {
        SysLib.cout( "verifySBElements\n" );
        byte[] superblock = new byte[Disk.blockSize];
        SysLib.rawread( 0, superblock );
        int offset = 0;

        int totalBlocks = SysLib.bytes2int(superblock, offset);
        offset += SysLib.CBINT;
        int inodeBlocks = SysLib.bytes2int(superblock, offset);
        offset += SysLib.CBINT;
        int freeList = SysLib.bytes2int(superblock, offset);
        
        if ( totalBlocks != 1000 )
        {
            SysLib.cout( "totalBlocks = " + totalBlocks + " (wrong)\n" );
            return false;
        }

        if ( inodeBlocks != formatFiles && 
            inodeBlocks != formatFiles / 16 )
        {
            SysLib.cout( "expected inodeBlocks " + formatFiles + 
                " != actual inodeBlocks " + inodeBlocks + " (wrong)\n" );
            return false;
        }

        if ( freeList != 1 + formatFiles / 16 && 
            freeList != 1 + formatFiles / 16 + 1 )
        {
            SysLib.cout( "freeList = " + freeList + " (wrong)\n" );
            return false;
        }
        
        return true;
    }

    private boolean doOpenTest(String testName, String fileName, 
        String mode, boolean successExpected)
    {
        int ret;
        SysLib.cout(testName + "\n");

        ret = SysLib.open(fileName, mode);
        if (ret < 0)
        {
            if (successExpected)
            {
                SysLib.cout("Could not open " + fileName + " for " + mode + "\n");
                return false;
            }

        }
        else
        {
            if (!successExpected)
            {
                SysLib.cout("Opened a file when failure was expected\n");
                SysLib.close(ret);
                return false;
            }

            SysLib.close(ret);
        }

        return true;
    }

    private boolean openConflictingModes()
    {
        SysLib.cout("openConflictingModes\n");

        int ret2;
        int ret = SysLib.open(testFileName, "r");
        if (ret < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read\n");
            return false;
        }
        
        ret2 = SysLib.open(testFileName, "w");
        if (ret2 > -1)
        {
            SysLib.cout("Opened " + testFileName + "for write when it " + 
                "was already opened for read\n");
            SysLib.close(ret2);
            return false;
        }

        SysLib.close(ret);
        ret = SysLib.open(testFileName, "w+");
        if (ret < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read/write\n");
            return false;
        }

        ret2 = SysLib.open(testFileName, "w");
        if (ret2 > -1)
        {
            SysLib.cout("Opened " + testFileName + "for write when it " + 
                "was already opened for write\n");
            SysLib.close(ret2);
            return false;
        }

        SysLib.close(ret);
        ret = SysLib.open(testFileName, "r");
        if (ret < 0)
        {
            SysLib.cout("Could not open " + testFileName + " for read\n");
            return false;
        }

        ret2 = SysLib.open(testFileName, "r");
        if (ret2 < 0)
        {
            SysLib.cout("Could not open " + testFileName + 
            " for read (again)\n");
            SysLib.close(ret);
            return false;
        }

        if (ret == ret2)
        {
            SysLib.cout("fds are identical and should be different.\n");
            return false;
        }

        SysLib.close(ret);
        SysLib.close(ret2);

        return true;
    }

    private boolean openMaxFiles()
    {
        SysLib.cout("openMaxFiles\n");
        boolean ret = true;
        int fd;
        final int maxFiles = 29;
        int i = 0;

        for (; i < maxFiles; i++)
        {
            fd = SysLib.open(testFileName + new Integer(i).toString(), "w");
            if (fd < 0)
            {
                SysLib.cout("Couldn't open max files\n");
                ret = false;
                break;
            }
        }

        if (ret)
        {
            String fn = testFileName + new Integer(++i).toString();
            fd = SysLib.open(fn, "w");
            if (fd > -1)
            {
                SysLib.cout("Opened more files than expected\n");
                SysLib.close(fd);
                SysLib.delete(fn);
                ret = false;
            }
        }

        for (i = 0; i < maxFiles; i++)
        {
            SysLib.close(i + 3);
            SysLib.delete(testFileName + new Integer(i).toString());
        }

        return ret;
    }
}

