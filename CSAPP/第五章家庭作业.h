#pragma once


#include <vector>
#include <iostream>


/* ��ֻ˵���㣺һ�������Ż������֮Դ������ǧ�����VS���Ż� */


/* 5.13 */
/*
* A. ��
* B. �������ӷ����ӳٽ���Ϊ3.0
* C. �����ӷ����ӳٽ���Ϊ1.0
* D. ��Ϊ�ؼ�·����ֻ�и������ӷ�
*/


/* 5.14 */
/*
* A. ��Ϊ�ؼ�·����һ�ε���6���ӷ�ָ���n/6�ε�����CPE���Ϊ1.0
* B. ѭ��չ����Ҫ�Ǽ����˵��������Ŀ�����Ȼ������ӷ����������û�м���
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
* ԭ������Ǹ�������������
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
		// ����д��ʵ����6*1a
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

	// ��ַ����
	auto schar = (unsigned char *)s;
	for (cnt = 0; cnt < n; ++cnt) {
		// ��ָ��ǿתΪ����������ʹ�����ĵ�ֵַ
		if ((size_t)schar % K == 0)
			break;
		*schar++ = (unsigned char)c;
	}

	// ����ull��ÿ��װ��8��uc
	auto uc = (unsigned char)c;
	auto ull = (unsigned long long)c;
	for (int i = 1; i < K; ++i)
		ull |= (ull << (i << 3));

	// �ּ���ʵ���������ּ���64λ��д����
	auto slonglong = (unsigned long long *)s;
	// limit = n - size + 1	��n��Сʱ��limit�����
	for (; cnt + K < n + 1; cnt += K) {
		*slonglong++ = ull;
	}
	
	// ʣ��λ���ֽڼ�д����
	schar = (unsigned char *)slonglong;
	for (; cnt < n; ++cnt)
		*schar++ = (unsigned char)c;

	return s;
}


/* 5.18 */
// ʵ���������������Ҳû�в��ԣ��Ż��ⶫ��ͦ�����ģ�ƽʱ����ע��һ��ͺ�
// �����˲��ò��Ż��ĵط���ȥ��ĥ�������Ż������֮Դ
// ����vs���ܣ�û�Ż��İ汾�еľ�Ȼ�����ġ�����
// �Ҷ����ﲻ�뿴������ˣ��Ժ���˵��
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

