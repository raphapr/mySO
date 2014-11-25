#include "paging.h"

void write_page(char *end, char *txt, int tam)
{
	int i, j;
	
	for (i = 0; i < 4096; i+=tam)
		for (j = 0; j < tam; ++j)
			end[i + j] = txt[j];
}
extern page_table_t *table_1;

void page_fault_h(registers_t regs)
{
	unsigned int faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	asm volatile("xchg %bx, %bx");
	
	unsigned int dirEnd = faulting_address / (1024 * 4096);

	if (dirEnd > 2) {
		monitor_write("\n");
		monitor_write("Permission Error!!!!");
		monitor_write("\n");
	} else {
		unsigned int numPag = (faulting_address >> 12) & 0x3FF;
		table_1->pages[numPag].present = 1;

		write_page((char *) faulting_address, "UFAL#", 5);
	}

	while(1);
}
