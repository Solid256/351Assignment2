#include "Process.h"

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

// Getters:
unsigned int Process::GetPID()
{
	return mPID;
}

unsigned int Process::GetArrivalTime()
{
	return mArrivalTime;
}

unsigned int Process::GetExecutionTime()
{
	return mExecutionTime;
}

std::vector<unsigned int> Process::GetMemoryChunks()
{
	return mMemoryChunks;
}
