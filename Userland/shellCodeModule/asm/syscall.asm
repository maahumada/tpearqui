GLOBAL print
GLOBAL readline
GLOBAL printScreen
GLOBAL getTimeString
GLOBAL zoomInScreen
GLOBAL zoomOutScreen
GLOBAL clearScreen

section .text

print:
    mov r9, 0x01 ; el "id" de la syscall
    int 80h
    ret  

readline:
    mov r9, 0x02
    int 80h
    ret

getChar:
    mov r9, 0x03
    int 80h
    ret

printScreen:
    mov r9, 0x04
    int 80h
    ret

getTimeString:
    mov r9, 0x05
    int 80h
    ret

zoomInScreen:
    mov r9, 0x06
    int 80h
    ret

zoomOutScreen:
    mov r9, 0x07
    int 80h
    ret

clearScreen:
    mov r9, 0x08
    int 80h
    ret