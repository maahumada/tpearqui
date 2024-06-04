GLOBAL cpuVendor
GLOBAL readKeyPol
GLOBAL copyRegisters
GLOBAL outb
GLOBAL inb

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


readKeyPol:
	push rbp		; stackframe
	mov rbp, rsp
	pushf ; los flags

.loop:
	mov rax, 0
	in al, 0x64 	; leyendo el status y lo guardo en al  
	and al, 0x01 	; me quedo con el bit 0
	cmp al, 0x01 	; me lo pide para poder leer despues con el 60h
	jne .loop

	in al, 0x60 	; todo siempre en eax (al es la parte mas mas baja)
	movzx rax, al
	popf
	mov rsp, rbp
	pop rbp 		; desarmado stackframe
	ret 			; en al quedó el caracter

copyRegisters:
	push rbp
	mov rbp, rsp
	
	add rsp, rsi
	mov rbx, 0

.L1:
	cmp rbx, 16
	je .end
	mov rax, qword[rsp + 8*rbx]
	inc rbx
	mov qword[rdi], rax
	add rdi, 8
	jmp .L1

.end:
	mov rax, [rsp + 8 * 18]
	mov qword[rdi], rax 

	mov rsp, rbp
	pop rbp
  	ret

outb:
	mov edx, edi
	mov eax, esi
	out dx, al ; portr en rax 
	ret

inb:
	mov edx, edi
	in al, dx ;(in dest, port)
	ret


section .data
