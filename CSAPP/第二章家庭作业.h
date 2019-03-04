#pragma once

#include <cstdlib>
#include <climits>
#include <cstring>


/* 2.62 �ڶ�int���͵���ʹ���������ƵĻ���������ʱ����1�������������0 */
// ˼·����һ�����������ж�
int int_shifts_are_arithmetic() {
	return !(~(INT_MIN >> ((sizeof(int) << 3) - 1)));
}


/* 2.63 ����srl��������������߼����ƣ�����sra���߼���������������� */
unsigned srl(unsigned x, int k) {
	unsigned xsra = (int)x >> k;
	return xsra & ~(-1 << ((sizeof(int) << 3) - k));
}

int sra(int x, int k) {
	int xsrl = (unsigned)x >> k;
	unsigned restbit = sizeof(int) << 3;
	return xsrl | (x >> (restbit - 1) & (-1 << (restbit - k)));
}


/* 2.64 ��x����������λ��Ϊ1ʱ����1�����򷵻�0 */
// ˼·��x���ĸ��ֽ�Ϊ1010ѭ��
int any_odd_one(unsigned x) {
	// ���ɣ�����!����������תΪ1��0
	return x == (x & 0xAAAAAAAA);
}


/* 2.65 ��x��λ��ʾ����������1ʱ����1�����򷵻�0 */
// ˼·��λ��λ����������㣬��ż����1ʱ���Ϊ0����������1ʱ���Ϊ1
int odd_ones(unsigned x) {
	// ���ɣ��԰�����ܽ���������ΪlogN�Σ�NΪλ��
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return x & 0x1;
}


/* 2.66 �������루mask������ʾx������ߵ�1 */
// ˼·�����ɵ���������[x...x10...0]�������Ƚ�xת��Ϊ����[0...01...1]��λ��������ͨ����λȡ����λ�����õ�����
int leftmost_one(unsigned x) {
	unsigned mask = x;
	// ���ɣ���������һλ��ȣ�����1��һλ����1��������������������1������������1������λ��ȣ����������������ĸ�1���Դ����ƣ�ֱ������1�ұ�ȫΪ1
	mask |= mask >> 1;
	mask |= mask >> 2;
	mask |= mask >> 4;
	mask |= mask >> 8;
	mask |= mask >> 16;
	// ע�ⲻ��mask = (~mask) >> 1��������Ϊȫ1�Ĳ�����
	mask = ~(mask >> 1);
	return x & mask;
}


/* 2.67 ��intΪ32λ�Ļ���������1�����򷵻�0 */
// ˼·����������λ�������ˣ�,16λ��Ҳ�ִ���λ
int int_size_is_32() {
	/*
	����Ϊԭ������룬Υ����C���Ա�׼�У�λ����Ӧ��С�ڴ���λֵ��λ�����Ĺ涨
	int set_msb = 1 << 31;
	int beyond_msb = 1 << 32;

	return set_msb && !beyond_msb;
	*/
	
	int set_msb = 1 << 31;
	int beyond_msb = 1 << 31;
	beyond_msb <<= 1;

	return set_msb && !beyond_msb;
}


/* 2.68 ����һ�����룬�����n����Чλ����Ϊ1��1 <= n <= w */
// ˼·����-1����ȡ���õ���ע��n = w������������Խ�2���ƺ��һ�õ�
int lower_one_mask(int n) {
	int mask = -1 << (n - 1);
	return ~(mask << 1);
	// return (2 << (n - 1)) - 1
}


/* 2.69 ָ��λ����ѭ�����ƣ�0 <= n < w */
// ˼·������nλ��¼������x����ӣ�ע��n = 0�����
unsigned rotate_left(unsigned x, int n) {
	unsigned tmp = x >> ((sizeof(x) << 3) - n - 1);
	return (x << n) + (tmp >> 1);
}


