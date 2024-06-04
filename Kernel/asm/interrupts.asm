GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq128Handler

GLOBAL _exception00Handler
GLOBAL _exception06Handler

GLOBAL copyRegisters
GLOBAL saveAndCopyRegisters

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN puts
EXTERN print
EXTERN getStackBase
EXTERN updateRegistersFromException
EXTERN saveRegisters


SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro irqHandlerMasterSysCalls 0
	pushState
	call syscallDispatcher
	popState
	iretq
%endmacro

%macro saveStateDump 0
	mov [registers+24], rax
	mov rax, [rsp] 		; Get RIP from ret in stack
	mov [registers+0], rax
	mov [registers+8], rsp
	mov [registers+16], rbp
	mov [registers+32], rbx
	mov [registers+40], rcx
	mov [registers+48], rdx
	mov [registers+56], rdi
	mov [registers+64], rsi
	mov [registers+72], r8
	mov [registers+80], r9
	mov [registers+88], r10
	mov [registers+96], r11
	mov [registers+104], r12
	mov [registers+112], r13
	mov [registers+120], r14
	mov [registers+128], r15
%endmacro

%macro saveState 0
	push rax
	mov rax, [rsp + 8]
	mov [registers+0], rax 	; RIP
	mov rax, [rsp + 8 + 8 * 3]
	mov [registers+8], rax 	; RSP
	pop rax
	
	mov [registers+16], rbp
	mov [registers+24], rax
	mov [registers+32], rbx
	mov [registers+40], rcx
	mov [registers+48], rdx
	mov [registers+56], rdi
	mov [registers+64], rsi
	mov [registers+72], r8
	mov [registers+80], r9
	mov [registers+88], r10
	mov [registers+96], r11
	mov [registers+104], r12
	mov [registers+112], r13
	mov [registers+120], r14
	mov [registers+128], r15
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Syscalls
_irq128Handler:
	irqHandlerMasterSysCalls

;Zero Division Exception
_exception00Handler:
	saveState
	pushState
	mov rdi, errorstr00
	mov rsi, color
	call puts
	call print
	call updateRegistersFromException
	popState
	call getStackBase
	mov [rsp + 24], rax
	mov [rsp + 16], DWORD 0x202
	mov rax, userland
	mov [rsp], rax
	iretq

;Invalid OPCode Exception
_exception06Handler:
	saveState
	pushState
	mov rdi, errorstr06
	mov rsi, color
	call puts
	call print
	call updateRegistersFromException
	popState
	call getStackBase
	mov [rsp + 24], rax
	mov [rsp + 16], DWORD 0x202
	mov rax, userland
	mov [rsp], rax
	iretq

saveAndCopyRegisters:
	saveStateDump
	call copyRegisters
	ret

haltcpu:
	cli
	hlt
	ret

copyRegisters:
	push rbp
	mov rbp, rsp
	
	mov rbx, 0

.L1:
	cmp rbx, 17
	je .end
	mov rax, qword[registers+rbx*8]
	inc rbx
	mov qword[rdi], rax
	add rdi, 8
	jmp .L1

.end:
	mov qword[rdi], rax 

	mov rsp, rbp
	pop rbp
  	ret

SECTION .data
	errorstr00 db "ERROR: ZERO DIVISION", 10, 0
	errorstr06 db "ERROR: INVALID OP CODE", 10, 0

SECTION .rodata
	userland equ 0x400000
	color equ 0xFF0000

SECTION .bss
	aux resq 1
	registers resq 17