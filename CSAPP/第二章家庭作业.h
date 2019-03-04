#pragma once

#include <cstdlib>
#include <climits>
#include <cstring>


/* 2.62 在对int类型的数使用算术右移的机器上运行时返回1，其他情况返回0 */
// 思路：对一个负数右移判断
int int_shifts_are_arithmetic() {
	return !(~(INT_MIN >> ((sizeof(int) << 3) - 1)));
}


/* 2.63 函数srl用算数右移完成逻辑右移；函数sra用逻辑右移完成算术右移 */
unsigned srl(unsigned x, int k) {
	unsigned xsra = (int)x >> k;
	return xsra & ~(-1 << ((sizeof(int) << 3) - k));
}

int sra(int x, int k) {
	int xsrl = (unsigned)x >> k;
	unsigned restbit = sizeof(int) << 3;
	return xsrl | (x >> (restbit - 1) & (-1 << (restbit - k)));
}


/* 2.64 当x的所有奇数位都为1时返回1，否则返回0 */
// 思路：x的四个字节为1010循环
int any_odd_one(unsigned x) {
	// 技巧：两个!运算符将结果转为1或0
	return x == (x & 0xAAAAAAAA);
}


/* 2.65 当x的位表示含有奇数个1时返回1，否则返回0 */
// 思路：位与位进行异或运算，有偶数个1时结果为0，有奇数个1时结果为1
int odd_ones(unsigned x) {
	// 技巧：对半异或，能将异或次数降为logN次，N为位数
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return x & 0x1;
}


/* 2.66 生成掩码（mask），表示x中最左边的1 */
// 思路：生成的掩码形如[x...x10...0]，可以先将x转换为形如[0...01...1]的位向量，再通过移位取反等位操作得到掩码
int leftmost_one(unsigned x) {
	unsigned mask = x;
	// 技巧：先与右移一位或等，最左1右一位被置1，现在最左有两个连续1，根据这两个1右移两位或等，现在最左有连续四个1，以此类推，直至最左1右边全为1
	mask |= mask >> 1;
	mask |= mask >> 2;
	mask |= mask >> 4;
	mask |= mask >> 8;
	mask |= mask >> 16;
	// 注意不能mask = (~mask) >> 1，对掩码为全1的不成立
	mask = ~(mask >> 1);
	return x & mask;
}


/* 2.67 在int为32位的机器上生成1，否则返回0 */
// 思路：分两次移位（蠢哭了）,16位版也分次移位
int int_size_is_32() {
	/*
	以下为原错误代码，违反了C语言标准中，位移量应该小于待移位值的位数，的规定
	int set_msb = 1 << 31;
	int beyond_msb = 1 << 32;

	return set_msb && !beyond_msb;
	*/
	
	int set_msb = 1 << 31;
	int beyond_msb = 1 << 31;
	beyond_msb <<= 1;

	return set_msb && !beyond_msb;
}


/* 2.68 生成一个掩码，将最低n个有效位设置为1，1 <= n <= w */
// 思路：将-1左移取反得到，注意n = w的情况；还可以将2左移后减一得到
int lower_one_mask(int n) {
	int mask = -1 << (n - 1);
	return ~(mask << 1);
	// return (2 << (n - 1)) - 1
}


/* 2.69 指定位数的循环左移，0 <= n < w */
// 思路：将高n位记录，右移x后相加，注意n = 0的情况
unsigned rotate_left(unsigned x, int n) {
	unsigned tmp = x >> ((sizeof(x) << 3) - n - 1);
	return (x << n) + (tmp >> 1);
}


/* 2.70 当x可以被表示为n位的补码时返回1，否则返回0，1 <= n <= w */
// 思路：将符号位之后的位数移走，剩下全为0（+）或全为1（-）就可以表示
int fits_bits(int x, int n) {
	x >>= n - 1;
	// 技巧：当x全为1或全为0时，它与自己取反的逻辑与必为0
	return !(x && ~x);
}


/* 2.71 A：原代码忽略了符号位 */
using packed_t = unsigned;
// B：思路：先将要取出的字节放在最高位，然后转型为int后再进行移位
int xbyte(packed_t word, int bytenum) {
	return ((int)(word << ((3 - bytenum) << 3))) >> 24;
}


/* 2.72 A：转为无符号数后任何结果都大于等于0 */
// B
void copy_int(int val, void *buf, unsigned maxbytes) {
	if (maxbytes >= sizeof(val))
		memcpy(buf, (void *)&val, sizeof(val));
}


