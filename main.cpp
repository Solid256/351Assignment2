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

int ReadProcessListFile(ProcessList& rProcessList, std::string fileName);

int ReadProcessListFile(ProcessList& rProcessList, std::string fileName)
{
	// Checks if an error has occured.
	bool success = true;


	// The number of processes being read.
	int numOfProcesses = -1;

	// The in file for the process list.
	std::ifstream inFile;

	// The current line being read.
	std::string curLine = "";

	std::cout << "Reading the process in-file.\n";

	inFile.open(fileName);

	if(inFile.is_open())
	{


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
		// std:: cout << "\nInitial numOfProcesses: " << numOfProcesses;

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

			int memSum = 0;

			// The final line of input per process has the number of memory chunks
			// followed by a sequence of numbers that need to be added together to get
			// the sum of the total amount of memory needed for that process

			// this will do the adding
			// Create each individual memory chunk.
			for(int j = 0; j < numOfMemChunks; j++)
			{

				inFile >> memChunkVal;
				memSum += memChunkVal;
				// The current memory chunk being created.
				MemoryChunk curMemoryChunk;

				curMemoryChunk.size = memChunkVal;

				desc.mMemoryChunks.push_back(curMemoryChunk);
			}


			// Assigning values to the new process
			desc.mPID = curPID;
			desc.mArrivalTime = arrivalTime;
			desc.mExecutionTime = executionTime;
			desc.mAmntPagesNeeded = numOfMemChunks;
			desc.mAmntMemNeeded = memSum;

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
			// std::cout << " Finished!\n";
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
	// std::cout<<"\nReturning numOfProcesses: " << numOfProcesses << "\n";
	return numOfProcesses;
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
	int success = true;

	// First, create the processes needed to be executed by the OS simulator.
	success = ReadProcessListFile(processList, "in1.txt");
	// std::cout << "\nprocessList after ReadProcessListFile: " << processList.size()
		// << "\n";
	int numOfProcesses = success;
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
		// std::cout << "\nprocessList after 1 " << processList.size();
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
		memory.Init(pageSize, memorySize, numOfProcesses);

		// The main loop for the OS simulator.
		//while there are processes in the Input Queue or Processes are still running
		int mainLoopCounter = 0;
		while(processList.size() > 0 ||
			memoryManager.GetNumProcessesRunning() > 0)
		{
			mainLoopCounter += 1;
			memoryManager.RunProcesses(InputQueue, processList, memory);

            // go through process list
			for (int i = 0; i < processList.size(); i++) {
				Process currentProcess = processList.at(i);
				int startingTime = currentProcess.GetArrivalTime();

			// if process startingTime is == time, then it is arriving
				if (time == startingTime) {
					// a new process has arrived
					int newProcessID = currentProcess.GetPID();
					std::cout << "\nt = " << time << ": " << "Process " << newProcessID
						<< " arrives\n";

					// add the current process to the InputQueue
					InputQueue.push_back(currentProcess);
					printInputQueue(InputQueue);
					if (memoryManager.AttemptAddProcess(currentProcess, memory)){
						std::cout << "\nProcess " << currentProcess.GetPID() <<
							" moved to memory";

							int removeThisID = currentProcess.GetPID();
							// std::cout << "\nRemoving process with ID: " << removeThisID << " from InputQueue\n";
							// std::cout << "\nInput Queue before: \n";
							// for (int i = 0; i < InputQueue.size(); i++) {
							// 	std::cout << InputQueue.at(i).GetPID() << " ";
							// }
							for (int i = 0; i < InputQueue.size(); i++) {
								if (InputQueue.at(i).GetPID() == removeThisID) {
									InputQueue.erase(InputQueue.begin() + i);
								}
							}
							// std::cout << "\nInput Queue after: \n";
							// for (int i = 0; i < InputQueue.size(); i++) {
							// 	std::cout << InputQueue.at(i).GetPID() << " ";
							// }
							memory.printMemoryMap();



					} else {
						std::cout << "\nProcess " << currentProcess.GetPID() <<
							" has to wait for memory to become free";
					}

				}

				// if process completes

			}
			++time;
			if (mainLoopCounter == 10000) {
				cout << "\nwent over 1000000 times\n";
				cout << "\nnum processes running " << memoryManager.GetNumProcessesRunning();
				cout << "\nnum of processes in process list " << processList.size();
				// exit(0);
			}
		}

	// TODO: Compute the Average Turnaround Time.
	float totalTime = 0;
	// std::cout << "\nProcess List size: " << processList.size();

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

void removeProcessFromQueue (int passedTime, ProcessList &passedList, int numProcs);
void removeProcessFromQueue (int passedTime, ProcessList &passedList, int numProcs) {
	for (int i = 0; i < numProcs; i++) {
		int timeInMem = passedTime - passedList.at(i).GetArrivalTime();
		if (timeInMem == passedList.at(i).GetExecutionTime()) {

		}
	}

}
