#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Process.h"
#include "MemoryManager.h"

// std::vector<int> iq;
// Reads the process list file and creates the process objects and stores them
// in a vector.
void printInputQueue(std::vector<Process> IQ);
void printInputQueue(std::vector<Process> IQ) {
	int size = IQ.size();
	std::cout << "Input Queue: [ ";
	for (int i = 0; i < size; i++) {
		std::cout << IQ.at(i).GetPID() << " ";
	}
	std::cout << "]";
}

bool ReadProcessListFile(ProcessList& rProcessList, std::string fileName);

bool ReadProcessListFile(ProcessList& rProcessList, std::string fileName)
{
	// Checks if an error has occured.
	bool success = true;

	// The in file for the process list.
	std::ifstream inFile;

	// The current line being read.
	std::string curLine = "";

	std::cout << "Reading the process in-file.\n";

	inFile.open(fileName);

	if(inFile.is_open())
	{
		// The number of processes being read.
		int numOfProcesses = -1;

		// The current process id.
		int curPID = -1;

		// The time it takes for the program to arrive.
		int arrivalTime = -1;

		// How long it takes to complete the current process.
		int executionTime = -1;

		// The number of memory chunks for the current process.
		int numOfMemChunks = -1;

		// The value of the current memory chunk.
		int memChunkVal = -1;

		inFile >> numOfProcesses;

		for(int i = 0; i < numOfProcesses; i++)
		{
			// The current process being created.
			Process curProcess;

			// The descriptor for the current process being created.
			ProcessDesc desc;

			inFile >> curPID;
			inFile >> arrivalTime;
			inFile >> executionTime;
			inFile >> numOfMemChunks;

			// The final line of input per process has the number of memory chunks
			// followed by a sequence of numbers that need to be added together to get
			// the sum of the total amount of memory needed for that process

			// this will do the adding
			// Create each individual memory chunk.
			for(int j = 0; j < numOfMemChunks; j++)
			{

				inFile >> memChunkVal;

				// The current memory chunk being created.
				MemoryChunk curMemoryChunk;

				curMemoryChunk.size = memChunkVal;

				desc.mMemoryChunks.push_back(curMemoryChunk);
			}


			// Assigning values to the new process
			desc.mPID = curPID;
			desc.mArrivalTime = arrivalTime;
			desc.mExecutionTime = executionTime;

			// initialize the new process with the values we have assigned
			curProcess.Init(desc);

			//add the new process to the process list
			// rProcessList is a ProcessList that is passed into this function we
			// are in right now. Its address is passed so that when we modify it
			// in this function, the original ProcessList that was passed is changed
			// outside of this scope as well
			rProcessList.push_back(curProcess);
			// std::cout<< "rProcessList.size() " << rProcessList.size();
		}

		inFile.close();

		// Check if successful.
		if(success)
		{
			std::cout << " Finished!\n";
		}
		else
		{
			std::cout << "ERROR: The in-file is corrupt and "
			"cannot be read!\n";
		}
	}
	else
	{
		std::cout << "ERROR: in-file for processes could not be opened!.\n";
		success = false;
	}

	return success;
}

int main()
{
	// The memory manager for this OS simulation.
	// This is the main MemoryManager object that will be used throughout the
	// program
	MemoryManager memoryManager;

	std::vector<Process> InputQueue;

	// The list of processes being read from the in file.
	// ProcessList is a typdef of vector<Process>
	ProcessList processList;

	// Checks if an error has occured while reading the file.
	bool success = true;

	// First, create the processes needed to be executed by the OS simulator.
	success = ReadProcessListFile(processList, "in1.txt");
	// std::cout << "\nprocessList after ReadProcessListFile: " << processList.size()
		// << "\n";

	// If successful, continue.
	if(success)
	{
		// The maximum memory size of the OS.
		int memorySize;

		// The maximum page size of the OS.
		int pageSize;

		// The user input.
		int userInput;

		// Checks if the user input is valid.
		bool validInput = true;

		do
		{
			validInput = true;

			std::cout << "Please input the Memory Size: ";
			std::cin >> memorySize;

			if(memorySize < 1)
			{
				validInput = false;
				std::cout << "\nError! You must have a positive memory size!\n\n";
			}
		} while(!validInput);

		do
		{
			validInput = true;

			std::cout << "Page Size (1: 100, 2: 200, 3: 400): ";
			std::cin >> userInput;

			switch (userInput)
			{
				case 1:
					pageSize = 100;
					break;
				case 2:
					pageSize = 200;
					break;
				case 3:
					pageSize = 400;
					break;
				default:
					validInput = false;
					std::cout << "\nError! Please select a valid input!\n\n";
					break;
			}
		}while(!validInput);
		std::cout << "\nprocessList after 1 " << processList.size();
		// The time elapsed in fake time units.
		// this is our virtual clock
		int time = 0;

		// The memory manager descriptor.
		// This will be passed to the MemoryManager constructor.
		MemoryManagerDesc desc;

		desc.maxMemorySize = memorySize;
		desc.maxPageSize = pageSize;
		desc.pTime = &time;

		// Initialize the memory manager.
		memoryManager.Init(desc);
		Memory memory;
		memory.Init(pageSize, memorySize);
		std::cout << "\nprocessList after 2 " << processList.size();
// ----------------------------------------------------------------------
		// TODO A: For testing purposes only! Remove if not needed.
		// while(processList.size() > 0)
		// {
		// 	// A reference to the current process being
		// 	// added to the memory manager.
		// 	// the back() function returns the object at the end of the vector
		// 	Process& rProcess = processList.back();
		//
		// 	// remove this process from the processList
		// 	processList.pop_back();
		// 	memoryManager.AttemptAddProcess(rProcess);
		// }
// ----------------------------------------------------------------------

		// The main loop for the OS simulator.
		//while there are processes in the Input Queue or Processes are still running
		std::cout << "Process list size3452dd " << processList.size() << "\n";
		while(processList.size() > 0 ||
			memoryManager.GetNumProcessesRunning() > 0)
		{
			++time;
			memoryManager.RunProcesses();

            // go through process list
			for (int i = 0; i < processList.size(); i++) {
				Process currentProcess = processList.at(i);
				int startingTime = currentProcess.GetArrivalTime();

			// if process startingTime is == time, then it is arriving
				if (time == startingTime) {
					// a new process has arrived
					int newProcessID = currentProcess.GetPID();
					std::cout << "\nt = " << time << ": " << "Process " << newProcessID
						<< " arrives";

					// add the current process to the InputQueue
					InputQueue.push_back(currentProcess);
					printInputQueue(InputQueue);
					memoryManager.AttemptAddProcess(currentProcess);

				}
			}
		}

	// TODO: Compute the Average Turnaround Time.
	float totalTime = 0;
	std::cout << "\nProcess List size: " << processList.size();

	for (int i = 0; i < processList.size(); i++ ) {

		Process currentProcess = processList.at(i);
		// currentProcess.printProcess();
		totalTime += currentProcess.GetExecutionTime();

	}
	float averageTurnAroundTime = totalTime / processList.size();
	printf("Average Turn Around Time: %2.2f\n", averageTurnAroundTime);


	}
	else {
		fprintf(stderr, "File was not processed correctly");
	}

	return 0;
}

// class MemoryMap{
//
// 	int sizeOfAPage = -1;
// 	int totalPossibleFrames = -1;
// 	int numberOfPages = -1;
//
// 	//will look something like 0, 400, 800... if the sizeOfAPage is 400 for ex
// 	//the number will indicate the starting point of open page
// 	int allPages [numberOfPages];
// 	vector<int> pagesThatAreTaken;
//
//
//
// }
