GLOBAL exception00
GLOBAL exception06

section .text

exception00:
    mov rax, 0
    div rax
    ret

exception06:
    db 0xC0DE, 0xC0DE
    ret