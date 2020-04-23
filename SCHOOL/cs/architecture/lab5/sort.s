	.file	"sort.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	.cfi_startproc
	leaq	-1(%rsi), %r9
	jmp	.L2
.L4:
	leaq	8(%rdi,%rax,8), %rsi
	movq	(%rsi), %rcx
	leaq	(%rdi,%rax,8), %rdx
	movq	(%rdx), %r8
	cmpq	%r8, %rcx
	jge	.L3
	movq	%r8, (%rsi)
	movq	%rcx, (%rdx)
.L3:
	addq	$1, %rax
	jmp	.L5
.L6:
	movl	$0, %eax
.L5:
	cmpq	%rax, %r9
	jg	.L4
	subq	$1, %r9
.L2:
	testq	%r9, %r9
	jg	.L6
	rep ret
	.cfi_endproc
.LFE0:
	.size	sort, .-sort
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
