	.file	"driver.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%ld: %10ld %016lx\n"
	.text
	.globl	print
	.type	print, @function
print:
.LFB13:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	movq	%rsi, %rbp
	movl	$0, %ebx
	jmp	.L2
.L3:
	movq	(%r12,%rbx,8), %rcx
	movq	%rcx, %r8
	movq	%rbx, %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	addq	$1, %rbx
.L2:
	cmpq	%rbp, %rbx
	jl	.L3
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	print, .-print
	.globl	main
	.type	main, @function
main:
.LFB14:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	movq	$4095, (%rsp)
	movq	$3872, 8(%rsp)
	movq	$-187, 16(%rsp)
	movq	$-1, 24(%rsp)
	movq	$1, 32(%rsp)
	movq	$3, 40(%rsp)
	movq	$4, 48(%rsp)
	movq	$10, 56(%rsp)
	movq	$9, 64(%rsp)
	movq	$17, 72(%rsp)
	movl	$10, %esi
	movq	%rsp, %rdi
	call	sort
	movl	$10, %esi
	movq	%rsp, %rdi
	call	print
	movq	88(%rsp), %rdx
	xorq	%fs:40, %rdx
	je	.L6
	call	__stack_chk_fail
.L6:
	movl	$0, %eax
	addq	$104, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE14:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
