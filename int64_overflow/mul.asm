
.code
	mult_qWord proc
        
			xor eax,eax
			cvtsi2sd xmm0,rcx
			cvtsi2sd xmm1,rdx
			imul rcx,rdx
			mulsd xmm0,xmm1
			mov [r8],rcx
			movsd real8 ptr [r9],xmm0
			jno @NOF
			xor eax,1			; integer overflow -> rax = 1
	@NOF:   ret
	mult_qWord endp

	mult proc

		; rcx == __int64 a
		; rdx == __int64 b
		; r8  == __int64 *lret
		; r9  == double *dret

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

	int64_mul_ovfl proc
		; rcx == __int64 a
		; rdx == __int64 b
		; r8  == __int64 *lret

		xor	rax,rax
		push	rdx
		imul	rdx,rcx
		mov	qword ptr[r8],rdx
		pop	rdx

		jno	done
		add	rax,1
done:
		ret
	int64_mul_ovfl endp
end
