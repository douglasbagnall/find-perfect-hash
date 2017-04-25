	.file	"find-perfect-hash.c"
# GNU C11 (GCC) version 7.0.0 20161108 (experimental) (x86_64-pc-linux-gnu)
#	compiled by GNU C version 7.0.0 20161108 (experimental), GMP version 6.0.0, MPFR version 3.1.2-p3, MPC version 1.0.2, isl version none
# warning: GMP header version 6.0.0 differs from library version 6.1.2.
# warning: MPFR header version 3.1.2-p3 differs from library version 3.1.5.
# warning: MPC header version 1.0.2 differs from library version 1.0.3.
# GGC heuristics: --param ggc-min-expand=30 --param ggc-min-heapsize=4096
# options passed:  -imultiarch x86_64-linux-gnu find-perfect-hash.c
# -mtune=generic -march=x86-64 -O3 -Wall -fverbose-asm
# options enabled:  -faggressive-loop-optimizations -falign-labels
# -fasynchronous-unwind-tables -fauto-inc-dec -fbranch-count-reg
# -fcaller-saves -fchkp-check-incomplete-type -fchkp-check-read
# -fchkp-check-write -fchkp-instrument-calls -fchkp-narrow-bounds
# -fchkp-optimize -fchkp-store-bounds -fchkp-use-static-bounds
# -fchkp-use-static-const-bounds -fchkp-use-wrappers -fcode-hoisting
# -fcombine-stack-adjustments -fcommon -fcompare-elim -fcprop-registers
# -fcrossjumping -fcse-follow-jumps -fdefer-pop
# -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively
# -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
# -fexpensive-optimizations -fforward-propagate -ffp-int-builtin-inexact
# -ffunction-cse -fgcse -fgcse-after-reload -fgcse-lm -fgnu-runtime
# -fgnu-unique -fguess-branch-probability -fhoist-adjacent-loads -fident
# -fif-conversion -fif-conversion2 -findirect-inlining -finline
# -finline-atomics -finline-functions -finline-functions-called-once
# -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf
# -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
# -fipa-ra -fipa-reference -fipa-sra -fipa-vrp -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots
# -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
# -fleading-underscore -flifetime-dse -flra-remat -flto-odr-type-merging
# -fmath-errno -fmerge-constants -fmerge-debug-strings
# -fmove-loop-invariants -fomit-frame-pointer -foptimize-sibling-calls
# -foptimize-strlen -fpartial-inlining -fpeel-loops -fpeephole -fpeephole2
# -fplt -fpredictive-commoning -fprefetch-loop-arrays -free
# -freg-struct-return -freorder-blocks -freorder-functions
# -frerun-cse-after-loop -fsched-critical-path-heuristic
# -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
# -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
# -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
# -fschedule-insns2 -fsemantic-interposition -fshow-column -fshrink-wrap
# -fshrink-wrap-separate -fsigned-zeros -fsplit-ivs-in-unroller
# -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-backprop
# -fssa-phiopt -fstdarg-opt -fstore-merging -fstrict-aliasing
# -fstrict-overflow -fstrict-volatile-bitfields -fsync-libcalls
# -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
# -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
# -ftree-copy-prop -ftree-cselim -ftree-dce -ftree-dominator-opts
# -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-distribute-patterns
# -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-loop-vectorize -ftree-parallelize-loops=
# -ftree-partial-pre -ftree-phiprop -ftree-pre -ftree-pta -ftree-reassoc
# -ftree-scev-cprop -ftree-sink -ftree-slp-vectorize -ftree-slsr -ftree-sra
# -ftree-switch-conversion -ftree-tail-merge -ftree-ter -ftree-vrp
# -funit-at-a-time -funswitch-loops -funwind-tables -fverbose-asm
# -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -malign-stringops -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387 -mfxsr
# -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone
# -msse -msse2 -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
	.p2align 4,,15
	.globl	cmp_uint32
	.type	cmp_uint32, @function
cmp_uint32:
.LFB43:
	.cfi_startproc
# find-perfect-hash.c:448: 	return *(uint32_t *)a - *(uint32_t *)b;
	movl	(%rdi), %eax	# MEM[(uint32_t *)a_5(D)], MEM[(uint32_t *)a_5(D)]
	subl	(%rsi), %eax	# MEM[(uint32_t *)b_6(D)], tmp93
# find-perfect-hash.c:449: }
	ret
	.cfi_endproc
.LFE43:
	.size	cmp_uint32, .-cmp_uint32
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	" mean %u stddev %u\n"
.LC5:
	.string	"range_top %u\n"
.LC6:
	.string	"\033[00;37m"
.LC7:
	.string	"%3u+ %5u \033[00;33m"
.LC8:
	.string	"%4u %5u \033[00;33m"
.LC9:
	.string	"\033[00m"
.LC10:
	.string	"buckets size %u\n"
	.text
	.p2align 4,,15
	.type	summarise_all_columns, @function
summarise_all_columns:
.LFB47:
	.cfi_startproc
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pxor	%xmm6, %xmm6	# vect_sum2_103.131
	pushq	%r14	#
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13	#
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	leaq	12(%rdi), %r13	#, ivtmp.158
	pushq	%r12	#
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp	#
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	leaq	15948(%rdi), %rdx	#, _334
	pushq	%rbx	#
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movdqa	%xmm6, %xmm5	#, vect_sum_102.126
	movdqa	%xmm6, %xmm3	# tmp26, vect_max_collisions_100.120
	movq	%rdi, %r9	# columns, columns
	subq	$56, %rsp	#,
	.cfi_def_cfa_offset 112
# find-perfect-hash.c:543: {
	movq	%r13, %rax	# ivtmp.158, ivtmp.165
	movdqa	.LC0(%rip), %xmm7	#, vect_min_collisions_101.123
	movdqa	.LC1(%rip), %xmm4	#, tmp481
	.p2align 4,,10
	.p2align 3
.L3:
	addq	$64, %rax	#, ivtmp.165
# find-perfect-hash.c:556: 		c = columns[i].collisions;
	movdqu	-48(%rax), %xmm2	# MEM[base: _287, offset: 16B], tmp486
	movdqu	-64(%rax), %xmm0	# MEM[base: _287, offset: 0B], vect_c_99.116
	movdqu	-32(%rax), %xmm1	# MEM[base: _287, offset: 32B], vect_c_99.118
	shufps	$136, %xmm2, %xmm0	#, tmp486, tmp295
	movdqu	-16(%rax), %xmm2	# MEM[base: _287, offset: 48B], tmp487
	cmpq	%rdx, %rax	# _334, ivtmp.165
	shufps	$136, %xmm2, %xmm1	#, tmp487, tmp298
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	movdqa	%xmm3, %xmm2	# vect_max_collisions_100.120, tmp307
# find-perfect-hash.c:556: 		c = columns[i].collisions;
	shufps	$136, %xmm1, %xmm0	#, tmp298, vect_perm_even_186
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	psubd	%xmm4, %xmm2	# tmp481, tmp307
	movdqa	%xmm0, %xmm1	# vect_perm_even_186, tmp308
# find-perfect-hash.c:559: 		sum += c;
	paddd	%xmm0, %xmm5	# vect_perm_even_186, vect_sum_102.126
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	psubd	%xmm4, %xmm1	# tmp481, tmp308
	pcmpgtd	%xmm1, %xmm2	# tmp308, tmp309
	pand	%xmm2, %xmm3	# tmp309, tmp310
	pandn	%xmm0, %xmm2	# vect_perm_even_186, tmp311
	por	%xmm2, %xmm3	# tmp311, vect_max_collisions_100.120
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	movdqa	%xmm7, %xmm2	# vect_min_collisions_101.123, tmp321
	psubd	%xmm4, %xmm2	# tmp481, tmp321
	pcmpgtd	%xmm2, %xmm1	# tmp321, tmp322
	pand	%xmm1, %xmm7	# tmp322, tmp323
	pandn	%xmm0, %xmm1	# vect_perm_even_186, tmp324
	por	%xmm1, %xmm7	# tmp324, vect_min_collisions_101.123
# find-perfect-hash.c:560: 		sum2 += c * c;
	movdqa	%xmm0, %xmm1	# vect_perm_even_186, tmp328
	pmuludq	%xmm0, %xmm1	# vect_perm_even_186, tmp328
	psrlq	$32, %xmm0	#, tmp330
	pshufd	$8, %xmm1, %xmm1	#, tmp328, tmp326
	pmuludq	%xmm0, %xmm0	# tmp330, tmp329
	pshufd	$8, %xmm0, %xmm0	#, tmp329, tmp327
	punpckldq	%xmm0, %xmm1	# tmp327, tmp326
	paddd	%xmm1, %xmm6	# vect__4.130, vect_sum2_103.131
	jne	.L3	#,
	movdqa	%xmm6, %xmm0	# vect_sum2_103.131, tmp332
# find-perfect-hash.c:556: 		c = columns[i].collisions;
	movl	15948(%r9), %r10d	# MEM[(struct columns *)columns_93(D) + 15936B].collisions, c
	movdqa	%xmm7, %xmm1	# vect_min_collisions_101.123, tmp336
	movl	15964(%r9), %r8d	# MEM[(struct columns *)columns_93(D) + 15952B].collisions, c
	psrldq	$8, %xmm0	#, tmp332
	paddd	%xmm0, %xmm6	# tmp332, vect_sum2_103.133
	movdqa	%xmm5, %xmm0	# vect_sum_102.126, tmp334
	psrldq	$8, %xmm1	#, tmp336
	movdqa	%xmm1, %xmm9	# tmp336, tmp339
	movdqa	%xmm3, %xmm10	# vect_max_collisions_100.120, tmp347
	psrldq	$8, %xmm0	#, tmp334
	paddd	%xmm0, %xmm5	# tmp334, vect_sum_102.128
	movdqa	%xmm7, %xmm0	# vect_min_collisions_101.123, tmp338
	psubd	%xmm4, %xmm9	# tmp481, tmp339
	psubd	%xmm4, %xmm10	# tmp481, tmp347
	movl	15980(%r9), %edi	# MEM[(struct columns *)columns_93(D) + 15968B].collisions, c
	psubd	%xmm4, %xmm0	# tmp481, tmp338
	pcmpgtd	%xmm9, %xmm0	# tmp339, tmp340
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	cmpl	%r8d, %r10d	# c, c
	movl	%r8d, %r12d	# c, tmp354
# find-perfect-hash.c:556: 		c = columns[i].collisions;
	movl	15996(%r9), %esi	# MEM[(struct columns *)columns_93(D) + 15984B].collisions, c
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	movl	%r8d, %r15d	# c, tmp365
	pand	%xmm0, %xmm1	# tmp340, tmp341
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	cmovnb	%r10d, %r12d	# c,, tmp354
	pandn	%xmm7, %xmm0	# vect_min_collisions_101.123, tmp342
	cmpl	%edi, %r12d	# c, tmp354
	cmovb	%edi, %r12d	# tmp354,, c, tmp353
# find-perfect-hash.c:559: 		sum += c;
	leal	(%r10,%r8), %ecx	#, tmp374
# find-perfect-hash.c:562: 	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	movl	$274877907, %ebx	#, tmp381
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	cmpl	%esi, %r12d	# c, tmp353
	movdqa	%xmm5, %xmm8	# vect_sum_102.128, tmp335
	movdqa	%xmm0, %xmm7	# tmp342, tmp342
	cmovb	%esi, %r12d	# tmp353,, c, tmp352
	movdqa	%xmm3, %xmm0	# vect_max_collisions_100.120, tmp344
	psrldq	$4, %xmm8	#, tmp335
# find-perfect-hash.c:559: 		sum += c;
	paddd	%xmm8, %xmm5	# tmp335, vect_sum_102.128
	por	%xmm1, %xmm7	# tmp341, tmp342
	movdqa	%xmm6, %xmm2	# vect_sum2_103.133, tmp333
	psrldq	$8, %xmm0	#, tmp344
	movdqa	%xmm0, %xmm1	# tmp344, tmp346
	psrldq	$4, %xmm2	#, tmp333
# find-perfect-hash.c:563: 	variance = ((sum2 - sum * sum  + N_CANDIDATES / 2) /
	paddd	%xmm2, %xmm6	# tmp333, vect_sum2_103.133
	psubd	%xmm4, %xmm1	# tmp481, tmp346
	pcmpgtd	%xmm10, %xmm1	# tmp347, tmp348
	movdqa	%xmm0, %xmm10	# tmp344, tmp349
	movdqa	%xmm7, %xmm9	# vect_min_collisions_101.125, tmp343
	movdqa	%xmm1, %xmm0	# tmp348, tmp348
	pand	%xmm1, %xmm10	# tmp348, tmp349
	psrldq	$4, %xmm9	#, tmp343
	pandn	%xmm3, %xmm0	# vect_max_collisions_100.120, tmp348
	por	%xmm10, %xmm0	# tmp349, vect_max_collisions_100.122
	movdqa	%xmm0, %xmm3	# vect_max_collisions_100.122, tmp351
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	movdqa	%xmm0, %xmm1	# vect_max_collisions_100.122, tmp357
	psrldq	$4, %xmm3	#, tmp351
	movdqa	%xmm3, %xmm10	# tmp351, tmp358
	psubd	%xmm4, %xmm1	# tmp481, tmp357
	psubd	%xmm4, %xmm10	# tmp481, tmp358
	pcmpgtd	%xmm10, %xmm1	# tmp358, tmp359
	pand	%xmm1, %xmm0	# tmp359, tmp360
	pandn	%xmm3, %xmm1	# tmp351, tmp361
	por	%xmm1, %xmm0	# tmp361, vect_max_collisions_100.122
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	movdqa	%xmm9, %xmm1	# tmp343, tmp369
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	movd	%xmm0, %eax	# vect_max_collisions_100.122, stmp_max_collisions_100.121
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	movdqa	%xmm7, %xmm0	# vect_min_collisions_101.125, tmp368
	psubd	%xmm4, %xmm1	# tmp481, tmp369
	psubd	%xmm4, %xmm0	# tmp481, tmp368
	pcmpgtd	%xmm1, %xmm0	# tmp369, tmp370
# find-perfect-hash.c:557: 		max_collisions = MAX(c, max_collisions);
	cmpl	%eax, %r12d	# stmp_max_collisions_100.121, tmp352
	cmovb	%eax, %r12d	# tmp352,, stmp_max_collisions_100.121, max_collisions
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	cmpl	%r8d, %r10d	# c, c
	pand	%xmm0, %xmm9	# tmp370, tmp371
	cmovbe	%r10d, %r15d	# c,, tmp365
	pandn	%xmm7, %xmm0	# vect_min_collisions_101.125, tmp372
	cmpl	%edi, %r15d	# c, tmp365
	cmova	%edi, %r15d	# tmp365,, c, tmp364
	pxor	%xmm1, %xmm1	# tmp398
	cmpl	%esi, %r15d	# c, tmp364
	por	%xmm9, %xmm0	# tmp371, vect_min_collisions_101.125
	cmova	%esi, %r15d	# tmp364,, c, tmp363
	movd	%xmm0, %eax	# vect_min_collisions_101.125, stmp_min_collisions_101.124
# find-perfect-hash.c:565: 	stddev = sqrtf(variance) + 0.5;
	pxor	%xmm0, %xmm0	# _9
# find-perfect-hash.c:558: 		min_collisions = MIN(c, min_collisions);
	cmpl	%eax, %r15d	# stmp_min_collisions_101.124, tmp363
	cmova	%eax, %r15d	# tmp363,, stmp_min_collisions_101.124, min_collisions
# find-perfect-hash.c:559: 		sum += c;
	addl	%edi, %ecx	# c, tmp375
	movd	%xmm5, %eax	# vect_sum_102.128, stmp_sum_102.127
	addl	%esi, %ecx	# c, tmp376
	addl	%eax, %ecx	# stmp_sum_102.127, sum
# find-perfect-hash.c:562: 	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	leal	500(%rcx), %edx	#, tmp379
# find-perfect-hash.c:560: 		sum2 += c * c;
	imull	%r10d, %r10d	# c, tmp382
# find-perfect-hash.c:562: 	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	movl	%edx, %eax	# tmp379, tmp379
	mull	%ebx	# tmp381
# find-perfect-hash.c:563: 	variance = ((sum2 - sum * sum  + N_CANDIDATES / 2) /
	movd	%xmm6, %eax	# vect_sum2_103.133, stmp_sum2_103.132
# find-perfect-hash.c:560: 		sum2 += c * c;
	imull	%r8d, %r8d	# c, tmp383
# find-perfect-hash.c:562: 	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	movl	%edx, %ebx	# tmp380, tmp380
# find-perfect-hash.c:560: 		sum2 += c * c;
	imull	%edi, %edi	# c, tmp386
# find-perfect-hash.c:563: 	variance = ((sum2 - sum * sum  + N_CANDIDATES / 2) /
	leal	500(%r10,%r8), %edx	#, tmp385
# find-perfect-hash.c:562: 	mean = (sum + N_CANDIDATES / 2) / N_CANDIDATES;
	shrl	$6, %ebx	#, tmp380
# find-perfect-hash.c:560: 		sum2 += c * c;
	imull	%esi, %esi	# c, tmp388
# find-perfect-hash.c:563: 	variance = ((sum2 - sum * sum  + N_CANDIDATES / 2) /
	addl	%edi, %edx	# tmp386, tmp387
	imull	%ecx, %ecx	# sum, tmp393
	addl	%esi, %edx	# tmp388, tmp389
	addl	%eax, %edx	# stmp_sum2_103.132, tmp392
	subl	%ecx, %edx	# tmp393, tmp394
	movl	$1125899907, %ecx	#, tmp397
	movl	%edx, %eax	# tmp394, tmp394
	mull	%ecx	# tmp397
	shrl	$18, %edx	#, variance
# find-perfect-hash.c:565: 	stddev = sqrtf(variance) + 0.5;
	cvtsi2ss	%edx, %xmm0	# variance, _9
	ucomiss	%xmm0, %xmm1	# _9, tmp398
	sqrtss	%xmm0, %xmm5	# _9, _10
	movss	%xmm5, 40(%rsp)	# _10, %sfp
	jbe	.L4	#,
	movaps	%xmm4, (%rsp)	# tmp481, %sfp
	movq	%r9, 32(%rsp)	# columns, %sfp
	call	sqrtf	#
	movq	32(%rsp), %r9	# %sfp, columns
	movdqa	(%rsp), %xmm4	# %sfp, tmp481
.L4:
	pxor	%xmm0, %xmm0	# tmp399
# find-perfect-hash.c:567: 	printf(" mean %u stddev %u\n", mean, stddev);
	movl	%ebx, %esi	# mean,
	xorl	%eax, %eax	#
	movl	$.LC4, %edi	#,
	movq	%r9, 32(%rsp)	# columns, %sfp
	movaps	%xmm4, (%rsp)	# tmp481, %sfp
# find-perfect-hash.c:565: 	stddev = sqrtf(variance) + 0.5;
	cvtss2sd	40(%rsp), %xmm0	# %sfp, tmp399
	addsd	.LC3(%rip), %xmm0	#, tmp400
	cvttsd2siq	%xmm0, %rbp	# tmp400, tmp402
# find-perfect-hash.c:567: 	printf(" mean %u stddev %u\n", mean, stddev);
	movl	%ebp, %edx	# tmp402,
	call	printf	#
# find-perfect-hash.c:569: 	range_top = MIN(max_collisions, mean + 4 * stddev);
	leal	(%rbx,%rbp,4), %eax	#, _14
# find-perfect-hash.c:572: 	step = MAX(range / 40, 1);
	movq	32(%rsp), %r9	# %sfp, columns
	movl	$1, %ebx	#, iftmp.47_63
	movdqa	(%rsp), %xmm4	# %sfp, tmp481
# find-perfect-hash.c:569: 	range_top = MIN(max_collisions, mean + 4 * stddev);
	cmpl	%r12d, %eax	# max_collisions, _14
	movl	%eax, %r10d	# _14, range_top
	movl	%eax, 44(%rsp)	# _14, %sfp
	cmova	%r12d, %r10d	# range_top,, max_collisions, range_top
# find-perfect-hash.c:570: 	range = range_top - min_collisions;
	movl	%r10d, %ebp	# range_top, range
	subl	%r15d, %ebp	# min_collisions, range
# find-perfect-hash.c:572: 	step = MAX(range / 40, 1);
	cmpl	$39, %ebp	#, range
	jbe	.L5	#,
# find-perfect-hash.c:572: 	step = MAX(range / 40, 1);
	movl	%ebp, %eax	# range, tmp485
	movl	$-858993459, %edx	#, tmp405
	mull	%edx	# tmp405
	movl	%ebp, %eax	# range, range
	movl	%edx, %ebx	# tmp404, tmp404
	xorl	%edx, %edx	# tmp407
	shrl	$5, %ebx	#, tmp404
	divl	%ebx	# iftmp.47_63
	movl	%eax, %ebp	# range, range
.L5:
# find-perfect-hash.c:574: 	counts = calloc(n_buckets + 1, sizeof(counts[0]));
	leal	2(%rbp), %edi	#, tmp408
# find-perfect-hash.c:573: 	n_buckets = range / step + 1;
	leal	1(%rbp), %r8d	#, _16
# find-perfect-hash.c:574: 	counts = calloc(n_buckets + 1, sizeof(counts[0]));
	movl	$4, %esi	#,
	movaps	%xmm4, 16(%rsp)	# tmp481, %sfp
	movslq	%edi, %rdi	# tmp408, tmp409
	movq	%r9, (%rsp)	# columns, %sfp
# find-perfect-hash.c:573: 	n_buckets = range / step + 1;
	movl	%r8d, 40(%rsp)	# _16, %sfp
	movl	%r10d, 32(%rsp)	# range_top, %sfp
# find-perfect-hash.c:574: 	counts = calloc(n_buckets + 1, sizeof(counts[0]));
	call	calloc	#
# find-perfect-hash.c:576: 	printf("range_top %u\n", range_top);
	movl	32(%rsp), %r10d	# %sfp, range_top
# find-perfect-hash.c:574: 	counts = calloc(n_buckets + 1, sizeof(counts[0]));
	movq	%rax, %r14	#, counts
# find-perfect-hash.c:576: 	printf("range_top %u\n", range_top);
	movl	$.LC5, %edi	#,
	xorl	%eax, %eax	#
	movl	%r10d, %esi	# range_top,
	call	printf	#
	movq	(%rsp), %r9	# %sfp, columns
	movl	40(%rsp), %r8d	# %sfp, _16
	movdqa	16(%rsp), %xmm4	# %sfp, tmp481
	leaq	16012(%r9), %rsi	#, _301
	.p2align 4,,10
	.p2align 3
.L6:
# find-perfect-hash.c:580: 		j = (c - min_collisions) / step;
	movl	0(%r13), %eax	# MEM[base: _303, offset: 0B], tmp411
	xorl	%edx, %edx	# tmp414
	subl	%r15d, %eax	# min_collisions, tmp411
	divl	%ebx	# iftmp.47_63
# find-perfect-hash.c:584: 		counts[j]++;
	cmpl	%eax, %r8d	# tmp413, _16
	cmovle	%ebp, %eax	# tmp413,, range, j
	addq	$16, %r13	#, ivtmp.158
	cltq
	addl	$1, (%r14,%rax,4)	#, *_26
