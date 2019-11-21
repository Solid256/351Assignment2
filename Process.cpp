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
