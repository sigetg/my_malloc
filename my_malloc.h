/* allocate size bytes from the heap */
void *malloc(size_t size);

/* return a previously allocated memory chunk to the allocator */
void free(void *ptr);