/* 2.73 实现一个饱和加法函数，即正溢出返回TMax，负溢出返回TMin */
// 思路：判断是否溢出，再根据判断结果返回值，这个函数参考百度，这谁想得出来啊。。。
int saturating_add(int x, int y) {
	int w = sizeof(int) << 3;
	int sum = x + y;

	// 如果溢出，overflow为111...11，否则000...00
	int overflow = ((x ^ sum) & (y ^ sum)) >> (w - 1);

	// 如果x为正，sign为000...000，否则111...11
	int sign = x >> (w - 1);

	/*
	情况一：无溢出，则overflow为000...00，则减号前段为sum本身，后段为0，返回值为sum本身；
	情况二：正溢出，则overflow为111...11，sign为000...00，则减号前段为111...11，后段为100...00，相减得到TMax；
	情况三：负溢出，则overflow为111...11, sign为111...11，则减号前段为111...11，后段为011...11，相减得到TMin；
	*/
	return (sum | overflow) - ((1 << (w - 1)) & overflow) ^ (sign & overflow);
}


/* 2.74 当计算x-y不溢出时返回1 */
// 思路：补码减法溢出情况有两种，第一正数减负数大于TMax溢出；第二负数减正数小于TMin溢出
int tsub_ok(int x, int y) {
	int result = x - y;
	// 这里要额外判断x=0时，因为|TMin| = TMax + 1，极限情况0-TMin会溢出
	if (x >= 0 && y < 0 && result < 0)
		return 0;
	// 这里不需要判断（我认为），极限情况TMin-0不会溢出
	if (x < 0 && y > 0 && result > 0)
		return 0;
	return 1;
}


/* 2.75 利用给定函数得到x*y的高w位 */
// 得到x和y在补码形式下的高w位
int signed_high_prod(int x, int y) {
	long long result = x * y;
	return (unsigned long long)result >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {
	int w = sizeof(unsigned) << 3;
	// 根据公式，将x和y转换成补码计算时，其低w位一致，高w位则相差(Xw-1 * y) + (Yw-1 * x)
	return signed_high_prod(x, y) + (x >> (w - 1))*y + (y >> (w - 1))*x;
}


/* 2.76 编写自己的calloc版本 */
void *my_calloc(size_t nmemb, size_t size) {
	if (!(nmemb | size))
		return NULL;

	// 思路：先得到总的需求空间，并判断是否产生溢出，判断方式参考68页的练习2.35
	size_t total = nmemb * size;
	if ((total / nmemb) == size) {
		void  *p = malloc(total);
		memset(p, 0, total);
		return p;
	}
	
	return NULL;
}


/* 2.77 
17[10001]，x<<4 + x；-7[1001]，x - x<<3；
60[110010]，x<<5 + x<<4 + x<<1；-112[10010000]，x<<4 - x<<7
*/


/* 2.78 用正确的舍入方法计算x/(2的k次幂） */
// 思路：就是当x<0时需要加上偏置数
int divide_power2(int x, int k) {
	int bias = (1 << k) - 1;
	int w = sizeof(int) << 3;
	int sign = x >> (w - 1);
	return (x + (bias & sign)) >> k;
}


/* 2.79 实现函数mul3div4，计算3*x时也会产生溢出 */
int mul3div4(int x) {
	x = (x << 1) + x;
	return divide_power2(x, 2);
}


/* 2.80 实现函数threefourths，它不会溢出 */
/* 思路：要求不会溢出，就需要先除以四后乘以三，但要注意除法结果的小数部分会被舍弃，
这里的做法是将x中小于4的那部分数单独拿出来处理，因为只有这部分数会产生小数 */
int threefourths(int x) {
	int w = sizeof(int) << 3;
	int sign = x >> (w - 1);

	// f表示x前30bits，l表示x后2bits
	int f = x & ~0x3;
	int l = x & 0x3;

	// f一定是4的倍数，所以不用考虑小数情况
	int fd4 = f >> 2;
	int fd4m3 = (fd4 << 1) + fd4;

	// 单独处理l，先乘再除可解决小数进位的问题，也因为只有两位，所以不会溢出，直接在这里处理偏置数即可
	int lm3 = (l << 1) + l;
	int bias = (1 << 2) - 1;
	int lm3d4 = (lm3 + bias & sign) >> 2;

	return fd4m3 + lm3d4;
}


/* 2.81 A：-1<<k；B:（~(-1<<k))<<j */


/* 2.82 A：(x<y)==(-x>-y) 错，x = TMin, y = 0；
B：((x+y)<<4)+y-x==17*x + 15*y 对，无论是否溢出，都对低32位没有影响；
C：~x+~y+1==~(x+y) 对，~x+~y+1 = ~x+1+~y+1-1 = -x-y-1 = -(x+y)-1 = ~(x+y)；
D：(ux-uy)==-(unsigned)(y-x) 对，有符号数和无符号数有相同的位级表示；
E：((x>>2)<<2)<=x 对，右移两位可能会造成正权值丢失 */


/* 2.83 A：令x为无穷串值，则x<<k = x + Y，则x = Y/(1<<k - 1)；
B：5/7  2/15  19/63 */


/* 2.84 return !((ux<<1)|(uy<<1)) || (sx&&!sy) || (!sx && !sy && (ux<=uy) || (sx && sy && (ux>=uy) */


/* 2.85到2.91 在书上 */


using float_bits = unsigned;

bool isNaN(float_bits f) {
	if (((f & 0x7F800000) == 0x7F800000) && ((f & 0x7FFFFF) != 0))
		return true;
	return false;
}


/* 2.92 计算-f，如果f为NaN，则返回f */
float_bits float_negate(float_bits f) {
	if (isNaN(f))
		return f;
	return f ^ 0x80000000;
}


/* 2.93 计算|f|，如果f为NaN，则返回f */
float_bits float_absval(float_bits f) {
	if (isNaN(f))
		return f;
	return f & 0x7fffffff;
}


/* 2.94 计算2.0*f，如果f为NaN，则返回f */
// 思路：分别考虑非规格化值、规格化值和溢出的情况
float_bits float_twice(float_bits f) {
	if (isNaN(f))
		return f;

	unsigned sign = f >> 31;
	unsigned exp = (f >> 23) & 0xff;
	unsigned frac = f & 0x7fffff;

	if (!exp)
		/* 直接左移frac，尽管可能转化为规格化值，但是值是对的
		// 非规范化值
		if (frac >> 22)
			// 小数部分首位为1，则转为规格化值，frac不变，exp+1，注意转换时E仍然为1-Bias，
			// 但由于默认整数位为1，相当于左移一位，这就是非规格化转规格化的平滑体现
			return (sign << 31) | ((exp + 1) << 23) | frac;
		else
			// 小数部分首位不为1，则小数部分左移一位
			return (sign << 31) | (exp << 23) | (frac << 1);
			*/
		frac <<= 1;
	else if (exp == 0xfe) {
		// 超出最大规格化阶数
		exp = 0xff;
		frac = 0;
	}
	else
		++exp;

	return (sign << 31) | (exp << 23) | frac;
}


/* 2.95 计算0.5*f，如果f为NaN，则返回f */
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
		// 补偿规格化值隐含的1
		frac = (frac >> 1) + 0x400000;
	}
	else if (exp < 0xff)
		--exp;

	return (sign << 31) | (exp << 23) | frac;
}


