//
//  lab10.cpp
//  Two-way communication using unnamed pipes
//
//	Code adapted from the dual unnamed pipes code seen in class.
//  Itself derived from from "Interprocess Communications in Unix - The Nooks & Crannies"
//						John Shapley Gray. Prentice Hall PTR, 1998.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

//--------------------------------------------------------------------------------
//	Custom Data Types
//--------------------------------------------------------------------------------

//	I use enums to code the different commands that our server can handle
enum CommandCode
{
	NO_CODE = -1,
	//
	ADD = 0,
	SUM,
	END
};

/**	An input line gets decoded into a command with possibly a value argument
 */
struct Command {

	/** To which child is the command addressed?
	 */
	int destIndex;

	/**	The code of the command (NO_CODE in case of error)
	 */
	CommandCode code;

	/** The message correspond to this command, to be sent on P2C pipe
	 */
	string message;
};

//--------------------------------------------------------------------------------
//	Function Prototypes
//--------------------------------------------------------------------------------

void childFunction(int pipeP2C[], int pipeC2P[]);
Command interpretCommandLine(void);

//--------------------------------------------------------------------------------
//	Global Variables
//--------------------------------------------------------------------------------
const int READ_END = 0;
const int WRITE_END = 1;
//
//const int NUM_PROCESSES = 6;

//--------------------------------------------------------------------------------
//	Function Implementations
//--------------------------------------------------------------------------------

int main(int argc, const char * argv[])
{	
	//--------------------------------------------------------------------------------
	//	Create the pipes to send data from the parent process to the child process
	//	and from the child to the parent.
	//--------------------------------------------------------------------------------
	int pipeP2C[2];
	int pipeC2P[2];	
	if (pipe(pipeP2C) == -1) 
	{
		cout << "Creation of P to C pipe failed" << endl << flush;
		exit(2);
	}
	if (pipe(pipeC2P) == -1) 
	{
		cout << "Creation of C to P pipe failed" << endl << flush;
		exit(3);
	}

	//--------------------------------------------------------------------------------
	//	After the fork, the pipes will be accessible by both the parent process and
	//	the child process.
	//--------------------------------------------------------------------------------	
	int p = fork();
	if (p < 0)
	{
		perror("Fork failed");
		exit(4);
	}
	// in the child
	if (p==0)		
	{
		childFunction(pipeP2C, pipeC2P);
	}
	
	//--------------------------------------------------------------------------------
	//	Only the parent gets here
	//--------------------------------------------------------------------------------
		
	//	not required but general good practice
	close(pipeP2C[READ_END]);
	close(pipeC2P[WRITE_END]);

	//	Keep processing commands by the user	
	bool keepGoing = true;
	while (keepGoing)	
	{	
		cout << "Enter a command (END to terminate): ";
		Command command = interpretCommandLine();

		//	Now I am ready to send the message and get back either a simple
		//	handshake or a result from the child
		switch (command.code)
		{
			// fall-through case
			case END:
				keepGoing = false;
			
			case ADD:
			case SUM:			
				if (write(pipeP2C[WRITE_END], command.message.c_str(), command.message.length()) != -1) 
				{
					cout << "Message sent by parent: [" << command.message << "]" << endl << flush;
				}
				else 
				{
					cout << "Parent write failed" << endl << flush;
					exit(5);
				}
		
				//-------------
				//	Get message (handshake or value) back from the child
				//--------------
				//	clear before reading into message buffer
				char childMessage[BUFSIZ];
				memset(childMessage, 0, BUFSIZ);			
			
				if (read(pipeC2P[READ_END], childMessage, BUFSIZ) != -1) 
				{
					if (command.code == SUM)
					{
						int sumVal;
						sscanf(childMessage, "%d", &sumVal);
						cout << "Parent received sum value " << sumVal << 
								" from child process." << endl << flush;
					
					}
					else
					{
						cout << "Handshake message received by parent" << endl << flush;
					}
				}
				else 
				{
					cout << "Parent read failed" << endl << flush;
					exit(6);
				}	
				break;
				
			case NO_CODE:
				cout << "Invalid command" << endl;
				break;

		}
	}
	
	//	Wait for the child to finish too
	pid_t termProcess;
	int statusVal;
	termProcess = waitpid(-1, &statusVal, 0);
	cout << "Parent process got termination signal for child" << endl;
		
	//	I could be doing some checking on statusVal, but here we just quit
	cout << "Parent process terminating" << endl;
		
    return 0;
}

void childFunction(int pipeP2C[], int pipeC2P[])
{
	//	We need that to store the values that we receive from the parent process
	vector<int> valueList;
	int valSum = 0;
	
	//	not required but general good practice
	close(pipeP2C[WRITE_END]);
	close(pipeC2P[READ_END]);

	//	Keep receiving commands from the parent
	bool keepGoing = true;
	while (keepGoing)
	{
		//	make sure that we are dealing with a 0-filled message buffer
		char message[BUFSIZ];
		memset(message, 0, BUFSIZ);
		
		if (read(pipeP2C[READ_END], message, BUFSIZ) != -1) 
		{
			//	I (child process) can receive 3 commands:
			//		D <value>
			//		S
			//		END
			char command[8];
			sscanf(message, "%s", command);
			if (strcmp(command, "D") == 0)
			{
				int value;
				if (sscanf(message, "%s%d", command, &value) != 2)
				{
					cout << "\tInvalid D command sent to child (value missing)" << endl << flush;			
				}
				else
				{
					cout << "\tAdding command received by child for value " << value << 
							", returning OK handshake" << endl << flush;
					valueList.push_back(value);
					valSum += value;
				}
				strcpy(message, "OK");		
			}
			else if (strcmp(command, "S") == 0)
			{
				cout << "\tSum command received by child, will return " << valSum << endl << flush;
				sprintf(message, "%d", valSum);
			}
			else if (strcmp(command, "END") == 0)
			{
				cout << "\tTermination command received by Child " << endl << flush;
				strcpy(message, "OK");	
				keepGoing = false;	
			}
		}
		else 
		{
			cout << "\tChild Read failed" << endl << flush;
			exit(14);
		}
		
		if (strcmp(message, "END") == 0)
		{
			keepGoing = false;
		}
		
		//	The child sends a message back
		if (write(pipeC2P[WRITE_END], message, strlen(message)) != -1) 
		{
			cout << "\tMessage sent back by child" << endl << flush;
		}
		else 
		{
			cout << "\tChild handshake failed" << endl << flush;
			exit(15);
		}
	}
	
	cout << "\tChild terminating" << endl << flush;
	
	exit(0);
}

Command interpretCommandLine(void)
{
	string lineStr;
	getline(cin, lineStr);
	char message[BUFSIZ];
	CommandCode commandCode = NO_CODE;

	//	Really, in single-child version, I could simply send the entire lineStr
	//	(or, really, lineStr.c_str()) to the child process, but I want to 
	//	prepare for the multiple-child version
	//	I (parent process) can receive 3 commands:
	//		D <value>
	//		S
	//		END
	istringstream lineStream(lineStr);
	string codeStr;
	lineStream >> codeStr;
	if (codeStr == "D")
	{
		int value;
		lineStream >> value;
		//	producing command for the child process
		sprintf(message, "D %d", value);
		commandCode = ADD;
	}
	else if (codeStr == "S")
	{
		sprintf(message, "S");
		commandCode = SUM;
		
	}
	else if (codeStr == "END")
	{
		sprintf(message, "END");
		commandCode = END;
	}
	
	string messageStr(message);
	Command command = {0, commandCode, messageStr};
	
	return command;
}

