

.code
	djb proc

		; rcx == char *str 
		; rdx == size_t len

		push rdi

		mov rdi,rcx
		push rcx

		push r8

		mov rax,1505h
		mov rcx,0 ; loop counter
l0:
		cmp rdx,rcx
		je end_l0

		; XXX use a lookup table for this?
		mov r8,rax
		shl rax,5
		add rax,r8

		movzx r8,byte ptr [rdi]
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
