#include <stddef.h>

#include <kernel/mm.h>

/**
 * Array that holds status for each page in the system.
 * Status:
 * - Allocated	= 1
 * - Free	= 0
 */
static unsigned short mem_map [PAGING_PAGES] = {0,};

uint64_t get_free_page()
{
	/* Iterate over all available memory pages */
	for (size_t i = 0; i < PAGING_PAGES; i++) {
		/* If the page is free */
		if (mem_map[i] == 0) {
			/* Mark as allocated */
			mem_map[i] = 1;
			/* Return the starting address of the page */
			return LOW_MEMORY + i * PAGE_SIZE;
		}
	}
	return 0;
}

void free_page(uint64_t p)
{
	/* Mark page as free */
	mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0;
}
