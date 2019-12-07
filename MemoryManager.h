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
	bool MemoryAvailable(int amountNeeded);

	//array that will have as many spots as there are pages in memory
	// each index indicates a page
	// each value in the array is the PID of the process that takes up that page
	// int pagesFilledWithProcesses [numberOfFrames / pageSize];
	std::vector<int> pagesFilledWithProcesses;
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
	void RunProcesses();

	// Attempts to add a process to the memory
	// manager. If there isn't enough room, the
	// process will not be added.
	void AttemptAddProcess(Process& rProcess, Memory & mem);


	// Getters:
	unsigned int GetNumProcessesRunning();

	// Variables:
	private:
	// The maximum memory size.
	int mMaxMemorySize;

	// The maximum page size.
	int mMaxPageSize;

	// The list of processes running in memory.
	// ProcessList is a typdef for vector<Process>
	ProcessList mProcessesRunning;

	// A pointer to the main timer.
	int* mpTime;
};





#endif
