#include <stdio.h>
#include <stdlib.h>

#include "hugeNumber.h"

//巨大数的乘法：
//结果最大长度：两个乘数的长度之和。 
//根据我们平时的运算习惯，我第一个循环控制num2，第二个循环控制num1。
//第一次乘完后，后面的每次运算需要加上前面的结果，并且需要错位。 
void multiHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result) {
	int i;
	int j;
	int carry = 0;
	int eachResult;
	int resIndex;
	
	result->dataCount = num1->dataCount + num2->dataCount;
	result->data = (int *) calloc(sizeof(int), result->dataCount);
	result->symbol = num1->symbol ^ num2->symbol;
	
	for (i = 0; i < num2->dataCount; i++) {
		carry = 0;
		resIndex = i;
		for (j = 0; j < num1->dataCount; j++) {
			eachResult = num1->data[j] * num2->data[i] + carry;
			carry = (eachResult + result->data[resIndex]) / 10000;
			result->data[resIndex] = (eachResult + result->data[resIndex]) % 10000;
			resIndex++;
		}
		result->data[resIndex] = carry;
	}
}

void subHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result) {
	opposite(num2);
	addHugeNumber(num1, num2, result);
}

//实现减法借位很麻烦，干脆求第二个数相反数，调用加法函数即可。 
void opposite(HUGE_NUMBER *num) {
	if (num->symbol == POSITIVE) {
		num->symbol = MINUS;
	} else {
		num->symbol = POSITIVE;
	}
}

//把结果写到output文件中 
void writeToFile(HUGE_NUMBER result, char *filePath, char symbol) {
	FILE *fp;
	int i;
	
	fp = fopen(filePath, "w");
	if (fp == NULL) {
		printf("打开文件失败！");
	}

	if (symbol == POSITIVE) {
		fprintf(fp, "[+]", symbol);
	} else {
		fprintf(fp, "[-]", symbol);
	}
	
	if (result.data[result.dataCount - 1] != 0) {	//若最高位不为0，则写入文件中 
		fprintf(fp, "%d", result.data[result.dataCount - 1]);
	}
	for (i = result.dataCount - 2; i >= 0; i--) {
		fprintf(fp, "%04d", result.data[i]);
	}	
}

//巨大数的加法：分为有进位和无进位两种情况，有进位存在进位饱和的状况。
//为解决进位饱和，就要将它视作无进位，实现这种方式的方法就是在原本数据的前面加上辅助位(故result长度多1个)：正数前面补上0000，负数前面补上9999。 
//假设（num1长度为2，num2长度为1）,例10000和9999。 
//申请一个存储加法结果result的空间，result的长度是num1和num2最大长度加1（考虑到进位），即3；
//第一个循环（实现相加和连续进位）：当巨大数长度和result的长度不相等时， 给巨大数前面补0。 
//result的长度比num1和num2的最大长度还大1。 
//result的符号是num1，num2，进位三者异或运算得来
//第二个循环：若第一个循环出现进位饱和的情况下，第二个循环才起作用。 
void addHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result) {
	int carry = 0;
	int i;
	int eachResult;
	
	result->dataCount = (num1->dataCount > num2->dataCount ? num1->dataCount : num2->dataCount) + 1;
	result->data = (int *) calloc(sizeof(int), result->dataCount);
	
	for (i = 0; i < result->dataCount; i++) {
		if (i >= num1->dataCount) {
			num1->data[i] = 0;
		} 
		if (i >= num2->dataCount) {
			num2->data[i] = 0;
		} 
		eachResult = getMecCode(num1->data[i], num1->symbol) + getMecCode(num2->data[i], num2->symbol) + carry;
		result->data[i] = eachResult % 10000;
		carry = eachResult / 10000;
	}
	result->symbol = num1->symbol ^ num2->symbol ^ carry;
	for (i = 0; i < result->dataCount; i++) {
		eachResult = result->data[i] + carry;
		result->data[i] = getMecCode(eachResult % 10000, result->symbol);
		carry = eachResult / 10000;
	}
}

//微译码补码：（与反码原理相似） 
//若为正数，补码与源码相等。
//若为负数，将负数的每一位与9取反（符号位除外）。 
int getMecCode(int data, char symbol) {
	return ((symbol == MINUS) ? (9999 - data) : data);
}

//销毁巨大数申请的空间 
void destoryHugeNumber(HUGE_NUMBER *num) {
	if (num->data == NULL) {
		return;
	}
	
	free(num->data);
	num->data = NULL;
}

//显示巨大数 
void showHugeNumber(HUGE_NUMBER num) {
	int i;
	
	printf("[%c]", num.symbol == POSITIVE ? '+' : '-');
	for (i = num.dataCount - 1; i >= 0; i--) {
		printf("%04d ", num.data[i]);
	}
	printf("\n");
}

//文件中巨大数是高位在前，低位在后。初始化时，应低位在前（下标小），高位在后（下标大），这样做的原因是方便进位。 
void initHugeNumber(HUGE_NUMBER *num, FILE *fp) {
	const char *format[4] = {"%4d", "%1d", "%2d", "%3d"};
	int fileLen;
	int firstSymbol;
	int i;
	
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	fileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	firstSymbol = fgetc(fp);
	if (firstSymbol == '+') {
		fileLen--;
		num->symbol = POSITIVE;
	} else if (firstSymbol == '-') {
		fileLen--;
		num->symbol = MINUS;
	} else {
		num->symbol = POSITIVE;
		fseek(fp, 0, SEEK_SET);
	}
	
	//dataCount = （数字的个数 + 3 ） /  4。例如154 2673 8265，dataCount  = （11 + 3）/ 4 = 3。
	num->dataCount = (fileLen + 3) / 4;
	num->data = (int *) calloc(sizeof(int), num->dataCount);
	
	//高位存到下标大的地方 
	fscanf(fp, format[fileLen % 4], &num->data[num->dataCount - 1]);
	for (i = num->dataCount - 2; i >= 0; i--) {
		fscanf(fp, "%4d", &num->data[i]);
	}
	
	fclose(fp);
}

//从文件中读取数据，调用initHugeNumber()函数初始化 
void readFile(HUGE_NUMBER *num, char *filePath) {
	FILE *fp;
	
	fp = fopen(filePath, "r");
	if (NULL == fp) {
		printf("打开文件失败！");
	}
	
	initHugeNumber(num, fp);
}
