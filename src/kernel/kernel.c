#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "timer.h"
#include "keyboard.h"

// main do kernel

extern enable_paging(void);
extern update_directory(unsigned int);

    // paginação divide o espaço da memória virtual em vários blocos, chamados de pages, de normalmente 4KB de tamanho.

 /*present     // Page present in memory*/
 /*rw          // Read-only if clear, readwrite if set*/
 /*user        // Supervisor level only if clear*/
 /*accessed    // Has the page been accessed since last refresh?*/
 /*dirty       // Has the page been written to since last refresh?*/
 /*unused      // Amalgamation of unused and reserved bits*/
 /*frame       // Frame address (shifted right 12 bits)*/

typedef struct page_entry {
	int present 		: 1; // página presente na memória
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


int kmain()
{
    //início
    asm volatile ("xchg %bx, %bx");

    monitor_clear(); //limpar a tela

    //inicializar a gdt do kernel (independente do bootloader)
    gdt_init();
    monitor_write("gdt_init()\n\n");
    
    asm volatile ("xchg %bx, %bx"); // breakpoint 1
    
    //inicializar a idt (tabela de instruções)
    idt_init(); 
    monitor_write("idt_init()\n\n");

    asm volatile ("xchg %bx, %bx"); // breakpoint 2

    //activate interruptions
    /*monitor_write("Ativando a flag de interrupcao (sti) \n\n");*/
    /*asm volatile("sti"); */

    /*asm volatile ("xchg %bx, %bx"); // breakpoint 3*/

    /*//interrupções*/
    /*monitor_write("Interrupcao (IRQ)\n");*/
    /*monitor_write("int $0x6...\n");    */
    /*asm volatile ("int $0x6");*/

    /*asm volatile ("xchg %bx, %bx"); //breakpoint 4*/

/*
    //timer
    monitor_write("\nEntrando no timer...\n\n");    
    asm volatile ("xchg %bx, %bx"); 
    init_timer(100); //inicializando o timer na frequência definida
*/


    //// teclado - desabilitar o teclado para ativar a paginação
    /*monitor_write("\nHandler do teclado ativado!\n\n");*/
    /*init_keyboard();*/
    /*asm volatile ("xchg %bx, %bx"); //breakpoint 5 */


    //////////////////////////////////////////////////////////////////////
	////////////////// criar minha tabela de página.
	
	int i;
	
	page_dir_t*  	directory;
	page_table_t* 	table0;
	
	
	directory = (page_dir_t*)   0x10000;
	table0 	  = (page_table_t*) 0x11000;
	
	
	int size = 4 * 1024;
	for (i = 0; i < 1024; i++)
	{
		
		table0->pages[i].present 	 = 1;
		table0->pages[i].read		 = 1;
		table0->pages[i].user		 = 1;
		table0->pages[i].write		 = 0;
		table0->pages[i].frame_base = i;
		
	}
	
	
	int *tmp = (((unsigned int) table0) | 0x07);
	
	
	
	//test(directory->tables[0]);
	//test(&(directory->tables[0]));
	
	directory->tables[0] = (page_table_t*) tmp;
		
	update_directory(directory);
	
	enable_paging();

    monitor_write("\nPaginacao...\n\n");
    asm volatile ("xchg %bx, %bx"); //break point 6
	
    //////////////////////////////////////////////////////////////////////

    while(1);

    return 1;
}
