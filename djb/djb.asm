

.code
	djb proc

		; rcx == char *str 
		; rdx == size_t len

		push r9
		push r8

		mov rax,1505h
		xor r9,r9 ; loop counter

		cmp rdx,r9
		je end_l0

l0:
		movzx r8,byte ptr [rcx+r9]
		add r8,rax
		shl rax,5
		add rax,r8

		add r9,1

		cmp rdx,r9
		jne l0
end_l0:		nop
	
		pop r8
		pop r9

		ret

	djb endp
end
