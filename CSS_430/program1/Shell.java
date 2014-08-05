/**
 * @author Ethan Crawford
 *
 */
public class Shell extends Thread
{
	/**
	 * null-parameter constructor. Invoked normally by java runtime
	 */
	public Shell()
	{
		this(null);
	}
	
	/**
	 * One-parameter constructor. Invoked by SysLib.exec().
	 * @param args
	 * Parameters passed by SysLib.exec()
	 */
	public Shell(String[] args)
	{
		run(args);
	}

	/**
	 * Implement shell functionality
	 * @param args
	 */
	public void run(String[] args)
	{
		while (true)
		{
			String[] tokens = null;
			int startIndex = -1;
			int argsLength = 0;
			boolean fWait = true;
			boolean fRun = false;

			SysLib.cout("shell[1]% ");
			
			// Interactive mode
			if (args == null)
			{
				// Read cin
				StringBuffer s = new StringBuffer();
				SysLib.cin(s);
				
				// Tokenize input
				tokens = SysLib.stringToArgs(s.toString());
			}
			// Batch mode - tokens passed in from SysLib.exec().
			else
			{
				tokens = new String[args.length];
				System.arraycopy(args, 0, tokens, 0, args.length);
				args = null;
			}

			if (tokens != null && tokens.length > 0)
			{
				// Special case - exit the shell if the string 'exit' is found in the first token
				if (tokens[0].equals("exit"))
				{
					break;
				}
				
				// Process the commandline
				for (int i = 0; i < tokens.length; i++)
				{
					// End of the array - have to run with what we have
					if (i == (tokens.length - 1))
					{
						fRun = true;
					}
					
					// Set concurrency flags
					if (tokens[i].equals(";"))
					{
						fWait = true;
						fRun = true;
					}
					else if (tokens[i].equals("&"))
					{
						fWait = false;
						fRun = true;
					}
					// Count contiguous tokens
					else if (!tokens[i].equals(null))
					{
						if (startIndex == -1)
						{
							startIndex = i;
						}
						
						argsLength++;
					}
	
					if (fRun && argsLength > 0)
					{
						// Copy params into new array
						String[] params = new String[argsLength];
						System.arraycopy(tokens, startIndex, params, 0, argsLength);
						
						// Run the provided command.
						int pid = SysLib.exec(params);
						
						if (-1 == pid)
						{
							SysLib.cerr("Error when running process " + params[0] + "\n");
							SysLib.exit();
						}
	
						// Wait for process exit, if so desired
						if (fWait && -1 != pid)
						{
							int joinedPid;
							// Wait for our process to exit
							do
							{
								joinedPid = SysLib.join();
							}
							while (joinedPid != pid && -1 != joinedPid);
							
							if (-1 == joinedPid)
							{
								SysLib.cerr("Could not wait on some process; received error -1\n");
								SysLib.exit();
							}
						}
						
						fRun = false;
						fWait = true;
						params = null;
						startIndex = -1;
						argsLength = 0;
					}
				}
			}
		}
		
        SysLib.exit();
	}
}
