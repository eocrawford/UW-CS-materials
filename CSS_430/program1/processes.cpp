//---------------------------------------------------------------------------
// processes.cpp
// Demonstration of Linix system commands for working with processes
// Author(s): Ethan Crawford
// Date: September 29, 2008
//---------------------------------------------------------------------------
// Implements processes functionality using the following system calls:
//     pid_t fork( void ); creates a child process that differs from the 
//     parent process only in terms of their process IDs.
//
//       A parent waits for a child
//       A child executes "wc -l"
//       A grand-child executes "grep argv[1]"
//       A great-grand-child executes "ps -A"
//       The same output as ps -A | grep argv[1] | wc -l
//---------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define STDIN         0
#define STDOUT        1

int main(int argc, char** argv)
{
	// Check parameters
	if (!argv[1])
	{
		cerr << "Please provide a process to search for" << endl;
		exit(1);
	}

	// Commands to be executed
	const char* cmd1[] = {"ps", "-A", (char*)0};
	const char* cmd2[] = {"grep", argv[1], (char*)0};
	const char* cmd3[] = {"wc", "-l", (char*)0};

	int fdPipe1[2];
	int fdPipe2[2];

	// Create a pipe to capture filtered output
	if (pipe(fdPipe1) < 0)
	{
		perror("Pipe error");
		exit(1);
	}

	pid_t pid1 = fork();

	if (pid1 < 0)
	{
		perror("Failed to fork");
		exit(1);
	}

	// Child process - filter the process list using grep
	if (0 == pid1)
	{
		// Redirect STDOUT through pipe1
		dup2(fdPipe1[1], STDOUT);
		close(fdPipe1[0]);

		// Create a pipe to be used in passing data from ps to grep
		if (pipe(fdPipe2) < 0)
		{
			perror("Pipe error");
			exit(1);
		}

		pid_t pid2 = fork();

		if (pid2 < 0)
		{
			perror("Failed to fork");
			exit(1);
		}

		// Great-grandchild: ps -A
		if (0 == pid2)
		{
			// Redirect STDOUT through pipe2
			dup2(fdPipe2[1], STDOUT);
			close(fdPipe2[0]);
			execlp(cmd1[0], cmd1[0], cmd1[1], cmd1[2]);
			perror("execlp");
			exit(1);
		}
		// Grandchild: grep argv[1]
		else
		{
			wait(NULL);
			// Read input from pipe2
			dup2(fdPipe2[0], STDIN);
			close(fdPipe2[1]);
			execlp(cmd2[0], cmd2[0], cmd2[1], cmd2[2]);
			perror("execlp");
			exit(1);
		}
	}
	// Child: wc -l
	else
	{
		wait(NULL);
		// Read input from pipe1, which now contains the filtered output 
        // from grep argv[1]
		dup2(fdPipe1[0], STDIN);
		close(fdPipe1[1]);
		execlp(cmd3[0], cmd3[0], cmd3[1], cmd3[2]);
		perror("execlp");
		exit(1);
	}

	return 0;
}