/* 2.70 ��x���Ա���ʾΪnλ�Ĳ���ʱ����1�����򷵻�0��1 <= n <= w */
// ˼·��������λ֮���λ�����ߣ�ʣ��ȫΪ0��+����ȫΪ1��-���Ϳ��Ա�ʾ
int fits_bits(int x, int n) {
	x >>= n - 1;
	// ���ɣ���xȫΪ1��ȫΪ0ʱ�������Լ�ȡ�����߼����Ϊ0
	return !(x && ~x);
}


/* 2.71 A��ԭ��������˷���λ */
using packed_t = unsigned;
// B��˼·���Ƚ�Ҫȡ�����ֽڷ������λ��Ȼ��ת��Ϊint���ٽ�����λ
int xbyte(packed_t word, int bytenum) {
	return ((int)(word << ((3 - bytenum) << 3))) >> 24;
}


/* 2.72 A��תΪ�޷��������κν�������ڵ���0 */
// B
void copy_int(int val, void *buf, unsigned maxbytes) {
	if (maxbytes >= sizeof(val))
		memcpy(buf, (void *)&val, sizeof(val));
}


/* 2.73 ʵ��һ�����ͼӷ������������������TMax�����������TMin */
// ˼·���ж��Ƿ�������ٸ����жϽ������ֵ����������ο��ٶȣ���˭��ó�����������
int saturating_add(int x, int y) {
	int w = sizeof(int) << 3;
	int sum = x + y;

	// ��������overflowΪ111...11������000...00
	int overflow = ((x ^ sum) & (y ^ sum)) >> (w - 1);

	// ���xΪ����signΪ000...000������111...11
	int sign = x >> (w - 1);

	/*
	���һ�����������overflowΪ000...00�������ǰ��Ϊsum�������Ϊ0������ֵΪsum����
	����������������overflowΪ111...11��signΪ000...00�������ǰ��Ϊ111...11�����Ϊ100...00������õ�TMax��
	����������������overflowΪ111...11, signΪ111...11�������ǰ��Ϊ111...11�����Ϊ011...11������õ�TMin��
	*/
	return (sum | overflow) - ((1 << (w - 1)) & overflow) ^ (sign & overflow);
}


/* 2.74 ������x-y�����ʱ����1 */
// ˼·��������������������֣���һ��������������TMax������ڶ�����������С��TMin���
int tsub_ok(int x, int y) {
	int result = x - y;
	// ����Ҫ�����ж�x=0ʱ����Ϊ|TMin| = TMax + 1���������0-TMin�����
	if (x >= 0 && y < 0 && result < 0)
		return 0;
	// ���ﲻ��Ҫ�жϣ�����Ϊ�����������TMin-0�������
	if (x < 0 && y > 0 && result > 0)
		return 0;
	return 1;
}


