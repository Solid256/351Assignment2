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

void MemoryManager::RunProcesses(std::vector<Process> &InputQueuePassed)
{

	// std::cout<< "\nEntering RunProcesses";
	// Each process has a variable called Lifetime that keeps track of how
	// long it should run.
	// To simulate a process running, every time we go through a loop, we subtract
	// the amount of time remaining in that process' Lifetime
	// The current process being executed.
	Process* pCurProcess = nullptr;

	// Execute each running process by one time unit.
	for(unsigned int i = 0; i < mProcessesRunning.size(); i++)
	{
		// std::cout<< "\nTrip number " << i << " through mProcessesRunning loop";
		// set the current process as the process at the current index of the
		// vector that contains all running processes

		pCurProcess = &mProcessesRunning[i];
		// std::cout<< "\nCurrent Process ID: " << pCurProcess->GetPID();
		// std::cout<< "\nCurrent Process Execution Time: " << pCurProcess->GetExecutionTime();
		pCurProcess->DecrementExecutionTime();
		// std::cout<< "\nCurrent Process Execution Time after DecrementExecutionTime: "
		// << pCurProcess->GetExecutionTime();

		// Remove the process if it has no more time left.
		if(pCurProcess->GetExecutionTime() == 0)
		{
			std::cout << "Process " << pCurProcess->GetPID()
				<< " completed!\n";

			// the process is no longer running so we need to remove it from the
			// running processes vector

			// int removeThisID = pCurProcess->GetPID();
			// std::cout << "\nRemoving process with ID: " << removeThisID << " from InputQueue\n";
			// std::cout << "\nInput Queue before: \n";
			// for (int i = 0; i < InputQueuePassed.size(); i++) {
			// 	std::cout << InputQueuePassed.at(i).GetPID() << " ";
			// }
			// for (int i = 0; i < InputQueuePassed.size(); i++) {
			// 	if (InputQueuePassed.at(i).GetPID() == removeThisID) {
			// 		InputQueuePassed.erase(InputQueuePassed.begin() + i);
			// 	}
			// }
			// std::cout << "\nInput Queue after: \n";
			// for (int i = 0; i < InputQueuePassed.size(); i++) {
			// 	std::cout << InputQueuePassed.at(i).GetPID() << " ";
			// }

			mProcessesRunning.erase(mProcessesRunning.begin() + i);
			// now that we removed the process, the next process that was at the next
			// index moved up one spot, so we need to make i go back
			--i;
		}
	}
}

bool Memory::MemoryAvailable(Process processPassed, int amountNeeded, std::vector<int> &passedVector) {

	// using this arithmetic to round up to nearest integer
	int pagesNeeded = (amountNeeded + pageSize - 1) / pageSize;
	std::cout << "\nNeed " << pagesNeeded << " pages \n";
	//if the amount needed less than a page size
		// find first available pageSize

		//We are looking for n consecutive pages
		// so we make an array that can hold n values
	// std::vector <int> pagesThatWouldWork (pagesNeeded, 0);
  std::vector <int> pagesThatWouldWork;
	bool foundEnoughPages = false;
	int numPagesFound = 0;
	int lastIndexGood = 0;
	// we are going to iterate through the vector holding every page
	// remember, in pagesFilledWithProcesses, if a value is != 0, it is not free
	// std::cout << std:: endl << "pagesFilledWithProcesses size " << pagesFilledWithProcesses.size();

	for (int i = 0; i < pagesFilledWithProcesses.size(); i++) {
		std::cout << std:: endl << "Step " << i + 1 << " in looping through pagesFilledWithProcesses";
		// we are looking at a page. at index i
		// if the page we are looking at is open, we need to go to the next one
		// then the next one and so on until we have as many as we need
		if (pagesFilledWithProcesses.at(i) == 0) {
			numPagesFound += 1;
		} else { numPagesFound = 0; };
		if (numPagesFound == pagesNeeded) {
			foundEnoughPages = true;
			lastIndexGood = i;
			break;
		}
	}

	//From here we know that we found enough pages and we know what the index
	// is of the final open page
	// so we can start at final index and count backwards by 1 for pagesNeeded times
	// and get the indexes we can put into the free frames
	std::cout << "\n pages needed " << pagesNeeded ;
	for (int i = 0; i < pagesNeeded; i++) {
		std::cout << "\n******************************************************";
		int checkval = lastIndexGood - i;
		if (checkval < 0) {
			std::cout << "\nERROR! lastIndexGood - i was negative. failure!\n";
			exit(0);
		}
		 else {
			 pagesThatWouldWork.push_back(checkval);
			 pagesFilledWithProcesses.at(checkval) = processPassed.GetPID();
			 std::cout<<"\nNow index " << checkval << " of pagesFilledWithProcesses " <<
			 "has value " << processPassed.GetPID() << "\n";
		 }
	}


	// print pages that would work
	for (int i = 0; i < pagesThatWouldWork.size(); i++) {
		std::cout << "\npage that would work " << pagesThatWouldWork.at(i) << "\n";
		passedVector.push_back(pagesThatWouldWork.at(i));

	}
	return foundEnoughPages;
	// else
		// divide amountNeeded / page size
		// that his how many consecutive pages we needs
		// i.e. we need that many vector indexes in a row
		;
}

