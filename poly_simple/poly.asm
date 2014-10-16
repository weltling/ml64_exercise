.code
	safe_address proc
		; rcx = size_t nmemb
		; rdx = size_t size
		; r8  = offset
		; r9  = int *overflow

		mov rax,rdx
		mul rcx
		jo ofl
		add rax,r8
		jo ofl

	nofl:
		mov r10,0
		mov [r9],r10
		jmp done

	ofl:
		xor rax,rax
		mov r10,1
		mov [r9],r10
		
	done:
		ret
	safe_address endp
end