# find-perfect-hash.c:578: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpq	%r13, %rsi	# ivtmp.158, _301
	jne	.L6	#,
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	testl	%r8d, %r8d	# _16
	jle	.L7	#,
	movq	%r14, %rdx	# counts, tmp418
	shrq	$2, %rdx	#, tmp418
	negq	%rdx	# tmp419
	andl	$3, %edx	#,
	cmpl	$6, %ebp	#, range
	jbe	.L22	#,
	testl	%edx, %edx	# prolog_loop_niters.100
	je	.L23	#,
	cmpl	$1, %edx	#, prolog_loop_niters.100
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movl	(%r14), %eax	# *counts_80, bucket_max
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	movl	$1, %r9d	#, i
	je	.L9	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movl	4(%r14), %esi	# MEM[(uint32_t *)counts_80 + 4B], MEM[(uint32_t *)counts_80 + 4B]
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	movl	$2, %r9d	#, i
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	cmpl	%esi, %eax	# MEM[(uint32_t *)counts_80 + 4B], bucket_max
	cmovb	%esi, %eax	# bucket_max,, MEM[(uint32_t *)counts_80 + 4B], bucket_max
	cmpl	$3, %edx	#, prolog_loop_niters.100
	jne	.L9	#,
	movl	8(%r14), %esi	# MEM[(uint32_t *)counts_80 + 8B], MEM[(uint32_t *)counts_80 + 8B]
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	movl	$3, %r9d	#, i
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	cmpl	%esi, %eax	# MEM[(uint32_t *)counts_80 + 8B], bucket_max
	cmovb	%esi, %eax	# bucket_max,, MEM[(uint32_t *)counts_80 + 8B], bucket_max
.L9:
	movl	%eax, 40(%rsp)	# bucket_max, %sfp
	movl	%r8d, %esi	# _16, niters.102
	leaq	(%r14,%rdx,4), %rdi	#, ivtmp.151
	movd	40(%rsp), %xmm5	# %sfp, bucket_max
	subl	%edx, %esi	# prolog_loop_niters.100, niters.102
# find-perfect-hash.c:586: 	bucket_max = 0;
	xorl	%edx, %edx	# ivtmp.146
	movl	%esi, %r10d	# niters.102, bnd.103
	pshufd	$0, %xmm5, %xmm0	# bucket_max, vect_bucket_max_91.109
	shrl	$2, %r10d	#, bnd.103
.L11:
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movdqa	(%rdi), %xmm2	# MEM[base: _319, offset: 0B], MEM[base: _319, offset: 0B]
	movdqa	%xmm0, %xmm1	# vect_bucket_max_91.109, tmp435
	addl	$1, %edx	#, ivtmp.146
	addq	$16, %rdi	#, ivtmp.151
	movdqa	%xmm2, %xmm3	# MEM[base: _319, offset: 0B], tmp436
	psubd	%xmm4, %xmm1	# tmp481, tmp435
	cmpl	%r10d, %edx	# bnd.103, ivtmp.146
	psubd	%xmm4, %xmm3	# tmp481, tmp436
	pcmpgtd	%xmm3, %xmm1	# tmp436, tmp437
	pand	%xmm1, %xmm0	# tmp437, tmp438
	pandn	%xmm2, %xmm1	# MEM[base: _319, offset: 0B], tmp439
	por	%xmm1, %xmm0	# tmp439, vect_bucket_max_91.109
	jb	.L11	#,
	movdqa	%xmm0, %xmm2	# vect_bucket_max_91.109, tmp440
	movl	%esi, %edx	# niters.102, niters_vector_mult_vf.104
	movdqa	%xmm0, %xmm3	# vect_bucket_max_91.109, tmp443
	andl	$-4, %edx	#, niters_vector_mult_vf.104
	psrldq	$8, %xmm2	#, tmp440
	movdqa	%xmm2, %xmm1	# tmp440, tmp442
	cmpl	%esi, %edx	# niters.102, niters_vector_mult_vf.104
	psubd	%xmm4, %xmm3	# tmp481, tmp443
	leal	(%rdx,%r9), %eax	#, tmp.105
	psubd	%xmm4, %xmm1	# tmp481, tmp442
	pcmpgtd	%xmm3, %xmm1	# tmp443, tmp444
	pand	%xmm1, %xmm2	# tmp444, tmp445
	pandn	%xmm0, %xmm1	# vect_bucket_max_91.109, tmp444
	movdqa	%xmm1, %xmm0	# tmp444, tmp446
	por	%xmm2, %xmm0	# tmp445, vect_bucket_max_91.111
	movdqa	%xmm0, %xmm2	# vect_bucket_max_91.111, tmp447
	movdqa	%xmm0, %xmm3	# vect_bucket_max_91.111, tmp451
	psrldq	$4, %xmm2	#, tmp447
	movdqa	%xmm2, %xmm1	# tmp447, tmp450
	psubd	%xmm4, %xmm3	# tmp481, tmp451
	psubd	%xmm4, %xmm1	# tmp481, tmp450
	pcmpgtd	%xmm3, %xmm1	# tmp451, tmp452
	pand	%xmm1, %xmm2	# tmp452, tmp453
	pandn	%xmm0, %xmm1	# vect_bucket_max_91.111, tmp454
	por	%xmm2, %xmm1	# tmp453, vect_bucket_max_91.111
	movd	%xmm1, 32(%rsp)	# vect_bucket_max_91.111, %sfp
	je	.L12	#,
.L8:
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%eax, %rdx	# tmp.105, tmp.105
	movl	32(%rsp), %edi	# %sfp, stmp_bucket_max_91.110
	movl	(%r14,%rdx,4), %edx	# *_7, *_7
	cmpl	%edx, %edi	# *_7, stmp_bucket_max_91.110
	movl	%edx, %ecx	# *_7, *_7
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	leal	1(%rax), %edx	#, i
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	cmovnb	%edi, %ecx	# stmp_bucket_max_91.110,, *_7
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	cmpl	%edx, %r8d	# i, _16
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movl	%ecx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	jle	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%edx, %rdx	# i, i
	movl	(%r14,%rdx,4), %edx	# *_131, *_131
	cmpl	%edx, %ecx	# *_131, stmp_bucket_max_91.110
	cmovnb	%ecx, %edx	# stmp_bucket_max_91.110,, *_131
	movl	%edx, %ecx	# *_131, stmp_bucket_max_91.110
	movl	%edx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	leal	2(%rax), %edx	#, i
	cmpl	%r8d, %edx	# _16, i
	jge	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%edx, %rdx	# i, i
	movl	(%r14,%rdx,4), %edx	# *_174, *_174
	cmpl	%edx, %ecx	# *_174, stmp_bucket_max_91.110
	cmovnb	%ecx, %edx	# stmp_bucket_max_91.110,, *_174
	movl	%edx, %ecx	# *_174, stmp_bucket_max_91.110
	movl	%edx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	leal	3(%rax), %edx	#, i
	cmpl	%edx, %r8d	# i, _16
	jle	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%edx, %rdx	# i, i
	movl	(%r14,%rdx,4), %edx	# *_193, *_193
	cmpl	%edx, %ecx	# *_193, stmp_bucket_max_91.110
	cmovnb	%ecx, %edx	# stmp_bucket_max_91.110,, *_193
	movl	%edx, %ecx	# *_193, stmp_bucket_max_91.110
	movl	%edx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	leal	4(%rax), %edx	#, i
	cmpl	%r8d, %edx	# _16, i
	jge	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%edx, %rdx	# i, i
	movl	(%r14,%rdx,4), %edx	# *_251, *_251
	cmpl	%edx, %ecx	# *_251, stmp_bucket_max_91.110
	cmovnb	%ecx, %edx	# stmp_bucket_max_91.110,, *_251
	movl	%edx, %ecx	# *_251, stmp_bucket_max_91.110
	movl	%edx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	leal	5(%rax), %edx	#, i
	cmpl	%edx, %r8d	# i, _16
	jle	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movslq	%edx, %rdx	# i, i
	movl	(%r14,%rdx,4), %edx	# *_259, *_259
	cmpl	%edx, %ecx	# *_259, stmp_bucket_max_91.110
	cmovnb	%ecx, %edx	# stmp_bucket_max_91.110,, *_259
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	addl	$6, %eax	#, i
	cmpl	%eax, %r8d	# i, _16
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	movl	%edx, 32(%rsp)	# stmp_bucket_max_91.110, %sfp
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	jle	.L12	#,
# find-perfect-hash.c:588: 		bucket_max = MAX(bucket_max, counts[i]);
	cltq
	movl	(%r14,%rax,4), %eax	# *_156, *_156
	cmpl	%eax, %edx	# *_156, stmp_bucket_max_91.110
	cmovnb	%edx, %eax	# stmp_bucket_max_91.110,, *_156
	movl	%eax, 32(%rsp)	# *_156, %sfp
.L12:
# find-perfect-hash.c:590: 	printf("buckets size %u\n", step);
	xorl	%eax, %eax	#
	movl	%ebx, %esi	# iftmp.47_63,
	movl	$.LC10, %edi	#,
	call	printf	#
# find-perfect-hash.c:595: 		uint32_t centre = min_collisions + i * step + step / 2;
	movl	%ebx, %eax	# iftmp.47_63, tmp478
	shrl	%eax	# tmp478
	addl	%r15d, %eax	# min_collisions, ivtmp.144
	cmpl	%r12d, 44(%rsp)	# max_collisions, %sfp
	movl	%eax, 40(%rsp)	# ivtmp.144, %sfp
	movl	%ebp, %eax	# range, range
	setb	16(%rsp)	#, %sfp
	addq	$1, %rax	#, _111
	xorl	%r15d, %r15d	# ivtmp.140
	movq	%rax, (%rsp)	# _111, %sfp
	.p2align 4,,10
	.p2align 3
.L19:
# find-perfect-hash.c:592: 		if (counts[i] == 0) {
	movl	(%r14,%r15,4), %r12d	# MEM[base: counts_80, index: ivtmp.140_202, step: 4, offset: 0B], _36
	testl	%r12d, %r12d	# _36
	je	.L41	#,
.L13:
# find-perfect-hash.c:596: 		if (i == n_buckets - 1 && range_top != max_collisions) {
	cmpl	%r15d, %ebp	# ivtmp.140, range
# find-perfect-hash.c:597: 			printf("%3u+ %5u \033[00;33m", centre, counts[i]);
	movl	%r12d, %edx	# _36,
	movl	40(%rsp), %esi	# %sfp,
# find-perfect-hash.c:596: 		if (i == n_buckets - 1 && range_top != max_collisions) {
	jne	.L14	#,
	cmpb	$0, 16(%rsp)	#, %sfp
	je	.L14	#,
# find-perfect-hash.c:597: 			printf("%3u+ %5u \033[00;33m", centre, counts[i]);
	movl	$.LC7, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
	.p2align 4,,10
	.p2align 3
.L17:
# find-perfect-hash.c:601: 		for (j = 0; j < (counts[i] * 80 + 40) / bucket_max; j++) {
	leal	(%r12,%r12,4), %eax	#, tmp473
	xorl	%edx, %edx	# tmp477
	xorl	%r13d, %r13d	# j
	sall	$4, %eax	#, tmp473
	movl	%eax, %r12d	# tmp473, tmp474
	addl	$40, %r12d	#, tmp475
	movl	%r12d, %eax	# tmp475, tmp475
	divl	32(%rsp)	# %sfp
	testl	%eax, %eax	# tmp476
	movl	%eax, %r12d	# tmp475, tmp476
	je	.L16	#,
	.p2align 4,,10
	.p2align 3
.L18:
# /usr/include/x86_64-linux-gnu/bits/stdio.h:81:   return _IO_putc (__c, stdout);
	movq	stdout(%rip), %rsi	# stdout,
	movl	$42, %edi	#,
# find-perfect-hash.c:601: 		for (j = 0; j < (counts[i] * 80 + 40) / bucket_max; j++) {
	addl	$1, %r13d	#, j
# /usr/include/x86_64-linux-gnu/bits/stdio.h:81:   return _IO_putc (__c, stdout);
	call	_IO_putc	#
# find-perfect-hash.c:601: 		for (j = 0; j < (counts[i] * 80 + 40) / bucket_max; j++) {
	cmpl	%r12d, %r13d	# tmp476, j
	jne	.L18	#,
.L16:
# find-perfect-hash.c:604: 		puts("\033[00m");
	movl	$.LC9, %edi	#,
	addq	$1, %r15	#, ivtmp.140
	call	puts	#
	addl	%ebx, 40(%rsp)	# iftmp.47_63, %sfp
# find-perfect-hash.c:591: 	for (i = 0; i < n_buckets; i++) {
	cmpq	%r15, (%rsp)	# ivtmp.140, %sfp
	jne	.L19	#,
.L20:
# find-perfect-hash.c:607: }
	addq	$56, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
# find-perfect-hash.c:606: 	free(counts);
	movq	%r14, %rdi	# counts,
# find-perfect-hash.c:607: }
	popq	%rbx	#
	.cfi_def_cfa_offset 48
	popq	%rbp	#
	.cfi_def_cfa_offset 40
	popq	%r12	#
	.cfi_def_cfa_offset 32
	popq	%r13	#
	.cfi_def_cfa_offset 24
	popq	%r14	#
	.cfi_def_cfa_offset 16
	popq	%r15	#
	.cfi_def_cfa_offset 8
# find-perfect-hash.c:606: 	free(counts);
	jmp	free	#
	.p2align 4,,10
	.p2align 3
.L14:
	.cfi_restore_state
# find-perfect-hash.c:599: 			printf("%4u %5u \033[00;33m", centre, counts[i]);
	movl	$.LC8, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
	jmp	.L17	#
	.p2align 4,,10
	.p2align 3
.L41:
# find-perfect-hash.c:593: 			printf("\033[00;37m");
	movl	$.LC6, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
	jmp	.L13	#
.L23:
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	xorl	%r9d, %r9d	# i
# find-perfect-hash.c:586: 	bucket_max = 0;
	xorl	%eax, %eax	# bucket_max
	jmp	.L9	#
.L7:
# find-perfect-hash.c:590: 	printf("buckets size %u\n", step);
	movl	%ebx, %esi	# iftmp.47_63,
	movl	$.LC10, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
	jmp	.L20	#
.L22:
# find-perfect-hash.c:587: 	for (i = 0; i < n_buckets; i++) {
	xorl	%eax, %eax	# tmp.105
# find-perfect-hash.c:586: 	bucket_max = 0;
	movl	$0, 32(%rsp)	#, %sfp
	jmp	.L8	#
	.cfi_endproc
.LFE47:
	.size	summarise_all_columns, .-summarise_all_columns
	.section	.rodata.str1.1
.LC11:
	.string	"seeding with %lu\n"
	.text
	.p2align 4,,15
	.type	rng_random_init, @function
rng_random_init:
.LFB35:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbp	# rng, rng
# find-perfect-hash.c:57: 	clock_gettime(CLOCK_REALTIME, &t);
	xorl	%edi, %edi	#
# find-perfect-hash.c:53: {
	subq	$24, %rsp	#,
	.cfi_def_cfa_offset 48
# find-perfect-hash.c:57: 	clock_gettime(CLOCK_REALTIME, &t);
	movq	%rsp, %rsi	#,
	call	clock_gettime	#
# find-perfect-hash.c:58: 	seed = ((uint64_t)t.tv_nsec << 20) + t.tv_sec;
	movq	8(%rsp), %rbx	# t.tv_nsec, tmp109
# find-perfect-hash.c:61: 	printf("seeding with %lu\n", seed);
	movl	$.LC11, %edi	#,
	xorl	%eax, %eax	#
# find-perfect-hash.c:58: 	seed = ((uint64_t)t.tv_nsec << 20) + t.tv_sec;
	salq	$20, %rbx	#, tmp109
	addq	(%rsp), %rbx	# t.tv_sec, seed
# find-perfect-hash.c:59: 	seed ^= (uintptr_t)rng;
	xorq	%rbp, %rbx	# rng, seed
# find-perfect-hash.c:60: 	seed += (uintptr_t)&rng_random_init;
	addq	$rng_random_init, %rbx	#, rng__b_lsm.171
# find-perfect-hash.c:61: 	printf("seeding with %lu\n", seed);
	movq	%rbx, %rsi	# rng__b_lsm.171,
	call	printf	#
	movq	%rbx, %rdx	# rng__b_lsm.171, rng__d_lsm.175
	movq	%rbx, %rcx	# rng__b_lsm.171, rng__c_lsm.173
	movl	$20, %esi	#, ivtmp_88
	movl	$4058668781, %eax	#, rng__a_lsm.169
	.p2align 4,,10
	.p2align 3
.L43:
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rbx, %rdi	# rng__b_lsm.171, tmp112
	rolq	$7, %rdi	#, tmp112
	subq	%rdi, %rax	# tmp112, rng__a_lsm.169
	movq	%rax, %rdi	# rng__a_lsm.169, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rcx, %rax	# rng__c_lsm.173, tmp113
	rolq	$13, %rax	#, tmp113
	xorq	%rbx, %rax	# rng__b_lsm.171, rng__a_lsm.169
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rdx, %rbx	# rng__d_lsm.175, tmp114
	rorq	$27, %rbx	#, tmp114
	addq	%rcx, %rbx	# rng__c_lsm.173, rng__b_lsm.171
# find-perfect-hash.c:47: 	for (i = 0; i < 20; ++i) {
	subl	$1, %esi	#, ivtmp_88
# find-perfect-hash.c:37: 	x->c = x->d + e;
	leaq	(%rdi,%rdx), %rcx	#, rng__c_lsm.173
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%rdi,%rax), %rdx	#, rng__d_lsm.175
# find-perfect-hash.c:47: 	for (i = 0; i < 20; ++i) {
	jne	.L43	#,
	movq	%rax, 0(%rbp)	# rng__a_lsm.169, rng_11(D)->a
	movq	%rbx, 8(%rbp)	# rng__b_lsm.171, rng_11(D)->b
	movq	%rcx, 16(%rbp)	# rng__c_lsm.173, rng_11(D)->c
	movq	%rdx, 24(%rbp)	# rng__d_lsm.175, rng_11(D)->d
# find-perfect-hash.c:63: }
	addq	$24, %rsp	#,
	.cfi_def_cfa_offset 24
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE35:
	.size	rng_random_init, .-rng_random_init
	.p2align 4,,15
	.globl	cmp_columns
	.type	cmp_columns, @function
cmp_columns:
.LFB49:
	.cfi_startproc
# find-perfect-hash.c:697: 	if (a == b) {
	cmpq	%rsi, %rdi	# vb, va
	je	.L49	#,
# find-perfect-hash.c:700: 	int c = (int)a->collisions - (int)b->collisions;
	movl	12(%rdi), %eax	# MEM[(struct columns *)va_7(D)].collisions, MEM[(struct columns *)va_7(D)].collisions
# find-perfect-hash.c:701: 	if (c != 0) {
	subl	12(%rsi), %eax	# MEM[(struct columns *)vb_8(D)].collisions, <retval>
	jne	.L46	#,
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	movl	8(%rdi), %edx	# MEM[(struct columns *)va_7(D)].n,
	movl	8(%rsi), %ecx	# MEM[(struct columns *)vb_8(D)].n, _12
# find-perfect-hash.c:707: 	return a->n - b->n;
	movl	%edx, %eax	# _11, <retval>
	subl	%ecx, %eax	# _12, <retval>
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	cmpl	%ecx, %edx	# _12, _11
	je	.L50	#,
.L46:
# find-perfect-hash.c:708: }
	rep ret
	.p2align 4,,10
	.p2align 3
.L49:
# find-perfect-hash.c:698: 		return 0;
	xorl	%eax, %eax	# <retval>
# find-perfect-hash.c:708: }
	ret
	.p2align 4,,10
	.p2align 3
.L50:
# find-perfect-hash.c:705: 		return memcmp(a->bits, b->bits, a->n);
	movq	(%rsi), %rsi	# MEM[(struct columns *)vb_8(D)].bits,
	movq	(%rdi), %rdi	# MEM[(struct columns *)va_7(D)].bits,
	jmp	memcmp	#
	.cfi_endproc
.LFE49:
	.size	cmp_columns, .-cmp_columns
	.section	.rodata.str1.1
.LC12:
	.string	"collisions%4u "
.LC13:
	.string	"bits: "
.LC14:
	.string	"\033[0%d;%dm"
.LC15:
	.string	"%3x "
.LC16:
	.string	"\033[01;33m"
.LC17:
	.string	" | "
.LC18:
	.string	"%22s & (1 << %2u)"
	.text
	.p2align 4,,15
	.type	describe_columns.isra.5, @function
describe_columns.isra.5:
.LFB60:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
# find-perfect-hash.c:455: 	int bright = (c->collisions < 80) ? 1 : 0;
	xorl	%eax, %eax	# bright
# find-perfect-hash.c:451: static void describe_columns(struct hashcontext *ctx,
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r15	#
	pushq	%r14	#
	pushq	%r13	#
	pushq	%r12	#
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	movq	%rdi, %r14	# ISRA.85, ISRA.85
	pushq	%rbx	#
	.cfi_offset 3, -56
	movq	%rsi, %rbx	# c, c
# find-perfect-hash.c:458: 	printf("collisions%4u ", c->collisions);
	movl	$.LC12, %edi	#,
# find-perfect-hash.c:451: static void describe_columns(struct hashcontext *ctx,
	subq	$24, %rsp	#,
# find-perfect-hash.c:455: 	int bright = (c->collisions < 80) ? 1 : 0;
	movl	12(%rsi), %esi	# c_2(D)->collisions, _3
	cmpl	$79, %esi	#, _3
	setbe	%al	#, bright
	movl	%eax, -52(%rbp)	# bright, %sfp
# find-perfect-hash.c:456: 	uint32_t bits2[c->n];
	movl	8(%rbx), %eax	# c_2(D)->n, c_2(D)->n
	leaq	18(,%rax,4), %rax	#, tmp133
	shrq	$4, %rax	#, tmp135
	salq	$4, %rax	#, tmp137
	subq	%rax, %rsp	# tmp137,
	leaq	3(%rsp), %rax	#, tmp139
	shrq	$2, %rax	#, tmp139
	movq	%rax, -64(%rbp)	# tmp140, %sfp
	leaq	0(,%rax,4), %r13	#, tmp141
# find-perfect-hash.c:458: 	printf("collisions%4u ", c->collisions);
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:459: 	printf("bits: ");
	xorl	%eax, %eax	#
	movl	$.LC13, %edi	#,
	call	printf	#
# find-perfect-hash.c:460: 	for (i = 0; i < c->n; i++) {
	movl	8(%rbx), %edx	# c_2(D)->n,
	testl	%edx, %edx	#
	je	.L52	#,
	xorl	%r12d, %r12d	# ivtmp.197
	xorl	%r15d, %r15d	# i
	.p2align 4,,10
	.p2align 3
.L53:
# find-perfect-hash.c:461: 		printf("\033[0%d;%dm", bright, 36 + (i & 1));
	movl	%r15d, %edx	# i, tmp142
	movl	-52(%rbp), %esi	# %sfp,
	movl	$.LC14, %edi	#,
	andl	$1, %edx	#, tmp142
	xorl	%eax, %eax	#
# find-perfect-hash.c:460: 	for (i = 0; i < c->n; i++) {
	addl	$1, %r15d	#, i
# find-perfect-hash.c:461: 		printf("\033[0%d;%dm", bright, 36 + (i & 1));
	addl	$36, %edx	#, tmp143
	call	printf	#
# find-perfect-hash.c:462: 		printf("%3x ", c->bits[i]);
	movq	(%rbx), %rax	# c_2(D)->bits, c_2(D)->bits
	movl	$.LC15, %edi	#,
	movl	(%rax,%r12), %esi	# *_16, *_16
	xorl	%eax, %eax	#
	addq	$4, %r12	#, ivtmp.197
	call	printf	#
# find-perfect-hash.c:460: 	for (i = 0; i < c->n; i++) {
	cmpl	%r15d, 8(%rbx)	# i, c_2(D)->n
	ja	.L53	#,
.L52:
# find-perfect-hash.c:464: 	printf("\033[01;33m\n");
	movl	$.LC16, %edi	#,
	call	puts	#
# find-perfect-hash.c:466: 	memcpy(bits2, c->bits, c->n * sizeof(uint32_t));
	movl	8(%rbx), %r12d	# c_2(D)->n, _22
	movq	(%rbx), %rsi	# c_2(D)->bits, c_2(D)->bits
	movq	%r13, %rdi	# tmp141,
	leaq	0(,%r12,4), %rdx	#, tmp146
	call	memcpy	#
