#pragma once

#include <cstdio>


/* 3.58 ��������������д���ȼ۵�C���� */
long decode2(long x, long y, long z) {
	y -= z;
	x *= y;
	long _y = y;
	_y <<= 63;
	_y >>= 63;
	return x ^ _y;
}


/* 3.59 ��������128λ�˷�������㷨���Ի��������ע�� */
/* ������X = 2^64 * Xh + Xl��Y = 2^64 * Yh + Yl
��X * Y = 2^128(XhYh) + 2^64(XhYl + XlYh) + XlYl����Z1 = XhYl��Z2 = XlYh��Z3 = XlYl����֪Z��Ϊ128λ
��X * Y = 2^128(XhYh + Z1h + Z2h) + 2^64(Z1l + Z2l + Z3h) + Z3l.
�������
���ȵ�2~5�������뽫x��y��չ��128λ��Ȼ��6~8�������Z1l+Z2l�����������%rcx�У�
����9������Z3������Z3l����%rax�У�Z3h����%rdx�У�֮��10������Z1l+Z2l+Z3h��������%rdx�У�
ʣ��ָ���򽫽������*dest�� */


/* 3.60 ���ݸ����Ļ������ƶ�C���� */
long loop(long x, int n) {
	long result = 0;
	long mask;
	for (mask = 1; mask != 0; mask = mask << n) {
		result |= x & mask;
	}
	return result;
}


/* 3.61 ��д����cread_alt */
long cread_alt(long *xp) {
	// ˼·�������ж�*xp����Ϊ�ж�xp��Ȼ�����ɵĻ����벢û��ʹ����������ָ��n 
	long t = 0;
	long *p = xp ? xp : &t;
	return *p;

	// ��һ����ʽ
	// return (!xp ? 0 : *xp);
}


/* 3.62 ���ݻ����벹ȫC���룬������ */
using mode_t = enum {MODE_A, MODE_B, MODE_C, MODE_D, MODE_E};

long switch3(long *p1, long *p2, mode_t action) {
	long result = 0;
	switch (action) {
	case MODE_A:
		result = *p2;
		*p2 = *p1;
		break;
	case MODE_B:
		result = *p1 + *p2;
		*p1 = result;
		break;
	case MODE_C:
		*p1 = 59;
		result = *p2;
		break;
	case MODE_D:
		result = *p2;
		*p1 = result;
		result = 27;
		break;
	case MODE_E:
		result = 27;
		break;
	default:
		result = 12;
	}
	return result;
}


/* 3.63 ���򹤳��Ƴ�switch���� */
long switch_prob(long x, long n) {
	long result = x;
	switch (n) {
	case 60:
		result = 8 * x;
		break;
	case 63:
		result = x >> 3;
		break;
	case 64:
		x = (x << 4) - x;
		result = x;
	case 65:
		x *= x;
	default:
		result = 75;
	}
	return result;
}


/* 3.64 ���򹤳��Ƴ�R��S��T��ֵ */
/* A. &A[i][j][k] = A + L(S*T*i + T*j + k) 
B. R = 7, T = 13, S = 5 */


/* 3.65 ���򹤳��Ƴ��Ĵ������� */
/* A. %rdx
B. %rax
C. M = 15 */


/* 3.66 ���򹤳��ƶϺ궨�� */
/* NR(n) = 3 * n
NC(n) = 4 * n + 1 */


/* 3.67 ���򹤳��ƶϽṹ�������ݵ�ʵ�� */
/* A. ����ջ֡%rsp~%rsp+104, (%rsp) x, (%rsp+8)y, (%rsp+16)&z, (%rsp+24)z, (%rsp+64)y, 
(%rsp+72)x, (%rsp+80)z 
B. ������ջ֡��ַ%rsp+64 
C.D. ���ǿ�%rsp����ƫ��ֵ���ʺ����� 
F. ������ͨ�����������ջ֡�еĿ������뱻�����ߣ���������ͨ��ջָ���ƫ��ֵ���� */


/* 3.68���ݻ������ƶϳ���ֵ */
/* 4 < B <= 8��6 < A <= 10��44 < AB <= 46
A = 9��B = 5 */


/* 3.69 ���ݻ�����õ�����ĳ����ͽṹ */
/* A. CNT = 7 
B. a_struct {long idx; long x[4]} */


/* 3.70 ���ݻ������ƶ� */
/* A. e1.p 0   e1.y 8   e2.x 0   e2.next 8
B. 16
C. up->e2.x = *(up->e2.next->e1.p) - up->e2.next->e1.y */


/* 3.71 ʵ�ֺ���good_echo */
constexpr int BUF_SIZE = 12;
void good_echo(void) {
	char buf[BUF_SIZE];
	while (true) {
		// ʵ�������ǲ���Ҫ��ʲô��fgets��֤��ÿ��ֻ����BUF_SIZE-1���ַ������ᷢ�����
		char *p = fgets(buf, BUF_SIZE, stdin);
		if (!p) break;
		printf("%s", p);
	}
}


/* 3.72 �����䳤ջ֡�ĵ�ʵ�ֺ�ʹ����� */
/* A. S2 = S1 - (8n + 30) & -16
B. p = (S2 + 15) & -16��ǰ�������֪&-16��Ϊ�˵�ַ16�ֽڶ���
C. ����e1���ܵ���0����Ϊe1 = 0��p��Ӧ�õ���S2��������ʽ��֪������
���ŷ����������nΪż������S2 = S1 - (8n + 16)����e1 + e2 = 16��
��e1 = 1��e2 = 15ʱ��e1��С����ʱS1Ӧ����S1 % 16 = 1��
��nΪ��������S2 = S1 - (8n + 24)����e1 + e2 = 24��
��e1 = 24��e2 = 0ʱ��e1��󣬴�ʱS1 % 16 = 0��
D. ����16λ���� */


/* 3.73 д��find_range�Ļ�����1 */
/* �޷����룬�Ժ���˵
using range_t = enum { NEG, ZERO, POS, OTHER };

range_t find_range(float x) {
	__asm {
		"xorps %xmm1, %xmm1, %xmm1\n\t"
		"ucomiss %xmm1, %xmm0\n\t"
		"jp .P\n\t"
		"jg .G\n\t"
		"jl .L\n\t"
		"je .E\n\t"
		".E:\n\t"
		"movl $1, %eax\n\t"
		"jmp .Done\n\t"
		".G:\n\t"
		"movl $2, %eax\n\t"
		"jmp .Done\n\t"
		".L:\n\t"
		"movl $0, %eax\n\t"
		"jmp .Done\n\t"
		".P:\n\t"
		"movl $3, %eax\n\t"
		".Done:\n\t"
	}
}
*/


/* 3.74 find_range���2 */
/*
range_t find_range(float x) {
  __asm__(
	  "vxorps %xmm1, %xmm1, %xmm1\n\t"
	  "movq $1, %rax\n\t"
	  "movq $2, %r8\n\t"
	  "movq $0, %r9\n\t"
	  "movq $3, %r10\n\t"
	  "vucomiss %xmm1, %xmm0\n\t"
	  "cmovg %r8, %rax\n\t"
	  "cmove %r9, %rax\n\t"
	  "cmovpq %r10, %rax\n\t"
	  );
}
*/
