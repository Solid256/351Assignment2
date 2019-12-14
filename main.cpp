#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Process.h"
#include "MemoryManager.h"


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

vector<Process> finishedProcesses;

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
			// are in right now.
			rProcessList.push_back(curProcess);
		}

		inFile.close();

		// Check if successful.
		if(!success)
		{
			std::cout << "ERROR: The in-file is corrupt and "
			"cannot be read!\n";
		} else {}
	}
	else
	{
		std::cout << "ERROR: in-file for processes could not be opened!.\n";
		success = false;
	}
	return numOfProcesses;
}

int main()
{
	// This is the main MemoryManager object that will be used throughout the
	// program
	MemoryManager memoryManager;

	std::vector<Process> InputQueue;

	// The list of processes being read from the in file.
	ProcessList processList;

	// Checks if an error has occured while reading the file.
	int success = true;

	// First, create the processes needed to be executed by the OS simulator.
	success = ReadProcessListFile(processList, "in1.txt");

	int numOfProcesses = success;
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
		// The time elapsed in fake time units.
		// this is our virtual clock
		pageSize = 400;
		memorySize = 2000;

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
		int checkProcessListSize = processList.size();
		int numProcessesRunningCheck = memoryManager.GetNumProcessesRunning();
		while(checkProcessListSize > 0 ||
			numProcessesRunningCheck > 0)
		{
			mainLoopCounter += 1;
			memoryManager.RunProcesses(InputQueue, processList, memory, time, finishedProcesses);

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

							// Remove from the Input Queue the process that was added
							// to memory
							for (int i = 0; i < InputQueue.size(); i++) {
								if (InputQueue.at(i).GetPID() == removeThisID) {
									InputQueue.erase(InputQueue.begin() + i);
								}
							}

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
				// cout << "\nmProcessrunning " << memoryManager.mProcessesRunning.size();
				// for (int jkl = 0; jkl < memoryManager.GetNumProcessesRunning(); jkl++) {
				// 	cout << "\nProcess running ID: " << memoryManager.mProcessesRunning.at(jkl).GetPID();
				// }
				// for (int i = 0; i < processList.size(); i++) {
				// 	// cout << "\nProcess list ID, timeBeenRunning, execution time, current time " <<
				// 	// processList.at(i).GetPID() << " " << processList.at(i).timeBeenRunning
				// 	// << " " << processList.at(i).GetExecutionTime() << " " << time;
				// }
				//
				// cout << "\nwent over 10000 times\n";
				// cout << "\nnum of processes in process list " << processList.size();
				// cout << "\nnum processes running!!! " << memoryManager.GetNumProcessesRunning();

			}
		 checkProcessListSize = processList.size();
		 numProcessesRunningCheck = memoryManager.GetNumProcessesRunning();
		}

	float totalTime = 0;

	for (int i = 0; i < processList.size(); i++ ) {

		Process currentProcess = processList.at(i);
		totalTime += currentProcess.GetExecutionTime();

	}

	int sumOfTurnAroundTime = 0;
	for (int f = 0; f < finishedProcesses.size(); f++)
	{
		int arrivalTime = finishedProcesses.at(f).GetArrivalTime();
		int endTime = finishedProcesses.at(f).endTime;
		int turnAroundTime = endTime - arrivalTime;
		sumOfTurnAroundTime += turnAroundTime;
	}
	float averageTurnAroundTime = sumOfTurnAroundTime / finishedProcesses.size();
	cout << "\nAverage Turn Around Time: " << averageTurnAroundTime << endl;


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