# find-perfect-hash.c:467: 	qsort(bits2, c->n, sizeof(uint32_t), cmp_uint32);
	movl	$cmp_uint32, %ecx	#,
	movl	$4, %edx	#,
	movq	%r12, %rsi	# _22,
	movq	%r13, %rdi	# tmp141,
	call	qsort	#
# find-perfect-hash.c:469: 	for (i = 0; i < c->n; i++) {
	movl	8(%rbx), %eax	# c_2(D)->n,
	testl	%eax, %eax	#
	je	.L56	#,
# find-perfect-hash.c:470: 		uint32_t b = bits2[i];
	movq	-64(%rbp), %rax	# %sfp, tmp140
# find-perfect-hash.c:472: 		uint32_t bit = b & 31;
	movl	$1, %r15d	#, ivtmp.189
# find-perfect-hash.c:470: 		uint32_t b = bits2[i];
	movl	0(,%rax,4), %edx	# MEM[(uint32_t[0:D.5200] *)bits2.52_9], b
# find-perfect-hash.c:471: 		uint32_t word = b >> 5;
	movl	%edx, %r12d	# b, word
# find-perfect-hash.c:472: 		uint32_t bit = b & 31;
	andl	$31, %edx	#, bit
# find-perfect-hash.c:471: 		uint32_t word = b >> 5;
	shrl	$5, %r12d	#,
	jmp	.L57	#
	.p2align 4,,10
	.p2align 3
.L55:
# find-perfect-hash.c:470: 		uint32_t b = bits2[i];
	movl	0(%r13,%r15,4), %edx	# MEM[base: bits2.52_9, index: ivtmp.189_69, step: 4, offset: 0B], b
# find-perfect-hash.c:474: 			puts(" | ");
	movl	$.LC17, %edi	#,
	addq	$1, %r15	#, ivtmp.189
# find-perfect-hash.c:471: 		uint32_t word = b >> 5;
	movl	%edx, %r12d	# b, word
# find-perfect-hash.c:472: 		uint32_t bit = b & 31;
	andl	$31, %edx	#, bit
	movl	%edx, -52(%rbp)	# bit, %sfp
# find-perfect-hash.c:471: 		uint32_t word = b >> 5;
	shrl	$5, %r12d	#,
# find-perfect-hash.c:474: 			puts(" | ");
	call	puts	#
	movl	-52(%rbp), %edx	# %sfp, bit
.L57:
# find-perfect-hash.c:476: 		printf("%22s & (1 << %2u)",
	movq	(%r14), %rax	# *ISRA.85_55(D), *ISRA.85_55(D)
	movl	$.LC18, %edi	#,
	movq	(%rax,%r12,8), %rsi	# *_33, *_33
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:469: 	for (i = 0; i < c->n; i++) {
	cmpl	%r15d, 8(%rbx)	# ivtmp.189, c_2(D)->n
	ja	.L55	#,
.L56:
# find-perfect-hash.c:479: 	printf("\033[00m\n");
	movl	$.LC9, %edi	#,
	call	puts	#
# find-perfect-hash.c:480: }
	leaq	-40(%rbp), %rsp	#,
	popq	%rbx	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE60:
	.size	describe_columns.isra.5, .-describe_columns.isra.5
	.section	.rodata.str1.1
.LC21:
	.string	"w"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC22:
	.string	"could not open '%s' for writing"
	.section	.rodata.str1.1
.LC23:
	.string	"P5\n%u %u\n255\n"
	.section	.rodata.str1.8
	.align 8
.LC27:
	.string	"wanted to write %zu bytes; fwrite said %zu\n"
	.text
	.p2align 4,,15
	.type	pgm_dump_double.constprop.10, @function
pgm_dump_double.constprop.10:
.LFB63:
	.cfi_startproc
	pushq	%r12	#
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp	#
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	%rsi, %rbp	# name, name
	pushq	%rbx	#
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
# find-perfect-hash.c:81: 	FILE *fh = fopen(name, "w");
	movl	$.LC21, %esi	#,
# find-perfect-hash.c:74: pgm_dump_double(const double *matrix, uint width, uint height,
	movq	%rdi, %rbx	# matrix, matrix
# find-perfect-hash.c:81: 	FILE *fh = fopen(name, "w");
	movq	%rbp, %rdi	# name,
# find-perfect-hash.c:74: pgm_dump_double(const double *matrix, uint width, uint height,
	subq	$16, %rsp	#,
	.cfi_def_cfa_offset 48
# find-perfect-hash.c:74: pgm_dump_double(const double *matrix, uint width, uint height,
	movsd	%xmm0, 8(%rsp)	# min, %sfp
	movsd	%xmm1, (%rsp)	# max, %sfp
# find-perfect-hash.c:81: 	FILE *fh = fopen(name, "w");
	call	fopen	#
# find-perfect-hash.c:82: 	if (fh == NULL){
	testq	%rax, %rax	# fh
	movsd	(%rsp), %xmm1	# %sfp, max
	movsd	8(%rsp), %xmm0	# %sfp, min
	je	.L84	#,
# find-perfect-hash.c:86: 	fprintf(fh, "P5\n%u %u\n255\n", width, height);
	movl	$3136, %edx	#,
	movq	%rax, %rdi	# fh,
	movq	%rax, %r12	#, fh
	movl	$3136, %ecx	#,
	movl	$.LC23, %esi	#,
	xorl	%eax, %eax	#
	movsd	%xmm1, 8(%rsp)	# max, %sfp
	movsd	%xmm0, (%rsp)	# min, %sfp
	call	fprintf	#
	leaq	78675968(%rbx), %rdx	#, _76
# find-perfect-hash.c:78: 	double high = -1e299;
	movsd	.LC19(%rip), %xmm2	#, high
# find-perfect-hash.c:79: 	double low = 1e299;
	movsd	.LC20(%rip), %xmm5	#, low
	movq	%rbx, %rax	# matrix, ivtmp.215
	movsd	8(%rsp), %xmm1	# %sfp, max
	movsd	(%rsp), %xmm0	# %sfp, min
	.p2align 4,,10
	.p2align 3
.L67:
# find-perfect-hash.c:89: 		low = MIN(low, matrix[i]);
	movsd	(%rax), %xmm4	# MEM[base: _78, offset: 0B], _10
	addq	$8, %rax	#, ivtmp.215
# find-perfect-hash.c:88: 	for (i = 0; i < width * height; i++){
	cmpq	%rdx, %rax	# _76, ivtmp.215
	movapd	%xmm4, %xmm3	# _10, tmp166
	minsd	%xmm4, %xmm5	# _10, low
	cmplesd	%xmm2, %xmm3	#, high, tmp166
	andpd	%xmm3, %xmm2	# tmp166, tmp167
	andnpd	%xmm4, %xmm3	# _10, tmp168
	orpd	%xmm3, %xmm2	# tmp168, high
	jne	.L67	#,
# find-perfect-hash.c:92: 	if (low < min) {
	maxsd	%xmm5, %xmm0	# low, min
# find-perfect-hash.c:100: 	uint8_t *im = malloc(size);
	movl	$9834496, %edi	#,
# find-perfect-hash.c:95: 	if (high > max) {
	minsd	%xmm2, %xmm1	# high, max
# find-perfect-hash.c:99: 	double scale = 255.99 / (high - low);
	movsd	.LC24(%rip), %xmm2	#, tmp156
	movsd	%xmm0, 8(%rsp)	# min, %sfp
	subsd	%xmm0, %xmm1	# min, tmp155
	divsd	%xmm1, %xmm2	# tmp155, scale
	movsd	%xmm2, (%rsp)	# scale, %sfp
# find-perfect-hash.c:100: 	uint8_t *im = malloc(size);
	call	malloc	#
	xorl	%edx, %edx	# ivtmp.201
	movq	%rax, %rbp	#, im
	pxor	%xmm3, %xmm3	# tmp165
# find-perfect-hash.c:105: 		} else if (x >= 255) {
	movsd	.LC26(%rip), %xmm4	#, tmp169
	movsd	8(%rsp), %xmm0	# %sfp, min
	movsd	(%rsp), %xmm2	# %sfp, scale
	jmp	.L75	#
	.p2align 4,,10
	.p2align 3
.L81:
	ucomisd	%xmm4, %xmm1	# tmp169, x
	jb	.L82	#,
# find-perfect-hash.c:106: 			im[i] = 255;
	movb	$-1, 0(%rbp,%rdx)	#, MEM[base: im_23, index: ivtmp.201_11, offset: 0B]
.L72:
	addq	$1, %rdx	#, ivtmp.201
# find-perfect-hash.c:101: 	for (i = 0; i < width * height; i++){
	cmpq	$9834496, %rdx	#, ivtmp.201
	je	.L85	#,
.L75:
# find-perfect-hash.c:102: 		double x = (matrix[i] - low) * scale;
	movsd	(%rbx,%rdx,8), %xmm1	# MEM[base: matrix_8(D), index: ivtmp.201_11, step: 8, offset: 0B], MEM[base: matrix_8(D), index: ivtmp.201_11, step: 8, offset: 0B]
	subsd	%xmm0, %xmm1	# min, tmp158
	mulsd	%xmm2, %xmm1	# scale, x
# find-perfect-hash.c:103: 		if (x <= 0) {
	ucomisd	%xmm1, %xmm3	# x, tmp165
	jb	.L81	#,
# find-perfect-hash.c:104: 			im[i] = 0;
	movb	$0, 0(%rbp,%rdx)	#, MEM[base: im_23, index: ivtmp.201_11, offset: 0B]
	addq	$1, %rdx	#, ivtmp.201
# find-perfect-hash.c:101: 	for (i = 0; i < width * height; i++){
	cmpq	$9834496, %rdx	#, ivtmp.201
	jne	.L75	#,
.L85:
# find-perfect-hash.c:111: 	size_t wrote = fwrite(im, 1, size, fh);
	movq	%r12, %rcx	# fh,
	movl	$1, %esi	#,
	movq	%rbp, %rdi	# im,
	call	fwrite	#
# find-perfect-hash.c:112: 	if (wrote != size){
	cmpq	$9834496, %rax	#, wrote
	je	.L76	#,
# find-perfect-hash.c:113: 		printf("wanted to write %zu bytes; fwrite said %zu\n",
	movq	%rax, %rdx	# wrote,
	movl	$9834496, %esi	#,
	movl	$.LC27, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:121: }
	addq	$16, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 32
# find-perfect-hash.c:115: 		return 1;
	movl	$1, %eax	#, <retval>
# find-perfect-hash.c:121: }
	popq	%rbx	#
	.cfi_def_cfa_offset 24
	popq	%rbp	#
	.cfi_def_cfa_offset 16
	popq	%r12	#
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L82:
	.cfi_restore_state
# find-perfect-hash.c:108: 			im[i] = (uint8_t)x;
	cvttsd2si	%xmm1, %eax	# x, tmp163
	movb	%al, 0(%rbp,%rdx)	# tmp163, MEM[base: im_23, index: ivtmp.201_11, offset: 0B]
	jmp	.L72	#
	.p2align 4,,10
	.p2align 3
.L76:
# find-perfect-hash.c:117: 	fflush(fh);
	movq	%r12, %rdi	# fh,
	call	fflush	#
# find-perfect-hash.c:118: 	fclose(fh);
	movq	%r12, %rdi	# fh,
	call	fclose	#
# find-perfect-hash.c:119: 	free(im);
	movq	%rbp, %rdi	# im,
	call	free	#
# find-perfect-hash.c:121: }
	addq	$16, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 32
# find-perfect-hash.c:120: 	return 0;
	xorl	%eax, %eax	# <retval>
# find-perfect-hash.c:121: }
	popq	%rbx	#
	.cfi_def_cfa_offset 24
	popq	%rbp	#
	.cfi_def_cfa_offset 16
	popq	%r12	#
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L84:
	.cfi_restore_state
# find-perfect-hash.c:83: 		printf("could not open '%s' for writing", name);
	movq	%rbp, %rsi	# name,
	movl	$.LC22, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:121: }
	addq	$16, %rsp	#,
	.cfi_def_cfa_offset 32
# find-perfect-hash.c:84: 		return 1;
	movl	$1, %eax	#, <retval>
# find-perfect-hash.c:121: }
	popq	%rbx	#
	.cfi_def_cfa_offset 24
	popq	%rbp	#
	.cfi_def_cfa_offset 16
	popq	%r12	#
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE63:
	.size	pgm_dump_double.constprop.10, .-pgm_dump_double.constprop.10
	.section	.rodata.str1.1
.LC28:
	.string	"dmod(r(djb,%2d) ^ fnv)"
.LC29:
	.string	"dmod2(r(djb,%2d) + fnv)"
.LC30:
	.string	"dmod3(r(djb,%2d), fnv)"
.LC31:
	.string	"hash.pbm"
	.section	.rodata.str1.8
	.align 8
.LC32:
	.string	"could not open '%s' for writing\n"
	.section	.rodata.str1.1
.LC33:
	.string	"P4\n%u %u\n"
	.text
	.p2align 4,,15
	.globl	new_hashdata
	.type	new_hashdata, @function
new_hashdata:
.LFB41:
	.cfi_startproc
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14	#
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13	#
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12	#
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp	#
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx	#
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$72, %rsp	#,
	.cfi_def_cfa_offset 128
# find-perfect-hash.c:365: {
	movq	%rdi, 32(%rsp)	# strings, %sfp
# find-perfect-hash.c:367: 	struct hashdata *data = calloc(strings->n_strings,
	movslq	(%rdi), %rdi	# strings_16(D)->n_strings,
# find-perfect-hash.c:365: {
	movq	%rsi, 48(%rsp)	# hash_names, %sfp
# find-perfect-hash.c:367: 	struct hashdata *data = calloc(strings->n_strings,
	movl	$408, %esi	#,
	movq	%rdi, %rbx	#,
	call	calloc	#
# find-perfect-hash.c:370: 	for (i = 0; i < strings->n_strings; i++) {
	testl	%ebx, %ebx	# _10
# find-perfect-hash.c:367: 	struct hashdata *data = calloc(strings->n_strings,
	movq	%rax, 56(%rsp)	# <retval>, %sfp
# find-perfect-hash.c:370: 	for (i = 0; i < strings->n_strings; i++) {
	jle	.L87	#,
	leaq	16(%rax), %r12	#, ivtmp.255
	movq	$0, 40(%rsp)	#, %sfp
	movl	$0, 28(%rsp)	#, %sfp
	.p2align 4,,10
	.p2align 3
.L99:
# find-perfect-hash.c:371: 		init_hash(&data[i], strings->strings[i],
	movl	28(%rsp), %eax	# %sfp,
	movq	40(%rsp), %rbx	# %sfp, ivtmp.257
	movl	$0, %r13d	#, tmp264
	testl	%eax, %eax	#
	movq	32(%rsp), %rax	# %sfp, strings
	cmove	48(%rsp), %r13	# %sfp,, tmp264
	movq	16(%rax), %rax	# strings_16(D)->strings, strings_16(D)->strings
	movq	%r13, 8(%rsp)	# tmp264, %sfp
	movq	(%rax,%rbx), %rsi	# *_6, _7
# find-perfect-hash.c:275: 	for (s = string; *s != '\0'; s++) {
	movzbl	(%rsi), %eax	# MEM[(const char *)_7], _24
	testb	%al, %al	# _24
	je	.L107	#,
	movq	%rsi, %rcx	# _7, s
# find-perfect-hash.c:272: 	uint32_t h = 5381;
	movl	$5381, %r11d	#, h
# find-perfect-hash.c:273: 	uint32_t h2 = 2166136261;
	movl	$-2128831035, %r14d	#, h2
	.p2align 4,,10
	.p2align 3
.L90:
# find-perfect-hash.c:279:                 h = ((h << 5) + h) ^ c;
	movl	%r11d, %edx	# h, tmp196
# find-perfect-hash.c:277: 		h2 ^= c;
	andl	$95, %eax	#, _27
# find-perfect-hash.c:275: 	for (s = string; *s != '\0'; s++) {
	addq	$1, %rcx	#, s
# find-perfect-hash.c:279:                 h = ((h << 5) + h) ^ c;
	sall	$5, %edx	#, tmp196
# find-perfect-hash.c:277: 		h2 ^= c;
	xorl	%eax, %r14d	# _27, h2
# find-perfect-hash.c:279:                 h = ((h << 5) + h) ^ c;
	addl	%edx, %r11d	# tmp196, _33
	xorl	%eax, %r11d	# _27, h
# find-perfect-hash.c:275: 	for (s = string; *s != '\0'; s++) {
	movzbl	(%rcx), %eax	# MEM[base: s_36, offset: 0B], _24
# find-perfect-hash.c:278: 		h2 *= FNV;
	imull	$16777619, %r14d, %r14d	#, h2, h2
# find-perfect-hash.c:275: 	for (s = string; *s != '\0'; s++) {
	testb	%al, %al	# _24
	jne	.L90	#,
	subl	%esi, %ecx	# _7, _286
.L89:
# find-perfect-hash.c:285: 	if (names != NULL) {
	cmpq	$0, 8(%rsp)	#, %sfp
# find-perfect-hash.c:281: 	hash->string = string;
	movq	%rsi, -16(%r12)	# _7, MEM[base: hash32_23, offset: -16B]
# find-perfect-hash.c:282: 	hash->stringlen = s - string;
	movl	%ecx, -8(%r12)	# _286, MEM[base: hash32_23, offset: -8B]
# find-perfect-hash.c:283: 	hash32[0] = h;
	movl	%r11d, (%r12)	# h, MEM[base: hash32_23, offset: 0B]
# find-perfect-hash.c:284: 	hash32[1] = h2;
	movl	%r14d, 4(%r12)	# h2, MEM[base: hash32_23, offset: 4B]
# find-perfect-hash.c:285: 	if (names != NULL) {
	je	.L91	#,
# find-perfect-hash.c:286: 		names[0] = strdup("djb");
	movl	$4, %edi	#,
	movl	%r11d, 20(%rsp)	# h, %sfp
	call	malloc	#
	testq	%rax, %rax	# __retval
	movl	20(%rsp), %r11d	# %sfp, h
	je	.L92	#,
	movl	$6449764, (%rax)	#, MEM[(void *)__retval_44]
.L92:
	movq	8(%rsp), %rbx	# %sfp, iftmp.0_13
# find-perfect-hash.c:287: 		names[1] = strdup("fnv");
	movl	$4, %edi	#,
	movl	%r11d, 20(%rsp)	# h, %sfp
# find-perfect-hash.c:286: 		names[0] = strdup("djb");
	movq	%rax, (%rbx)	# __retval, *iftmp.0_13
# find-perfect-hash.c:287: 		names[1] = strdup("fnv");
	call	malloc	#
	testq	%rax, %rax	# __retval
	movl	20(%rsp), %r11d	# %sfp, h
	je	.L93	#,
	movl	$7761510, (%rax)	#, MEM[(void *)__retval_47]
.L93:
	movq	8(%rsp), %rbx	# %sfp, iftmp.0_13
	movq	%rax, 8(%rbx)	# __retval, MEM[(char * *)iftmp.0_13 + 8B]
.L91:
	movq	8(%rsp), %rax	# %sfp, iftmp.0_13
# find-perfect-hash.c:273: 	uint32_t h2 = 2166136261;
	xorl	%r15d, %r15d	# ivtmp.233
	movl	%r11d, %r13d	# h, h
	leaq	16(%rax), %rbp	#, ivtmp.240
	.p2align 4,,10
	.p2align 3
.L94:
# find-perfect-hash.c:291: 		uint64_t rh = (i == 0) ? h : ROTATE(h, i);
	testq	%r15, %r15	# ivtmp.233
	movl	%r15d, %ebx	# ivtmp.233, i
	je	.L95	#,
.L127:
	movl	%r13d, %esi	# h, iftmp.5_52
	movl	%r15d, %ecx	# ivtmp.233, tmp313
	roll	%cl, %esi	# tmp313, iftmp.5_52
.L96:
# find-perfect-hash.c:337: 		uint32_t tmp = rh ^ h2;
	movl	%r14d, %ecx	# h2, tmp
# find-perfect-hash.c:338: 		hash32[offset] = ((tmp % 333337) << 16) ^ (tmp % 1234577);
	movl	$-917304729, %eax	#, tmp293
# find-perfect-hash.c:337: 		uint32_t tmp = rh ^ h2;
	xorl	%esi, %ecx	# iftmp.5_52, tmp
# find-perfect-hash.c:338: 		hash32[offset] = ((tmp % 333337) << 16) ^ (tmp % 1234577);
	mull	%ecx	# tmp
	movl	%ecx, %edi	# tmp, tmp205
	movl	%ecx, %eax	# tmp, tmp294
	shrl	$18, %edx	#, tmp201
	imull	$333337, %edx, %edx	#, tmp201, tmp204
	subl	%edx, %edi	# tmp204, tmp205
	movl	$1823944407, %edx	#, tmp209
	mull	%edx	# tmp209
	sall	$16, %edi	#, tmp206
	movl	$-1480203455, %eax	#, tmp295
	shrl	$19, %edx	#, tmp207
	imull	$1234577, %edx, %edx	#, tmp207, tmp210
	subl	%edx, %ecx	# tmp210, tmp211
	xorl	%edi, %ecx	# tmp206, tmp212
	leal	(%r14,%rsi), %edi	#, _308
	movl	%ecx, 8(%r12,%r15,4)	# tmp212, MEM[base: hash32_23, index: ivtmp.233_276, step: 4, offset: 8B]
	mull	%edi	# _308
	movl	%edi, %ecx	# _308, tmp217
	movl	%edi, %eax	# _308, tmp296
	shrl	$17, %edx	#, tmp213
	imull	$199999, %edx, %edx	#, tmp213, tmp216
	subl	%edx, %ecx	# tmp216, tmp217
	movl	$-1282743679, %edx	#, tmp221
	mull	%edx	# tmp221
	movabsq	$-8430186356568095855, %rax	#, tmp297
	sall	$16, %ecx	#, tmp217
	shrl	$18, %edx	#, tmp219
	imull	$373777, %edx, %edx	#, tmp219, tmp222
	subl	%edx, %edi	# tmp222, tmp223
	mulq	%rsi	# iftmp.5_52
	movq	%rsi, %rax	# iftmp.5_52, tmp227
	leal	(%rcx,%rdi), %ecx	#, _315
	movq	%rsi, %rdi	# iftmp.5_52, tmp231
	subq	%rdx, %rax	# tmp225, tmp227
	shrq	%rax	# tmp228
	addq	%rax, %rdx	# tmp228, tmp229
	movabsq	$-6108418940689283625, %rax	#, tmp298
	shrq	$21, %rdx	#, tmp224
	imulq	$2718281, %rdx, %rdx	#, tmp224, tmp230
	subq	%rdx, %rdi	# tmp230, tmp231
	mulq	%rsi	# iftmp.5_52
	movq	%rsi, %rax	# iftmp.5_52, tmp236
	sall	$15, %edi	#, tmp232
	subq	%rdx, %rax	# tmp234, tmp236
	shrq	%rax	# tmp237
	addq	%rax, %rdx	# tmp237, tmp238
	shrq	$18, %rdx	#, tmp233
	imulq	$314159, %rdx, %rdx	#, tmp233, tmp239
	subq	%rdx, %rsi	# tmp239, tmp240
	leal	(%rdi,%rsi), %r8d	#, tmp241
	xorl	%r14d, %r8d	# h2, _326
# find-perfect-hash.c:339: 		if (names != NULL) {
	cmpq	$0, 8(%rsp)	#, %sfp
	je	.L97	#,
# find-perfect-hash.c:340: 			asprintf(&names[offset], "dmod(r(djb,%2d) ^ fnv)", i);
	movq	%rbp, %rdi	# ivtmp.240,
	movl	%ebx, %edx	# i,
	movl	$.LC28, %esi	#,
	xorl	%eax, %eax	#
	movl	%r8d, 24(%rsp)	# _326, %sfp
	movl	%ecx, 20(%rsp)	# _315, %sfp
	call	asprintf	#
