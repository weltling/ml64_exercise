

.code
	djb proc

		; rcx == char *str 
		; rdx == size_t len

		push rdi

		mov rdi,rcx
		push rcx

		push r8

		mov rax,1505h
		xor rcx,rcx ; loop counter

l0:		cmp rdx,rcx
		je end_l0

		movzx r8,byte ptr [rdi]
		add r8,rax
		shl rax,5
		add rax,r8

		inc rcx
		inc rdi

		jmp l0
end_l0:		nop
	
		pop r8
		pop rcx
		pop rdi

		ret

	djb endp
end
