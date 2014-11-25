#ifndef PAGING_H
#define PAGING_H

#include "isr.h"
#include "monitor.h"

typedef struct page_entry {
	int present 		: 1;
	int read			: 1;
	int user			: 1;
	int write			: 1;
	int cache			: 1;
	int accessed		: 1;
	int size			: 1;
	int ignored			: 1;
	int available		: 4;
	int frame_base		: 20;
} page_entry_t;

typedef struct page_table {
	
	page_entry_t pages[1024];
	
} page_table_t;

typedef struct page_dir {
	
	page_table_t* tables[1024];
	
} page_dir_t;

void write_page(char *end, char *txt, int tam);
void page_fault_h(registers_t regs);

#endif //PAGING_H
