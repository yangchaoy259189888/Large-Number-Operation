#include <stdio.h>
#include <stdlib.h>

#include "hugeNumber.h"

//�޴����ĳ˷���
//�����󳤶ȣ����������ĳ���֮�͡� 
//��������ƽʱ������ϰ�ߣ��ҵ�һ��ѭ������num2���ڶ���ѭ������num1��
//��һ�γ���󣬺����ÿ��������Ҫ����ǰ��Ľ����������Ҫ��λ�� 
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

//ʵ�ּ�����λ���鷳���ɴ���ڶ������෴�������üӷ��������ɡ� 
void opposite(HUGE_NUMBER *num) {
	if (num->symbol == POSITIVE) {
		num->symbol = MINUS;
	} else {
		num->symbol = POSITIVE;
	}
}

//�ѽ��д��output�ļ��� 
void writeToFile(HUGE_NUMBER result, char *filePath, char symbol) {
	FILE *fp;
	int i;
	
	fp = fopen(filePath, "w");
	if (fp == NULL) {
		printf("���ļ�ʧ�ܣ�");
	}

	if (symbol == POSITIVE) {
		fprintf(fp, "[+]", symbol);
	} else {
		fprintf(fp, "[-]", symbol);
	}
	
	if (result.data[result.dataCount - 1] != 0) {	//�����λ��Ϊ0����д���ļ��� 
		fprintf(fp, "%d", result.data[result.dataCount - 1]);
	}
	for (i = result.dataCount - 2; i >= 0; i--) {
		fprintf(fp, "%04d", result.data[i]);
	}	
}

//�޴����ļӷ�����Ϊ�н�λ���޽�λ����������н�λ���ڽ�λ���͵�״����
//Ϊ�����λ���ͣ���Ҫ���������޽�λ��ʵ�����ַ�ʽ�ķ���������ԭ�����ݵ�ǰ����ϸ���λ(��result���ȶ�1��)������ǰ�油��0000������ǰ�油��9999�� 
//���裨num1����Ϊ2��num2����Ϊ1��,��10000��9999�� 
//����һ���洢�ӷ����result�Ŀռ䣬result�ĳ�����num1��num2��󳤶ȼ�1�����ǵ���λ������3��
//��һ��ѭ����ʵ����Ӻ�������λ�������޴������Ⱥ�result�ĳ��Ȳ����ʱ�� ���޴���ǰ�油0�� 
//result�ĳ��ȱ�num1��num2����󳤶Ȼ���1�� 
//result�ķ�����num1��num2����λ��������������
//�ڶ���ѭ��������һ��ѭ�����ֽ�λ���͵�����£��ڶ���ѭ���������á� 
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

//΢���벹�룺���뷴��ԭ�����ƣ� 
//��Ϊ������������Դ����ȡ�
//��Ϊ��������������ÿһλ��9ȡ��������λ���⣩�� 
int getMecCode(int data, char symbol) {
	return ((symbol == MINUS) ? (9999 - data) : data);
}

//���پ޴�������Ŀռ� 
void destoryHugeNumber(HUGE_NUMBER *num) {
	if (num->data == NULL) {
		return;
	}
	
	free(num->data);
	num->data = NULL;
}

//��ʾ�޴��� 
void showHugeNumber(HUGE_NUMBER num) {
	int i;
	
	printf("[%c]", num.symbol == POSITIVE ? '+' : '-');
	for (i = num.dataCount - 1; i >= 0; i--) {
		printf("%04d ", num.data[i]);
	}
	printf("\n");
}

//�ļ��о޴����Ǹ�λ��ǰ����λ�ں󡣳�ʼ��ʱ��Ӧ��λ��ǰ���±�С������λ�ں��±�󣩣���������ԭ���Ƿ����λ�� 
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
	
	//dataCount = �����ֵĸ��� + 3 �� /  4������154 2673 8265��dataCount  = ��11 + 3��/ 4 = 3��
	num->dataCount = (fileLen + 3) / 4;
	num->data = (int *) calloc(sizeof(int), num->dataCount);
	
	//��λ�浽�±��ĵط� 
	fscanf(fp, format[fileLen % 4], &num->data[num->dataCount - 1]);
	for (i = num->dataCount - 2; i >= 0; i--) {
		fscanf(fp, "%4d", &num->data[i]);
	}
	
	fclose(fp);
}

//���ļ��ж�ȡ���ݣ�����initHugeNumber()������ʼ�� 
void readFile(HUGE_NUMBER *num, char *filePath) {
	FILE *fp;
	
	fp = fopen(filePath, "r");
	if (NULL == fp) {
		printf("���ļ�ʧ�ܣ�");
	}
	
	initHugeNumber(num, fp);
}
