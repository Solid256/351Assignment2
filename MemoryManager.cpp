#include <iostream>

#include "MemoryManager.h"

MemoryManager::MemoryManager() :
	mMaxMemorySize(-1),
	mMaxPageSize(-1),
	mpTime(nullptr)
{

}

void MemoryManager::Init(MemoryManagerDesc& rDesc)
{
	mMaxMemorySize = rDesc.maxMemorySize;
	mMaxPageSize = rDesc.maxPageSize;
	mpTime = rDesc.pTime;
}

void MemoryManager::RunProcesses()
{
	// The current process being executed.
	Process* pCurProcess = nullptr;

	// Execute each running process by one time unit.
	for(unsigned int i = 0; i < mProcessesRunning.size(); i++)
	{
		pCurProcess = &mProcessesRunning[i];
		pCurProcess->DecrementExecutionTime();

		// Remove the process if it has no more time left.
		if(pCurProcess->GetExecutionTime() == 0)
		{
			std::cout << "Process " << pCurProcess->GetPID() 
				<< " completed!\n";

			mProcessesRunning.erase(mProcessesRunning.begin() + i);
			--i;
		}
	}
}

void MemoryManager::AttemptAddProcess(Process& rProcess)
{
	// TODO A: Check if it can fit into memory first. Use the upper and lower 
	// bounds of the memory chunks in each process to determine where and if 
	// the new processes and its memory chunks can fit.

	int curLowerBound = -1;
	int curUpperBound = -1;
	bool canFit = true;

	// The memory chunks of the current process.
	MemoryChunks* pMemoryChunks = rProcess.GetMemoryChunksPtr();

    
    
	if(canFit)
	{
		// Add the process to the processes running list.
		mProcessesRunning.push_back(rProcess);

		// TODO A: Display the memory map just like in the examples.
	}
}

// Getters:
unsigned int MemoryManager::GetNumProcessesRunning()
{
	return mProcessesRunning.size();
}
