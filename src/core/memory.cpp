#include "memory.h"

namespace orion {

	void *allocAligned(size_t size) {
#if defined(ORION_IN_WINDOWS)
		return _aligned_malloc(size, 64);
#else
		return memalign(64, size);
#endif
	}

	void freeAligned(void *ptr)
	{
		if (!ptr) return;
#ifdef ORION_IN_WINDOWS
		_aligned_free(ptr);
#else
		free(ptr);
#endif // ORION_IN_WINDOWS
	}
	MemoryArena::~MemoryArena()
	{
		freeAligned(currentBlock);
		for (auto &block : usedBlocks) freeAligned(block.second);
		for (auto &block : availableBlocks) freeAligned(block.second);
	}
	void * MemoryArena::alloc(size_t nBytes)
	{
		// Round up _nBytes_ to minimum machine alignment
		nBytes = ((nBytes + 15) & (~15));
		if (currentBlockPos + nBytes > currentAllocSize) {
			// Add current block to _usedBlocks_ list
			if (currentBlock) {
				usedBlocks.push_back(
					std::make_pair(currentAllocSize, currentBlock));
				currentBlock = nullptr;
				currentAllocSize = 0;
			}

			// Get new block of memory for _MemoryArena_

			// Try to get memory block from _availableBlocks_
			for (auto iter = availableBlocks.begin();
				iter != availableBlocks.end(); ++iter) {
				if (iter->first >= nBytes) {
					currentAllocSize = iter->first;
					currentBlock = iter->second;
					availableBlocks.erase(iter);
					break;
				}
			}
			if (!currentBlock) {
				currentAllocSize = std::max(nBytes, blockSize);
				currentBlock = allocAligned<uint8_t>(currentAllocSize);
			}
			currentBlockPos = 0;
		}
		void *ret = currentBlock + currentBlockPos;
		currentBlockPos += nBytes;
		return ret;
	}
	void MemoryArena::reset()
	{
		currentBlockPos = 0;
		availableBlocks.splice(availableBlocks.begin(), usedBlocks);
	}
	size_t MemoryArena::totalAllocated() const
	{
		size_t total = currentAllocSize;
		for (const auto &alloc : usedBlocks) total += alloc.first;
		for (const auto &alloc : availableBlocks) total += alloc.first;
		return total;
	}
}
