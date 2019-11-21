struct MemoryManagerDesc
{
	// The maximum memory size.
	int memorySize = -1;

	// The maximum page size.
	int pageSize = -1;

	// A pointer reference to the main timer.
	int* pTime = nullptr;
};

class MemoryManager
{
	public:
	MemoryManager();

	// Initializes the memory manager.
	void Init(MemoryManagerDesc& rDesc);

	// Variables:
	private:
	int mMaxMemorySize;
	int mMaxPageSize;
	int* mpTime;
};