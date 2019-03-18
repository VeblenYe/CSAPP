#pragma once


/* ˵���������µ���ҵ�𰸱Ƚ��鷳������������������⣬����Ȥ�Ŀ�ȥ������ַ�鿴��
https://dreamanddead.gitbooks.io/csapp-3e-solutions/content/ */


/* 4.45 */
/*
A��û����ȷ��������Ӧѹ��ԭ%rspֵ���ټ�8������˳�����
B��
subq $8, %rsp
movq 8(%rsp), REG
*/


/* 4.46 */
/*
A��û����ȷ������Ӧ�ý�������ԭ��%rsp��ֵ����%rsp+8����
B��
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
��Manjaro�ϱ���Ļ�����
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

�޸�ΪY86-64��
�������꣬˵ʵ��û����˼���Ժ���ʱ���������ɣ������ϱ��˵Ĵ���
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
A��ԭ������/ʹ��ð�յ����� E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB }
�����˼���ת�������/ʹ��ð�յ�����
E_icode in { IMRMOVQ, IPOPQ } &&
( E_dstM == d_srcB || (E_dstM == d_srcA && !(D_icode in { IRMMOVQ, IPUSHQ })));
������
���ȼ���ת����ʹ�������Ƕ�����������ݵĲ�����ǰһ��ָ����Ҫ��д�ؽ׶Σ���һ��ָ����Ҫ�ڷô�׶Ρ�
���ŷ�����ʱ����/ʹ��ð��
Ҫ����/ʹ��ð��ǰһ��ָ���ΪIMRMOVQ��IPOPQ����E_dstM����ΪRNONE��
E_dstM��d_srcA��d_srcB��ƥ��һ�������������11��10��01��00��ֻ��ǰ���ֿ���Ҫ����/ʹ��ð��
��E_dstM==d_srcBʱ����������ִ�н׶���ɣ�����ʹ�ü���ת���������/ʹ��ð�գ�
����E_dstM==d_srcAʱ����������д�ؽ׶���ɣ�����ʹ�ü���ת��
���ڿ��Ǻ�һ��ָ��Ҳ����D_icode������Ҫ��������Ϊ��d_srcA��ʹ��Ҫ�ڷô�׶Σ�
�����֪IRMMOVQ��IPUSHQ����ʹ�ü���ת��
���տɵ��µ��߼��ж�ʽ
*/


/* 4.59 */
/*
4.47 CPL=0.5*5+0.5*(7+2)=7
4.48 CPL=9
4.49 CPL=11
4.47�������
*/