# find-perfect-hash.c:346: 		hash32[offset] = ((tmp2 % 199999) << 16) + (tmp2 % 373777);
	movl	20(%rsp), %ecx	# %sfp, _315
# find-perfect-hash.c:348: 			asprintf(&names[offset], "dmod2(r(djb,%2d) + fnv)", i);
	leaq	256(%rbp), %rdi	#, tmp242
	movl	%ebx, %edx	# i,
	movl	$.LC29, %esi	#,
	xorl	%eax, %eax	#
# find-perfect-hash.c:346: 		hash32[offset] = ((tmp2 % 199999) << 16) + (tmp2 % 373777);
	movl	%ecx, 136(%r12,%r15,4)	# _315, MEM[base: hash32_23, index: ivtmp.233_276, step: 4, offset: 136B]
# find-perfect-hash.c:348: 			asprintf(&names[offset], "dmod2(r(djb,%2d) + fnv)", i);
	call	asprintf	#
# find-perfect-hash.c:353: 		hash32[offset] = h2 ^ (((rh % 2718281) << 15) + (rh % 314159));
	movl	24(%rsp), %r8d	# %sfp, _326
# find-perfect-hash.c:355: 			asprintf(&names[offset], "dmod3(r(djb,%2d), fnv)", i);
	leaq	512(%rbp), %rdi	#, tmp243
	xorl	%eax, %eax	#
	movl	%ebx, %edx	# i,
	movl	$.LC30, %esi	#,
	addq	$8, %rbp	#, ivtmp.240
# find-perfect-hash.c:353: 		hash32[offset] = h2 ^ (((rh % 2718281) << 15) + (rh % 314159));
	movl	%r8d, 264(%r12,%r15,4)	# _326, MEM[base: hash32_23, index: ivtmp.233_276, step: 4, offset: 264B]
	addq	$1, %r15	#, ivtmp.233
# find-perfect-hash.c:355: 			asprintf(&names[offset], "dmod3(r(djb,%2d), fnv)", i);
	call	asprintf	#
# find-perfect-hash.c:290: 	for (int i = 0; i < 32; i++) {
	cmpq	$32, %r15	#, ivtmp.233
	jne	.L94	#,
.L105:
# find-perfect-hash.c:370: 	for (i = 0; i < strings->n_strings; i++) {
	movq	32(%rsp), %rbx	# %sfp, strings
	addl	$1, 28(%rsp)	#, %sfp
	addq	$408, %r12	#, ivtmp.255
	movl	28(%rsp), %eax	# %sfp, i
	addq	$8, 40(%rsp)	#, %sfp
	movl	(%rbx), %ebx	# strings_16(D)->n_strings, _10
	cmpl	%eax, %ebx	# i, _10
	jg	.L99	#,
.L87:
# find-perfect-hash.c:140: 	FILE *fh = fopen(name, "w");
	movl	$.LC21, %esi	#,
	movl	$.LC31, %edi	#,
	call	fopen	#
# find-perfect-hash.c:141: 	if (fh == NULL){
	testq	%rax, %rax	# fh
# find-perfect-hash.c:140: 	FILE *fh = fopen(name, "w");
	movq	%rax, %rbp	#, fh
# find-perfect-hash.c:141: 	if (fh == NULL){
	je	.L126	#,
# find-perfect-hash.c:146: 	fprintf(fh, "P4\n%u %u\n", width, height);
	movq	%rax, %rdi	# fh,
	movl	%ebx, %ecx	# _10,
	xorl	%eax, %eax	#
	movl	$3264, %edx	#,
	movl	$.LC33, %esi	#,
	call	fprintf	#
# find-perfect-hash.c:149: 	for (uint y = 0; y < height; y++){
	testl	%ebx, %ebx	# _10
	je	.L102	#,
	leal	-1(%rbx), %r15d	#, tmp245
	movq	56(%rsp), %rbx	# %sfp, <retval>
# find-perfect-hash.c:153: 			byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	movabsq	$8623620610, %r14	#, tmp250
	movabsq	$1136090292240, %r13	#, tmp252
	movabsq	$18032007892189201, %r12	#, tmp254
	addq	$1, %r15	#, tmp246
	imulq	$408, %r15, %r15	#, tmp246, tmp247
	addq	%rbx, %r15	# <retval>, tmp247
	movq	%r15, 8(%rsp)	# tmp247, %sfp
	.p2align 4,,10
	.p2align 3
.L104:
	movq	%rbx, %r15	# row, ivtmp.225
	addq	$408, %rbx	#, row
	.p2align 4,,10
	.p2align 3
.L103:
	movzbl	(%r15), %esi	# MEM[base: _302, offset: 0B], MEM[base: _302, offset: 0B]
	addq	$1, %r15	#, ivtmp.225
	imulq	%r14, %rsi	# tmp250, tmp249
	andq	%r13, %rsi	# tmp252, tmp251
	movq	%rsi, %rax	# tmp251, tmp299
	mulq	%r12	# tmp254
	movq	%rsi, %rax	# tmp251, tmp255
	subq	%rdx, %rax	# tmp253, tmp255
	shrq	%rax	# tmp256
	addq	%rax, %rdx	# tmp256, tmp257
	shrq	$9, %rdx	#, _109
	movq	%rdx, %rax	# _109, tmp259
	salq	$10, %rax	#, tmp259
	subq	%rdx, %rax	# _109, tmp260
	subq	%rax, %rsi	# tmp260, tmp251
# find-perfect-hash.c:154: 			putc(~byte, fh);
	movzbl	%sil, %edi	# _109, _109
	movq	%rbp, %rsi	# fh,
	notl	%edi	# tmp262
	call	_IO_putc	#
# find-perfect-hash.c:150: 		for (uint x = 0; x < width / 8; x++){
	cmpq	%r15, %rbx	# ivtmp.225, row
	jne	.L103	#,
# find-perfect-hash.c:149: 	for (uint y = 0; y < height; y++){
	cmpq	8(%rsp), %rbx	# %sfp, row
	jne	.L104	#,
.L102:
# find-perfect-hash.c:158: 	fflush(fh);
	movq	%rbp, %rdi	# fh,
	call	fflush	#
# find-perfect-hash.c:159: 	fclose(fh);
	movq	%rbp, %rdi	# fh,
	call	fclose	#
.L86:
# find-perfect-hash.c:377: }
	movq	56(%rsp), %rax	# %sfp,
	addq	$72, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx	#
	.cfi_def_cfa_offset 48
	popq	%rbp	#
	.cfi_def_cfa_offset 40
	popq	%r12	#
	.cfi_def_cfa_offset 32
	popq	%r13	#
	.cfi_def_cfa_offset 24
	popq	%r14	#
	.cfi_def_cfa_offset 16
	popq	%r15	#
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L97:
	.cfi_restore_state
# find-perfect-hash.c:346: 		hash32[offset] = ((tmp2 % 199999) << 16) + (tmp2 % 373777);
	movl	%ecx, 136(%r12,%r15,4)	# _315, MEM[base: hash32_23, index: ivtmp.233_276, step: 4, offset: 136B]
# find-perfect-hash.c:353: 		hash32[offset] = h2 ^ (((rh % 2718281) << 15) + (rh % 314159));
	movl	%r8d, 264(%r12,%r15,4)	# _326, MEM[base: hash32_23, index: ivtmp.233_276, step: 4, offset: 264B]
	addq	$1, %r15	#, ivtmp.233
	addq	$8, %rbp	#, ivtmp.240
# find-perfect-hash.c:290: 	for (int i = 0; i < 32; i++) {
	cmpq	$32, %r15	#, ivtmp.233
	je	.L105	#,
# find-perfect-hash.c:291: 		uint64_t rh = (i == 0) ? h : ROTATE(h, i);
	testq	%r15, %r15	# ivtmp.233
	movl	%r15d, %ebx	# ivtmp.233, i
	jne	.L127	#,
.L95:
	movl	%r13d, %esi	# h, iftmp.5_52
	jmp	.L96	#
.L107:
# find-perfect-hash.c:275: 	for (s = string; *s != '\0'; s++) {
	xorl	%ecx, %ecx	# _286
# find-perfect-hash.c:272: 	uint32_t h = 5381;
	movl	$5381, %r11d	#, h
# find-perfect-hash.c:273: 	uint32_t h2 = 2166136261;
	movl	$-2128831035, %r14d	#, h2
	jmp	.L89	#
.L126:
# find-perfect-hash.c:142: 		fprintf(stderr, "could not open '%s' for writing\n", name);
	movq	stderr(%rip), %rdi	# stderr,
	movl	$.LC31, %edx	#,
	movl	$.LC32, %esi	#,
	xorl	%eax, %eax	#
	call	fprintf	#
	jmp	.L86	#
	.cfi_endproc
.LFE41:
	.size	new_hashdata, .-new_hashdata
	.section	.rodata.str1.1
.LC35:
	.string	"r"
.LC36:
	.string	"could not open %s\n"
	.section	.rodata.str1.8
	.align 8
.LC37:
	.string	"could not allocate %d bytes?!\n"
	.align 8
.LC38:
	.string	"gah, C file handling, wanted %d, got %lu\n"
	.section	.rodata.str1.1
.LC39:
	.string	"n_bits %u\n"
.LC40:
	.string	"matrix[0][111] %f\n"
.LC41:
	.string	"matrix[1][111] %f\n"
.LC44:
	.string	"correlation-full.pgm"
.LC45:
	.string	"correlation-clipped-%f.pgm"
.LC47:
	.string	"initial state "
.LC48:
	.string	"list seems unsorted!"
.LC49:
	.string	"%3d\303\227 "
.LC50:
	.string	"worsening %d %u > %u\n"
.LC51:
	.string	"%d\n"
.LC52:
	.string	"round %d; temp %u\n"
	.section	.rodata.str1.8
	.align 8
.LC53:
	.string	"anneal_changes %u (average %u, last %d)\n"
	.section	.rodata.str1.1
.LC54:
	.string	"skipped %u (average %u)\n"
.LC55:
	.string	"worsenings %u\n"
.LC56:
	.string	"inbred %u\n"
.LC57:
	.string	"breeding.."
.LC58:
	.string	"killed %u; bred from %u\n"
.LC59:
	.string	"columns-%d.pbm"
	.text
	.p2align 4,,15
	.globl	find_hash
	.type	find_hash, @function
find_hash:
.LFB53:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r15	#
	pushq	%r14	#
	pushq	%r13	#
	pushq	%r12	#
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	movq	%rdx, %r14	# rng, rng
	pushq	%rbx	#
	.cfi_offset 3, -56
	movq	%rdi, %rbx	# filename, filename
	subq	$296, %rsp	#,
# find-perfect-hash.c:843: {
	movl	%esi, -216(%rbp)	# bits, %sfp
# find-perfect-hash.c:179: 	FILE *f = fopen(filename, "r");
	movl	$.LC35, %esi	#,
	call	fopen	#
# find-perfect-hash.c:180: 	if (f == NULL) {
	testq	%rax, %rax	# f
	je	.L323	#,
# find-perfect-hash.c:184: 	fseek(f, 0, SEEK_END);
	xorl	%esi, %esi	#
	movl	$2, %edx	#,
	movq	%rax, %r13	#, f
	movq	%rax, %rdi	# f,
	call	fseek	#
# find-perfect-hash.c:185: 	len = ftell(f);
	movq	%r13, %rdi	# f,
	call	ftell	#
	movq	%rax, %r12	#, _143
# find-perfect-hash.c:186: 	rewind(f);
	movq	%r13, %rdi	# f,
# find-perfect-hash.c:188: 	s.mem = malloc(len + 1);
	leal	1(%r12), %r15d	#, _145
# find-perfect-hash.c:186: 	rewind(f);
	call	rewind	#
# find-perfect-hash.c:188: 	s.mem = malloc(len + 1);
	movslq	%r15d, %rdi	# _145, _145
	call	malloc	#
# find-perfect-hash.c:189: 	if (s.mem == NULL) {
	testq	%rax, %rax	# _147
# find-perfect-hash.c:188: 	s.mem = malloc(len + 1);
	movq	%rax, %rbx	#, _147
# find-perfect-hash.c:189: 	if (s.mem == NULL) {
	je	.L324	#,
# find-perfect-hash.c:193: 	read = fread(s.mem, 1, len, f);
	movslq	%r12d, %r15	# _143, _148
	movq	%r13, %rcx	# f,
	movl	$1, %esi	#,
	movq	%r15, %rdx	# _148,
	movq	%rax, %rdi	# _147,
	call	fread	#
# find-perfect-hash.c:194: 	if (read != len) {
	cmpq	%rax, %r15	# read, _148
	jne	.L132	#,
# find-perfect-hash.c:200: 	for (i = 0; i < len; i++) {
	testl	%r12d, %r12d	# _143
	leaq	0(,%r15,8), %rdi	#, _2950
	jle	.L325	#,
	leal	-1(%r12), %r13d	#, _2476
	movq	%rbx, %rax	# _147, ivtmp.508
# find-perfect-hash.c:177: 	struct strings s = {0, 0, 0};
	xorl	%r12d, %r12d	# s$n_strings
	leaq	1(%rbx,%r13), %rdx	#, _2464
.L135:
# find-perfect-hash.c:201: 		if (s.mem[i] == '\n') {
	cmpb	$10, (%rax)	#, MEM[base: _2480, offset: 0B]
	je	.L326	#,
	addq	$1, %rax	#, ivtmp.508
# find-perfect-hash.c:200: 	for (i = 0; i < len; i++) {
	cmpq	%rax, %rdx	# ivtmp.508, _2464
	jne	.L135	#,
.L138:
# find-perfect-hash.c:206: 	s.strings = malloc(len * sizeof(char*));
	call	malloc	#
	leaq	1(%rbx), %rdx	#, ivtmp.504
	leaq	2(%rbx,%r13), %rsi	#, _2484
# find-perfect-hash.c:208: 	s.strings[0] = s.mem;
	movq	%rbx, (%rax)	# _147, MEM[(char * *)_712]
# find-perfect-hash.c:209: 	j = 1;
	movl	$1, %ecx	#, j
.L248:
# find-perfect-hash.c:211: 		if (s.mem[i] == '\0' && j < s.n_strings) {
	cmpb	$0, -1(%rdx)	#, MEM[base: _2496, offset: -1B]
	jne	.L139	#,
	cmpl	%ecx, %r12d	# j, s$n_strings
	jle	.L139	#,
# find-perfect-hash.c:212: 			s.strings[j] = s.mem + i + 1;
	movslq	%ecx, %rdi	# j, j
# find-perfect-hash.c:213: 			j++;
	addl	$1, %ecx	#, j
# find-perfect-hash.c:212: 			s.strings[j] = s.mem + i + 1;
	movq	%rdx, (%rax,%rdi,8)	# ivtmp.504, *_168
	addq	$1, %rdx	#, ivtmp.504
# find-perfect-hash.c:210: 	for (i = 0; i < len; i++) {
	cmpq	%rsi, %rdx	# _2484, ivtmp.504
	jne	.L248	#,
	jmp	.L141	#
.L323:
# find-perfect-hash.c:181: 		printf("could not open %s\n", filename);
	movq	%rbx, %rsi	# filename,
	movl	$.LC36, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:182: 		return s;
	movl	$0, -208(%rbp)	#, MEM[(struct strings *)&strings]
	movq	$0, -200(%rbp)	#, MEM[(struct strings *)&strings + 8B]
	movq	$0, -192(%rbp)	#, MEM[(struct strings *)&strings + 16B]
.L130:
# find-perfect-hash.c:846: 	char **hashnames = calloc(HASH_WORDS * 2, sizeof(char*));
	movl	$8, %esi	#,
	movl	$98, %edi	#,
	call	calloc	#
# find-perfect-hash.c:847: 	struct hashdata *data = new_hashdata(&strings, hashnames);
	leaq	-208(%rbp), %rdi	#, tmp1593
	movq	%rax, %rsi	# tmp835,
# find-perfect-hash.c:846: 	char **hashnames = calloc(HASH_WORDS * 2, sizeof(char*));
	movq	%rax, %rbx	#, tmp835
# find-perfect-hash.c:847: 	struct hashdata *data = new_hashdata(&strings, hashnames);
	call	new_hashdata	#
	movq	%rax, %r12	#, data
# find-perfect-hash.c:848: 	uint32_t max_columns = MIN(bits + 10, 30);
	movl	-216(%rbp), %eax	# %sfp, bits
	leal	10(%rax), %edi	#, max_columns
	movl	$30, %eax	#, tmp837
	cmpl	$30, %edi	#, max_columns
	cmovbe	%edi, %eax	# max_columns,, tmp837
	movl	%eax, %esi	# tmp837, max_columns
	movl	%eax, -244(%rbp)	# max_columns, %sfp
# find-perfect-hash.c:850: 	uint64_t *hits = calloc((1 << max_columns) / 64 + 1,
	movl	$1, %eax	#, tmp839
	movl	%esi, %ecx	# max_columns, tmp1600
	movl	$8, %esi	#,
	sall	%cl, %eax	# tmp1600, tmp839
	movl	%eax, %edi	# tmp839, tmp838
	sarl	$6, %edi	#, tmp840
	addl	$1, %edi	#, tmp841
	movslq	%edi, %rdi	# tmp841, tmp842
	call	calloc	#
# find-perfect-hash.c:853: 	struct hashcontext ctx = {data, strings.n_strings, hits, hashnames};
	movq	%rax, -224(%rbp)	# tmp843, %sfp
	pxor	%xmm1, %xmm1	# tmp1347
	movl	-208(%rbp), %edx	# strings.n_strings, strings.n_strings
	movq	-224(%rbp), %xmm0	# %sfp, tmp845
	movq	%rbx, -224(%rbp)	# tmp835, %sfp
# find-perfect-hash.c:387: 	double *matrix = calloc(n_bits * n_bits, sizeof(matrix[0]));
	movl	$8, %esi	#,
	movl	$9834496, %edi	#,
# find-perfect-hash.c:853: 	struct hashcontext ctx = {data, strings.n_strings, hits, hashnames};
	movhps	-224(%rbp), %xmm0	# %sfp, tmp845
	movq	$0, -64(%rbp)	#, ctx
	movaps	%xmm1, -112(%rbp)	# tmp1347, ctx
	movl	%edx, -104(%rbp)	# strings.n_strings, ctx.n
	movq	%r12, -112(%rbp)	# data, ctx.data
	movaps	%xmm1, -80(%rbp)	# tmp1347, ctx
	movaps	%xmm0, -96(%rbp)	# tmp845, MEM[(void *)&ctx + 16B]
# find-perfect-hash.c:387: 	double *matrix = calloc(n_bits * n_bits, sizeof(matrix[0]));
	call	calloc	#
# find-perfect-hash.c:388: 	uint *pairs = calloc(n_bits * n_bits * 4, sizeof(uint));
	movl	$4, %esi	#,
	movl	$39337984, %edi	#,
# find-perfect-hash.c:387: 	double *matrix = calloc(n_bits * n_bits, sizeof(matrix[0]));
	movq	%rax, %r15	#, matrix
# find-perfect-hash.c:388: 	uint *pairs = calloc(n_bits * n_bits * 4, sizeof(uint));
	call	calloc	#
# find-perfect-hash.c:389: 	printf("n_bits %u\n", n_bits);
	movl	$3136, %esi	#,
# find-perfect-hash.c:388: 	uint *pairs = calloc(n_bits * n_bits * 4, sizeof(uint));
	movq	%rax, -232(%rbp)	#, %sfp
