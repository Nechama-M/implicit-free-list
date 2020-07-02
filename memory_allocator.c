#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory_allocator.h"
#include <assert.h>


static const size_t LSB = 1;
static const size_t BlockSize = 8;

struct MemoryAllocator
{
	size_t m_size;
	void * m_memoryPool;
};

/* Aux. function*/
bool IsFreeBlock(void *temp_ptr)
{
	return !(*((size_t*)temp_ptr))&1;

}
bool BlokIsAvaible(void * block,size_t size)
{
	return size <= *((size_t* )block);
}
void * NextBlock(void * block)
{
	char * temp = block;
	temp +=  (*((size_t*)block))^LSB;
	block = (void *)temp;
	return block;
}

void MergesBlocks(void * block)
{
	void * temp = NextBlock(block);
	size_t temp_size = *((size_t *)block)+ *((size_t *)temp) - sizeof(size_t);
	*((size_t *)block) = temp_size; 
}

void * a_malloc(void * block,size_t size)
{
	if(size < *((size_t *)block))
	{
		char * next_block = (char *)block + size;
		*((size_t *)next_block) =*((size_t *)block) - size + sizeof(size_t);
	}
	*((size_t *)block) = size + sizeof(size_t);
	*((size_t *)block) = *((size_t *)block) | LSB;
	return  (void *)((size_t *)(block)+1);

}

/*end Aux. function*/

/* memoryPool is a ptr to an already-existing large memoryblock */
MemoryAllocator* MemoryAllocator_init(void* memoryPool, size_t size)
{
	assert(size>=BlockSize);
	MemoryAllocator * heap = (MemoryAllocator *)malloc(sizeof(MemoryAllocator));
	
	heap->m_size =size%BlockSize==0 ? size : size - size%BlockSize;
	heap->m_memoryPool = memoryPool;
	*(size_t*)(heap->m_memoryPool) = heap->m_size;
	// the LSB ==0 
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
	size = size%BlockSize==0 ? size+sizeof(size_t) : size + (BlockSize-size%BlockSize)+sizeof(size_t);
	void * temp_ptr = allocator->m_memoryPool;
	size_t sum =0;
	while(sum <= allocator->m_size)
	{
		if(IsFreeBlock(temp_ptr))
		{
			if(BlokIsAvaible(temp_ptr,size))
			{
				return a_malloc(temp_ptr,size);
			}
			else
			{
				while(IsFreeBlock(NextBlock(temp_ptr)) && !(BlokIsAvaible(temp_ptr,size)))
				{
					MergesBlocks(temp_ptr);	
				}
				if(BlokIsAvaible(temp_ptr,size))
				{
					return a_malloc(temp_ptr,size);
				}
			}	
		}
		sum  += *((size_t*)temp_ptr);
		temp_ptr = NextBlock(temp_ptr);
	}
	return NULL;
	
}

/* Merge the next adjacent block is free */
void MemoryAllocator_free(MemoryAllocator* allocator, void* ptr)
{
		
}

/* Merges all adjacent free blocks, and returns the size of
largest free block */
size_t MemoryAllocator_optimize(MemoryAllocator* allocator)
{
	size_t sum = 0;
	size_t max_size = 0;
	void * temp_ptr = allocator->m_memoryPool;
	while(sum<= allocator->m_size)
	{
		if(IsFreeBlock(temp_ptr))
		{
			while(IsFreeBlock(NextBlock(temp_ptr))== true )
			{
				MergesBlocks(temp_ptr);
			}
			if(*((size_t*)temp_ptr)>max_size)
			{
				max_size = *((size_t*)temp_ptr);
			}
		}
		sum  += *((size_t*)temp_ptr);
		temp_ptr = NextBlock(temp_ptr);
	}
	return max_size;
	
	
	
	
	
	
	
}