/* 2.75 ���ø��������õ�x*y�ĸ�wλ */
// �õ�x��y�ڲ�����ʽ�µĸ�wλ
int signed_high_prod(int x, int y) {
	long long result = x * y;
	return (unsigned long long)result >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {
	int w = sizeof(unsigned) << 3;
	// ���ݹ�ʽ����x��yת���ɲ������ʱ�����wλһ�£���wλ�����(Xw-1 * y) + (Yw-1 * x)
	return signed_high_prod(x, y) + (x >> (w - 1))*y + (y >> (w - 1))*x;
}


/* 2.76 ��д�Լ���calloc�汾 */
void *my_calloc(size_t nmemb, size_t size) {
	if (!(nmemb | size))
		return NULL;

	// ˼·���ȵõ��ܵ�����ռ䣬���ж��Ƿ����������жϷ�ʽ�ο�68ҳ����ϰ2.35
	size_t total = nmemb * size;
	if ((total / nmemb) == size) {
		void  *p = malloc(total);
		memset(p, 0, total);
		return p;
	}
	
	return NULL;
}


/* 2.77 
17[10001]��x<<4 + x��-7[1001]��x - x<<3��
60[110010]��x<<5 + x<<4 + x<<1��-112[10010000]��x<<4 - x<<7
*/


/* 2.78 ����ȷ�����뷽������x/(2��k���ݣ� */
// ˼·�����ǵ�x<0ʱ��Ҫ����ƫ����
int divide_power2(int x, int k) {
	int bias = (1 << k) - 1;
	int w = sizeof(int) << 3;
	int sign = x >> (w - 1);
	return (x + (bias & sign)) >> k;
}


/* 2.79 ʵ�ֺ���mul3div4������3*xʱҲ�������� */
int mul3div4(int x) {
	x = (x << 1) + x;
	return divide_power2(x, 2);
}


/* 2.80 ʵ�ֺ���threefourths����������� */
/* ˼·��Ҫ�󲻻����������Ҫ�ȳ����ĺ����������Ҫע����������С�����ֻᱻ������
����������ǽ�x��С��4���ǲ����������ó���������Ϊֻ���ⲿ���������С�� */
int threefourths(int x) {
	int w = sizeof(int) << 3;
	int sign = x >> (w - 1);

	// f��ʾxǰ30bits��l��ʾx��2bits
	int f = x & ~0x3;
	int l = x & 0x3;

	// fһ����4�ı��������Բ��ÿ���С�����
	int fd4 = f >> 2;
	int fd4m3 = (fd4 << 1) + fd4;

	// ��������l���ȳ��ٳ��ɽ��С����λ�����⣬Ҳ��Ϊֻ����λ�����Բ��������ֱ�������ﴦ��ƫ��������
	int lm3 = (l << 1) + l;
	int bias = (1 << 2) - 1;
	int lm3d4 = (lm3 + bias & sign) >> 2;

	return fd4m3 + lm3d4;
}


/* 2.81 A��-1<<k��B:��~(-1<<k))<<j */


/* 2.82 A��(x<y)==(-x>-y) ��x = TMin, y = 0��
B��((x+y)<<4)+y-x==17*x + 15*y �ԣ������Ƿ���������Ե�32λû��Ӱ�죻
C��~x+~y+1==~(x+y) �ԣ�~x+~y+1 = ~x+1+~y+1-1 = -x-y-1 = -(x+y)-1 = ~(x+y)��
D��(ux-uy)==-(unsigned)(y-x) �ԣ��з��������޷���������ͬ��λ����ʾ��
E��((x>>2)<<2)<=x �ԣ�������λ���ܻ������Ȩֵ��ʧ */


/* 2.83 A����xΪ���ֵ����x<<k = x + Y����x = Y/(1<<k - 1)��
B��5/7  2/15  19/63 */


/* 2.84 return !((ux<<1)|(uy<<1)) || (sx&&!sy) || (!sx && !sy && (ux<=uy) || (sx && sy && (ux>=uy) */


/* 2.85��2.91 ������ */


using float_bits = unsigned;

bool isNaN(float_bits f) {
	if (((f & 0x7F800000) == 0x7F800000) && ((f & 0x7FFFFF) != 0))
		return true;
	return false;
}


/* 2.92 ����-f�����fΪNaN���򷵻�f */
float_bits float_negate(float_bits f) {
	if (isNaN(f))
		return f;
	return f ^ 0x80000000;
}


/* 2.93 ����|f|�����fΪNaN���򷵻�f */
float_bits float_absval(float_bits f) {
	if (isNaN(f))
		return f;
	return f & 0x7fffffff;
}


/* 2.94 ����2.0*f�����fΪNaN���򷵻�f */
// ˼·���ֱ��Ƿǹ��ֵ�����ֵ����������
float_bits float_twice(float_bits f) {
	if (isNaN(f))
		return f;

	unsigned sign = f >> 31;
	unsigned exp = (f >> 23) & 0xff;
	unsigned frac = f & 0x7fffff;

	if (!exp)
		/* ֱ������frac�����ܿ���ת��Ϊ���ֵ������ֵ�ǶԵ�
		// �ǹ淶��ֵ
		if (frac >> 22)
			// С��������λΪ1����תΪ���ֵ��frac���䣬exp+1��ע��ת��ʱE��ȻΪ1-Bias��
			// ������Ĭ������λΪ1���൱������һλ������Ƿǹ��ת��񻯵�ƽ������
			return (sign << 31) | ((exp + 1) << 23) | frac;
		else
			// С��������λ��Ϊ1����С����������һλ
			return (sign << 31) | (exp << 23) | (frac << 1);
			*/
		frac <<= 1;
	else if (exp == 0xfe) {
		// ��������񻯽���
		exp = 0xff;
		frac = 0;
	}
	else
		++exp;

	return (sign << 31) | (exp << 23) | frac;
}


/* 2.95 ����0.5*f�����fΪNaN���򷵻�f */
float_bits float_half(float_bits f) {
	if (isNaN(f))
		return f;

	unsigned sign = f >> 31;
	unsigned exp = (f >> 23) & 0xff;
	unsigned frac = f & 0x7fffff;

	if (!exp)
		frac >>= 1;
	else if (exp == 0x1) {
		--exp;
		// �������ֵ������1
		frac = (frac >> 1) + 0x400000;
	}
	else if (exp < 0xff)
		--exp;

	return (sign << 31) | (exp << 23) | frac;
}


/* 2.96 ����(int)f */
int float_f2i(float_bits f) {
	int sign = f >> 31;
	sign = sign == 0 ? 1 : -1;
	unsigned exp = (f >> 23) & 0xff;
	unsigned frac = f & 0x7fffff;

	unsigned Bias = 127;
	int E = exp - Bias;
	// FΪ1.frac����23λ����
	int F = frac + (1 << 23);
	int result;

	// �ǹ�����ͱ�1С����
	if (!exp || E < 0)
		result = 0;
	// �����Ͻ�
	else if (E > 31 /*|| (E == 31 && sign == 1)*/)
		result = 0x80000000;
	else {
		// ���f��ʾ����2^31����ô������ɻ���0x80000000
		// ����E��С��λ��23�Ĵ�С��ϵ������F���ƻ�������
		if (E - 23 >= 0)
			result = sign * (F << (E - 23));
		else
			result = sign * (F >> (23 - E));
	}
	
	return result;
}


/* ����(float)i */
// ˼·���������֪��int���͵���������Ϊ�ǹ淶��ֵ��������С��1��
float_bits float_i2f(int i) {
	if (!i)
		return 0;

	// �Ը�����
	int sign = i < 0;
	// cΪcomplement
	int ic = sign == 1 ? -i : i;
	// �ҳ����λ��1�ǵڼ�λ
	int firstOne;
	for (firstOne = 31; firstOne >= 0; --firstOne)
		if ((ic >> firstOne) & 0x1)
			break;

	// ���ý�����ƫ��
	int E = firstOne;
	int Bias = 127;
	// С������Ϊ1.x...x������ʡ��1��ֻ����x...x�������ƣ����߼�����
	int frac = ((unsigned)ic << (32 - firstOne)) >> (31 - 22);

	// �����ȵ�С������ֻ��23λ���������ص�һλ��24λ�����ic��λ����ʾ����24λ����Ҫ�������룬�����������ż������
	// rounding��Ϊ����
	int firstRound = firstOne - 24;
	if (firstRound >= 0) {
		// С������ֻ������.XXXXY100...��ʵ����ż������
		if ((ic & (((1 << (firstRound + 1)) - 1))) == (1 << firstRound)) {
			// ���С�����ֵ����һλΪ1�����λ
			if ((frac & 0x1)) {
				// ���������С������ȫΪ1
				if (frac == 0x7fffff) {
					++E;
					frac = 0;
				}
				else
					++frac;
			}
		}
		// ���������ͨ���룬��ʱ����λ���Ϊ1����һ����������
		else if ((ic >> firstRound) & 0x1) {
			// �����������
			if (frac == 0x7fffff) {
				++E;
				frac = 0;
			}
			else
				++frac;
		}
	}

	return (sign << 31) | (E + Bias) << 23 | frac;
}