# find-perfect-hash.c:389: 	printf("n_bits %u\n", n_bits);
	movl	$.LC39, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:392: 		for (k = 0; k < ctx->n; k++) {
	movl	-104(%rbp), %eax	# MEM[(uint32_t *)&ctx + 8B],
# find-perfect-hash.c:393: 			uint64_t *hash = ctx->data[k].hash;
	movq	-112(%rbp), %rcx	# MEM[(struct hashdata * *)&ctx], _61
	testl	%eax, %eax	# _1068
	je	.L142	#,
	leal	-1(%rax), %edx	#, tmp849
	leaq	16(%rcx), %r12	#, ivtmp.489
	xorl	%r10d, %r10d	# ivtmp.498
	imulq	$408, %rdx, %rdx	#, tmp849, tmp850
	leaq	424(%rcx,%rdx), %rbx	#, _2509
# find-perfect-hash.c:390: 	for (i = 0; i < n_bits; i++) {
	xorl	%edx, %edx	# i
.L146:
# find-perfect-hash.c:394: 			uint a = READ_HASH_BIT(hash, i) ? 2 : 0;
	movl	%edx, %edi	# i, tmp852
	movl	$1, %r11d	#, shifttmp_327
	movl	%edx, %ecx	# i, tmp1606
	sarl	$6, %edi	#, tmp852
	salq	%cl, %r11	# tmp1606, shifttmp_327
	movq	%r12, %rsi	# ivtmp.489, ivtmp.489
	movslq	%edi, %rdi	# tmp852, tmp853
	salq	$3, %rdi	#, _70
.L145:
	xorl	%r8d, %r8d	# ivtmp.482
	testq	%r11, (%rsi,%rdi)	# shifttmp_327, MEM[base: hash_66, index: _70, offset: 0B]
	movq	-232(%rbp), %r13	# %sfp, pairs
	setne	%r8b	#, ivtmp.482
# find-perfect-hash.c:395: 			for (j = 0; j < n_bits; j++) {
	xorl	%ecx, %ecx	# j
# find-perfect-hash.c:394: 			uint a = READ_HASH_BIT(hash, i) ? 2 : 0;
	addl	%r8d, %r8d	# ivtmp.482
.L144:
# find-perfect-hash.c:396: 				uint b = READ_HASH_BIT(hash, j);
	movl	%ecx, %r9d	# j, tmp857
	sarl	$6, %r9d	#, tmp857
	movslq	%r9d, %r9	# tmp857, tmp858
	movq	(%rsi,%r9,8), %r9	# *_80, *_80
	shrq	%cl, %r9	# j, tmp860
# find-perfect-hash.c:395: 			for (j = 0; j < n_bits; j++) {
	addl	$1, %ecx	#, j
# find-perfect-hash.c:396: 				uint b = READ_HASH_BIT(hash, j);
	andl	$1, %r9d	#, b
# find-perfect-hash.c:397: 				row[j * 4 + a + b]++;
	addl	%r8d, %r9d	# ivtmp.482, tmp864
	addl	$4, %r8d	#, ivtmp.482
	leaq	(%r10,%r9,4), %r9	#, tmp866
	addl	$1, 0(%r13,%r9)	#, *_94
# find-perfect-hash.c:395: 			for (j = 0; j < n_bits; j++) {
	cmpl	$3136, %ecx	#, j
	jne	.L144	#,
	addq	$408, %rsi	#, ivtmp.489
# find-perfect-hash.c:392: 		for (k = 0; k < ctx->n; k++) {
	cmpq	%rsi, %rbx	# ivtmp.489, _2509
	jne	.L145	#,
# find-perfect-hash.c:390: 	for (i = 0; i < n_bits; i++) {
	addl	$1, %edx	#, i
	addq	$50176, %r10	#, ivtmp.498
	cmpl	$3136, %edx	#, i
	jne	.L146	#,
.L142:
# find-perfect-hash.c:401: 	double n = ctx->n;
	pxor	%xmm3, %xmm3	# n
	movq	%r15, -256(%rbp)	# matrix, %sfp
	movq	$0, -240(%rbp)	#, %sfp
	cvtsi2sdq	%rax, %xmm3	# _1068, n
	movq	-232(%rbp), %rax	# %sfp, pairs
	leaq	50176(%rax), %r12	#, ivtmp.467
	movsd	%xmm3, -224(%rbp)	# n, %sfp
.L149:
	leaq	-50176(%r12), %r13	#, ivtmp.455
# find-perfect-hash.c:395: 			for (j = 0; j < n_bits; j++) {
	movq	-256(%rbp), %rbx	# %sfp, ivtmp.456
	jmp	.L169	#
	.p2align 4,,10
	.p2align 3
.L327:
# find-perfect-hash.c:405: 			double e = 0;
	pxor	%xmm2, %xmm2	# e
	movapd	%xmm4, %xmm6	# tmp1611, tmp1614
# find-perfect-hash.c:408: 				if (p != 0) {
	jne	.L263	#,
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	movl	4(%r13), %edx	# MEM[base: _2606, index: _2541, step: 4, offset: 0B], MEM[base: _2606, index: _2541, step: 4, offset: 0B]
	pxor	%xmm1, %xmm1	# p
	cvtsi2sdq	%rdx, %xmm1	# MEM[base: _2606, index: _2541, step: 4, offset: 0B], p
# find-perfect-hash.c:408: 				if (p != 0) {
	ucomisd	%xmm6, %xmm1	# tmp1614, p
	jp	.L264	#,
.L328:
	jne	.L264	#,
.L156:
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	movl	8(%r13), %edx	# MEM[base: _2606, index: _2548, step: 4, offset: 0B], MEM[base: _2606, index: _2548, step: 4, offset: 0B]
	pxor	%xmm1, %xmm1	# p
# find-perfect-hash.c:408: 				if (p != 0) {
	pxor	%xmm3, %xmm3	# tmp1616
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	cvtsi2sdq	%rdx, %xmm1	# MEM[base: _2606, index: _2548, step: 4, offset: 0B], p
# find-perfect-hash.c:408: 				if (p != 0) {
	ucomisd	%xmm3, %xmm1	# tmp1616, p
	jp	.L265	#,
	jne	.L265	#,
.L160:
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	movl	12(%r13), %edx	# MEM[base: _2606, index: _2570, step: 4, offset: 0B], MEM[base: _2606, index: _2570, step: 4, offset: 0B]
	pxor	%xmm1, %xmm1	# p
# find-perfect-hash.c:408: 				if (p != 0) {
	pxor	%xmm5, %xmm5	# tmp1618
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	cvtsi2sdq	%rdx, %xmm1	# MEM[base: _2606, index: _2570, step: 4, offset: 0B], p
# find-perfect-hash.c:408: 				if (p != 0) {
	ucomisd	%xmm5, %xmm1	# tmp1618, p
	jp	.L266	#,
	jne	.L266	#,
	addq	$16, %r13	#, ivtmp.455
# find-perfect-hash.c:413: 			matrix[i * n_bits + j] = e;
	movsd	%xmm2, (%rbx)	# e, MEM[base: _2605, offset: 0B]
	addq	$8, %rbx	#, ivtmp.456
# find-perfect-hash.c:404: 		for (j = 0; j < n_bits; j++) {
	cmpq	%r13, %r12	# ivtmp.455, ivtmp.467
	je	.L167	#,
.L169:
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	movl	0(%r13), %edx	# MEM[base: _2606, offset: 0B], MEM[base: _2606, offset: 0B]
	pxor	%xmm1, %xmm1	# p
# find-perfect-hash.c:408: 				if (p != 0) {
	pxor	%xmm4, %xmm4	# tmp1611
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	cvtsi2sdq	%rdx, %xmm1	# MEM[base: _2606, offset: 0B], p
# find-perfect-hash.c:408: 				if (p != 0) {
	ucomisd	%xmm4, %xmm1	# tmp1611, p
	jnp	.L327	#,
.L263:
# find-perfect-hash.c:409: 					p /= n;
	divsd	-224(%rbp), %xmm1	# %sfp, p
# find-perfect-hash.c:410: 					e -= p * log2(p);
	movapd	%xmm1, %xmm0	# p,
	movsd	%xmm1, -264(%rbp)	# p, %sfp
	call	log2	#
	movsd	-264(%rbp), %xmm1	# %sfp, p
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	movl	4(%r13), %edx	# MEM[base: _2606, index: _2541, step: 4, offset: 0B], MEM[base: _2606, index: _2541, step: 4, offset: 0B]
# find-perfect-hash.c:410: 					e -= p * log2(p);
	pxor	%xmm2, %xmm2	# e
	mulsd	%xmm0, %xmm1	# _1083, tmp877
	pxor	%xmm6, %xmm6	# tmp1614
	subsd	%xmm1, %xmm2	# tmp877, e
# find-perfect-hash.c:407: 				double p = pairs[(i * n_bits + j) * 4 + k];
	pxor	%xmm1, %xmm1	# p
	cvtsi2sdq	%rdx, %xmm1	# MEM[base: _2606, index: _2541, step: 4, offset: 0B], p
# find-perfect-hash.c:408: 				if (p != 0) {
	ucomisd	%xmm6, %xmm1	# tmp1614, p
	jnp	.L328	#,
.L264:
# find-perfect-hash.c:409: 					p /= n;
	divsd	-224(%rbp), %xmm1	# %sfp, p
	movsd	%xmm2, -272(%rbp)	# e, %sfp
# find-perfect-hash.c:410: 					e -= p * log2(p);
	movapd	%xmm1, %xmm0	# p,
	movsd	%xmm1, -264(%rbp)	# p, %sfp
	call	log2	#
	movsd	-264(%rbp), %xmm1	# %sfp, p
	movsd	-272(%rbp), %xmm2	# %sfp, e
	mulsd	%xmm0, %xmm1	# _1104, tmp887
	subsd	%xmm1, %xmm2	# tmp887, e
	jmp	.L156	#
.L266:
# find-perfect-hash.c:409: 					p /= n;
	divsd	-224(%rbp), %xmm1	# %sfp, p
	movsd	%xmm2, -272(%rbp)	# e, %sfp
	addq	$16, %r13	#, ivtmp.455
	addq	$8, %rbx	#, ivtmp.456
# find-perfect-hash.c:410: 					e -= p * log2(p);
	movapd	%xmm1, %xmm0	# p,
	movsd	%xmm1, -264(%rbp)	# p, %sfp
	call	log2	#
	movsd	-264(%rbp), %xmm1	# %sfp, p
	movsd	-272(%rbp), %xmm2	# %sfp, e
	mulsd	%xmm0, %xmm1	# _1146, tmp910
	subsd	%xmm1, %xmm2	# tmp910, e
# find-perfect-hash.c:413: 			matrix[i * n_bits + j] = e;
	movsd	%xmm2, -8(%rbx)	# e, MEM[base: _57, offset: 0B]
# find-perfect-hash.c:404: 		for (j = 0; j < n_bits; j++) {
	cmpq	%r12, %r13	# ivtmp.467, ivtmp.455
	jne	.L169	#,
.L167:
	addq	$12544, -240(%rbp)	#, %sfp
	addq	$50176, %r12	#, ivtmp.467
	addq	$25088, -256(%rbp)	#, %sfp
	movq	-240(%rbp), %rax	# %sfp, ivtmp.470
# find-perfect-hash.c:403: 	for (i = 0; i < n_bits; i++) {
	cmpq	$39337984, %rax	#, ivtmp.470
	jne	.L149	#,
# find-perfect-hash.c:418: 	printf("matrix[0][111] %f\n",
	movsd	888(%r15), %xmm0	# MEM[(double *)matrix_55 + 888B],
	movl	$.LC40, %edi	#,
	movl	$1, %eax	#,
# find-perfect-hash.c:423: 	pgm_dump_double(matrix, n_bits, n_bits, "correlation-full.pgm",
	movl	$12, %ebx	#, ivtmp_2926
# find-perfect-hash.c:418: 	printf("matrix[0][111] %f\n",
	call	printf	#
# find-perfect-hash.c:420: 	printf("matrix[1][111] %f\n",
	movsd	25976(%r15), %xmm0	# MEM[(double *)matrix_55 + 25976B],
	movl	$.LC41, %edi	#,
	movl	$1, %eax	#,
	call	printf	#
# find-perfect-hash.c:423: 	pgm_dump_double(matrix, n_bits, n_bits, "correlation-full.pgm",
	movsd	.LC42(%rip), %xmm1	#,
	movl	$.LC44, %esi	#,
	movsd	.LC43(%rip), %xmm0	#,
	movq	%r15, %rdi	# matrix,
	call	pgm_dump_double.constprop.10	#
# find-perfect-hash.c:425: 	double clip = 1.0;
	movsd	.LC34(%rip), %xmm2	#, clip
.L170:
# find-perfect-hash.c:428: 		sprintf(s, "correlation-clipped-%f.pgm", clip);
	movapd	%xmm2, %xmm0	# clip,
	leaq	-176(%rbp), %rdi	#, tmp1623
	movl	$.LC45, %esi	#,
	movl	$1, %eax	#,
	movsd	%xmm2, -224(%rbp)	# clip, %sfp
	call	sprintf	#
# find-perfect-hash.c:430: 		pgm_dump_double(matrix, n_bits, n_bits, s,
	movsd	-224(%rbp), %xmm2	# %sfp, clip
	leaq	-176(%rbp), %rsi	#, tmp1625
	movsd	.LC46(%rip), %xmm0	#, tmp918
	movq	%r15, %rdi	# matrix,
	movsd	.LC46(%rip), %xmm1	#,
	subsd	%xmm2, %xmm0	# clip, tmp918
	call	pgm_dump_double.constprop.10	#
# find-perfect-hash.c:432: 		clip *= 0.5;
	movsd	-224(%rbp), %xmm2	# %sfp, clip
# find-perfect-hash.c:426: 	for (i = 0; i < 12; i++) {
	subl	$1, %ebx	#, ivtmp_2926
# find-perfect-hash.c:432: 		clip *= 0.5;
	mulsd	.LC3(%rip), %xmm2	#, clip
# find-perfect-hash.c:426: 	for (i = 0; i < 12; i++) {
	jne	.L170	#,
# find-perfect-hash.c:434: 	free(pairs);
	movq	-232(%rbp), %rdi	# %sfp,
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %r12	#, tmp947
# find-perfect-hash.c:434: 	free(pairs);
	call	free	#
# find-perfect-hash.c:857: 	uint32_t *column_mem = calloc(N_CANDIDATES * max_columns,
	imull	$1000, -244(%rbp), %edi	#, %sfp,
	movl	$4, %esi	#,
# find-perfect-hash.c:855: 	ctx.correlations = column_correlation(&ctx);
	movq	%r15, -64(%rbp)	# matrix, ctx.correlations
# find-perfect-hash.c:857: 	uint32_t *column_mem = calloc(N_CANDIDATES * max_columns,
	call	calloc	#
# find-perfect-hash.c:859: 	struct columns *columns = calloc(N_CANDIDATES, sizeof(struct columns));
	movl	$1000, %edi	#,
	movl	$16, %esi	#,
# find-perfect-hash.c:857: 	uint32_t *column_mem = calloc(N_CANDIDATES * max_columns,
	movq	%rax, -224(%rbp)	#, %sfp
# find-perfect-hash.c:859: 	struct columns *columns = calloc(N_CANDIDATES, sizeof(struct columns));
	call	calloc	#
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movl	-216(%rbp), %edi	# %sfp, bits
# find-perfect-hash.c:859: 	struct columns *columns = calloc(N_CANDIDATES, sizeof(struct columns));
	movq	%rax, %rbx	#, columns
	movq	%rax, -280(%rbp)	# columns, %sfp
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movl	$1, %eax	#, tmp927
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movq	%rbx, %r13	# ivtmp.441, ivtmp.441
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movl	%edi, %ecx	# bits, tmp1628
	sall	%cl, %eax	# tmp1628, tmp927
	movl	%eax, %edx	# tmp927, tmp926
	leal	7(%rax), %eax	#, tmp930
	testl	%edx, %edx	# tmp926
	cmovns	%edx, %eax	# tmp930,, tmp926, tmp929
	xorl	%r15d, %r15d	# ivtmp.440
	sarl	$3, %eax	#, tmp931
	cltq
	movq	%rax, -232(%rbp)	# _221, %sfp
	leaq	16000(%rbx), %rax	#, _2623
	movq	%rax, -264(%rbp)	# _2623, %sfp
	leal	-2(%rdi), %eax	#, tmp933
	leaq	8(,%rax,4), %rax	#, _2647
	movq	%rax, -288(%rbp)	# _2647, %sfp
	leal	-1(%rdi), %eax	#, tmp936
	leaq	4(,%rax,4), %rax	#, _2700
	movq	%rax, -296(%rbp)	# _2700, %sfp
.L183:
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _226
# find-perfect-hash.c:861: 		columns[i].bits = column_mem + i * max_columns;
	movq	-224(%rbp), %rbx	# %sfp, column_mem
	movl	%r15d, %eax	# ivtmp.440, ivtmp.440
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	(%r14), %rcx	# rng_44(D)->a, e
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %r8	# rng_44(D)->d, _232
# find-perfect-hash.c:861: 		columns[i].bits = column_mem + i * max_columns;
	leaq	(%rbx,%rax,4), %r9	#, _18
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdx, %rax	# _226, tmp940
	rolq	$7, %rax	#, tmp940
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%r8, %rdi	# _232, tmp943
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %rcx	# tmp940, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %rax	# rng_44(D)->c, _229
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdi	#, tmp943
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rcx, %r8	# e, rng__c_lsm.272
# find-perfect-hash.c:861: 		columns[i].bits = column_mem + i * max_columns;
	movq	%r9, 0(%r13)	# _18, MEM[base: _2638, offset: 0B]
# find-perfect-hash.c:37: 	x->c = x->d + e;
	movq	%r8, 16(%r14)	# rng__c_lsm.272, rng_44(D)->c
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rax, %rsi	# _229, tmp942
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rax, %rdi	# _229, rng__b_lsm.270
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	rolq	$13, %rsi	#, tmp942
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rdi, 8(%r14)	# rng__b_lsm.270, rng_44(D)->b
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	xorq	%rdx, %rsi	# _226, rng__a_lsm.268
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rsi, %rcx	# rng__a_lsm.268, rng__d_lsm.274
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rsi, (%r14)	# rng__a_lsm.268, rng_44(D)->a
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rcx, %rdx	# rng__d_lsm.274, tmp945
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%rcx, 24(%r14)	# rng__d_lsm.274, rng_44(D)->d
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$6, %rdx	#, tmp945
	movq	%rdx, %rax	# tmp945, tmp945
	mulq	%r12	# tmp947
	movq	%rcx, %rax	# rng__d_lsm.274, tmp949
	imulq	$3136, %rdx, %rdx	#, tmp946, tmp948
	subq	%rdx, %rax	# tmp948, tmp949
# find-perfect-hash.c:522: 	for (i = 1; i < n; i++) {
	cmpl	$1, -216(%rbp)	#, %sfp
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%eax, (%r9)	# tmp949, *_18
# find-perfect-hash.c:522: 	for (i = 1; i < n; i++) {
	jbe	.L171	#,
# find-perfect-hash.c:528: 				e += ctx->correlations[a * HASH_BITS + b];
	movq	-64(%rbp), %rax	# MEM[(double * *)&ctx + 48B], _260
# find-perfect-hash.c:519: 	double best_e = 0;
	pxor	%xmm0, %xmm0	# e
	leaq	4(%r9), %r11	#, ivtmp.430
# find-perfect-hash.c:528: 				e += ctx->correlations[a * HASH_BITS + b];
	movq	%rax, -240(%rbp)	# _260, %sfp
	movq	-288(%rbp), %rax	# %sfp, _2647
	addq	%r9, %rax	# _18, _2645
	movq	%rax, -272(%rbp)	# _2645, %sfp
.L178:
	movapd	%xmm0, %xmm1	# e, best_e
	movl	$50, %r10d	#, ivtmp_2592
.L172:
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdi, %rax	# rng__b_lsm.270, tmp950
# find-perfect-hash.c:525: 			double e = 0;
	pxor	%xmm0, %xmm0	# e
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	rolq	$7, %rax	#, tmp950
	subq	%rax, %rsi	# tmp950, rng__a_lsm.268
	movq	%rsi, %rax	# rng__a_lsm.268, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r8, %rsi	# rng__c_lsm.272, tmp951
	rolq	$13, %rsi	#, tmp951
	xorq	%rdi, %rsi	# rng__b_lsm.270, rng__a_lsm.268
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rcx, %rdi	# rng__d_lsm.274, tmp952
	rorq	$27, %rdi	#, tmp952
	addq	%r8, %rdi	# rng__c_lsm.272, rng__b_lsm.270
# find-perfect-hash.c:37: 	x->c = x->d + e;
	leaq	(%rax,%rcx), %r8	#, rng__c_lsm.272
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%rax,%rsi), %rcx	#, rng__d_lsm.274
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rcx, %rdx	# rng__d_lsm.274, tmp953
	movq	%rcx, %rbx	# rng__d_lsm.274, _251
# find-perfect-hash.c:525: 			double e = 0;
	movq	%rcx, -304(%rbp)	# rng__d_lsm.274, %sfp
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$6, %rdx	#, tmp953
	movq	-240(%rbp), %rcx	# %sfp, _260
	movq	%rdx, %rax	# tmp953, tmp953
	mulq	%r12	# tmp947
	imulq	$3136, %rdx, %rdx	#, tmp954, tmp956
	subq	%rdx, %rbx	# tmp956, _251
# find-perfect-hash.c:528: 				e += ctx->correlations[a * HASH_BITS + b];
	imull	$3136, %ebx, %eax	#, _251, _261
	movl	%eax, -256(%rbp)	# _261, %sfp
	movq	%r9, %rax	# _18, ivtmp.417
.L173:
	movl	-256(%rbp), %edx	# %sfp, tmp957
	addl	(%rax), %edx	# MEM[base: _2668, offset: 0B], tmp957
	addq	$4, %rax	#, ivtmp.417
# find-perfect-hash.c:526: 			for (j = 0; j < i; j++) {
	cmpq	%rax, %r11	# ivtmp.417, ivtmp.430
# find-perfect-hash.c:528: 				e += ctx->correlations[a * HASH_BITS + b];
	movslq	%edx, %rdx	# tmp957, tmp959
	addsd	(%rcx,%rdx,8), %xmm0	# *_265, e
# find-perfect-hash.c:526: 			for (j = 0; j < i; j++) {
	jne	.L173	#,
# find-perfect-hash.c:530: 			if (e > best_e) {
	ucomisd	%xmm1, %xmm0	# best_e, e
	movq	-304(%rbp), %rcx	# %sfp, rng__d_lsm.274
	jbe	.L315	#,
# find-perfect-hash.c:523: 		for (k = 0; k < 50; k++) {
	subl	$1, %r10d	#, ivtmp_2592
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%ebx, (%r11)	# _251, MEM[base: _2644, offset: 0B]
# find-perfect-hash.c:523: 		for (k = 0; k < 50; k++) {
	je	.L177	#,
# find-perfect-hash.c:528: 				e += ctx->correlations[a * HASH_BITS + b];
	movapd	%xmm0, %xmm1	# e, best_e
	jmp	.L172	#
.L265:
# find-perfect-hash.c:409: 					p /= n;
	divsd	-224(%rbp), %xmm1	# %sfp, p
	movsd	%xmm2, -272(%rbp)	# e, %sfp
# find-perfect-hash.c:410: 					e -= p * log2(p);
	movapd	%xmm1, %xmm0	# p,
	movsd	%xmm1, -264(%rbp)	# p, %sfp
	call	log2	#
	movsd	-264(%rbp), %xmm1	# %sfp, p
	movsd	-272(%rbp), %xmm2	# %sfp, e
	mulsd	%xmm0, %xmm1	# _1125, tmp897
	subsd	%xmm1, %xmm2	# tmp897, e
	jmp	.L160	#
.L326:
# find-perfect-hash.c:202: 			s.mem[i] = '\0';
	movb	$0, (%rax)	#, MEM[base: _2480, offset: 0B]
	addq	$1, %rax	#, ivtmp.508
# find-perfect-hash.c:203: 			s.n_strings++;
	addl	$1, %r12d	#, s$n_strings
# find-perfect-hash.c:200: 	for (i = 0; i < len; i++) {
	cmpq	%rax, %rdx	# ivtmp.508, _2464
	jne	.L135	#,
	jmp	.L138	#
.L325:
# find-perfect-hash.c:206: 	s.strings = malloc(len * sizeof(char*));
	call	malloc	#
# find-perfect-hash.c:177: 	struct strings s = {0, 0, 0};
	xorl	%r12d, %r12d	# s$n_strings
# find-perfect-hash.c:208: 	s.strings[0] = s.mem;
	movq	%rbx, (%rax)	# _147, MEM[(char * *)_776]
.L141:
# find-perfect-hash.c:216: 	return s;
	movl	%r12d, -208(%rbp)	# s$n_strings, MEM[(struct strings *)&strings]
	movq	%rbx, -200(%rbp)	# _147, MEM[(struct strings *)&strings + 8B]
	movq	%rax, -192(%rbp)	# _283, MEM[(struct strings *)&strings + 16B]
	jmp	.L130	#
.L139:
	addq	$1, %rdx	#, ivtmp.504
# find-perfect-hash.c:210: 	for (i = 0; i < len; i++) {
	cmpq	%rdx, %rsi	# ivtmp.504, _2484
	jne	.L248	#,
	jmp	.L141	#
.L132:
# find-perfect-hash.c:195: 		printf("gah, C file handling, wanted %d, got %lu\n",
	movq	%rax, %rdx	# read,
	movl	%r12d, %esi	# _143,
	movl	$.LC38, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:197: 		free(s.mem);
	movq	%rbx, %rdi	# _147,
	call	free	#
# find-perfect-hash.c:198: 		return s;
	movl	$0, -208(%rbp)	#, MEM[(struct strings *)&strings]
	movq	%rbx, -200(%rbp)	# _147, MEM[(struct strings *)&strings + 8B]
	movq	$0, -192(%rbp)	#, MEM[(struct strings *)&strings + 16B]
	jmp	.L130	#
.L324:
# find-perfect-hash.c:190: 		printf("could not allocate %d bytes?!\n", len + 1);
	movl	%r15d, %esi	# _145,
	movl	$.LC37, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:191: 		return s;
	movl	$0, -208(%rbp)	#, MEM[(struct strings *)&strings]
	movq	$0, -200(%rbp)	#, MEM[(struct strings *)&strings + 8B]
	movq	$0, -192(%rbp)	#, MEM[(struct strings *)&strings + 16B]
	jmp	.L130	#
	.p2align 4,,10
	.p2align 3
.L315:
# find-perfect-hash.c:523: 		for (k = 0; k < 50; k++) {
	subl	$1, %r10d	#, ivtmp_2592
	jne	.L172	#,
	movapd	%xmm1, %xmm0	# best_e, e
.L177:
	addq	$4, %r11	#, ivtmp.430
# find-perfect-hash.c:522: 	for (i = 1; i < n; i++) {
	cmpq	%r11, -272(%rbp)	# ivtmp.430, %sfp
	jne	.L178	#,
	movq	%rsi, (%r14)	# rng__a_lsm.268, rng_44(D)->a
	movq	%rdi, 8(%r14)	# rng__b_lsm.270, rng_44(D)->b
	movq	%r8, 16(%r14)	# rng__c_lsm.272, rng_44(D)->c
	movq	%rcx, 24(%r14)	# rng__d_lsm.274, rng_44(D)->d
.L171:
# find-perfect-hash.c:537: 	c->n = n;
	movl	-216(%rbp), %eax	# %sfp, bits
	movq	-96(%rbp), %rdi	# ctx.hits, pretmp_2503
	movl	%eax, 8(%r13)	# bits, MEM[base: _2638, offset: 8B]
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	movl	-104(%rbp), %eax	# ctx.n, _1168
	testl	%eax, %eax	# _1168
	je	.L252	#,
	subl	$1, %eax	#, tmp961
# find-perfect-hash.c:492: 		uint64_t *fullhash = ctx->data[j].hash;
	movq	-112(%rbp), %rdx	# ctx.data, _174
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%ebx, %ebx	# collisions
	imulq	$408, %rax, %rax	#, tmp961, tmp962
	leaq	16(%rdx), %rsi	#, ivtmp.411
	leaq	424(%rdx,%rax), %r11	#, _2672
	movq	-296(%rbp), %rax	# %sfp, _2700
	leaq	(%r9,%rax), %r10	#, _2698
