#ifndef _HUGE_NUMBER_H_
#define _HUGE_NUMBER_H_

#define POSITIVE 	0
#define MINUS 		1

typedef struct HUGE_NUMBER {
	char symbol;	//�洢���ļ��ж�ȡ���ľ޴����ķ���
	int *data;		//�洢�޴��������ֲ���
	int dataCount;	//�洢�޴������ֲ�������Ԫ�صĸ���
} HUGE_NUMBER;

void readFile(HUGE_NUMBER *num, char *filePath);
void initHugeNumber(HUGE_NUMBER *num, FILE *fp);
void showHugeNumber(HUGE_NUMBER num);
void destoryHugeNumber(HUGE_NUMBER *num);
int getMecCode(int data, char symbol);
void addHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result);
void writeToFile(HUGE_NUMBER result, char *filePath, char symbol);
void opposite(HUGE_NUMBER *num);
void subHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result);
void multiHugeNumber(HUGE_NUMBER *num1, HUGE_NUMBER *num2, HUGE_NUMBER *result);

#endif
