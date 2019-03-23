#pragma once


#include <vector>
#include <iostream>


/* 我只说两点：一、过早优化是万恶之源；二、千万别在VS上优化 */


/* 5.13 */
/*
* A. 略
* B. 浮点数加法的延迟界限为3.0
* C. 整数加法的延迟界限为1.0
* D. 因为关键路径上只有浮点数加法
*/


/* 5.14 */
/*
* A. 因为关键路径上一次迭代6个加法指令，共n/6次迭代，CPE最低为1.0
* B. 循环展开主要是减少了迭代次数的开销，然而浮点加法运算次数并没有减少
*/
void inner5(std::vector<long> &u, std::vector<long> &v, std::vector<long>::size_type *dest) {
	long i;
	long length = u.size();
	std::vector<long>::size_type sum = 0;

	long limit = length - 5;
	for (i = 0; i < limit; i += 6)
		sum += u[i] * v[i]
		+ u[i + 1] * v[i + 1]
		+ u[i + 2] * v[i + 2]
		+ u[i + 3] * v[i + 3]
		+ u[i + 4] * v[i + 4]
		+ u[i + 5] * v[i + 5];

	for (; i < length; ++i)
		sum += u[i] * v[i];

	*dest = sum;
}


/* 5.15 */
/*
* 原因可能是浮点数数量过多
*/
void inner6(std::vector<long> &u, std::vector<long> &v, std::vector<long>::size_type *dest) {
	long i;
	long length = u.size();
	std::vector<long>::size_type acc0 = 0;
	std::vector<long>::size_type acc1 = 0;
	std::vector<long>::size_type acc2 = 0;
	std::vector<long>::size_type acc3 = 0;
	std::vector<long>::size_type acc4 = 0;
	std::vector<long>::size_type acc5 = 0;

	long limit = length - 5;
	for (i = 0; i < limit; i += 6) {
		acc0 += u[i] * v[i];
		acc1 += u[i + 1] * v[i + 1];
		acc2 += u[i + 2] * v[i + 2];
		acc3 += u[i + 3] * v[i + 3];
		acc4 += u[i + 4] * v[i + 4];
		acc5 += u[i + 5] * v[i + 5];
	}

	for (; i < length; ++i)
		acc0 += u[i] * v[i];

	*dest = (acc0 + acc1 + acc2 + acc3 + acc4 + acc5);
}


/* 5.16 */
void inner7(std::vector<long> &u, std::vector<long> &v, std::vector<long>::size_type *dest) {
	long i;
	long length = u.size();
	std::vector<long>::size_type sum = 0;

	long limit = length - 5;
	for (i = 0; i < limit; i += 6)
		// 这样写其实就是6*1a
		sum += u[i] * v[i]
		+ u[i + 1] * v[i + 1]
		+ u[i + 2] * v[i + 2]
		+ u[i + 3] * v[i + 3]
		+ u[i + 4] * v[i + 4]
		+ u[i + 5] * v[i + 5];

	for (; i < length; ++i)
		sum += u[i] * v[i];

	*dest = sum;
}


/* 5.17 */
void *basic_memset(void *s, int c, size_t n) {
	auto K = sizeof(unsigned long long);
	size_t cnt;

	// 地址对齐
	auto schar = (unsigned char *)s;
	for (cnt = 0; cnt < n; ++cnt) {
		// 将指针强转为整数类型来使用它的地址值
		if ((size_t)schar % K == 0)
			break;
		*schar++ = (unsigned char)c;
	}

	// 利用ull，每次装载8个uc
	auto uc = (unsigned char)c;
	auto ull = (unsigned long long)c;
	for (int i = 1; i < K; ++i)
		ull |= (ull << (i << 3));

	// 字级（实际上是四字级，64位）写遍历
	auto slonglong = (unsigned long long *)s;
	// limit = n - size + 1	当n过小时，limit会溢出
	for (; cnt + K < n + 1; cnt += K) {
		*slonglong++ = ull;
	}
	
	// 剩余位用字节级写遍历
	schar = (unsigned char *)slonglong;
	for (; cnt < n; ++cnt)
		*schar++ = (unsigned char)c;

	return s;
}


/* 5.18 */
// 实际最后两个函数我也没有测试，优化这东西挺玄乎的，平时编码注意一点就好
// 真有了不得不优化的地方再去琢磨，过早优化是万恶之源
// 我在vs上跑，没优化的版本有的居然是最快的。。。
// 我都无语不想看反汇编了，以后再说吧
double poly_6_3a(double a[], double x, long degree) {
	long i = 1;
	double result = a[0];
	double result1 = 0;
	double result2 = 0;

	double xpwr = x;
	double xpwr1 = x * x * x;
	double xpwr2 = x * x * x * x * x;

	double xpwr_step = x * x * x * x * x * x;
	for (; i <= degree - 6; i += 6) {
		result = result + (a[i] * xpwr + a[i + 1] * xpwr * x);
		result1 = result1 + (a[i + 2] * xpwr1 + a[i + 3] * xpwr1 * x);
		result2 = result2 + (a[i + 4] * xpwr2 + a[i + 5] * xpwr2 * x);

		xpwr *= xpwr_step;
		xpwr1 *= xpwr_step;
		xpwr2 *= xpwr_step;
	}

	for (; i <= degree; i++) {
		result = result + a[i] * xpwr;
		xpwr *= x;
	}

	return result + result1 + result2;
}



/* 5.19 */
void psum(double a[], double p[], long n) {
	long i;
	double last_val = p[0] = a[0];

	double acc0;
	double acc1;
	double acc2;
	double acc3;
	
	long limit = n - 3;
	for (i = 1; i < n - 1; i += 4) {
		acc0 = last_val + a[i];
		acc1 = acc0 + a[i + 1];
		acc2 = acc1 + a[i + 2];
		acc3 = acc2 + a[i + 3];
		p[i] = acc0;
		p[i + 1] = acc1;
		p[i + 2] = acc2;
		p[i + 3] = acc3;
		last_val = acc3;
	}

	for (; i < n; ++i) {
		last_val += a[i];
		p[i] = last_val;
	}
}

