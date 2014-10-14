
.code
	mult proc

		; rcx == __int64 a
		; rdx == __int64 b
		; r8  == __int64 *lret
		; xmm0 == double *dret

		push	rdx ; imul stores result into rdx
		imul	rdx,rcx

		jo	ovfl
ok:
		mov	[r8],rdx
		xor	rax,rax
		pop	rdx
		jmp	done
ovfl:
		pop	rdx
		cvtsi2sd	xmm0,rdx
		cvtsi2sd	xmm1,rcx
		mulsd	xmm0,xmm1

		movsd	qword ptr[r9],xmm0		

		mov rax,1
done:
		ret
	mult endp
end