.L182:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movl	-216(%rbp), %ecx	# %sfp,
	testl	%ecx, %ecx	#
	je	.L253	#,
	movq	%r9, %r8	# _18, ivtmp.405
# find-perfect-hash.c:491: 		uint32_t hash = 0;
	xorl	%eax, %eax	# hash
.L181:
# find-perfect-hash.c:494: 			uint32_t b = columns->bits[i];
	movl	(%r8), %ecx	# MEM[base: _2704, offset: 0B], b
# find-perfect-hash.c:495: 			hash <<= 1;
	addl	%eax, %eax	# hash
	addq	$4, %r8	#, ivtmp.405
# find-perfect-hash.c:496: 			hash |= READ_HASH_BIT(fullhash, b);
	movl	%ecx, %edx	# b, tmp964
	shrl	$6, %edx	#,
	movq	(%rsi,%rdx,8), %rdx	# *_191, *_191
	shrq	%cl, %rdx	# b, tmp967
	andl	$1, %edx	#, tmp969
	orl	%edx, %eax	# tmp969, hash
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	cmpq	%r8, %r10	# ivtmp.405, _2698
	jne	.L181	#,
	movl	%eax, %ecx	# hash, tmp1665
	movl	$1, %edx	#, prephitmp_2556
	shrl	$6, %eax	#,
	salq	%cl, %rdx	# tmp1665, prephitmp_2556
	leaq	(%rdi,%rax,8), %rcx	#, prephitmp_2562
	jmp	.L180	#
.L253:
	movq	%rdi, %rcx	# pretmp_2503, prephitmp_2562
	movl	$1, %edx	#, prephitmp_2556
.L180:
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	movq	(%rcx), %rax	# *prephitmp_2562, _209
	xorl	%r8d, %r8d	# tmp976
	testq	%rdx, %rax	# prephitmp_2556, _209
	setne	%r8b	#, tmp976
	addq	$408, %rsi	#, ivtmp.411
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	orq	%rdx, %rax	# prephitmp_2556, tmp978
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	addl	%r8d, %ebx	# tmp976, collisions
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	cmpq	%rsi, %r11	# ivtmp.411, _2672
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	movq	%rax, (%rcx)	# tmp978, *prephitmp_2562
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	jne	.L182	#,
	jmp	.L179	#
.L252:
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%ebx, %ebx	# collisions
.L179:
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movq	-232(%rbp), %rdx	# %sfp,
	xorl	%esi, %esi	#
	addq	$16, %r13	#, ivtmp.441
	call	memset	#
# find-perfect-hash.c:863: 		columns[i].collisions = test_one_subset(&ctx, &columns[i]);
	movl	%ebx, -4(%r13)	# collisions, MEM[base: _2638, offset: 12B]
	addl	-244(%rbp), %r15d	# %sfp, ivtmp.440
# find-perfect-hash.c:860: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpq	%r13, -264(%rbp)	# ivtmp.441, %sfp
	jne	.L183	#,
# find-perfect-hash.c:799: 	printf("initial state \n");
	movl	$.LC47, %edi	#,
	movq	%r13, -320(%rbp)	# ivtmp.441, %sfp
# find-perfect-hash.c:763: 		qsort(columns, N_CANDIDATES, sizeof(struct columns),
	xorl	%r13d, %r13d	# count
# find-perfect-hash.c:799: 	printf("initial state \n");
	call	puts	#
# find-perfect-hash.c:800: 	summarise_all_columns(columns);
	movq	-280(%rbp), %r12	# %sfp, columns
	movq	%r12, %rdi	# columns,
# find-perfect-hash.c:763: 		qsort(columns, N_CANDIDATES, sizeof(struct columns),
	movq	%r12, %rbx	# columns, ivtmp.400
# find-perfect-hash.c:800: 	summarise_all_columns(columns);
	call	summarise_all_columns	#
# find-perfect-hash.c:763: 		qsort(columns, N_CANDIDATES, sizeof(struct columns),
	movl	$cmp_columns, %ecx	#,
	movl	$16, %edx	#,
	movl	$1000, %esi	#,
	movq	%r12, %rdi	# columns,
	call	qsort	#
.L192:
# find-perfect-hash.c:697: 	if (a == b) {
	cmpq	%r12, %rbx	# c, ivtmp.400
	je	.L184	#,
# find-perfect-hash.c:700: 	int c = (int)a->collisions - (int)b->collisions;
	movl	12(%r12), %eax	# c_1199->collisions, c_1199->collisions
# find-perfect-hash.c:701: 	if (c != 0) {
	subl	12(%rbx), %eax	# MEM[base: d_318, offset: 12B], c
	jne	.L185	#,
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	movl	8(%r12), %edx	# c_1199->n,
	movl	8(%rbx), %ecx	# MEM[base: d_318, offset: 8B], _339
# find-perfect-hash.c:707: 	return a->n - b->n;
	movl	%edx, %eax	# _338, c
	subl	%ecx, %eax	# _339, c
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	cmpl	%ecx, %edx	# _339, _338
	jne	.L187	#,
# find-perfect-hash.c:705: 		return memcmp(a->bits, b->bits, a->n);
	movq	(%rbx), %rsi	# MEM[base: d_318, offset: 0B],
	movq	(%r12), %rdi	# c_1199->bits,
	call	memcmp	#
.L187:
# find-perfect-hash.c:772: 		if (cmp == 0) {
	testl	%eax, %eax	# c
	je	.L184	#,
.L185:
# find-perfect-hash.c:776: 		if (cmp > 0) {
	testl	%eax, %eax	# c
	jg	.L329	#,
# find-perfect-hash.c:780: 		printf("%3d× ", count);
	movl	%r13d, %esi	# count,
	movl	$.LC49, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:781: 		describe_columns(ctx, c);
	leaq	-88(%rbp), %rdi	#, tmp1669
	movq	%r12, %rsi	# c,
	call	describe_columns.isra.5	#
# find-perfect-hash.c:782: 		if (c->collisions < d->collisions) {
	movl	12(%rbx), %eax	# MEM[base: d_318, offset: 12B], tmp1670
	cmpl	%eax, 12(%r12)	# tmp1670, c_1199->collisions
	jb	.L190	#,
	movq	%rbx, %r12	# ivtmp.400, c
# find-perfect-hash.c:785: 		count = 0;
	xorl	%r13d, %r13d	# count
.L188:
	addq	$16, %rbx	#, ivtmp.400
# find-perfect-hash.c:769: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpq	%rbx, -320(%rbp)	# ivtmp.400, %sfp
	jne	.L192	#,
.L190:
	movq	-280(%rbp), %rax	# %sfp, columns
# find-perfect-hash.c:785: 		count = 0;
	movl	$0, -288(%rbp)	#, %sfp
	movl	$10, -244(%rbp)	#, %sfp
	addq	$8000, %rax	#, ivtmp.325
	movq	%rax, -336(%rbp)	# ivtmp.325, %sfp
.L247:
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	-244(%rbp), %eax	# %sfp, temp
# find-perfect-hash.c:806: 		ctx->stats.worsenings = 0;
	movq	$0, -80(%rbp)	#, MEM[(unsigned int *)&ctx + 32B]
# find-perfect-hash.c:805: 		ctx->stats.inbreds = 0;
	movl	$0, -72(%rbp)	#, MEM[(unsigned int *)&ctx + 40B]
	movl	$500, -324(%rbp)	#, %sfp
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	addl	$1, %eax	#, tmp1335
	movl	%eax, -328(%rbp)	# tmp1335, %sfp
.L193:
	movl	-328(%rbp), %eax	# %sfp, tmp1336
# find-perfect-hash.c:685: 		n_changes++;
	movq	-280(%rbp), %r13	# %sfp, ivtmp.381
# find-perfect-hash.c:617: 	for (i = 0; i < N_CANDIDATES; i++) {
	movl	$0, -304(%rbp)	#, %sfp
	movq	%r13, %r15	# ivtmp.381, ivtmp.381
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rax, -272(%rbp)	# tmp1336, %sfp
.L222:
# find-perfect-hash.c:622: 		uint32_t best_drop[c->n + candidates];
	movl	8(%r15), %r9d	# MEM[base: _2729, offset: 8B],
# find-perfect-hash.c:620: 		uint32_t old_collisions = c->collisions;
	movl	12(%r15), %eax	# MEM[base: _2729, offset: 12B], old_collisions
# find-perfect-hash.c:630: 		bool unique = true;
	movl	$1, %ebx	#, unique
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _312
	movq	(%r14), %rsi	# rng_44(D)->a, e
# find-perfect-hash.c:617: 	for (i = 0; i < N_CANDIDATES; i++) {
	movq	%rsp, -312(%rbp)	#, %sfp
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %rdi	# rng_44(D)->d, _593
# find-perfect-hash.c:620: 		uint32_t old_collisions = c->collisions;
	movl	%eax, -248(%rbp)	# old_collisions, %sfp
# find-perfect-hash.c:627: 			c->bits[c->n] = rand_range(rng, 0, HASH_BITS - 1);
	movq	(%r15), %r13	# MEM[base: _2729, offset: 0B], _430
# find-perfect-hash.c:622: 		uint32_t best_drop[c->n + candidates];
	leal	5(%r9), %eax	#, tmp995
	movl	%r9d, -296(%rbp)	# _353, %sfp
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rdi, %r8	# _593, tmp1011
# find-perfect-hash.c:622: 		uint32_t best_drop[c->n + candidates];
	leaq	18(,%rax,4), %rax	#, tmp999
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %r8	#, tmp1011
# find-perfect-hash.c:622: 		uint32_t best_drop[c->n + candidates];
	shrq	$4, %rax	#, tmp1001
	salq	$4, %rax	#, tmp1003
	subq	%rax, %rsp	# tmp1003,
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdx, %rax	# _312, tmp1008
	rolq	$7, %rax	#, tmp1008
# find-perfect-hash.c:622: 		uint32_t best_drop[c->n + candidates];
	movq	%rsp, -264(%rbp)	#, %sfp
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %rsi	# tmp1008, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %rax	# rng_44(D)->c, _286
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rsi, %rdi	# e, _576
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rax, %rcx	# _286, tmp1010
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rax, %r8	# _286, _489
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %rax	#, tmp1580
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	rolq	$13, %rcx	#, tmp1010
	xorq	%rdx, %rcx	# _312, _488
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rcx, %rsi	# _488, _467
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rsi, %rdx	# _467, tmp1014
	shrq	$6, %rdx	#, tmp1014
	mulq	%rdx	# tmp1014
	movq	%rsi, %rax	# _467, tmp1018
	imulq	$3136, %rdx, %rdx	#, tmp1015, tmp1017
	subq	%rdx, %rax	# tmp1017, tmp1018
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%r8, %rdx	# _489, tmp1020
	rolq	$7, %rdx	#, tmp1020
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%eax, 0(%r13,%r9,4)	# tmp1018, *_329
# find-perfect-hash.c:628: 			c->n++;
	movl	8(%r15), %eax	# MEM[base: _2729, offset: 8B], tmp1687
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rdx, %rcx	# tmp1020, _488
	movq	%rcx, %rdx	# _488, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rdi, %rcx	# _576, tmp1021
	rolq	$13, %rcx	#, tmp1021
# find-perfect-hash.c:628: 			c->n++;
	leal	1(%rax), %r9d	#,
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %rax	#, tmp1581
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	xorq	%r8, %rcx	# _489, _663
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rsi, %r8	# _467, tmp1022
	rorq	$27, %r8	#, tmp1022
# find-perfect-hash.c:628: 			c->n++;
	movl	%r9d, 8(%r15)	# _524, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%r8, %rdi	# tmp1022, _755
# find-perfect-hash.c:37: 	x->c = x->d + e;
	leaq	(%rsi,%rdx), %r8	#, _694
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%rdx,%rcx), %rsi	#, _738
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rsi, %rdx	# _738, tmp1025
	shrq	$6, %rdx	#, tmp1025
	mulq	%rdx	# tmp1025
	movq	%rsi, %rax	# _738, tmp1029
	imulq	$3136, %rdx, %rdx	#, tmp1026, tmp1028
	subq	%rdx, %rax	# tmp1028, tmp1029
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdi, %rdx	# _755, tmp1031
	rolq	$7, %rdx	#, tmp1031
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%eax, 0(%r13,%r9,4)	# tmp1029, *_793
# find-perfect-hash.c:628: 			c->n++;
	movl	8(%r15), %eax	# MEM[base: _2729, offset: 8B], tmp1695
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rdx, %rcx	# tmp1031, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r8, %rdx	# _694, tmp1032
	rolq	$13, %rdx	#, tmp1032
	xorq	%rdx, %rdi	# tmp1032, _989
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rsi, %rdx	# _738, tmp1033
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rcx, %rsi	# e, _995
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdx	#, tmp1033
# find-perfect-hash.c:628: 			c->n++;
	leal	1(%rax), %r9d	#,
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %rax	#, tmp1582
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rdx, %r8	# tmp1033, _993
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rdi, %rcx	# _989, _997
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rcx, %rdx	# _997, tmp1036
# find-perfect-hash.c:628: 			c->n++;
	movl	%r9d, 8(%r15)	# _199, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$6, %rdx	#, tmp1036
	mulq	%rdx	# tmp1036
	movq	%rcx, %rax	# _997, tmp1040
	imulq	$3136, %rdx, %rdx	#, tmp1037, tmp1039
	subq	%rdx, %rax	# tmp1039, tmp1040
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%r8, %rdx	# _993, tmp1042
	rolq	$7, %rdx	#, tmp1042
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%eax, 0(%r13,%r9,4)	# tmp1040, *_622
# find-perfect-hash.c:628: 			c->n++;
	movl	8(%r15), %eax	# MEM[base: _2729, offset: 8B], tmp1702
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rdx, %rdi	# tmp1042, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rsi, %rdx	# _995, tmp1043
	rolq	$13, %rdx	#, tmp1043
	xorq	%rdx, %r8	# tmp1043, _1020
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rcx, %rdx	# _997, tmp1044
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rdi, %rcx	# e, _1026
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdx	#, tmp1044
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%r8, %rdi	# _1020, _1028
# find-perfect-hash.c:628: 			c->n++;
	leal	1(%rax), %r9d	#,
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rdx, %rsi	# tmp1044, _1024
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rdi, %rdx	# _1028, tmp1047
	movabsq	$376464164769582687, %rax	#, tmp1583
	shrq	$6, %rdx	#, tmp1047
# find-perfect-hash.c:628: 			c->n++;
	movl	%r9d, 8(%r15)	# _1003, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	mulq	%rdx	# tmp1047
	movq	%rdi, %rax	# _1028, tmp1051
	imulq	$3136, %rdx, %rdx	#, tmp1048, tmp1050
	subq	%rdx, %rax	# tmp1050, tmp1051
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rsi, %rdx	# _1024, tmp1053
	rolq	$7, %rdx	#, tmp1053
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%eax, 0(%r13,%r9,4)	# tmp1051, *_1013
# find-perfect-hash.c:628: 			c->n++;
	movl	8(%r15), %eax	# MEM[base: _2729, offset: 8B], tmp1709
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rdx, %r8	# tmp1053, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rcx, %rdx	# _1026, tmp1054
	rolq	$13, %rdx	#, tmp1054
	xorq	%rdx, %rsi	# tmp1054, _1051
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rdi, %rdx	# _1028, tmp1055
# find-perfect-hash.c:628: 			c->n++;
	addl	$1, %eax	#, _1034
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdx	#, tmp1055
# find-perfect-hash.c:628: 			c->n++;
	movl	%eax, 8(%r15)	# _1034, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rsi, (%r14)	# _1051, rng_44(D)->a
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rdx, %rcx	# tmp1055, tmp1056
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%r8, %rdi	# e, tmp1057
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rcx, 8(%r14)	# tmp1056, rng_44(D)->b
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%r8,%rsi), %rcx	#, _1059
# find-perfect-hash.c:627: 			c->bits[c->n] = rand_range(rng, 0, HASH_BITS - 1);
	movl	%eax, %esi	# _1034, _1034
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %rax	#, tmp1584
# find-perfect-hash.c:37: 	x->c = x->d + e;
	movq	%rdi, 16(%r14)	# tmp1057, rng_44(D)->c
	movq	%r13, %rdi	# _430, _430
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rcx, %rdx	# _1059, tmp1060
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%rcx, 24(%r14)	# _1059, rng_44(D)->d
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$6, %rdx	#, tmp1060
	mulq	%rdx	# tmp1060
	imulq	$3136, %rdx, %rdx	#, tmp1061, tmp1063
	subq	%rdx, %rcx	# tmp1063, tmp1064
	movl	%ecx, 0(%r13,%rsi,4)	# tmp1064, *_1044
# find-perfect-hash.c:628: 			c->n++;
	movl	8(%r15), %eax	# MEM[base: _2729, offset: 8B], tmp1715
	leal	1(%rax), %esi	#,
	movl	%esi, 8(%r15)	# prephitmp_2600, MEM[base: _2729, offset: 8B]
.L194:
# find-perfect-hash.c:632: 			qsort(c->bits, c->n, sizeof(uint32_t), cmp_uint32);
	movl	$4, %edx	#,
	movl	$cmp_uint32, %ecx	#,
	call	qsort	#
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	movl	8(%r15), %esi	# MEM[base: _2729, offset: 8B],
# find-perfect-hash.c:633: 			uint32_t prev = c->bits[0];
	movq	(%r15), %rdi	# MEM[base: _2729, offset: 0B], _430
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	movl	$2, %r8d	#, ivtmp.369
# find-perfect-hash.c:633: 			uint32_t prev = c->bits[0];
	movl	(%rdi), %edx	# *_385, prev
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	cmpl	$1, %esi	#, prephitmp_2600
	jbe	.L196	#,
# find-perfect-hash.c:635: 				if (c->bits[j] == prev) {
	movl	-4(%rdi,%r8,4), %eax	# MEM[base: _385, index: ivtmp.369_2778, step: 4, offset: -4B], _391
	cmpl	%edx, %eax	# prev, _391
	je	.L330	#,
	.p2align 4,,10
	.p2align 3
.L197:
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	movl	%r8d, %edx	# ivtmp.369, j.41_410
	addq	$1, %r8	#, ivtmp.369
	cmpl	%esi, %edx	# prephitmp_2600, j.41_410
	jnb	.L196	#,
# find-perfect-hash.c:637: 					unique = false;
	movl	%eax, %edx	# _391, prev
.L331:
# find-perfect-hash.c:635: 				if (c->bits[j] == prev) {
	movl	-4(%rdi,%r8,4), %eax	# MEM[base: _385, index: ivtmp.369_2778, step: 4, offset: -4B], _391
	cmpl	%edx, %eax	# prev, _391
	jne	.L197	#,
.L330:
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %r9	# rng_44(D)->c, _397
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _394
	movq	(%r14), %rbx	# rng_44(D)->a, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r9, %rcx	# _397, tmp1069
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdx, %rax	# _394, tmp1067
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	rolq	$13, %rcx	#, tmp1069
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	rolq	$7, %rax	#, tmp1067
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	xorq	%rdx, %rcx	# _394, _399
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %rdx	# rng_44(D)->d, _400
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %rbx	# tmp1067, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rcx, (%r14)	# _399, rng_44(D)->a
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rbx, %rcx	# e, _404
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$376464164769582687, %rax	#, tmp1585
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%rcx, 24(%r14)	# _404, rng_44(D)->d
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rdx, %rsi	# _400, tmp1070
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rbx, %rdx	# e, tmp1072
	movq	%rdx, 16(%r14)	# tmp1072, rng_44(D)->c
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rcx, %rdx	# _404, tmp1074
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rsi	#, tmp1070
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$6, %rdx	#, tmp1074
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%r9, %rsi	# _397, tmp1071
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	mulq	%rdx	# tmp1074
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rsi, 8(%r14)	# tmp1071, rng_44(D)->b
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	imulq	$3136, %rdx, %rdx	#, tmp1075, tmp1077
	subq	%rdx, %rcx	# tmp1077, tmp1078
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	movl	%r8d, %edx	# ivtmp.369, j.41_110
# find-perfect-hash.c:636: 					c->bits[j] = rand_range(rng, 0, HASH_BITS - 1);
	movl	%ecx, -4(%rdi,%r8,4)	# tmp1078, MEM[base: _385, index: ivtmp.369_2778, step: 4, offset: -4B]
	movl	8(%r15), %esi	# MEM[base: _2729, offset: 8B],
	addq	$1, %r8	#, ivtmp.369
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	%ecx, %eax	# tmp1078, _391
# find-perfect-hash.c:634: 			for (j = 1; j < c->n; j++) {
	cmpl	%esi, %edx	# prephitmp_2600, j.41_110
	jnb	.L199	#,
# find-perfect-hash.c:637: 					unique = false;
	xorl	%ebx, %ebx	# unique
	movl	%eax, %edx	# _391, prev
	jmp	.L331	#
.L184:
# find-perfect-hash.c:773: 			count++;
	addl	$1, %r13d	#, count
	jmp	.L188	#
	.p2align 4,,10
	.p2align 3
.L196:
# find-perfect-hash.c:641: 		} while (! unique);
	testb	%bl, %bl	# unique
	jne	.L332	#,
.L199:
	xorl	%ebx, %ebx	# unique
	jmp	.L194	#
.L332:
# find-perfect-hash.c:645: 		while (c->n > old_n) {
	cmpl	%esi, -296(%rbp)	# prephitmp_2600, %sfp
	movq	%rdi, %r13	# _430, _430
	jnb	.L200	#,
.L211:
# find-perfect-hash.c:646: 			best_collisions = ctx->n;
	movl	-104(%rbp), %eax	# ctx.n, best_collisions
	xorl	%r12d, %r12d	# ivtmp.364
# find-perfect-hash.c:647: 			best_drop_n = 0;
	movl	$0, -256(%rbp)	#, %sfp
# find-perfect-hash.c:646: 			best_collisions = ctx->n;
	movl	%eax, -240(%rbp)	# best_collisions, %sfp
	.p2align 4,,10
	.p2align 3
.L208:
# find-perfect-hash.c:651: 				c->bits[j] = c->bits[c->n - 1];
	leal	-1(%rsi), %edx	#, tmp1080
	leaq	0(,%r12,4), %rax	#, _2947
	movl	%r12d, -216(%rbp)	# ivtmp.364, %sfp
	movq	-96(%rbp), %rdi	# ctx.hits, pretmp_2630
	movl	0(%r13,%rdx,4), %edx	# *_423, _424
	movq	%rax, -232(%rbp)	# _2947, %sfp
# find-perfect-hash.c:650: 				uint32_t tmp = c->bits[j];
	addq	%r13, %rax	# _430, _417
	movl	(%rax), %ebx	# *_417, tmp
# find-perfect-hash.c:651: 				c->bits[j] = c->bits[c->n - 1];
	movl	%edx, (%rax)	# _424, *_417
# find-perfect-hash.c:652: 				c->n--;
	movl	8(%r15), %edx	# MEM[base: _2729, offset: 8B], _425
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	movl	-104(%rbp), %eax	# ctx.n, _267
# find-perfect-hash.c:650: 				uint32_t tmp = c->bits[j];
	movl	%ebx, -224(%rbp)	# tmp, %sfp
# find-perfect-hash.c:652: 				c->n--;
	leal	-1(%rdx), %r10d	#, _426
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	testl	%eax, %eax	# _267
# find-perfect-hash.c:652: 				c->n--;
	movl	%r10d, 8(%r15)	# _426, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	je	.L256	#,
	subl	$1, %eax	#, tmp1082
