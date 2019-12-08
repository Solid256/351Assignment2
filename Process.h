#include <vector>

#ifndef PROCESS_H
#define PROCESS_H

struct MemoryChunk
{
	// The size in bytes of the memory chunk.
	int size = -1;

	// The lower bound address of the memory chunk in memory.
	// Determined when added to the memory manager.
	int lowerBound = -1;

	// The upper bound address of the memory chunk in memory.
	// Determined when added to the memory manager.
	int upperBound = -1;
};

// A typedef for the memory chunks vector.
typedef std::vector<MemoryChunk> MemoryChunks;

// The descriptor for the process class.
struct ProcessDesc
{
	// The process ID.
	int mPID = -1;

	// The time it takes for the program to enter the ready queue.
	int mArrivalTime = -1;

	// The time it takes to execute the process.
	int mExecutionTime = -1;

	// The memory chunks that make up the process.
	MemoryChunks mMemoryChunks;

};

class Process
{
public:
	Process();

	// Methods:

	// Initializes the process.
	void Init(ProcessDesc& rDesc);

	// Decrements the amount of execution time by 1 time unit.
	void DecrementExecutionTime();

	//Used for debugging
	void printProcess();

	// void setMemoryChunks()

	// Getters:
	int GetPID();
	int GetArrivalTime();
	int GetExecutionTime();
	MemoryChunks* GetMemoryChunksPtr();
	int GetAmntMemNeeded();
	int GetAmntPagesNeeded();
	std::vector <int> pagesUsing;

	// Variables:
private:
	// The process ID.
	int mPID;

	// The time it takes for the program to enter the ready queue.
	int mArrivalTime;

	// The time it takes to execute the process.
	int mExecutionTime;

	// The memory chunks that make up the process.
	MemoryChunks mMemoryChunks;

};

// Typedefs:
typedef std::vector<Process> ProcessList;



#endif
