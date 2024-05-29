GLOBAL puts
GLOBAL readline
GLOBAL getChar
GLOBAL printScreen
GLOBAL getTimeString
GLOBAL zoomInScreen
GLOBAL zoomOutScreen
GLOBAL blackOut
GLOBAL putChar
GLOBAL removeChar
GLOBAL getRegisters
GLOBAL clearScreen
GLOBAL putSquare
GLOBAL sleep
GLOBAL getLastPressed
GLOBAL printImage
GLOBAL makeBeep
GLOBAL exception00
GLOBAL exception06

section .text

puts:
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

blackOut:
    mov r9, 0x08
    int 80h
    ret

putChar:
    mov r9, 0x09
    int 80h
    ret
    
removeChar:
    mov r9, 0x0A
    int 80h
    ret

getRegisters:
   mov r9, 0x0B
   int 80h
   ret

clearScreen:
    mov r9, 0x0C
    int 80h
    ret

putSquare:
    mov r9, 0x0D
    int 80h
    ret

sleep:
    mov r9, 0x0E
    int 80h
    ret

getLastPressed:
    mov r9, 0x0F
    int 80h
    ret

printImage:
    mov r9, 0x11
    int 80h
    ret
    
makeBeep:
    mov r9, 0x10
    int 80h
    ret

exception00:
    mov rax, 0
    div rax
    ret

exception06:
    db 0xC0DE, 0xC0DE
    ret