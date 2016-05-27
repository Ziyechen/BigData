#pragma once

#include <string>
#include <assert.h>
#include <iostream>
using namespace std;

#define MAX_INT64 0X7FFFFFFFFFFFFFFF
#define MIN_INT64 0X8000000000000000

#define UN_INIT 0XCCCCCCCCCCCCCCCC
typedef long long INT64;

class BigData
{
public:
	BigData(INT64 value = UN_INIT);
	BigData(const char *pData);//对大数进行处理，优化

	BigData operator+(const BigData & bigData);
	BigData operator-(const BigData & bigData);
	BigData operator*(const BigData & bigData);
	BigData operator/(const BigData & bigData);

private:
	string Add(string left, string right);			
	string Sub(string left, string right);
	string Mul(string left, string right);
	string Div(string left, string right);

	bool IsINT64OverFlow()const;//判断大数是否溢出
	void INT64ToString();//由于值在_value中，需将_value转化成string类型
	bool IsLeftStrBig(const char *pLeft, int LSize, const char *pRight, int RSize);//判断是否left大于right
	char SubLoop(char *pLeft, int LSize, const char *pRight, int RSize);//循环相减

	friend ostream & operator<<(ostream & output, const BigData & bigData);

private:
	INT64 _value;
	string _strData;
};
