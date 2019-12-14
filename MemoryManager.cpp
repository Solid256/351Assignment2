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

void freeUpSpace (vector<int> & pagesPassedList, int passedPID) {
	for (int i = 0; i < pagesPassedList.size(); i++) {
		if (pagesPassedList.at(i) == passedPID) {
			pagesPassedList.at(i) = 0;
		}
	}
}

void MemoryManager::Init(MemoryManagerDesc& rDesc)
{
	mMaxMemorySize = rDesc.maxMemorySize;
	mMaxPageSize = rDesc.maxPageSize;
	mpTime = rDesc.pTime;
}

void printProcessList (vector<Process> passed) {
	for (int i = 0; i < passed.size(); i++) {
		cout << "\n" << passed.at(i).GetPID();
	}
}

void MemoryManager::RunProcesses(std::vector<Process> &InputQueuePassed, std::vector <Process> &processList,
Memory & passedMemory, int passedTime, vector<Process> &passedFinishedV)
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
		pCurProcess->timeBeenRunning++;
		// std::cout<< "\nCurrent Process Execution Time after DecrementExecutionTime: "
		// << pCurProcess->GetExecutionTime();

		// Remove the process if it has no more time left.
		if(pCurProcess->GetExecutionTime() == 0)
		{
			std::cout << "\nProcess " << pCurProcess->GetPID()
				<< " completed!";
				// cout << "current time is " << passedTime;
				pCurProcess->setEndTime(passedTime);
				// cout << "now current process time is " << pCurProcess->endTime;
				passedFinishedV.push_back(*pCurProcess);
				freeUpSpace(passedMemory.pagesFilledWithProcesses, pCurProcess->GetPID());
				passedMemory.printMemoryMap();
				int index;
				for (int l = 0; l < processList.size(); l++) {
					int pid = processList.at(l).GetPID();

					if (pid = pCurProcess->GetPID()) {
						cout << "\nErasing " << pid << " from process list.";
						processList.erase(processList.begin() + l+1);
						break;
					}
				}
			cout << "\nErasing " << mProcessesRunning.at(i).GetPID() << " from mProcessesRunning.";
			cout << "\nProcess " << pCurProcess->GetPID() << " timeBeenRunning: " << pCurProcess->timeBeenRunning;
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
	//if the amount needed less than a page size
		// find first available pageSize

		//We are looking for n consecutive pages
		// so we make an array that can hold n values
  std::vector <int> pagesThatWouldWork;
	bool foundEnoughPages = false;
	int numPagesFound = 0;
	int lastIndexGood = 0;

	// we are going to iterate through the vector holding every page
	// remember, in pagesFilledWithProcesses, if a value is != 0, it is not free
	for (int i = 0; i < pagesFilledWithProcesses.size(); i++) {
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
	for (int i = 0; i < pagesNeeded; i++) {
		// std::cout << "\n******************************************************";
		int checkval = lastIndexGood - i;
		if (checkval < 0) {
		}
		 else {
			 pagesThatWouldWork.push_back(checkval);
			 pagesFilledWithProcesses.at(checkval) = processPassed.GetPID();
		 }
	}
	// print pages that would work
	for (int i = 0; i < pagesThatWouldWork.size(); i++) {
		passedVector.push_back(pagesThatWouldWork.at(i));

	}
	return foundEnoughPages;
		;
}

bool MemoryManager::AttemptAddProcess(Process& rProcess, Memory & mem)
{
	bool canFit;
	// The memory chunks of the current process.
	MemoryChunks* pMemoryChunks = rProcess.GetMemoryChunksPtr();
	// std::cout << "pMemoryChunks->size() " << pMemoryChunks->size() << std::endl;

	int amountOfMemoryCurrentProcessNeeds = 0;
	for (int i = 0; i < pMemoryChunks->size(); i++) {
		amountOfMemoryCurrentProcessNeeds += pMemoryChunks->at(i).size;
	}

	// Now we look in the memory to see if there are 500 frames available
	Memory * m = &mem;
	std::vector <int> freeFramesVector;
	canFit = m->MemoryAvailable(rProcess, amountOfMemoryCurrentProcessNeeds, freeFramesVector);
	if(canFit)
	{
		// Add the process to the processes running list.
		mProcessesRunning.push_back(rProcess);
	}
	return canFit;
}

//PFWP short for "pagesFilledWithProcesses"
void Memory:: updatePFWP (std::vector <int> passedVector){
	// first lets make sure each vector is the right size
	if (pagesFilledWithProcesses.size() != 0 && passedVector.size() <=
	pagesFilledWithProcesses.size()) {
		for (int i = 0; i < passedVector.size(); i++) {
		}
	}
}

void printIntVector (std::vector <int> vec) {

	for (int i = 0 ; i < vec.size() ; i ++) {
		std::cout << "\n" << vec.at(i) << "\n";
	}
}



void Memory:: printMemoryMap(){

	std::cout << "\n\nMemory Map:";
	int check = 0;
	int nextProccesID = 0;
	int repeated = 0;

	for (int i = 0; i < pagesFilledWithProcesses.size(); i++) {
		int currentProcessID = pagesFilledWithProcesses.at(i);
		int previousID = -1;
		if (i > 0) previousID = pagesFilledWithProcesses.at(i-1);
		if (currentProcessID == 0) {
			std::cout << "\n" << i * pageSize << "-" << i * pageSize + pageSize - 1
				<< ": Free frame(s)";
		} else {
				if (previousID == currentProcessID) {
					repeated += 1;
				}
				else {
					repeated = 0;
				}
			std::cout << "\n" << i * pageSize << "-" << (i * pageSize) + (pageSize - 1)
			 	<< ": Process " << currentProcessID << ", Page " << 1 +  (1 * repeated);
		}
	}
	cout << endl;
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
