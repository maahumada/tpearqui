GLOBAL cpuVendor
GLOBAL readKeyPol

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