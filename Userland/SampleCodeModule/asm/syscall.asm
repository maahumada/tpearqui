GLOBAL print, readline, printScreen

print:
    mov r9, 0x01 ; el "id" de la syscall
    int 80h
    ret  

readline:
    mov r9, 0x02
    int 80h
    ret

printScreen:
    mov r9, 0x04
    int 80h
    ret