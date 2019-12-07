#include "Process.h"

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

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
	void AttemptAddProcess(Process& rProcess);

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
