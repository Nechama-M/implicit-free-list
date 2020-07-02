#include "memory_allocator.h"
#include <stdlib.h>
int main()
{
	void * temp = malloc(60);
	MemoryAllocator * heap = MemoryAllocator_init(temp,33);
	MemoryAllocator_allocate(heap,12);
	printf("%ld",MemoryAllocator_optimize(heap));
	temp = MemoryAllocator_release(heap);
	free(temp);
	return 0;
}
