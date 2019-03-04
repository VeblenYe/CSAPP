#pragma once

#include <cstdio>


/* 3.58 根据所给汇编代码写出等价的C代码 */
long decode2(long x, long y, long z) {
	y -= z;
	x *= y;
	long _y = y;
	_y <<= 63;
	_y >>= 63;
	return x ^ _y;
}


/* 3.59 描述计算128位乘法运算的算法，对汇编代码加以注释 */
/* 分析：X = 2^64 * Xh + Xl，Y = 2^64 * Yh + Yl
则X * Y = 2^128(XhYh) + 2^64(XhYl + XlYh) + XlYl，令Z1 = XhYl，Z2 = XlYh，Z3 = XlYl，可知Z都为128位
则X * Y = 2^128(XhYh + Z1h + Z2h) + 2^64(Z1l + Z2l + Z3h) + Z3l.
分析完毕
首先的2~5条汇编代码将x和y扩展到128位，然后6~8条计算出Z1l+Z2l，并将其放入%rcx中，
接着9条计算Z3，并将Z3l放入%rax中，Z3h放入%rdx中，之后10条计算Z1l+Z2l+Z3h，并放入%rdx中，
剩下指令则将结果放入*dest中 */


/* 3.60 根据给出的汇编代码推断C代码 */
long loop(long x, int n) {
	long result = 0;
	long mask;
	for (mask = 1; mask != 0; mask = mask << n) {
		result |= x & mask;
	}
	return result;
}


/* 3.61 编写函数cread_alt */
long cread_alt(long *xp) {
	// 思路，不能判断*xp，改为判断xp，然而生成的汇编代码并没有使用条件传送指令n 
	long t = 0;
	long *p = xp ? xp : &t;
	return *p;

	// 另一种形式
	// return (!xp ? 0 : *xp);
}


/* 3.62 根据汇编代码补全C代码，纯翻译 */
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


/* 3.63 逆向工程推出switch主体 */
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


/* 3.64 逆向工程推出R、S和T的值 */
/* A. &A[i][j][k] = A + L(S*T*i + T*j + k) 
B. R = 7, T = 13, S = 5 */


/* 3.65 逆向工程推出寄存器内容 */
/* A. %rdx
B. %rax
C. M = 15 */


/* 3.66 逆向工程推断宏定义 */
/* NR(n) = 3 * n
NC(n) = 4 * n + 1 */


/* 3.67 逆向工程推断结构参数传递的实现 */
/* A. 整个栈帧%rsp~%rsp+104, (%rsp) x, (%rsp+8)y, (%rsp+16)&z, (%rsp+24)z, (%rsp+64)y, 
(%rsp+72)x, (%rsp+80)z 
B. 传递了栈帧地址%rsp+64 
C.D. 都是靠%rsp加上偏移值访问和设置 
F. 调用者通过分配调用者栈帧中的空区域交与被调用者，被调用者通过栈指针加偏移值访问 */


/* 3.68根据汇编代码推断常量值 */
/* 4 < B <= 8，6 < A <= 10，44 < AB <= 46
A = 9，B = 5 */


/* 3.69 根据汇编代码得到定义的常量和结构 */
/* A. CNT = 7 
B. a_struct {long idx; long x[4]} */


/* 3.70 根据汇编代码推断 */
/* A. e1.p 0   e1.y 8   e2.x 0   e2.next 8
B. 16
C. up->e2.x = *(up->e2.next->e1.p) - up->e2.next->e1.y */


/* 3.71 实现函数good_echo */
constexpr int BUF_SIZE = 12;
void good_echo(void) {
	char buf[BUF_SIZE];
	while (true) {
		// 实际上我们不需要做什么，fgets保证了每次只读入BUF_SIZE-1个字符，不会发生溢出
		char *p = fgets(buf, BUF_SIZE, stdin);
		if (!p) break;
		printf("%s", p);
	}
}


/* 3.72 分析变长栈帧的的实现和使用情况 */
/* A. S2 = S1 - (8n + 30) & -16
B. p = (S2 + 15) & -16，前面分析得知&-16是为了地址16字节对齐
C. 首先e1不能等于0，因为e1 = 0，p就应该等于S2，分析公式可知不可能
接着分类分析，若n为偶数，则S2 = S1 - (8n + 16)，则e1 + e2 = 16，
当e1 = 1，e2 = 15时，e1最小，此时S1应满足S1 % 16 = 1；
若n为奇数，则S2 = S1 - (8n + 24)，则e1 + e2 = 24，
当e1 = 24，e2 = 0时，e1最大，此时S1 % 16 = 0；
D. 都是16位对齐 */


/* 3.73 写出find_range的汇编代码1 */
/* 无法编译，以后再说
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


/* 3.74 find_range汇编2 */
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
