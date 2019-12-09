#include "Process.h"

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
using namespace std;

struct takenFrame {
	int processNumber;
	int pageNumberForProcess;
	int lowerBound;
	int upperBound;
};


class Memory {

public:

	void Init(int pSize, int nFrames, int numProcs);
	int pageSize;
	int numberOfFrames;
	std::vector<int> freeFrames;
	std::vector<takenFrame> takenFrames;

	void printFreeFrames();
	void printMemoryMap();
	void printTakenFrames();
	bool MemoryAvailable(Process processPassed, int amountNeeded, std::vector<int> &passedVector);
	bool freeUpSpace(std::vector<int> indexVects);
	//array that will have as many spots as there are pages in memory
	// each index indicates a page
	// each value in the array is the PID of the process that takes up that page
	// int pagesFilledWithProcesses [numberOfFrames / pageSize];
	std::vector<int> pagesFilledWithProcesses;
	void updatePFWP (std::vector <int> passedVector);
	std::vector<int> getEmptyIndexesOfPFWP (std::vector<int> passed);
};


struct MemoryManagerDesc
{
	// The maximum memory size.
	int maxMemorySize = -1;

	// The maximum page size.
	int maxPageSize = -1;

	// A pointer reference to the main timer.
	int* pTime = nullptr;
};

class MemoryManager
{
	public:
	MemoryManager();

	// Initializes the memory manager.
	void Init(MemoryManagerDesc& rDesc);

	// Runs each process in memory for one time unit.
	void RunProcesses(std::vector<Process> &InputQueuePassed, std::vector <Process> &processList,
	Memory & passedMemory, int passedTime, vector<Process> &passedFinishedV);

	// Attempts to add a process to the memory
	// manager. If there isn't enough room, the
	// process will not be added.
	bool AttemptAddProcess(Process& rProcess, Memory & mem);


	// Getters:
	unsigned int GetNumProcessesRunning();

	ProcessList mProcessesRunning;

	// Variables:
	private:
	// The maximum memory size.
	int mMaxMemorySize;

	// The maximum page size.
	int mMaxPageSize;

	// The list of processes running in memory.
	// ProcessList is a typdef for vector<Process>


	// A pointer to the main timer.
	int* mpTime;
};





#endif