bool MemoryManager::AttemptAddProcess(Process& rProcess, Memory & mem)
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
	// std::cout << "we are going to print the pmemorychunks\n" << "test size : " << pMemoryChunks->size() << std::endl;
	for (int i = 0; i < pMemoryChunks->size(); i++) {
		// std::cout << "\n we are looking at memory chunk at index " << i << "\n";
		// std:: cout << "Memory chunk size: " << pMemoryChunks->at(i).size << "\n";
		amountOfMemoryCurrentProcessNeeds += pMemoryChunks->at(i).size;
	}
	std::cout << "\nThis process needs " << amountOfMemoryCurrentProcessNeeds <<
		" frames of Memory\n";

	// Now we look in the memory to see if there are 500 frames available
	Memory * m = &mem;

	m->printFreeFrames();

	std::vector <int> freeFramesVector;

	canFit = m->MemoryAvailable(rProcess, amountOfMemoryCurrentProcessNeeds, freeFramesVector);

	std::cout << "we are now out of MemoryAvailable() so we should have freeFramesVector with values in it\nfreeFramesVector size: "
	<< freeFramesVector.size() << "\n" << "\nFreeframesvalues : \n";
	for (int i = 0; i < freeFramesVector.size(); i++) {
		std::cout << "\n" << freeFramesVector.at(i);
	}

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
		// std::cout << "\nwe got through the canFit condition\n";
		// Add the process to the processes running list.
		std::cout << "\nmProcessesRunning had size " << mProcessesRunning.size() << "\n";
		mProcessesRunning.push_back(rProcess);
		std::cout << "\n now hit has size " << mProcessesRunning.size() << "\n";

		// TODO A: Display the memory map just like in the examples.

		//now we go through the pagesFilledWithProcesses and see which ones we can have
	}
	return canFit;
}

void Memory:: updatePFWP (std::vector <int> passedVector){
	// first lets make sure each vector is the right size
	if (pagesFilledWithProcesses.size() != 0 && passedVector.size() <=
	pagesFilledWithProcesses.size()) {
		for (int i = 0; i < passedVector.size(); i++) {

		}
	}
}

// Getters:
unsigned int MemoryManager::GetNumProcessesRunning()
{
	return mProcessesRunning.size();
}


void Memory::Init(int pSize, int nFrames, int numProcs) {
	pageSize = pSize;
	numberOfFrames = nFrames;

	int numPages = numberOfFrames / pageSize;
	for (int i = 0; i < numPages; i++) {
		freeFrames.push_back(i);
	}

	std::cout << "numProcs " << numProcs;
	// set every value of pagesFilledWithProcesses to zero to start with
	for (int i = 0; i < numPages; i++) {
		pagesFilledWithProcesses.push_back(0);
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
