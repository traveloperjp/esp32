	.file	"test2.c"
	.text
	.globl	plus                            // -- Begin function plus
	.p2align	2
	.type	plus,@function
plus:                                   // @plus
	.cfi_startproc
// %bb.0:
	add	w0, w1, w0
	ret
.Lfunc_end0:
	.size	plus, .Lfunc_end0-plus
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	mov	w0, #7                          // =0x7
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        // -- End function
	.ident	"Apple clang version 21.0.0 (clang-2100.3.34.2)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
