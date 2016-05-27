#define _CRT_SECURE_NO_WARNINGS 1

#include "BIgData.h"
#include <stdlib.h>

//BigData
void test1()
{
	//"1234567" "-1234567" "+1234567" "1234567qwead" "+" "   " "-0000123456qwed" ...
	BigData d1(123456789);
	BigData d2("9223372036854775808");
	BigData d3("-1234567");
	BigData d4("+1234567");
	BigData d5("    ");
	BigData d6("+");
	BigData d7("0000123456");
	BigData d8("-0000123456qwed");

	cout << d1;
	cout << d2;
	cout << d3;
	cout << d4;
	cout << d5;
	cout << d6;
	cout << d7;
	cout << d8;

}

void test2()
{
	//BigData l(1234);
	//BigData r(-7);
	//cout << l + r<< endl;

	//BigData l1(9223372036854775807);
	//BigData r1(2);
	//cout << (l1 + r1) << endl;

	//BigData l2("-9223372036854775808");
	//BigData r2(-3);
	//cout << (l2 + r2) << endl;

	//BigData l3(9223372036854775807);
	//BigData r3("-9223372036854775808");
	//cout << l3 + r3 << endl;

	//(出错)
	BigData l4("-22222222222222222222222222222222222");
	BigData r4("22222222222222222222222222222222223");
	cout << l4 + r4 << endl;
}

void test3()
{
	//BigData l(9223372036854775807);
	//BigData r(1);
	//cout << l - r << endl;

	//BigData l("-9223372036854775808");
	//BigData r(-3);
	//cout << l - r << endl;

	//BigData l("9223372036854775808");
	//BigData r("9223372036854775809");
	//cout << l - r << endl;

	//BigData l1("9223372036854775809");
	//BigData r1(-1);
	//cout << l1 - r1 << endl;

	BigData l1("22222222222222222222222222222");
	BigData r1("-22222222222222222222222222223");
	cout << l1 - r1 << endl;
}

void test4()
{
	BigData l("-9223372036854775808");
	BigData r(100);
	cout << l * r << endl;
}

void test5()
{
	BigData l("222222222222222222222222222222222222");
	BigData r("33");
	cout << l / r << endl;
}

int main()
{
	//test1();
	//test2();
	//test3();
	//test4();
	test5();

	system("pause");
}
