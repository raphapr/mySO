#include "gdt.h"

#define NUM_SEGMENTS 3

//; ARQUIVO EXTERNO ASSEMBLY 

//gdt_load.asm
//função de carregamento de GDT, passando o ponteiro no parâmetro
extern void gdt_load(unsigned int); 


//; BLOCO DE DADOS DA GDT DO KERNEL

struct gdt_entry_struct {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));


typedef struct gdt_entry_struct gdt_entry_t;


struct gdt_ptr_struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


typedef struct gdt_ptr_struct gdt_ptr_t;


//; VARIÁVEIS GLOBAIS

int const NULL_SEGMENT = 0;
int const DATA_SEGMENT = 1;
int const CODE_SEGMENT = 2;

gdt_entry_t gdt[NUM_SEGMENTS]; 
gdt_ptr_t  gdt_ptr;


//; FUNÇÕES DA GDT DO KERNEL

void gdt_add_seg(int num_segment, unsigned long base, unsigned long limit, 
        unsigned char access, unsigned char granularity)
{
    gdt[num_segment].base_low = (base & 0xFFFF);
    gdt[num_segment].base_middle = (base >> 16) & 0xFF;
    gdt[num_segment].base_high = (base >> 24) & 0xFF;
 
    gdt[num_segment].limit_low = (limit & 0xFFFF);
    gdt[num_segment].granularity = ((limit >> 16) & 0x0F);
 
    gdt[num_segment].granularity |= (granularity & 0xF0);
    gdt[num_segment].access = access;
}

//inicia a gdt, seta o limite e o tamanho, cria os segmentos e inicializa
void gdt_init(void)
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * NUM_SEGMENTS) - 1;   //((tamanho da gdt (struct)) * num_segmentos) - 1
    gdt_ptr.base  = (unsigned int) &gdt; //ponteiro para a tabela gdt

    gdt_add_seg(NULL_SEGMENT, 0, 0, 0, 0);          //NULL segment
    gdt_add_seg(DATA_SEGMENT, 0, 0xFFFFFFFF, 0x92, 0xCF);  //DATA segment
    gdt_add_seg(CODE_SEGMENT, 0, 0xFFFFFFFF, 0x9A, 0xCF);  //CODE segment

    //chama o ponteiro e carrega a gdt para utilização
    gdt_load((unsigned int) &gdt_ptr);
}
