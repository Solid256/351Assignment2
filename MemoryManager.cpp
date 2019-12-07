#include <iostream>
#include <vector>

#include "MemoryManager.h"
using namespace std;

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

bool Memory::MemoryAvailable(int amountNeeded, std::vector<int> &passedV) {

	// using this arithmetic to round up to nearest integer
	int pagesNeeded = (amountNeeded + pageSize - 1) / pageSize;
	std::cout << "\nNeed " << pagesNeeded << " pages \n";
	//if the amount needed less than a page size
		// find first available pageSize

		//We are looking for n consecutive pages
		// so we make an array that can hold n values
	std::vector <int> pagesThatWouldWork;
	bool foundEnoughPages = false;
	// we are going to iterate through the vector holding every page
	// remember, in pagesFilledWithProcesses, if a value is != 0, it is not free
	for (int i = 0; i < pagesFilledWithProcesses.size(); i++) {

		// we are looking at a page. at index i
		// if the page we are looking at is open, we need to go to the next one
		// then the next one and so on until we have as many as we need
		if (pagesFilledWithProcesses.at(i) == 0) {
			for (int j = 0; j < pagesNeeded; j++) {
				if (pagesFilledWithProcesses[i+j] != 0) break;
				pagesThatWouldWork.at(j) = pagesFilledWithProcesses[i+j];
				if (j == pagesNeeded - 1) {
					foundEnoughPages = true;
				}
			}
		}
	}

	return foundEnoughPages;
	// else
		// divide amountNeeded / page size
		// that his how many consecutive pages we needs
		// i.e. we need that many vector indexes in a row
		;
}

void MemoryManager::AttemptAddProcess(Process& rProcess, Memory & mem)
{
	// TODO A: Check if it can fit into memory first. Use the upper and lower
	// bounds of the memory chunks in each process to determine where and if
	// the new processes and its memory chunks can fit.

	// int curLowerBound = -1;
	// int curUpperBound = -1;
	bool canFit;

	// std::cout << "We are in AttemptAddProcess(Process& rProcess). I am printing rProcess...\n";
	// std::cout << "\nmProcessesRunning size: " << mProcessesRunning.size() << std::endl;
	// for (int i = 0; i < mProcessesRunning.size(); i++) {
	// 	std::cout << "\nPrinting process " << i << " out of " << mProcessesRunning.size();
	// 	mProcessesRunning.at(i).printProcess();
	// }



	// The memory chu nks of the current process.
	MemoryChunks* pMemoryChunks = rProcess.GetMemoryChunksPtr();
	// std::cout << "pMemoryChunks->size() " << pMemoryChunks->size() << std::endl;

	int amountOfMemoryCurrentProcessNeeds = 0;
	std::cout << "we are going to print the pmemorychunks\n" << "test size : " << pMemoryChunks->size() << std::endl;
	for (int i = 0; i < pMemoryChunks->size(); i++) {
		std::cout << "\n we are looking at memory chunk at index " << i << "\n";
		std:: cout << "Memory chunk size: " << pMemoryChunks->at(i).size << "\n";
		amountOfMemoryCurrentProcessNeeds += pMemoryChunks->at(i).size;
	}
	std::cout << "\nThis process needs " << amountOfMemoryCurrentProcessNeeds <<
		" frames of Memory\n";

	// Now we look in the memory to see if there are 500 frames available
	Memory * m = &mem;

	m->printFreeFrames();

	std::vector<int> checkVector;

	canFit = m->MemoryAvailable(amountOfMemoryCurrentProcessNeeds, checkVector);

	std::cout << "Canfit = " << canFit;
	exit(0);


	// // iterate through each process that is running
	// for (int i = 0; i < mProcessesRunning.size(); i++) {
	//
	// 	// std::cout << "\n\nIterating through a new running process. Printing process: \n";
	// 	Process currentProcess =  mProcessesRunning.at(i);
	// 	// currentProcess.printProcess();
	//
	// 	// iterate through currentProcess' memory chunks vector
	// 	MemoryChunks* curProcMemChunksVecPtr = currentProcess.GetMemoryChunksPtr();
	// 	int memChunksSize = curProcMemChunksVecPtr->size();
	// 	// std::cout << "\nmemChunksSize: " << memChunksSize << std::endl;
	// 	for(int j = 0; j < memChunksSize; j ++) {
	// 		// std::cout << "\nNow we are dealing with individual memory chunk";
	// 		// we are now dealing with individual memory chunk
	// 		MemoryChunk currentMemoryChunk = curProcMemChunksVecPtr->at(j);
	// 		int lower = currentMemoryChunk.lowerBound;
	// 		int upper = currentMemoryChunk.upperBound;
	// 		// std::cout << "\nlower: " << lower << " upper: " << upper;
	// 	}

	// }

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

	// set every value of pagesFilledWithProcesses to zero to start with
	for (int i = 0; i < pagesFilledWithProcesses.size(); i++) {
		pagesFilledWithProcesses[i] = 0;
	}
}

void Memory::printFreeFrames () {
	// iterate through the freeFrames vector
	int size = freeFrames.size();
	for (int i = 0; i < size; i++) {
		// multiply the number by size of page
		std::cout << std::endl << i * pageSize << "-" << i*pageSize + pageSize - 1 << ": Free frame(s)";
	}

}
