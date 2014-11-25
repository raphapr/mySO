# Makefile pro assemble, compilação e linkagem do kernel

#Formato de saída do NASM

NFORMAT=elf32

all: init bootloader kernel monitor utils gdt idt idt_load isr isr_load timer keyboard pag linking
	cat dist/boot.bin dist/kernel.bin > dist/floppy.bin
	sudo dd if=dist/floppy.bin of=/dev/fd0
	
init: 
	mkdir -p dist

kernel:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/kernel.o src/kernel/kernel.c
	#gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/hang.o src/kernel/hang.c

gdt:
	nasm -f elf src/kernel/gdt_load.asm -o dist/gdt_load.o 
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/gdt.o src/kernel/gdt.c

# escrita na tela
monitor:
	gcc -Wall -O -fno-stack-protector -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/monitor.o src/kernel/monitor.c

# funções úteis para uso de strings e escrita de bytes na tela
utils:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/utils.o src/kernel/utils.c
	
bootloader:
	nasm src/bootloader/boot.asm -f bin -o dist/boot.bin
	
clean:
	rm -rf dist

idt:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/idt.o src/kernel/idt.c
idt_load:
	nasm -f elf src/kernel/idt_load.asm -o dist/idt_load.o

isr:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/isr.o src/kernel/isr.c
isr_load:
	nasm -f elf src/kernel/isr_load.asm -o dist/isr_load.o

timer:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/timer.o src/kernel/timer.c

# leitura buffer por interrupção
keyboard:
	gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -c -o dist/keyboard.o src/kernel/keyboard.c

pag:
	nasm -f elf src/kernel/paging.asm -o dist/paging.o

linking:
	ld -T link.ld dist/kernel.o dist/monitor.o dist/utils.o dist/gdt.o dist/gdt_load.o dist/idt.o dist/idt_load.o dist/isr_load.o dist/isr.o dist/timer.o dist/keyboard.o dist/paging.o -o dist/kernel.bin