# find-perfect-hash.c:492: 		uint64_t *fullhash = ctx->data[j].hash;
	movq	-112(%rbp), %rcx	# ctx.data, _499
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%ebx, %ebx	# collisions
	imulq	$408, %rax, %rax	#, tmp1082, tmp1083
	leaq	16(%rcx), %r8	#, ivtmp.360
	leaq	424(%rcx,%rax), %r11	#, _1193
	leal	-2(%rdx), %eax	#, tmp1086
	leaq	4(%r13,%rax,4), %r9	#, _2917
	.p2align 4,,10
	.p2align 3
.L204:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	testl	%r10d, %r10d	# _426
	je	.L257	#,
	movq	%r13, %rsi	# _430, ivtmp.354
# find-perfect-hash.c:491: 		uint32_t hash = 0;
	xorl	%eax, %eax	# hash
	.p2align 4,,10
	.p2align 3
.L203:
# find-perfect-hash.c:494: 			uint32_t b = columns->bits[i];
	movl	(%rsi), %ecx	# MEM[base: _2911, offset: 0B], b
# find-perfect-hash.c:495: 			hash <<= 1;
	addl	%eax, %eax	# hash
	addq	$4, %rsi	#, ivtmp.354
# find-perfect-hash.c:496: 			hash |= READ_HASH_BIT(fullhash, b);
	movl	%ecx, %edx	# b, tmp1089
	shrl	$6, %edx	#,
	movq	(%r8,%rdx,8), %rdx	# *_516, *_516
	shrq	%cl, %rdx	# b, tmp1092
	andl	$1, %edx	#, tmp1094
	orl	%edx, %eax	# tmp1094, hash
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	cmpq	%r9, %rsi	# _2917, ivtmp.354
	jne	.L203	#,
	movl	%eax, %ecx	# hash, tmp1729
	movl	$1, %edx	#, _2682
	shrl	$6, %eax	#,
	salq	%cl, %rdx	# tmp1729, _2682
	leaq	(%rdi,%rax,8), %rcx	#, _2688
.L202:
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	movq	(%rcx), %rax	# *prephitmp_2689, _534
	xorl	%esi, %esi	# tmp1101
	testq	%rdx, %rax	# _2682, _534
	setne	%sil	#, tmp1101
	addq	$408, %r8	#, ivtmp.360
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	orq	%rax, %rdx	# _534, tmp1103
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	addl	%esi, %ebx	# tmp1101, collisions
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	cmpq	%r8, %r11	# ivtmp.360, _1193
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	movq	%rdx, (%rcx)	# tmp1103, *prephitmp_2689
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	jne	.L204	#,
.L201:
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movl	$1, %edx	#, tmp1105
	movl	%r10d, %ecx	# _426, tmp1731
	movl	%edx, %eax	# tmp1105, tmp1105
	sall	%cl, %eax	# tmp1731, tmp1105
	leal	7(%rax), %edx	#, tmp1108
	testl	%eax, %eax	# tmp1104
	cmovns	%eax, %edx	# tmp1108,, tmp1104, tmp1107
	xorl	%esi, %esi	#
	sarl	$3, %edx	#, tmp1109
	movslq	%edx, %rdx	# tmp1109, tmp1110
	call	memset	#
# find-perfect-hash.c:657: 				if (temp) {
	movl	-244(%rbp), %edx	# %sfp,
# find-perfect-hash.c:655: 				c->bits[j] = tmp;
	movq	(%r15), %r13	# MEM[base: _2729, offset: 0B], _430
	movq	-232(%rbp), %rax	# %sfp, _2947
	movl	-224(%rbp), %edi	# %sfp, tmp
# find-perfect-hash.c:654: 				c->n++;
	addl	$1, 8(%r15)	#, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:657: 				if (temp) {
	testl	%edx, %edx	#
# find-perfect-hash.c:655: 				c->bits[j] = tmp;
	movl	%edi, 0(%r13,%rax)	# tmp, *_431
# find-perfect-hash.c:657: 				if (temp) {
	je	.L205	#,
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _433
	movq	(%r14), %rdi	# rng_44(D)->a, e
	movq	%rdx, %rax	# _433, tmp1115
	rolq	$7, %rax	#, tmp1115
	subq	%rax, %rdi	# tmp1115, e
	movq	%rdi, %rax	# e, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %rdi	# rng_44(D)->c, _436
	movq	%rdi, %rcx	# _436, tmp1117
	rolq	$13, %rcx	#, tmp1117
	xorq	%rcx, %rdx	# tmp1117, _438
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %rcx	# rng_44(D)->d, _439
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rdx, (%r14)	# _438, rng_44(D)->a
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rcx, %rsi	# _439, tmp1118
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rax, %rcx	# e, tmp1120
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rdx, %rax	# _438, _443
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	xorl	%edx, %edx	# tmp1124
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%rax, 24(%r14)	# _443, rng_44(D)->d
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rsi	#, tmp1118
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	divq	-272(%rbp)	# %sfp
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rdi, %rsi	# _436, tmp1119
# find-perfect-hash.c:37: 	x->c = x->d + e;
	movq	%rcx, 16(%r14)	# tmp1120, rng_44(D)->c
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rsi, 8(%r14)	# tmp1119, rng_44(D)->b
# find-perfect-hash.c:658: 					collisions += rand_range(rng, 0, temp);
	addl	%edx, %ebx	# tmp1124, collisions
.L205:
# find-perfect-hash.c:660: 				if (collisions < best_collisions) {
	cmpl	%ebx, -240(%rbp)	# collisions, %sfp
	jbe	.L206	#,
# find-perfect-hash.c:662: 					best_drop[0] = j;
	movq	-264(%rbp), %rax	# %sfp, tmp1007
	movl	-216(%rbp), %edi	# %sfp, j.44_1006
	movl	%ebx, -240(%rbp)	# collisions, %sfp
# find-perfect-hash.c:663: 					best_drop_n = 1;
	movl	$1, -256(%rbp)	#, %sfp
# find-perfect-hash.c:662: 					best_drop[0] = j;
	movl	%edi, (%rax)	# j.44_1006, MEM[(uint32_t[0:D.5145] *)best_drop.39_357]
.L207:
# find-perfect-hash.c:649: 			for (j = 0; j < c->n; j++) {
	movl	-216(%rbp), %eax	# %sfp, j.44_1006
	movl	8(%r15), %esi	# MEM[base: _2729, offset: 8B], prephitmp_2600
	addq	$1, %r12	#, ivtmp.364
	addl	$1, %eax	#, j.44_1006
	cmpl	%esi, %eax	# prephitmp_2600, tmp1127
	jb	.L208	#,
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %r8	# rng_44(D)->c, _459
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _456
	movq	(%r14), %rbx	# rng_44(D)->a, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r8, %rcx	# _459, tmp1130
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdx, %rax	# _456, tmp1128
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	rolq	$13, %rcx	#, tmp1130
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	rolq	$7, %rax	#, tmp1128
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	xorq	%rcx, %rdx	# tmp1130, _461
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %rcx	# rng_44(D)->d, _462
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %rbx	# tmp1128, e
	movq	%rbx, %rax	# e, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rdx, (%r14)	# _461, rng_44(D)->a
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rdx, %rax	# _461, _466
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	xorl	%edx, %edx	# tmp1136
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rcx, %rdi	# _462, tmp1131
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rbx, %rcx	# e, tmp1133
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%rax, 24(%r14)	# _466, rng_44(D)->d
# find-perfect-hash.c:37: 	x->c = x->d + e;
	movq	%rcx, 16(%r14)	# tmp1133, rng_44(D)->c
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movl	-256(%rbp), %ecx	# %sfp, best_drop_n
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdi	#, tmp1131
# find-perfect-hash.c:671: 			b = best_drop[j];
	movq	-264(%rbp), %rbx	# %sfp, tmp1007
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%r8, %rdi	# _459, tmp1132
	movq	%rdi, 8(%r14)	# tmp1132, rng_44(D)->b
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	divq	%rcx	# best_drop_n
# find-perfect-hash.c:671: 			b = best_drop[j];
	movslq	%edx, %rax	# tmp1136, tmp1138
	movl	(%rbx,%rax,4), %edx	# MEM[(uint32_t[0:D.5145] *)best_drop.39_357], j
# find-perfect-hash.c:672: 			for (j = b; j < c->n; j++) {
	cmpl	%edx, %esi	# j, prephitmp_2600
	jbe	.L209	#,
	movslq	%edx, %rax	# j, j
	leaq	0(%r13,%rax,4), %rax	#, ivtmp.345
	.p2align 4,,10
	.p2align 3
.L210:
# find-perfect-hash.c:673: 				c->bits[j - 1] = c->bits[j];
	movl	(%rax), %ecx	# MEM[base: _2903, offset: 0B], MEM[base: _2903, offset: 0B]
# find-perfect-hash.c:672: 			for (j = b; j < c->n; j++) {
	addl	$1, %edx	#, j
	addq	$4, %rax	#, ivtmp.345
# find-perfect-hash.c:673: 				c->bits[j - 1] = c->bits[j];
	movl	%ecx, -8(%rax)	# MEM[base: _2903, offset: 0B], MEM[base: _2903, offset: -4B]
# find-perfect-hash.c:672: 			for (j = b; j < c->n; j++) {
	movl	8(%r15), %esi	# MEM[base: _2729, offset: 8B], prephitmp_2600
	cmpl	%edx, %esi	# j, prephitmp_2600
	ja	.L210	#,
.L209:
# find-perfect-hash.c:675: 			c->n--;
	subl	$1, %esi	#, prephitmp_2600
# find-perfect-hash.c:645: 		while (c->n > old_n) {
	cmpl	%esi, -296(%rbp)	# prephitmp_2600, %sfp
# find-perfect-hash.c:675: 			c->n--;
	movl	%esi, 8(%r15)	# prephitmp_2600, MEM[base: _2729, offset: 8B]
# find-perfect-hash.c:645: 		while (c->n > old_n) {
	jb	.L211	#,
.L200:
	movl	%esi, %ecx	# prephitmp_2600, tmp1755
	movl	$1, %edx	#, tmp1143
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	movl	-104(%rbp), %eax	# ctx.n, _1158
	sall	%cl, %edx	# tmp1755, tmp1142
	movq	-96(%rbp), %rdi	# ctx.hits, pretmp_2713
	leal	7(%rdx), %ecx	#, tmp1146
	testl	%edx, %edx	# tmp1142
	cmovs	%ecx, %edx	# tmp1146,, tmp1145
	sarl	$3, %edx	#, tmp1147
	testl	%eax, %eax	# _1158
	movslq	%edx, %rbx	# tmp1147, _2712
	movq	%rbx, -216(%rbp)	# _2712, %sfp
	je	.L212	#,
	subl	$1, %eax	#, tmp1149
# find-perfect-hash.c:492: 		uint64_t *fullhash = ctx->data[j].hash;
	movq	-112(%rbp), %rdx	# ctx.data, _549
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%ebx, %ebx	# collisions
	imulq	$408, %rax, %rax	#, tmp1149, tmp1150
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movl	$1, %r12d	#, _2765
	leaq	16(%rdx), %r9	#, ivtmp.338
	leaq	424(%rdx,%rax), %r11	#, _2898
	leal	-1(%rsi), %eax	#, tmp1153
	leaq	4(%r13,%rax,4), %r10	#, _2869
.L215:
	testl	%esi, %esi	# prephitmp_2600
	je	.L258	#,
	movq	%r13, %r8	# _430, ivtmp.332
# find-perfect-hash.c:491: 		uint32_t hash = 0;
	xorl	%eax, %eax	# hash
	.p2align 4,,10
	.p2align 3
.L214:
# find-perfect-hash.c:494: 			uint32_t b = columns->bits[i];
	movl	(%r8), %ecx	# MEM[base: _2863, offset: 0B], b
# find-perfect-hash.c:495: 			hash <<= 1;
	addl	%eax, %eax	# hash
	addq	$4, %r8	#, ivtmp.332
# find-perfect-hash.c:496: 			hash |= READ_HASH_BIT(fullhash, b);
	movl	%ecx, %edx	# b, tmp1156
	shrl	$6, %edx	#,
	movq	(%r9,%rdx,8), %rdx	# *_566, *_566
	shrq	%cl, %rdx	# b, tmp1159
	andl	$1, %edx	#, tmp1161
	orl	%edx, %eax	# tmp1161, hash
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	cmpq	%r10, %r8	# _2869, ivtmp.332
	jne	.L214	#,
	movl	%eax, %ecx	# hash, tmp1759
	movq	%r12, %rdx	# _2765, _2765
	shrl	$6, %eax	#,
	salq	%cl, %rdx	# tmp1759, _2765
	leaq	(%rdi,%rax,8), %rcx	#, _2771
.L213:
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	movq	(%rcx), %rax	# *prephitmp_2772, _584
	xorl	%r8d, %r8d	# tmp1168
	testq	%rdx, %rax	# _2765, _584
	setne	%r8b	#, tmp1168
	addq	$408, %r9	#, ivtmp.338
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	orq	%rax, %rdx	# _584, tmp1170
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	addl	%r8d, %ebx	# tmp1168, collisions
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	cmpq	%r11, %r9	# _2898, ivtmp.338
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	movq	%rdx, (%rcx)	# tmp1170, *prephitmp_2772
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	jne	.L215	#,
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	movq	-216(%rbp), %rdx	# %sfp,
	xorl	%esi, %esi	#
	call	memset	#
# find-perfect-hash.c:678: 		if (c->collisions > old_collisions) {
	cmpl	%ebx, -248(%rbp)	# collisions, %sfp
# find-perfect-hash.c:677: 		c->collisions = test_one_subset(ctx, c);
	movl	%ebx, 12(%r15)	# collisions, MEM[base: _2729, offset: 12B]
# find-perfect-hash.c:678: 		if (c->collisions > old_collisions) {
	jnb	.L216	#,
# find-perfect-hash.c:679: 			if (temp == 0) {
	movl	-244(%rbp), %eax	# %sfp,
	testl	%eax, %eax	#
	je	.L333	#,
.L217:
# find-perfect-hash.c:683: 			ctx->stats.worsenings++;
	addl	$1, -80(%rbp)	#, ctx.stats.worsenings
.L216:
# find-perfect-hash.c:685: 		n_changes++;
	movl	-304(%rbp), %edi	# %sfp, i
# find-perfect-hash.c:686: 		if (i % 100 == 0) {
	movl	$1374389535, %edx	#, tmp1177
# find-perfect-hash.c:685: 		n_changes++;
	movl	%edi, %eax	# i, i
	leal	1(%rdi), %ebx	#, n_changes
# find-perfect-hash.c:686: 		if (i % 100 == 0) {
	imull	%edx	# tmp1177
	sarl	$5, %edx	#, tmp1176
	movl	%edx, %eax	# tmp1176, tmp1178
	movl	%edi, %edx	# i, tmp1179
	sarl	$31, %edx	#, tmp1179
	subl	%edx, %eax	# tmp1179, tmp1175
	imull	$100, %eax, %eax	#, tmp1175, tmp1180
	cmpl	%eax, %edi	# tmp1180, i
	je	.L334	#,
	addq	$16, %r15	#, ivtmp.381
# find-perfect-hash.c:617: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpl	$1000, %ebx	#, n_changes
	movq	-312(%rbp), %rsp	# %sfp,
	je	.L220	#,
.L219:
# find-perfect-hash.c:685: 		n_changes++;
	movl	%ebx, -304(%rbp)	# n_changes, %sfp
	jmp	.L222	#
	.p2align 4,,10
	.p2align 3
.L257:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movq	%rdi, %rcx	# pretmp_2630, _2688
	movl	$1, %edx	#, _2682
	jmp	.L202	#
	.p2align 4,,10
	.p2align 3
.L206:
# find-perfect-hash.c:664: 				} else if (collisions == best_collisions) {
	jne	.L207	#,
# find-perfect-hash.c:665: 					best_drop[best_drop_n] = j;
	movl	-256(%rbp), %eax	# %sfp,
	movq	-264(%rbp), %rdi	# %sfp, tmp1007
	movl	-216(%rbp), %esi	# %sfp, j.44_1006
	movq	%rax, %rbx	#,
	movl	%esi, (%rdi,%rax,4)	# j.44_1006, MEM[(uint32_t[0:D.5145] *)best_drop.39_357]
# find-perfect-hash.c:666: 					best_drop_n++;
	addl	$1, %ebx	#, best_drop_n
	movl	%ebx, -256(%rbp)	# best_drop_n, %sfp
	jmp	.L207	#
.L256:
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%ebx, %ebx	# collisions
	jmp	.L201	#
.L258:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movq	%rdi, %rcx	# pretmp_2713, _2771
	movl	$1, %edx	#, _2765
	jmp	.L213	#
.L334:
# find-perfect-hash.c:687: 			printf("%d\n", i);
	movl	%edi, %esi	# i,
	xorl	%eax, %eax	#
	movl	$.LC51, %edi	#,
	call	printf	#
	addq	$16, %r15	#, ivtmp.381
# find-perfect-hash.c:617: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpl	$1000, %ebx	#, n_changes
	movq	-312(%rbp), %rsp	# %sfp,
	jne	.L219	#,
.L220:
# find-perfect-hash.c:808: 		for (j = 0; j < anneals; j++) {
	subl	$1, -324(%rbp)	#, %sfp
	jne	.L193	#,
# find-perfect-hash.c:818: 		printf("round %d; temp %u\n", i, temp);
	movl	-244(%rbp), %edx	# %sfp,
	movl	-288(%rbp), %esi	# %sfp,
	movl	$.LC52, %edi	#,
	xorl	%eax, %eax	#
# find-perfect-hash.c:821: 		printf("skipped %u (average %u)\n",
	movl	$274877907, %ebx	#, tmp1185
# find-perfect-hash.c:818: 		printf("round %d; temp %u\n", i, temp);
	call	printf	#
# find-perfect-hash.c:819: 		printf("anneal_changes %u (average %u, last %d)\n",
	movl	$1000, %ecx	#,
	movl	$1000, %edx	#,
	movl	$500000, %esi	#,
	movl	$.LC53, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:822: 		       ctx->stats.skipped, (ctx->stats.skipped + (j / 2)) / j);
	movl	-72(%rbp), %esi	# ctx.stats.skipped, _300
# find-perfect-hash.c:821: 		printf("skipped %u (average %u)\n",
	movl	$.LC54, %edi	#,
# find-perfect-hash.c:822: 		       ctx->stats.skipped, (ctx->stats.skipped + (j / 2)) / j);
	leal	250(%rsi), %edx	#, tmp1182
# find-perfect-hash.c:821: 		printf("skipped %u (average %u)\n",
	movl	%edx, %eax	# tmp1182, tmp1182
	mull	%ebx	# tmp1185
	xorl	%eax, %eax	#
	shrl	$5, %edx	#, tmp1183
	call	printf	#
# find-perfect-hash.c:823: 		printf("worsenings %u\n", ctx->stats.worsenings / j);
	movl	%ebx, %eax	# tmp1185, tmp1185
	movl	$.LC55, %edi	#,
	mull	-80(%rbp)	# ctx.stats.worsenings
	xorl	%eax, %eax	#
	movl	%edx, %esi	# tmp1188, tmp1188
	shrl	$5, %esi	#, tmp1188
	call	printf	#
# find-perfect-hash.c:824: 		printf("inbred %u\n", ctx->stats.inbreds);
	movl	-76(%rbp), %esi	# ctx.stats.inbreds,
	movl	$.LC56, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:826: 		summarise_all_columns(columns);
	movq	-280(%rbp), %rbx	# %sfp, columns
	movq	%rbx, %rdi	# columns,
	call	summarise_all_columns	#
# find-perfect-hash.c:720: 	printf("breeding..\n");
	movl	$.LC57, %edi	#,
	call	puts	#
# find-perfect-hash.c:722: 	qsort(columns, N_CANDIDATES, sizeof(struct columns), cmp_columns);
	movq	%rbx, %rdi	# columns,
	movl	$cmp_columns, %ecx	#,
	movl	$16, %edx	#,
	movl	$1000, %esi	#,
	call	qsort	#
	movq	-336(%rbp), %rax	# %sfp, ivtmp.325
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movq	%rax, %rbx	# ivtmp.325, ivtmp.325
.L232:
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	8(%r14), %rdx	# rng_44(D)->b, _631
	movq	(%r14), %r13	# rng_44(D)->a, e
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	24(%r14), %rsi	# rng_44(D)->d, _637
# find-perfect-hash.c:732: 		a = &columns[rand_range(rng, 0, n_breeders - 1)];
	movq	-280(%rbp), %r11	# %sfp, columns
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdx, %rax	# _631, tmp1191
	rolq	$7, %rax	#, tmp1191
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%rsi, %rdi	# _637, tmp1194
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %r13	# tmp1191, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	16(%r14), %rax	# rng_44(D)->c, _634
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %rdi	#, tmp1194
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%r13, %rsi	# e, _640
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%rsi, %r8	# _640, tmp1203
	movq	%rax, %rcx	# _634, tmp1193
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rax, %rdi	# _634, _639
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$2361183241434822607, %rax	#, tmp1589
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	rolq	$13, %rcx	#, tmp1193
	rolq	$13, %r8	#, tmp1203
	xorq	%rdx, %rcx	# _631, _636
	xorq	%rdi, %r8	# _639, rng__a_lsm.260
# find-perfect-hash.c:38: 	x->d = e + x->a;
	addq	%rcx, %r13	# _636, _641
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r8, (%r14)	# rng__a_lsm.260, rng_44(D)->a
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%r13, %rdx	# _641, tmp1196
	movq	%r13, %r15	# _641, tmp1200
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%r13, %r10	# _641, tmp1204
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$2, %rdx	#, tmp1196
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	rorq	$27, %r10	#, tmp1204
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	mulq	%rdx	# tmp1196
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	addq	%rsi, %r10	# _640, rng__b_lsm.262
	movq	%r10, 8(%r14)	# rng__b_lsm.262, rng_44(D)->b
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%rdx, %rax	# tmp1197, tmp1197
	shrq	$4, %rax	#, tmp1197
	imulq	$500, %rax, %rax	#, tmp1195, tmp1199
	subq	%rax, %r15	# tmp1199, tmp1200
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%rdi, %rax	# _639, tmp1202
	rolq	$7, %rax	#, tmp1202
# find-perfect-hash.c:732: 		a = &columns[rand_range(rng, 0, n_breeders - 1)];
	salq	$4, %r15	#, tmp1201
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	subq	%rax, %rcx	# tmp1202, e
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movabsq	$2361183241434822607, %rax	#, tmp1590
# find-perfect-hash.c:732: 		a = &columns[rand_range(rng, 0, n_breeders - 1)];
	addq	%r11, %r15	# columns, a
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%rcx,%r8), %r12	#, rng__d_lsm.266
# find-perfect-hash.c:37: 	x->c = x->d + e;
	addq	%rcx, %r13	# e, rng__c_lsm.264
	movq	%r13, 16(%r14)	# rng__c_lsm.264, rng_44(D)->c
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	movq	%r12, %rdx	# rng__d_lsm.266, tmp1206
	movq	%r12, %rcx	# rng__d_lsm.266, tmp1210
# find-perfect-hash.c:38: 	x->d = e + x->a;
	movq	%r12, 24(%r14)	# rng__d_lsm.266, rng_44(D)->d
# find-perfect-hash.c:69: 	return low + r % (high - low + 1);
	shrq	$2, %rdx	#, tmp1206
	mulq	%rdx	# tmp1206
	movq	%rdx, %rax	# tmp1207, tmp1207
	shrq	$4, %rax	#, tmp1207
	imulq	$500, %rax, %rax	#, tmp1205, tmp1209
	subq	%rax, %rcx	# tmp1209, tmp1210
# find-perfect-hash.c:733: 		b = &columns[rand_range(rng, 0, n_breeders - 1)];
	salq	$4, %rcx	#, tmp1211
	addq	%r11, %rcx	# columns, b
# find-perfect-hash.c:697: 	if (a == b) {
	cmpq	%rcx, %r15	# b, a
	je	.L259	#,
