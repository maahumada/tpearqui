GLOBAL printf

printf:
    mov rax, 1 // el "id" de la syscall
    int 80h
    ret    