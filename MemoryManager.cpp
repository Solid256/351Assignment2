#include <iostream>
#include <vector>

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
	// Each process has a variable called Lifetime that keeps track of how
	// long it should run.
	// To simulate a process running, every time we go through a loop, we subtract
	// the amount of time remaining in that process' Lifetime
	// The current process being executed.
	Process* pCurProcess = nullptr;

	// Execute each running process by one time unit.
	for(unsigned int i = 0; i < mProcessesRunning.size(); i++)
	{
		// set the current process as the process at the current index of the
		// vector that contains all running processes
		pCurProcess = &mProcessesRunning[i];
		pCurProcess->DecrementExecutionTime();

		// Remove the process if it has no more time left.
		if(pCurProcess->GetExecutionTime() == 0)
		{
			std::cout << "Process " << pCurProcess->GetPID()
				<< " completed!\n";

			// the process is no longer running so we need to remove it from the
			// running processes vector
			mProcessesRunning.erase(mProcessesRunning.begin() + i);

			// now that we removed the process, the next process that was at the next
			// index moved up one spot, so we need to make i go back
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

	// std::cout << "We are in AttemptAddProcess(Process& rProcess). I am printing rProcess...\n";
	// std::cout << "\nmProcessesRunning size: " << mProcessesRunning.size() << std::endl;
	// for (int i = 0; i < mProcessesRunning.size(); i++) {
	// 	std::cout << "\nPrinting process " << i << " out of " << mProcessesRunning.size();
	// 	mProcessesRunning.at(i).printProcess();
	// }



	// The memory chu nks of the current process.
	MemoryChunks* pMemoryChunks = rProcess.GetMemoryChunksPtr();
	// std::cout << "pMemoryChunks->size() " << pMemoryChunks->size() << std::endl;

	//vector that will hold upper and lower bounds
	// in the form upper, lower, upper, lower, etc...
	std::vector<int> bounds;


	// iterate through each process that is running
	for (int i = 0; i < mProcessesRunning.size(); i++) {

		// std::cout << "\n\nIterating through a new running process. Printing process: \n";
		Process currentProcess =  mProcessesRunning.at(i);
		// currentProcess.printProcess();

		// iterate through currentProcess' memory chunks vector
		MemoryChunks* curProcMemChunksVecPtr = currentProcess.GetMemoryChunksPtr();
		int memChunksSize = curProcMemChunksVecPtr->size();
		// std::cout << "\nmemChunksSize: " << memChunksSize << std::endl;
		for(int j = 0; j < memChunksSize; j ++) {
			// std::cout << "\nNow we are dealing with individual memory chunk";
			// we are now dealing with individual memory chunk
			MemoryChunk currentMemoryChunk = curProcMemChunksVecPtr->at(j);
			int lower = currentMemoryChunk.lowerBound;
			int upper = currentMemoryChunk.upperBound;
			// std::cout << "\nlower: " << lower << " upper: " << upper;
		}

	}

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


void Memory::Init(int pSize, int nFrames) {
	pageSize = pSize;
	numberOfFrames = nFrames;

	int numPages = numberOfFrames / pageSize;
	for (int i = 0; i < numPages; i++) {
		freeFrames.push_back(i);
	}
}

void Memory::printFreeFrames () {
	// iterate through the freeFrames vector
	int size = freeFrames.size();
	for (int i = 0; i < size; i++) {
		// multiply the number by size of page
		std::cout << i * pageSize << "-" << i + pageSize << ": Free frame(s)";
	}


}
