#include "isr.h"
#include "monitor.h"
#include "utils.h"

// Serviço de tratamento de Interrupções


// VARIÁVEIS GLOBAIS

//armazenador das chamadas
isr_t interrupt_handlers[256];


// HANDLER DE INTERRUPÇÃO (CAPTURA DE INTERRUPÇÃO)

//chamada quando uma interrupção for recebida
void isr_handler(registers_t regs)
{
   monitor_write("recieved interrupt: ");
   monitor_write_dec(regs.int_no);
   monitor_put('\n');

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}

// HANDLER DE REQUEST DE INTERRUPÇÃO COM ATUALIZADOR (IRQ)

void irq_handler(registers_t regs)
{
   //caso a interrupção se envolva com o slave
   if (regs.int_no >= 40)
   {
       outb(0xA0, 0x20);
   }
   //mandar sinal para o master
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
} 

//atualizando os registros de interrupções
void register_interrupt_handler(unsigned char n, isr_t handler)
{
	/*
	  monitor_write("register_interrupt_handler(");
	  monitor_write_dec(n);
	  monitor_write(")\n");
	*/
  interrupt_handlers[n] = handler;
} 

