#include "utils.h"

// Algumas funções úteis para uso de strings e escrita de bytes na tela  


void outb(unsigned short port, unsigned char value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

unsigned char inportb(unsigned short port)
{
    unsigned char ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

unsigned short inw(unsigned short port)
{
    unsigned short ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

//copiar a quantidade de "len" bytes de "src" para "dest"
void memcpy(unsigned char *dest, const unsigned char *src, unsigned int len)
{
    const unsigned char *sp = (const unsigned char *)src;
    unsigned char *dp = (unsigned char *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

//escrever len cópias de "val" para "dest"
void memset(unsigned char *dest, unsigned char val, unsigned int len)
{
    unsigned char *temp = (unsigned char *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

//comparar duas strings: retorna -1 se str1<str2,  0 se elas forem iguais, caso contrário 1 
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

//cópia de string
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

//concatenação de string
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}
