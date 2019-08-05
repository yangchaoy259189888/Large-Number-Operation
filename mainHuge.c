#include <stdio.h>

#include "hugeNumber.c"

int main() {
	char *fileOnePath = ".\\input1.txt";
	char *fileTwoPath = ".\\input2.txt";
	char *fileOutputPath = ".\\output.txt";
	HUGE_NUMBER num1 = {0};
	HUGE_NUMBER num2 = {0};
	HUGE_NUMBER addResult = {0};
	HUGE_NUMBER subResult = {0};
	HUGE_NUMBER multiResult = {0};
	HUGE_NUMBER diviResult = {0};
	
	readFile(&num1, fileOnePath);
	readFile(&num2, fileTwoPath);
	
	printf("*****ԭ����ʾ*****\n");
	showHugeNumber(num1);
	showHugeNumber(num2);
	
	// printf("\n*****�������*****\n");
	// addHugeNumber(&num1, &num2, &addResult);
	// showHugeNumber(addResult);
	// writeToFile(addResult, fileOutputPath, subResult.symbol); 
	
	// printf("\n*****�������*****\n");
	// subHugeNumber(&num1, &num2, &subResult);
	// showHugeNumber(subResult);
	// writeToFile(subResult, fileOutputPath, subResult.symbol);
	
	printf("\n*****�������*****\n");
	multiHugeNumber(&num1, &num2, &multiResult);
	showHugeNumber(multiResult);
	writeToFile(multiResult, fileOutputPath, subResult.symbol);
	
	destoryHugeNumber(&num1);
	destoryHugeNumber(&num2);
	
	return 0;
}
