#define _XOPEN_SOURCE 500 // needed for sbrk() on cslab

#include <unistd.h>
#include <stdio.h>
typedef struct freenode
{
	size_t size;
	struct freenode *next;
} freenode;

#define HEAP_CHUNK_SIZE 4096

// head node for the freelist
freenode *freelist = NULL;

/* allocate size bytes from the heap */
void *malloc(size_t size)
{
	// can't have less than 1 byte requested
	if (size < 1)
	{
		return NULL;
	}

	// add 8 bytes for bookkeeping
	size += 8;

	// 32 bytes is the minimum allocation
	if (size < 32)
	{
		size = 32;
	}

	// round up to the nearest 16-byte multiple
	else if (size%16 != 0)
	{
		size = ((size/16)+1)*16;
	}

	freenode *previous = NULL;
	freenode *current;
	void *address;
	// if we have no memory, grab one chunk to start
	if (freelist == NULL)
	{
		address = sbrk(HEAP_CHUNK_SIZE);
		if (address == (void *)-1)
		{
			return NULL;
		}
		// skip the first 8 bytes so that we will return 16-byte aligned
		// addresses, after we put our 8 bytes of bookkeeping in front
		address += 8;
		freelist = (freenode *)address;
		freelist->size = HEAP_CHUNK_SIZE - 8;
	}
        current = freelist;
	// look for a freenode that's large enough for this request
	// have to track the previous node also for list manipulation later
	while (current != NULL)
	{
		if (current->size >= size)
		{
			break;
		}
		previous = current;
		current = current->next;
	}

	// if there is no freenode that's large enough, allocate more memory
	if (size > current->size)
	{
		size_t needed_bytes = size;
		int total_new_bytes = ((needed_bytes/HEAP_CHUNK_SIZE)+1)*HEAP_CHUNK_SIZE;
		address = sbrk(total_new_bytes);
		if (address == (void *)-1)
		{
			return NULL;
		}
		address += 8;
		current->next = (freenode *)address;
		previous = current; // is this needed!?!?!?!?!  
		current = current->next;
		current->size = total_new_bytes-8;
	}

	// here, should have a freenode with enough space for the request
	// - if there would be less than 32 bytes left, then return the entire chunk
	// - if there are remaining bytes, then break up the chunk into two pieces
	//     return the front of this memory chunk to the user
	//     and put the rest into the freelist 
	freenode *split_node;
	if (current->size - size < 32)
	{
		if (previous != NULL)
		{
			previous->next = current->next;
		}	
		else
		{
			freelist = current->next;
		}
		address = (void *)current;
		return address + 8;
	}

	if (previous == NULL)
	{
		split_node = (freenode *)((void *)current + size);
		split_node->size = current->size-size;
		current->size = size;
		address = (void *)current;
		freelist = split_node;
		return address + 8;
	}
	else
	{
		split_node = (freenode *)((void *)current + size);
		split_node->size = current->size-size;
		current->size = size;
		address = (void *)current;
		previous->next = split_node;
		return address + 8;
	}
	// here, get the address for the chunk being returned to the user and return it
}

/* return a previously allocated memory chunk to the allocator */
void free(void *ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	// make a new freenode starting 8 bytes before the provided address
	freenode *new_node = (freenode *)(ptr-8);

	// the size is already in memory at the right location (ptr-8)

	// add this memory chunk back to the beginning of the freelist
	new_node->next = freelist;
	freelist = new_node;

	return;
}
