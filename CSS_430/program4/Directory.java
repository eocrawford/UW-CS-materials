import java.util.*;
import java.lang.*;
import java.util.concurrent.*;

/**
 * Directory class
 * 
 * Description:
 * Stores a list of file names and the iNumbers that correspond to them.
 * Directory contents are stored as a file (within an Inode).
 *
 * Assumptions and limitations:
 * Only one directory path ("/") is supported
 * Maximum number of files is maximum number of Inodes
 * Directory max files cannot be changed
 * Deleted files still open for use take up one directory listing.
 * File names must <= maxChars; longer than this causes an error.
 * 
 * @author      Ethan Crawford
 * @date        Dec 4, 2008
 * @course      CSS 430
 * @professor   Sung
 * @version     0.01
 */

/**
* Directory constructor
*
* @param maxFiles Maximum number of files to be stored in this directory
*/
public class Directory 
{
    // Maximum file name length
    public static final int maxChars = 30;

    /**
     * 2-parameter constructor
     * 
     * @param inode Inode that stores directory info
     * @param mf maximum # of files that can be stored in the directory
     */
    public Directory(Inode in, int mf)
    {
        inode = in;
        maxFiles = mf;
        format(maxFiles);
    }

    /**
     * Retrieve the iNumber associated with the filename
     * @param fileName Name of file
     * @param fCreate Create a new file if one does not already exist
     * @return valid iNumber on success, -1 on failure
     */
    public synchronized int getiNumber(String fileName, boolean fCreate)
    {
        int iNumber = -1;
        int firstEmpty = -1;

        // Bad parameter
        if (fileName == null)
        {
            return -1;
        }

        // Return an error if the filename is too long
        if (fileName.length() > maxChars)
        {
            return -1;
        }

        // Must provide at least one character
        if (fileName.length() < 1)
        {
            return -1;
        }
        
        // Search the vector for the filename, 
        // keeping track of the first empty element
        for (int i = 0; i < files.size(); i++)
        {
            if (files.get(i).equals(fileName))
            {
                iNumber = i;
                break;
            }
            else if (files.get(i).equals("") && firstEmpty < 0)
            { 
                // Confirm that the Inode is not being used.
                // (Inodes with count > 0 that aren't in the
                // directory are probably marked for deletion)
                Inode inode = new Inode(i);

                if (inode.getCount() < 1)
                {
                    firstEmpty = i;
                }
            }
        }

        // If the file cannot be found, create a new one 
        // at the first empty element.
        if (iNumber < 0 && fCreate)
        {
            // Out of directory space
            if (firstEmpty < 0)
            {
                return -1;
            }
            
            // Add the listing
            iNumber = firstEmpty;
            files.removeElementAt(iNumber);
            files.add(iNumber, fileName);
            flush();
        }

        return iNumber;
    }

    /**
     * Deletes an entry from the directory listing
     * @param fileName The name of the file to be deleted
     * @return The iNumber of the deleted item on success, -1 on failure.
     */
    public synchronized int deleteEntry(String fileName)
    {
        int iNumber = -1;

        // Return an error if the filename is too long
        if (fileName.length() > maxChars)
        {
            return -1;
        }

        // Search the vector for the filename
        for (int i = 0; i < files.size(); i++)
        {
            if (files.get(i).equals(fileName))
            {
                // Remove it from the vector
                iNumber = i;
                files.add(iNumber, new String(""));
                files.removeElementAt(iNumber + 1);
                flush();
                break;
            }
        }

        return iNumber;
    }

    /**
     * Format the directory structure.
     * @param mf Maximum number of files that the directory can store
     * @return 0 on success, -1 on failure
     */
    public synchronized int format(int mf)
    {
        maxFiles = mf;
        // Add the string '/' to the String vector
        files.clear();
        files.add(new String("/"));

        // Add an empty string for 1 to maxFiles
        for (int i = 1; i < maxFiles; i++)
        {
            files.add(new String(""));
        }

        inode.flags |= FileSystem.ACCESS_READWRITE;

        return 0;
    }

    /**
     * Load the directory structure from disk
     * @return 0 on success, -1 on failure
     */
    public synchronized int load()
    {
        int cb = (maxFiles * maxChars * SysLib.CBCHAR) + 
            (maxFiles * SysLib.CBINT);
        byte b[] = new byte[cb];
        files.clear();
        int fileNameOffset = maxFiles * SysLib.CBINT;
        int offset = 0;

        // Read the entire potential directory listing
        if (inode.read(0, b) < 0)
        {
            return -1;
        }

        for (int i = 0; i < maxFiles; i++)
        {
            // Read the file name length
            int strLen = SysLib.bytes2int(b, offset);
            offset += SysLib.CBINT;

            // Read the file name, if one is stored
            if (strLen > 0)
            {
                byte strBytes[] = new byte[strLen * SysLib.CBCHAR];

                System.arraycopy(b, fileNameOffset, strBytes, 0,
                    strBytes.length);

                files.add(new String(strBytes));

                fileNameOffset += strLen;
            }
            // Add a zero-length placeholder for this listing
            else
            {
                files.add(new String(""));
            }
        }

        return 0;
    }

    /**
     * Flush the directory structure to disk
     * @return 0 on success, -1 on failure
     */
    public synchronized int flush()
    {
        int fileCount = files.size();
        int cb = (maxFiles * maxChars * SysLib.CBCHAR) + 
            (maxFiles * SysLib.CBINT);
        byte b[] = new byte[cb];
        int inodeOffset = 0;
        int fileNameOffset = fileCount * SysLib.CBINT;

        for (int i = 0; i < fileCount; i++)
        {
            byte strBytes[] = files.get(i).getBytes();
            int cbStr = strBytes.length;

            SysLib.int2bytes(cbStr, b, inodeOffset);
            inodeOffset += SysLib.CBINT;

            // Don't write zero-length strings
            if (strBytes.length > 0)
            {
                System.arraycopy(strBytes, 0, b, fileNameOffset, cbStr);

                fileNameOffset += cbStr;
            }
        }

        if (inode.write(0, b) < 0)
        {
            return -1;
        }

        return 0;
    }

    private Vector<String> files = new Vector<String>();
    private Inode inode;
    private int maxFiles;
}

