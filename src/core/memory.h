/************************************************************************ 
 * @description :  
 * @author		:  $username$
 * @creat 		:  $time$
************************************************************************ 
 * Copyright @ OscarShen 2017. All rights reserved. 
************************************************************************/  
#pragma once
#ifndef ORION_CORE_MEMORY_H_
#define ORION_CORE_MEMORY_H_
#include <orion.h>
namespace orion {

#define ARENA_ALLOC(arena, Type) new ((arena).alloc(sizeof(Type))) Type

	void *allocAligned(size_t size);
	template <typename T>
	T *allocAligned(size_t count) { return (T*)allocAligned(count * sizeof(T)); }

	void freeAligned(void *ptr);

	class alignas(64) MemoryArena
	{
	private:
		const size_t blockSize;
		size_t currentBlockPos = 0, currentAllocSize = 0;
		uint8_t *currentBlock = nullptr;
		std::list<std::pair<size_t, uint8_t *>> usedBlocks, availableBlocks;

	public:
		MemoryArena(size_t blockSize = 262144) : blockSize(blockSize) {}
		~MemoryArena();

		void *alloc(size_t nBytes);
		template <typename T>
		T *alloc(size_t n = 1, bool runConstructor = true) {
			T *ret = (T *)alloc(n * sizeof(T));
			if (runConstructor)
				for (size_t i = 0; i < n; ++i) new (&ret[i]) T();
			return ret;
		}
		void reset();
		size_t totalAllocated() const;

	private:
		MemoryArena(const MemoryArena &) = delete;
		MemoryArena &operator=(const MemoryArena &) = delete;
	};

}

#endif // !ORION_CORE_MEMORY_H_

