	.file	"scale.c"
	.text
	.globl	scale
	.type	scale, @function
scale:
.LFB23:
	.cfi_startproc
	leaq	(%rdi,%rsi,4), %rax
	leaq	(%rdx,%rdx,2), %rcx
	leaq	0(,%rcx,4), %rdx
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE23:
	.size	scale, .-scale
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
