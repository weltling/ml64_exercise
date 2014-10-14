
.code
	mult proc

		; rcx == __int64 a
		; rdx == __int64 b
		; r8  == __int64 *lret
		; xmm0 == double *dret

		mov	rax,rcx
		mov	r10,rdx
		push rdx ; imul stores result into rdx
		imul	r10
		pop	rdx

		jo	ovfl
ok:
		mov	[r8],rax
		xor	rax,rax
		jmp	done
ovfl:
		cvtsi2sd	xmm0,rdx
		cvtsi2sd	xmm1,rcx
		mulsd	xmm0,xmm1

		movsd	qword ptr[r9],xmm0		

		mov rax,1
done:
		ret
	mult endp
end
