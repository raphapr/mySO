;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Função de carregamento de GDT, passando o ponteiro no parâmetro ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;-------------- INFORMAÇÕES PRELIMINARES --------------------;

bits 32     ;32-bit Protected Mode


;-------------- FUNÇÃO DE CARREGAMENTO DA GDT ----------------;

;carrega uma gdt a partir do parâmetro, que é o ponteiro da mesma
global gdt_load
gdt_load:
    
    push ebp
    mov ebp, esp

    ;Lembrando que na passagem de parametros a captura aqui é inversamente da passagem, antes da call

    mov eax, [ebp+8]  ; parâmetro que mostra o ponteiro da gdt a ser carregada
    
    ;xchg bx, bx
    
    lgdt [eax]        ; carrega a GDT com o nosso ponteiro passado no parametro
    
    ;os valores atualizados nos registradores de segmentos
    mov ax, 0x08      
    mov ds, ax      ; 0x08 é o offset na GDT para data segment
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;xchg bx, bx

    jmp 0x10:complete_flush     ;0x10 é o offset para o segmento de código
                    ;com este artifício (Far JUMP) ele pega a base e atualiza o valor do cs (registrador de semento de código)
                        ;movendo o 0x10 para o cs, utilizou apenas para setar o valor do cs
complete_flush:
   
    leave
    ret               ; returning!
