#include <stdlib.h>
#include <stdbool.h>
#include "memory_allocator.h"
#include <assert.h>


static const size_t LSB = 1;
static const size_t BlockSize = 8;
static const size_t IgnoreLSB = ~(size_t)1;
static const char SizeOfMetaData = sizeof(size_t);


struct MemoryAllocator
{
	size_t m_size;
	void * m_memoryPool;
};

/* Aux. function*/
static size_t AlignBlock(size_t size)
{
	return size % BlockSize == 0 ? size + SizeOfMetaData : size + (BlockSize- (size % BlockSize)) + SizeOfMetaData;
}

bool IsFreeBlock(void *block)
{
	return !(*(size_t*)block & LSB) ;
}

bool BlokIsAvaible(void * block,size_t size)
{
	return size+SizeOfMetaData <= *((size_t* )block);
}

void * GetPayload(void *ptr)
{
	return (void *)((size_t *)ptr + 1);
}

bool EndOfMemory(MemoryAllocator* allocator,void * ptr)
{
	return (void *)((char *)(allocator->m_memoryPool) + allocator->m_size) > ptr;
}

void * NextBlock(void * block)
{
	if (!EndOfMemory)
	{
		void * temp = block;
		temp = (char *)temp + ((*((size_t*)block))^LSB);
		block = (void *)temp;
		
		return block;
	}
	else
	{
		return NULL;
	}
}

void MergesBlocks(void * block)
{
	void * temp = NextBlock(block);
	size_t temp_size = *((size_t *)block) + *((size_t *)temp) - SizeOfMetaData;
	*((size_t *)block) = temp_size;
}

void * a_malloc(void * block,size_t size)
{
	if (size < *((size_t *)block))
	{
		void * next_block = (char *)block + size;
		*((size_t *)next_block) = (*(size_t *)block) - size;
	}
	*((size_t *)block) = size + SizeOfMetaData;
	*((size_t *)block) = (*((size_t *)block) | LSB);
	
	return  GetPayload(block);
}
/*end Aux. function*/


/* memoryPool is a ptr to an already-existing large memoryblock */
MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size)
{
	assert(size>=BlockSize);
	MemoryAllocator * heap = (MemoryAllocator *)malloc(sizeof(MemoryAllocator));
	heap->m_size = size % BlockSize ==0 ? size : size - size % BlockSize;
	heap->m_memoryPool = memoryPool;
	*((size_t*)(heap->m_memoryPool)) = size;
	
	return heap;
}

/* Returns a ptr to the memoryPool */
void* MemoryAllocator_release(MemoryAllocator* allocator)
{
	void * temp = allocator->m_memoryPool;
	free(allocator);
	
	return temp;
}

void* MemoryAllocator_allocate(MemoryAllocator* allocator, size_t size)
{
	size = AlignBlock(size);
	void * temp_ptr = allocator->m_memoryPool;
	size_t sum =0;
	while (!EndOfMemory(allocator,temp_ptr))
	{
		if (IsFreeBlock(temp_ptr))
		{
			if (BlokIsAvaible(temp_ptr,size))
			{
				return a_malloc(temp_ptr,size);
			}
			else
			{
				while (IsFreeBlock(NextBlock(temp_ptr)) && !(BlokIsAvaible(temp_ptr,size)))
				{
					MergesBlocks(temp_ptr);	
				}
				if (BlokIsAvaible(temp_ptr,size))
				{
					return a_malloc(temp_ptr,size);
				}
			}	
		}
		temp_ptr = NextBlock(temp_ptr);
	}
	
	return NULL;
}

/* Merge the next adjacent block is free */
void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr)
{
	if (ptr == NULL)
		return;

	void *tmp = NextBlock(ptr - SizeOfMetaData);
	size_t sum = *(size_t*)ptr;
	while ((!IsFreeBlock(tmp)) && (sum <= allocator->m_size))
	{
		tmp = NextBlock(tmp);
		sum += *((size_t*)tmp);
	}
	*(size_t*)ptr = tmp - ptr;	
}

/* Merges all adjacent free blocks, and returns the size of
largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator)
{
	size_t sum = 0;
	size_t max_size = 0;
	void * temp_ptr = allocator->m_memoryPool;
    	sum  += (*((size_t*)temp_ptr) ^ 1);

    	while (sum<= allocator->m_size)
	{
		if (IsFreeBlock(temp_ptr))
		{
			while (IsFreeBlock(NextBlock(temp_ptr))== true )
			{
				MergesBlocks(temp_ptr);
			}
			if (*((size_t*)temp_ptr)>max_size)
			{
				max_size = *((size_t*)temp_ptr);
			}
		}
		sum  += *((size_t*)(NextBlock(temp_ptr)));
		temp_ptr = NextBlock(temp_ptr);
	}
	
	return max_size;	
}
