#pragma once


/* 说明：第四章的作业答案比较麻烦，这里就随意做了两题，有兴趣的可去下面网址查看答案
https://dreamanddead.gitbooks.io/csapp-3e-solutions/content/ */


/* 4.45 */
/*
A：没有正确描述，本应压入原%rsp值后再减8，这里顺序错误
B：
subq $8, %rsp
movq 8(%rsp), REG
*/


/* 4.46 */
/*
A：没有正确描述，应该将弹出的原（%rsp）值放入%rsp+8处；
B：
addq $8, %rsp
movq -8(%rsp), REG
*/


/* 4.47 */
void bubble_a(long *data, long count) {
	long *i, *last;
	for (last = data + count - 1; last > data; --last) {
		for (i = data; i < last; ++i) {
			if (*(i + 1) < *i) {
				long t = *(i + 1);
				*(i + 1) = *i;
				*i = t;
			}
		}
	}
}
/*
在Manjaro上编译的汇编代码
	.file	"bubble_sort.c"
	.text
	.globl	bubble_sort
	.type	bubble_sort, @function
bubble_sort:
.LFB0:
	.cfi_startproc
	leaq	-8(%rdi,%rsi,8), %rsi
	jmp	.L2
.L3:
	addq	$8, %rax
.L5:
	cmpq	%rsi, %rax
	jnb	.L7
	movq	8(%rax), %rdx
	movq	(%rax), %rcx
	cmpq	%rcx, %rdx
	jge	.L3
	movq	%rcx, 8(%rax)
	movq	%rdx, (%rax)
	jmp	.L3
.L7:
	subq	$8, %rsi
.L2:
	cmpq	%rdi, %rsi
	jbe	.L8
	movq	%rdi, %rax
	jmp	.L5
.L8:
	ret
	.cfi_endproc
.LFE0:
	.size	bubble_sort, .-bubble_sort
	.ident	"GCC: (GNU) 8.2.1 20181127"
	.section	.note.GNU-stack,"",@progbits

修改为Y86-64版
啊，烦躁，说实话没得意思，以后有时间再来补吧，先贴上别人的代码
ysBubbleP:
  jmp L2
L4:
  mrmovq 8(%rax), %r9
  mrmovq (%rax), %r10
  rrmovq %r9, %r8
  subq %r10, %r8
  jge L3
  rmmovq %r10, 8(%rax)
  rmmovq %r9, (%rax)
L3:
  irmovq $8, %r8
  addq %r8, %rax
  jmp L5
L6:
  rrmovq %rdi, %rax
L5:
  rrmovq %rsi, %r8
  subq %rax, %r8
  jg L4
  irmovq $8, %r8
  subq %r8, %rsi
L2:
  rrmovq %rsi, %r8
  subq %rdi, %r8
  jg L6
	ret
*/


/* 4.48 */
/*
ysBubbleP:
  jmp L2
L4:
  mrmovq 8(%rax), %r9
  mrmovq (%rax), %r10
  rrmovq %r9, %r8
  subq %r10, %r8
##############################################################
# begin differences
##############################################################
  cmovl %r9, %r11
  cmovl %r10, %r9
  cmovl %r11, %r10
  rmmovq %r9, 8(%rax)
  rmmovq %r10, (%rax)
##############################################################
# end
##############################################################
  irmovq $8, %r8
  addq %r8, %rax
  jmp L5
L6:
  rrmovq %rdi, %rax
L5:
  rrmovq %rsi, %r8
  subq %rax, %r8
  jg L4
  irmovq $8, %r8
  subq %r8, %rsi
L2:
  rrmovq %rsi, %r8
  subq %rdi, %r8
  jg L6
	ret
*/


/* 4.49 */
/*
ysBubbleP:
  jmp L2
L4:
  mrmovq 8(%rax), %r9
  mrmovq (%rax), %r10
############################################
# begin differences
############################################
  rrmovq %r9, %r8
  rrmovq %r10, %r11
  xorq %r9, %r10
  subq %r11, %r8
  cmovge %r11, %r9
  xorq %r10, %r9
  xorq %r9, %r10
  rmmovq %r9, 8(%rax)
  rmmovq %r10, (%rax)
############################################
# end
############################################
  irmovq $8, %r8
  addq %r8, %rax
  jmp L5
L6:
  rrmovq %rdi, %rax
L5:
  rrmovq %rsi, %r8
  subq %rax, %r8
  jg L4
  irmovq $8, %r8
  subq %r8, %rsi
L2:
  rrmovq %rsi, %r8
  subq %rdi, %r8
  jg L6
	ret
*/


/* 4.51 */
/*
iaddq V, rB
F	icode:ifun <-- M1[PC]
	rA:rB <-- M1[PC+1]
	valC <-- M8[PC+2]
	valP <-- PC+10
D	valB <-- R[rB]
E	valE <-- valC+valB
M
W	R[rB] = valE
U	PC <-- valP
*/


/* 4.57 */
/*
A：原来加载/使用冒险的条件 E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }
加入了加载转发后加载/使用冒险的条件
E_icode in { IMRMOVQ, IPOPQ } &&
( E_dstM == d_srcB || (E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })));
分析：
首先加载转发的使用条件是对于相关联数据的操作，前一条指令需要在写回阶段，后一条指令需要在访存阶段。
接着分析何时加载/使用冒险
要加载/使用冒险前一条指令必为IMRMOVQ或IPOPQ，则E_dstM不能为RNONE，
E_dstM与d_srcA、d_srcB的匹配一共四种情况——11，10，01，00，只有前三种可能要加载/使用冒险
当E_dstM==d_srcB时，操作均在执行阶段完成，不能使用加载转发避免加载/使用冒险，
而当E_dstM==d_srcA时，操作均在写回阶段完成，可以使用加载转发
现在考虑后一条指令也就是D_icode，它需要满足条件为对d_srcA的使用要在访存阶段，
查书可知IRMMOVQ和IPUSHQ可以使用加载转发
最终可得新的逻辑判断式
*/


/* 4.59 */
/*
4.47 CPL=0.5*5+0.5*(7+2)=7
4.48 CPL=9
4.49 CPL=11
4.47性能最高
*/
