	.file	"check.c"
	.text
	.globl	check
	.type	check, @function
check:
.LFB0:
	.cfi_startproc
	movl	$0, %eax
	jmp	.L2
.L4:
	movq	8(%rdi,%rax,8), %rcx
	cmpq	%rcx, (%rdi,%rax,8)
	jg	.L5
	addq	$1, %rax
.L2:
	leaq	-1(%rsi), %rdx
	cmpq	%rdx, %rax
	jl	.L4
	movl	$1, %eax
	ret
.L5:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	check, .-check
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
