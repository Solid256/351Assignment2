#include "Process.h"
#include <iostream>

Process::Process() :
	mPID(0),
	mArrivalTime(0),
	mExecutionTime(0)
{

}

void Process::Init(ProcessDesc& rDesc)
{
	mPID = rDesc.mPID;
	mArrivalTime = rDesc.mArrivalTime;
	mExecutionTime = rDesc.mExecutionTime;
	mMemoryChunks = rDesc.mMemoryChunks;
}

void Process::DecrementExecutionTime()
{
	--mExecutionTime;
}

// Getters:
int Process::GetPID()
{
	return mPID;
}

int Process::GetArrivalTime()
{
	return mArrivalTime;
}

int Process::GetExecutionTime()
{
	return mExecutionTime;
}

MemoryChunks* Process::GetMemoryChunksPtr()
{
	return &mMemoryChunks;
}


// Using this function for debugging

void Process::printProcess()
{
	std::cout << "\n\nPrinting current Process...";
	std::cout << "\nPID: " << Process::GetPID();
	std::cout << "\nArrival Time: " << Process::GetArrivalTime();
	std::cout << "\nExecution Time: " << Process::GetExecutionTime();

	int sizeOfMemoryChunks = mMemoryChunks.size();
	printf("\n\nPrinting Process Memory Chunks Vector: ");
	for (int i = 0; i < sizeOfMemoryChunks; i++) {
		printf("\nThis memory chunk's values: ");
		std::cout << "\nSize: " << mMemoryChunks.at(i).size << std::endl;
		std::cout << "\nUpper: " << mMemoryChunks.at(i).upperBound << std::endl;
		std::cout << "\nLower: " << mMemoryChunks.at(i).lowerBound << std::endl;
	}

}