/* 2.96 计算(int)f */
int float_f2i(float_bits f) {
	int sign = f >> 31;
	sign = sign == 0 ? 1 : -1;
	unsigned exp = (f >> 23) & 0xff;
	unsigned frac = f & 0x7fffff;

	unsigned Bias = 127;
	int E = exp - Bias;
	// F为1.frac左移23位的数
	int F = frac + (1 << 23);
	int result;

	// 非规格化数和比1小的数
	if (!exp || E < 0)
		result = 0;
	// 超出上界
	else if (E > 31 /*|| (E == 31 && sign == 1)*/)
		result = 0x80000000;
	else {
		// 如果f表示的是2^31，那么结果依旧会是0x80000000
		// 根据E和小数位数23的大小关系，决定F左移还是右移
		if (E - 23 >= 0)
			result = sign * (F << (E - 23));
		else
			result = sign * (F >> (23 - E));
	}
	
	return result;
}


/* 计算(float)i */
// 思路：由上题可知，int类型的数不可能为非规范化值（不可能小于1）
float_bits float_i2f(int i) {
	if (!i)
		return 0;

	// 对负数求补
	int sign = i < 0;
	// c为complement
	int ic = sign == 1 ? -i : i;
	// 找出最高位的1是第几位
	int firstOne;
	for (firstOne = 31; firstOne >= 0; --firstOne)
		if ((ic >> firstOne) & 0x1)
			break;

	// 设置阶数和偏置
	int E = firstOne;
	int Bias = 127;
	// 小数部分为1.x...x，这里省略1，只保留x...x，先左移，后逻辑右移
	int frac = ((unsigned)ic << (32 - firstOne)) >> (31 - 22);

	// 单精度的小数部分只有23位，加上隐藏的一位是24位，如果ic的位级表示超过24位，就要进行舍入，舍入规则是向偶数舍入
	// rounding意为舍入
	int firstRound = firstOne - 24;
	if (firstRound >= 0) {
		// 小数部分只有形如.XXXXY100...才实行向偶数舍入
		if ((ic & (((1 << (firstRound + 1)) - 1))) == (1 << firstRound)) {
			// 如果小数部分的最后一位为1，则进位
			if ((frac & 0x1)) {
				// 特殊情况，小数部分全为1
				if (frac == 0x7fffff) {
					++E;
					frac = 0;
				}
				else
					++frac;
			}
		}
		// 否则进行普通舍入，这时舍入位如果为1，则一定超过半数
		else if ((ic >> firstRound) & 0x1) {
			// 处理特殊情况
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
