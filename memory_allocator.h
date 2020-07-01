
#ifndef __MEMORY_ALLOCATOR_H__
#define __MEMORY_ALLOCATOR_H__
#include <stdio.h>

typedef struct MemoryAllocator
{
	size_t m_size;
	void * m_memoryPool;
} MemoryAllocator;

typedef struct MetaData
{
	size_t m_size;
}MetaData;


/* memoryPool is a ptr to an already-existing large memoryblock */
MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size);

/* Returns a ptr to the memoryPool */
void* MemoryAllocator_release(MemoryAllocator* allocator);

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size);

/* Merge the next adjacent block is free */
void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr);

/* Merges all adjacent free blocks, and returns the size of
largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator);


#endif/*__MEMORY_ALLOCATOR_H__*/‏






