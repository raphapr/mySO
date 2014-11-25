#include "monitor.h"
#include "utils.h"

//; VARIÁVEIS GLOBAIS ;

//o framebuffer de vídeo (chamado de VGA - Video Graphics Array) começa no endereço 0xB8000
unsigned short *video_memory = (unsigned short *)0xB8000;

//posição do cursor
unsigned char cursor_x = 0;
unsigned char cursor_y = 0;

//;-------------- FUNÇÕES DO MONITOR (MOSTRAGEM NA TELA) ----------------;

//atualizar a posição do cursor
static void move_cursor()
{
    //lembrando que a telinha tem dimensão de: 80x25

    unsigned short cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4,14);                  //setando o byte do cursor para a screen
    outb(0x3D5,cursorLocation >> 8); 
    outb(0x3D4,15);                
    outb(0x3D5,cursorLocation);
}

//rolar o texto da tela em uma linha
static void scroll()
{
    //captura o caractere com os atributos padrões
    unsigned char attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);

    //rolar 25 em y, significa que nós precisamos dar um scroll pra cima
    if(cursor_y >= 25)
    {
        //mover o texto
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        //a última linha deve estar em branco
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }
	//cursor na última linha
        cursor_y = 24;
    }
}

//escrever um caractere na tela
void monitor_put(char c)
{
    //background colour = black (0),foreground = white (15).
    unsigned char backColour = 0;
    unsigned char foreColour = 15;

    //bytes que definem a foreground colour, e o background colour.
    unsigned char  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    unsigned short attribute = attributeByte << 8;
    unsigned short *location;

    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
    }

    scroll();
    move_cursor();
}

//limpar a tela
void monitor_clear()
{
    unsigned char attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    unsigned short blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++)
    {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

//escrever uma string na tela
void monitor_write(char *c)
{
    int i = 0;
    while (c[i])
    {
        monitor_put(c[i++]);
    }
}

//escrever um decimal na tela
void monitor_write_dec(unsigned int d)
{
    int n;
    int top=-1;
    char *digits="0123456789ABCDEF";
    char stack[10];

    while (d>0)
    {
        n = d % 10;
        stack[++top] = digits[n];
        d /= 10;
    }
    if (top==-1) monitor_put(digits[0]);
    while (top>=0)
    {
        monitor_put(stack[top--]);
    }
}

//escrever o hexadecimal
void monitor_write_hex(unsigned int d)
{
    int i, shiftd;
    char *digits="0123456789ABCDEF";

    for (i=0; i<8; i++)
    {
        shiftd = d >> (32 - 4 - 4*i) & 0x0F;
        monitor_put(digits[shiftd]);
    }
}

