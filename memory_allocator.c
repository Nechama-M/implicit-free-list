#include <stdio.h>
#include <stdlib.h>
#include "memory_allocator.h"



/* memoryPool is a ptr to an already-existing large memoryblock */
MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size)
{
	MemoryAllocator * heap = (MemoryAllocator *)malloc(sizeof(MemoryAllocator));
	heap->m_size = size;
	*((char *)&heap) & 0;// memory is free
	heap->m_memoryPool = memoryPool;
	
	return heap;
}

/* Returns a ptr to the memoryPool */
void* MemoryAllocator_release(MemoryAllocator* allocator)
{
	free(allocator);
	return NULL;
}

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size)
{

}

/* Merge the next adjacent block is free */
void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr)
{

}

/* Merges all adjacent free blocks, and returns the size of
largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator)
{

}
