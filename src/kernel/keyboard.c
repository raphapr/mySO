#include "keyboard.h"
#include "monitor.h"
#include "utils.h"
#include "isr.h"

// Algumas funções úteis para interrupção de teclado ;;


//; VARIÁVEIS GLOBAIS 

//quantidade de teclas

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};		


//;-------------- HANDLER DO TECLADO (INTERRUPÇÃO) ----------------;

/* Handles the keyboard interrupt */
void keyboard_handler(registers_t regs)
{
    //caractere a ser escaneado
    unsigned char scancode;

    //leitura do buffer de dados do teclado
    scancode = inportb(0x60);

    if (scancode & 0x80)
    {
	//espaço para tratamento do shift, alt, e control
    }
    else
    {
	//printar o caractere na tela
	monitor_put(kbdus[scancode]);
    }
}


void init_keyboard()
{
	//primeiro interruption request
	int const IRQ = 33;

	register_interrupt_handler(IRQ, keyboard_handler);
}

