#include <vector>

// The descriptor for the process class.
struct ProcessDesc
{
	// The process ID.
	unsigned int mPID = 0;
	
	// The time it takes for the program to enter the ready queue.
	unsigned int mArrivalTime = 0;

	// The time it takes to execute the process.
	unsigned int mExecutionTime = 0;

	// The memory chunks that make up the process.
	std::vector<unsigned int> mMemoryChunks;
};

class Process
{
public:
	Process();

	// Methods:

	// Initializes the process.
	void Init(ProcessDesc& rDesc);

	// Getters:
	unsigned int GetPID();
	unsigned int GetArrivalTime();
	unsigned int GetExecutionTime();
	std::vector<unsigned int> GetMemoryChunks();

	// Variables:
private:
	// The process ID.
	unsigned int mPID;
	
	// The time it takes for the program to enter the ready queue.
	unsigned int mArrivalTime;

	// The time it takes to execute the process.
	unsigned int mExecutionTime;

	// The memory chunks that make up the process.
	std::vector<unsigned int> mMemoryChunks;
};

// Typedefs:
typedef std::vector<Process> ProcessList;