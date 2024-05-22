GLOBAL print

print:
    mov r9, 0x01 ; el "id" de la syscall
    int 80h
    ret  