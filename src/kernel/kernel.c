#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "timer.h"
#include "paging.h"
#include "keyboard.h"

// main do kernel

page_dir_t  	*directory = (page_dir_t*)   0x10000;
page_table_t 	*table_0 = (page_table_t*) 0x11000, 
				*table_1 = (page_table_t*) 0x12000;

    // paginação divide o espaço da memória virtual em vários blocos, chamados de pages, de normalmente 4KB de tamanho.

 /*present     // Page present in memory*/
 /*rw          // Read-only if clear, readwrite if set*/
 /*user        // Supervisor level only if clear*/
 /*accessed    // Has the page been accessed since last refresh?*/
 /*dirty       // Has the page been written to since last refresh?*/
 /*unused      // Amalgamation of unused and reserved bits*/
 /*frame       // Frame address (shifted right 12 bits)*/
// main do kernel

int kmain()
{
    int i;
    //início
    asm volatile ("xchg %bx, %bx");

    monitor_clear(); //limpar a tela

    //inicializar a gdt do kernel (independente do bootloader)
    gdt_init();
    monitor_write("gdt_init()\n");
    
    asm volatile ("xchg %bx, %bx"); // breakpoint 1
    
    //inicializar a idt (tabela de instruções)
    idt_init(); 
    monitor_write("idt_init()\n");

    asm volatile ("xchg %bx, %bx"); // breakpoint 2

    //activate interruptions
    /*monitor_write("Ativando a flag de interrupcao (sti) \n\n");*/
    asm volatile("sti");

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
	
	// FASE 1
	for (i = 0; i < 1024; i++) {
		
		table_0->pages[i].present 	 = 1;
		table_0->pages[i].read		 = 1;
		table_0->pages[i].user		 = 1;
		table_0->pages[i].write		 = 0;
		table_0->pages[i].frame_base = i;
		
	}

	for (i = 0; i < 1024; i++) {
		int cond;
		if (!(i % 2)) {
			cond = 0;
		} else {
			cond  = 1;		
		}
		table_1->pages[i].present 	 = cond;
		table_1->pages[i].read		 = 1;
		table_1->pages[i].user		 = 1;
		table_1->pages[i].write		 = 0;
		table_1->pages[i].frame_base = i + 2048;
	}
	
	directory->tables[0] = (((unsigned int) table_0) | 0x07);//?
	directory->tables[1] = (((unsigned int) table_1) | 0x07);//?
		
	update_directory(directory);
	
	enable_paging();
	monitor_write("Paginacao\n");

	asm volatile("xchg %bx, %bx");

	// FASE 2
	char *pagina11 =  0x40B000;
	write_page(pagina11, "IC#", 3);
	
	asm volatile ("xchg %bx, %bx");

	// FASE 3
	
	init_keyboard();
	monitor_write("Teclado habilitado!\n"); 
	asm volatile ("xchg %bx, %bx");
	
	//asm volatile ("xchg %bx, %bx");
	//asm volatile ("int $0x3");
	//asm volatile ("int $0x4");

	//init_timer(500); // Initialise timer to 50Hz 

	while(1);
	
	return 1;

}
