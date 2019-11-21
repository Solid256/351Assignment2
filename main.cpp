#include <iostream>
#include <string>
#include <fstream>

#include "Process.h"
#include "MemoryManager.h"

// Reads the process list file and creates the process objects and stores them
// in a vector.
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
		unsigned int numOfProcesses = 0;

		// The current process id.
		unsigned int curPID = 0;

		// The time it takes for the program to arrive.
		unsigned int arrivalTime = 0;

		// How long it takes to complete the current process.
		unsigned int executionTime = 0;

		// The number of memory chunks for the current process.
		unsigned int numOfMemChunks = 0;

		// The value of the current memory chunk.
		unsigned int memChunkVal = 0;

		inFile >> numOfProcesses;

		for(unsigned int i = 0; i < numOfProcesses; i++)
		{
			// The current process being created.
			Process curProcess;

			// The descriptor for the current process being created.
			ProcessDesc desc;
			
			inFile >> curPID;
			inFile >> arrivalTime;
			inFile >> executionTime;
			inFile >> numOfMemChunks;
			
			// Create each individual memory chunk.
			for(unsigned int j = 0; j < numOfMemChunks; j++)
			{
				inFile >> memChunkVal;
				desc.mMemoryChunks.push_back(memChunkVal);
			}

			desc.mPID = curPID;
			desc.mArrivalTime = arrivalTime;
			desc.mExecutionTime = executionTime;

			curProcess.Init(desc);
			
			rProcessList.push_back(curProcess);
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
	MemoryManager memoryManager;

	// The list of processes being read from the in file.
	ProcessList processList;
	
	// Checks if an error has occured.
	bool success = true;

	// First, create the processes needed to be executed by the OS simulator.
	success = ReadProcessListFile(processList, "in1.txt");

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
		}while(!validInput);

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

		// The time elapsed in fake time units.
		int time = 0;
		
		// The memory manager descriptor.
		MemoryManagerDesc desc;

		desc.memorySize = memorySize;
		desc.pageSize = pageSize;
		desc.pTime = &time;

		// Initialize the memory manager.
		memoryManager.Init(desc);

		// TODO: Add processes to processing queue in a loop.

		// The main loop for the OS simulator.
		while(processList.size() > 0)
		{
			++time;
			processList.pop_back();
		}

		// TODO: Compute the Average Turnaround Time.
	}

	return 0;
}