# find-perfect-hash.c:701: 	if (c != 0) {
	movl	12(%rcx), %eax	# b_657->collisions, tmp1791
	cmpl	%eax, 12(%r15)	# tmp1791, a_645->collisions
	jne	.L223	#,
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	movl	8(%r15), %edx	# a_645->n,
	movl	8(%rcx), %esi	# b_657->n, _703
# find-perfect-hash.c:707: 	return a->n - b->n;
	movl	%edx, %eax	# _702, _664
	subl	%esi, %eax	# _703, _664
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	cmpl	%esi, %edx	# _703, _702
	jne	.L225	#,
# find-perfect-hash.c:705: 		return memcmp(a->bits, b->bits, a->n);
	movq	(%rcx), %rsi	# b_657->bits,
	movq	(%r15), %rdi	# a_645->bits,
	movq	%r8, -232(%rbp)	# rng__a_lsm.260, %sfp
	movq	%r10, -224(%rbp)	# rng__b_lsm.262, %sfp
	movq	%rcx, -216(%rbp)	# b, %sfp
	call	memcmp	#
	movq	-232(%rbp), %r8	# %sfp, rng__a_lsm.260
	movq	-224(%rbp), %r10	# %sfp, rng__b_lsm.262
	movq	-216(%rbp), %rcx	# %sfp, b
.L225:
# find-perfect-hash.c:734: 		if (cmp_columns(a, b) == 0) {
	testl	%eax, %eax	# _664
	jne	.L223	#,
# find-perfect-hash.c:736: 				ctx->stats.inbreds++;
	addl	$1, -76(%rbp)	#, ctx.stats.inbreds
# find-perfect-hash.c:731: 		c = &columns[i];
	movq	%rbx, %r15	# ivtmp.325, a
.L223:
# find-perfect-hash.c:742: 		for (j = 0; j < c->n; j++) {
	movl	8(%rbx), %r9d	# MEM[base: c_629, offset: 8B], _375
	movq	(%rbx), %r11	# MEM[base: c_629, offset: 0B], pretmp_2799
	testl	%r9d, %r9d	# _375
	je	.L260	#,
	movl	$1, %eax	#, ivtmp.314
.L227:
# find-perfect-hash.c:743: 			c->bits[j] = a->bits[j];
	movq	(%r15), %rdx	# a_376->bits, a_376->bits
	movq	%rcx, %rsi	# b, tmp1223
	movl	-4(%rdx,%rax,4), %edx	# *_672, _675
	movl	%edx, -4(%r11,%rax,4)	# _675, MEM[base: pretmp_2799, index: ivtmp.314_2840, step: 4, offset: -4B]
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	movq	%r10, %rdx	# rng__b_lsm.262, tmp1220
# find-perfect-hash.c:742: 		for (j = 0; j < c->n; j++) {
	movl	8(%rbx), %r9d	# MEM[base: c_629, offset: 8B], _375
# find-perfect-hash.c:34: 	uint64_t e = x->a - ROTATE(x->b, 7);
	rolq	$7, %rdx	#, tmp1220
	subq	%rdx, %r8	# tmp1220, rng__a_lsm.260
	movq	%r8, %rdx	# rng__a_lsm.260, e
# find-perfect-hash.c:35: 	x->a = x->b ^ ROTATE(x->c, 13);
	movq	%r13, %r8	# rng__c_lsm.264, tmp1221
	rolq	$13, %r8	#, tmp1221
	xorq	%r10, %r8	# rng__b_lsm.262, rng__a_lsm.260
# find-perfect-hash.c:36: 	x->b = x->c + ROTATE(x->d, 37);
	movq	%r12, %r10	# rng__d_lsm.266, tmp1222
	rorq	$27, %r10	#, tmp1222
	addq	%r13, %r10	# rng__c_lsm.264, rng__b_lsm.262
# find-perfect-hash.c:37: 	x->c = x->d + e;
	leaq	(%rdx,%r12), %r13	#, rng__c_lsm.264
# find-perfect-hash.c:38: 	x->d = e + x->a;
	leaq	(%rdx,%r8), %r12	#, rng__d_lsm.266
# find-perfect-hash.c:744: 			if ((rand64(rng) & 31) < 6) {
	movq	%r12, %rdx	# rng__d_lsm.266, _688
	andl	$31, %edx	#, _688
	cmpq	$5, %rdx	#, _688
# find-perfect-hash.c:742: 		for (j = 0; j < c->n; j++) {
	movl	%eax, %edx	# ivtmp.314, j.51_691
	cmova	%r15, %rsi	# a,, tmp1223
	cmovbe	%r15, %rcx	# a,, b
	addq	$1, %rax	#, ivtmp.314
	cmpl	%edx, %r9d	# j.51_691, _375
	movq	%rsi, %r15	# tmp1223, a
	ja	.L227	#,
	movl	$1, %eax	#, tmp1226
	movl	%r9d, %ecx	# _375, tmp1799
	movq	%r8, (%r14)	# rng__a_lsm.260, rng_44(D)->a
	sall	%cl, %eax	# tmp1799, tmp1225
	movq	%r10, 8(%r14)	# rng__b_lsm.262, rng_44(D)->b
	movq	%r13, 16(%r14)	# rng__c_lsm.264, rng_44(D)->c
	leal	7(%rax), %edx	#, tmp1229
	testl	%eax, %eax	# tmp1225
	movq	%r12, 24(%r14)	# rng__d_lsm.266, rng_44(D)->d
	movq	-96(%rbp), %rdi	# ctx.hits, pretmp_2823
	cmovns	%eax, %edx	# tmp1229,, tmp1225, tmp1228
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	movl	-104(%rbp), %eax	# ctx.n, _617
	sarl	$3, %edx	#, tmp1230
	movslq	%edx, %rdx	# tmp1230, prephitmp_2819
	testl	%eax, %eax	# _617
	je	.L261	#,
.L337:
	subl	$1, %eax	#, tmp1232
# find-perfect-hash.c:492: 		uint64_t *fullhash = ctx->data[j].hash;
	movq	-112(%rbp), %rcx	# ctx.data, _711
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%r12d, %r12d	# collisions
	imulq	$408, %rax, %rax	#, tmp1232, tmp1233
	leaq	16(%rcx), %r10	#, ivtmp.309
	leaq	424(%rcx,%rax), %r15	#, _2836
	leal	-1(%r9), %eax	#, tmp1236
	leaq	4(%r11,%rax,4), %r13	#, _2829
.L231:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	testl	%r9d, %r9d	# _375
	je	.L262	#,
	movq	%r11, %r8	# pretmp_2799, ivtmp.303
# find-perfect-hash.c:491: 		uint32_t hash = 0;
	xorl	%eax, %eax	# hash
.L230:
# find-perfect-hash.c:494: 			uint32_t b = columns->bits[i];
	movl	(%r8), %ecx	# MEM[base: _2811, offset: 0B], b
# find-perfect-hash.c:495: 			hash <<= 1;
	leal	(%rax,%rax), %esi	#, hash
	addq	$4, %r8	#, ivtmp.303
# find-perfect-hash.c:496: 			hash |= READ_HASH_BIT(fullhash, b);
	movl	%ecx, %eax	# b, tmp1239
	shrl	$6, %eax	#,
	movq	(%r10,%rax,8), %rax	# *_728, *_728
	shrq	%cl, %rax	# b, tmp1242
	andl	$1, %eax	#, tmp1244
	orl	%esi, %eax	# hash, hash
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	cmpq	%r13, %r8	# _2829, ivtmp.303
	jne	.L230	#,
	movl	%eax, %ecx	# hash, tmp1802
	movl	$1, %esi	#, _2875
	shrl	$6, %eax	#,
	salq	%cl, %rsi	# tmp1802, _2875
	movq	%rsi, %rcx	# _2875, _2875
	leaq	(%rdi,%rax,8), %rsi	#, _2881
.L229:
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	movq	(%rsi), %rax	# *prephitmp_2882, _746
	xorl	%r8d, %r8d	# tmp1251
	testq	%rcx, %rax	# _2875, _746
	setne	%r8b	#, tmp1251
	addq	$408, %r10	#, ivtmp.309
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	orq	%rcx, %rax	# _2875, tmp1253
# find-perfect-hash.c:503: 		collisions += (ctx->hits[f] & g) ? 1 : 0;
	addl	%r8d, %r12d	# tmp1251, collisions
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	cmpq	%r15, %r10	# _2836, ivtmp.309
# find-perfect-hash.c:504: 		ctx->hits[f] |= g;
	movq	%rax, (%rsi)	# tmp1253, *prephitmp_2882
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	jne	.L231	#,
.L228:
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	xorl	%esi, %esi	#
	addq	$16, %rbx	#, ivtmp.325
	call	memset	#
# find-perfect-hash.c:750: 		c->collisions = test_one_subset(ctx, c);
	movl	%r12d, -4(%rbx)	# collisions, MEM[base: c_629, offset: 12B]
# find-perfect-hash.c:730: 	for (i = n_breeders; i < N_CANDIDATES; i++) {
	cmpq	%rbx, -320(%rbp)	# ivtmp.325, %sfp
	jne	.L232	#,
# find-perfect-hash.c:752: 	printf("killed %u; bred from %u\n", N_CANDIDATES - n_breeders,
	movl	$500, %edx	#,
	movl	$500, %esi	#,
	movl	$.LC58, %edi	#,
	xorl	%eax, %eax	#
	movq	%rbx, %r15	# ivtmp.325, ivtmp.325
	xorl	%r13d, %r13d	# count
	call	printf	#
	movq	-280(%rbp), %r12	# %sfp, columns
	movq	%r12, %rbx	# columns, ivtmp.298
.L241:
# find-perfect-hash.c:697: 	if (a == b) {
	cmpq	%r12, %rbx	# c, ivtmp.298
	je	.L233	#,
# find-perfect-hash.c:700: 	int c = (int)a->collisions - (int)b->collisions;
	movl	12(%r12), %eax	# c_1204->collisions, c_1204->collisions
# find-perfect-hash.c:701: 	if (c != 0) {
	subl	12(%rbx), %eax	# MEM[base: d_606, offset: 12B], c
	jne	.L234	#,
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	movl	8(%r12), %edx	# c_1204->n,
	movl	8(%rbx), %ecx	# MEM[base: d_606, offset: 8B], _762
# find-perfect-hash.c:707: 	return a->n - b->n;
	movl	%edx, %eax	# _761, c
	subl	%ecx, %eax	# _762, c
# find-perfect-hash.c:704: 	if (a->n == b->n) {
	cmpl	%ecx, %edx	# _762, _761
	jne	.L236	#,
# find-perfect-hash.c:705: 		return memcmp(a->bits, b->bits, a->n);
	movq	(%rbx), %rsi	# MEM[base: d_606, offset: 0B],
	movq	(%r12), %rdi	# c_1204->bits,
	call	memcmp	#
.L236:
# find-perfect-hash.c:772: 		if (cmp == 0) {
	testl	%eax, %eax	# c
	je	.L233	#,
.L234:
# find-perfect-hash.c:776: 		if (cmp > 0) {
	testl	%eax, %eax	# c
	jg	.L335	#,
# find-perfect-hash.c:780: 		printf("%3d× ", count);
	movl	%r13d, %esi	# count,
	movl	$.LC49, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:781: 		describe_columns(ctx, c);
	leaq	-88(%rbp), %rdi	#, tmp1805
	movq	%r12, %rsi	# c,
	call	describe_columns.isra.5	#
# find-perfect-hash.c:782: 		if (c->collisions < d->collisions) {
	movl	12(%rbx), %eax	# MEM[base: d_606, offset: 12B], tmp1806
	cmpl	%eax, 12(%r12)	# tmp1806, c_1204->collisions
	jb	.L239	#,
	movq	%rbx, %r12	# ivtmp.298, c
# find-perfect-hash.c:785: 		count = 0;
	xorl	%r13d, %r13d	# count
.L237:
	addq	$16, %rbx	#, ivtmp.298
# find-perfect-hash.c:769: 	for (i = 0; i < N_CANDIDATES; i++) {
	cmpq	%r15, %rbx	# ivtmp.325, ivtmp.298
	jne	.L241	#,
.L239:
# find-perfect-hash.c:832: 		sprintf(s, "columns-%d.pbm", i);
	movl	-288(%rbp), %edx	# %sfp,
	leaq	-176(%rbp), %rdi	#, tmp1808
	movl	$.LC59, %esi	#,
	xorl	%eax, %eax	#
	call	sprintf	#
# find-perfect-hash.c:140: 	FILE *fh = fopen(name, "w");
	leaq	-176(%rbp), %rdi	#, tmp1809
	movl	$.LC21, %esi	#,
# find-perfect-hash.c:833: 		pbm_dump(columns, sizeof(columns[0]) * 8, ctx->n,
	movl	-104(%rbp), %r12d	# ctx.n, _306
# find-perfect-hash.c:140: 	FILE *fh = fopen(name, "w");
	call	fopen	#
# find-perfect-hash.c:141: 	if (fh == NULL){
	testq	%rax, %rax	# fh
# find-perfect-hash.c:140: 	FILE *fh = fopen(name, "w");
	movq	%rax, %rbx	#, fh
# find-perfect-hash.c:141: 	if (fh == NULL){
	je	.L336	#,
# find-perfect-hash.c:146: 	fprintf(fh, "P4\n%u %u\n", width, height);
	movq	%rax, %rdi	# fh,
	movl	%r12d, %ecx	# _306,
	xorl	%eax, %eax	#
	movl	$128, %edx	#,
	movl	$.LC33, %esi	#,
	call	fprintf	#
# find-perfect-hash.c:149: 	for (uint y = 0; y < height; y++){
	testl	%r12d, %r12d	# _306
	je	.L244	#,
	leal	-1(%r12), %r15d	#, tmp1272
	movq	-280(%rbp), %r12	# %sfp, columns
	addq	$1, %r15	#, tmp1273
	salq	$4, %r15	#, tmp1274
	addq	%r12, %r15	# columns, _2790
.L246:
	movq	%r12, %r13	# row, ivtmp.285
	addq	$16, %r12	#, row
.L245:
# find-perfect-hash.c:153: 			byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	movzbl	0(%r13), %ecx	# MEM[base: _2783, offset: 0B], MEM[base: _2783, offset: 0B]
	movabsq	$8623620610, %rax	#, tmp1812
# find-perfect-hash.c:154: 			putc(~byte, fh);
	movq	%rbx, %rsi	# fh,
	addq	$1, %r13	#, ivtmp.285
# find-perfect-hash.c:153: 			byte = (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
	imulq	%rax, %rcx	# tmp1812, tmp1276
	movabsq	$1136090292240, %rax	#, tmp1813
	andq	%rax, %rcx	# tmp1813, tmp1278
	movabsq	$18032007892189201, %rax	#, tmp1591
	mulq	%rcx	# tmp1278
	movq	%rcx, %rax	# tmp1278, tmp1282
	subq	%rdx, %rax	# tmp1280, tmp1282
	shrq	%rax	# tmp1283
	addq	%rdx, %rax	# tmp1280, tmp1284
	shrq	$9, %rax	#, _782
	movq	%rax, %rdx	# _782, tmp1286
	salq	$10, %rdx	#, tmp1286
	subq	%rax, %rdx	# _782, tmp1286
	subq	%rdx, %rcx	# tmp1287, tmp1278
# find-perfect-hash.c:154: 			putc(~byte, fh);
	movzbl	%cl, %eax	# _782, _782
	notl	%eax	# _782
	movl	%eax, %edi	# _782, tmp1289
	call	_IO_putc	#
# find-perfect-hash.c:150: 		for (uint x = 0; x < width / 8; x++){
	cmpq	%r12, %r13	# row, ivtmp.285
	jne	.L245	#,
# find-perfect-hash.c:149: 	for (uint y = 0; y < height; y++){
	cmpq	%r15, %r12	# _2790, row
	jne	.L246	#,
.L244:
# find-perfect-hash.c:158: 	fflush(fh);
	movq	%rbx, %rdi	# fh,
	call	fflush	#
# find-perfect-hash.c:159: 	fclose(fh);
	movq	%rbx, %rdi	# fh,
	call	fclose	#
.L243:
# find-perfect-hash.c:837: 		temp = temp * 4 / 5;
	movl	-244(%rbp), %edx	# %sfp, temp
# find-perfect-hash.c:803: 	for (i = 0; i < 1000; i++) {
	addl	$1, -288(%rbp)	#, %sfp
# find-perfect-hash.c:837: 		temp = temp * 4 / 5;
	movl	$-858993459, %ecx	#, tmp1291
	sall	$2, %edx	#, temp
	movl	%edx, %eax	# _308, _308
	mull	%ecx	# tmp1291
# find-perfect-hash.c:803: 	for (i = 0; i < 1000; i++) {
	movl	-288(%rbp), %eax	# %sfp, i
# find-perfect-hash.c:837: 		temp = temp * 4 / 5;
	shrl	$2, %edx	#, tmp1290
# find-perfect-hash.c:803: 	for (i = 0; i < 1000; i++) {
	cmpl	$1000, %eax	#, i
# find-perfect-hash.c:837: 		temp = temp * 4 / 5;
	movl	%edx, -244(%rbp)	# tmp1290, %sfp
# find-perfect-hash.c:803: 	for (i = 0; i < 1000; i++) {
	jne	.L247	#,
# find-perfect-hash.c:868: }
	leaq	-40(%rbp), %rsp	#,
	xorl	%eax, %eax	#
	popq	%rbx	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L212:
	.cfi_restore_state
	movq	%rbx, %rdx	# _2712,
# find-perfect-hash.c:511: 	memset(ctx->hits, 0, (1 << columns->n) / 8);
	xorl	%esi, %esi	#
	call	memset	#
# find-perfect-hash.c:677: 		c->collisions = test_one_subset(ctx, c);
	movl	$0, 12(%r15)	#, MEM[base: _2729, offset: 12B]
	jmp	.L216	#
.L333:
# find-perfect-hash.c:680: 				printf("worsening %d %u > %u\n",
	movl	-248(%rbp), %ecx	# %sfp,
	movl	-304(%rbp), %esi	# %sfp,
	movl	%ebx, %edx	# collisions,
	movl	$.LC50, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
	jmp	.L217	#
.L329:
# find-perfect-hash.c:777: 			printf("list seems unsorted!\n");
	movl	$.LC48, %edi	#,
	call	puts	#
	jmp	.L190	#
	.p2align 4,,10
	.p2align 3
.L262:
# find-perfect-hash.c:493: 		for (i = 0; i < columns->n; i++) {
	movq	%rdi, %rsi	# pretmp_2823, _2881
	movl	$1, %ecx	#, _2875
	jmp	.L229	#
.L260:
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	movl	-104(%rbp), %eax	# ctx.n, _617
# find-perfect-hash.c:742: 		for (j = 0; j < c->n; j++) {
	xorl	%edx, %edx	# prephitmp_2819
	movq	-96(%rbp), %rdi	# ctx.hits, pretmp_2823
# find-perfect-hash.c:490: 	for (j = 0; j < ctx->n; j++) {
	testl	%eax, %eax	# _617
	jne	.L337	#,
.L261:
# find-perfect-hash.c:486: 	uint32_t collisions = 0;
	xorl	%r12d, %r12d	# collisions
	jmp	.L228	#
.L259:
# find-perfect-hash.c:731: 		c = &columns[i];
	movq	%rbx, %r15	# ivtmp.325, a
	jmp	.L223	#
.L233:
# find-perfect-hash.c:773: 			count++;
	addl	$1, %r13d	#, count
	jmp	.L237	#
.L335:
# find-perfect-hash.c:777: 			printf("list seems unsorted!\n");
	movl	$.LC48, %edi	#,
	call	puts	#
	jmp	.L239	#
.L336:
# find-perfect-hash.c:142: 		fprintf(stderr, "could not open '%s' for writing\n", name);
	movq	stderr(%rip), %rdi	# stderr,
	leaq	-176(%rbp), %rdx	#, tmp1810
	movl	$.LC32, %esi	#,
	xorl	%eax, %eax	#
	call	fprintf	#
	jmp	.L243	#
	.cfi_endproc
.LFE53:
	.size	find_hash, .-find_hash
	.section	.rodata.str1.8
	.align 8
.LC60:
	.string	"usage: %s <string list> <hash bits>\n\n"
	.align 8
.LC61:
	.string	"string list is one string per line."
	.section	.rodata.str1.1
.LC62:
	.string	"got %s %s %s\n"
	.section	.rodata.str1.8
	.align 8
.LC63:
	.string	"A %u bit hash is too big for me! try 28 or less.\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB54:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx	# argv, argv
	subq	$40, %rsp	#,
	.cfi_def_cfa_offset 64
# find-perfect-hash.c:873: 	if (argc < 3) {
	cmpl	$2, %edi	#, argc
	movq	(%rsi), %rsi	# *argv_13(D), prephitmp_24
	jle	.L343	#,
.L339:
# find-perfect-hash.c:878: 	printf("got %s %s %s\n",
	movq	16(%rbx), %rcx	# MEM[(char * *)argv_13(D) + 16B], MEM[(char * *)argv_13(D) + 16B]
	movq	8(%rbx), %rdx	# MEM[(char * *)argv_13(D) + 8B], MEM[(char * *)argv_13(D) + 8B]
	movl	$.LC62, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:881: 	uint32_t bits = strtoul(argv[2], NULL, 10);
	movq	16(%rbx), %rdi	# MEM[(char * *)argv_13(D) + 16B], MEM[(char * *)argv_13(D) + 16B]
	xorl	%esi, %esi	#
	movl	$10, %edx	#,
	call	strtoul	#
# find-perfect-hash.c:882: 	if (bits > 28) {
	cmpl	$28, %eax	#, _6
# find-perfect-hash.c:881: 	uint32_t bits = strtoul(argv[2], NULL, 10);
	movq	%rax, %rbp	#, _6
# find-perfect-hash.c:882: 	if (bits > 28) {
	ja	.L344	#,
# find-perfect-hash.c:891: 	rng_random_init(&rng);
	movq	%rsp, %rdi	#,
	call	rng_random_init	#
# find-perfect-hash.c:893:        	return find_hash(argv[1], bits, &rng);
	movq	8(%rbx), %rdi	# MEM[(char * *)argv_13(D) + 8B], MEM[(char * *)argv_13(D) + 8B]
	movq	%rsp, %rdx	#,
	movl	%ebp, %esi	# _6,
	call	find_hash	#
.L338:
# find-perfect-hash.c:894: }
	addq	$40, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret
.L343:
	.cfi_restore_state
# find-perfect-hash.c:874: 		printf("usage: %s <string list> <hash bits>\n\n",
	movl	$.LC60, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:876: 		printf("string list is one string per line.\n");
	movl	$.LC61, %edi	#,
	call	puts	#
	movq	(%rbx), %rsi	# *argv_13(D), prephitmp_24
	jmp	.L339	#
.L344:
# find-perfect-hash.c:883: 		printf("A %u bit hash is too big for me! try 28 or less.\n",
	movl	%eax, %esi	# _6,
	movl	$.LC63, %edi	#,
	xorl	%eax, %eax	#
	call	printf	#
# find-perfect-hash.c:885: 		return 2;
	movl	$2, %eax	#, <retval>
	jmp	.L338	#
	.cfi_endproc
.LFE54:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.align 16
.LC1:
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.long	-2147483648
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC3:
	.long	0
	.long	1071644672
	.align 8
.LC19:
	.long	966391728
	.long	-33350403
	.align 8
.LC20:
	.long	966391728
	.long	2114133245
	.align 8
.LC24:
	.long	343597384
	.long	1081081774
	.align 8
.LC26:
	.long	0
	.long	1081073664
	.align 8
.LC34:
	.long	0
	.long	1072693248
	.align 8
.LC42:
	.long	2726797102
	.long	1417495214
	.align 8
.LC43:
	.long	2726797102
	.long	-729988434
	.align 8
.LC46:
	.long	0
	.long	1073741824
	.ident	"GCC: (GNU) 7.0.0 20161108 (experimental)"
	.section	.note.GNU-stack,"",@progbits
