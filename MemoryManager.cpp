#include "MemoryManager.h"

MemoryManager::MemoryManager() :
	mMaxMemorySize(-1),
	mMaxPageSize(-1),
	mpTime(nullptr)
{

}

void MemoryManager::Init(MemoryManagerDesc& rDesc)
{
	mMaxMemorySize = rDesc.memorySize;
	mMaxPageSize = rDesc.pageSize;
	mpTime = rDesc.pTime;
}