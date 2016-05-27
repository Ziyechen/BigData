#define _CRT_SECURE_NO_WARNINGS 1

#include "BIgData.h"

BigData::BigData(INT64 value)
	:_value(value)
{
	INT64ToString();
}

BigData::BigData(const char *pData)
{
	//"+123456789"  "-123456789" "123456789fafca" "    "  "000123546"
	assert(pData);

	char cSymbol = pData[0];
	char *pStr = (char *)pData;

	if (cSymbol == '+' || cSymbol == '-')
	{
		pStr++;
	}
	else if (cSymbol >= '0' && cSymbol <= '9')
	{
		cSymbol = '+';
	}
	else
	{
		return;
	}
	
	//"000123546"
	while (*pStr == '0')
	{
		pStr++;
	}

	//"123456789qwera"
	_strData.resize(strlen(pData) + 1);
	_strData[0] = cSymbol;

	_value = 0;
	int iCount = 1;

	while (*pStr >= '0' && *pStr <= '9')
	{
		_value = _value * 10 + (*pStr - '0');
		_strData[iCount++] = *pStr;

		pStr++;
	}

	_strData.resize(iCount);

	if (cSymbol == '-')
	{
		_value = 0 - _value;
	}
}

BigData BigData::operator+(const BigData & bigData)
{
	//两个都没溢出
	if (IsINT64OverFlow() && bigData.IsINT64OverFlow())
	{
		//同号/异号
		if (_strData[0] != bigData._strData[0])
		{
			return BigData(_value + bigData._value);
		}
		else
		{
			//溢出/没有溢出
			if ((_value > 0 && MAX_INT64 - _value >= bigData._value)
				|| (_value < 0 && MIN_INT64 - _value <= bigData._value))
			{
				return BigData(_value + bigData._value);
			}
		}
	}

	//有一个溢出 & 结果溢出
	if (_strData[0] == bigData._strData[0])
	{
		return BigData(Add(_strData, bigData._strData).c_str());
	}
	return BigData(Sub(_strData, bigData._strData).c_str());
}

BigData BigData::operator-(const BigData & bigData)
{
	//都没溢出
	if (IsINT64OverFlow() && bigData.IsINT64OverFlow())
	{
		//同号/异号
		if (_strData[0] == bigData._strData[0])
		{
			return BigData(_value - bigData._value);
		}
		else
		{
			if ((_value > 0 && MAX_INT64 + bigData._value >= _value)
				|| (_value < 0 && MIN_INT64 + bigData._value <= _value))
			{
				return BigData(_value - bigData._value);
			}
		}
	}
	//至少有一个溢出 & 结果溢出
	if (_strData[0] != bigData._strData[0])
	{
		return BigData(Add(_strData, bigData._strData).c_str());
	}
	return BigData(Sub(_strData, bigData._strData).c_str());
}

BigData BigData::operator*(const BigData & bigData)
{
	if (_value == 0 || bigData._value == 0)
	{
		return BigData(INT64(0));
	}

	if (IsINT64OverFlow() && bigData.IsINT64OverFlow())
	{
		if (_strData[0] == bigData._strData[0])
		{
			if ((_value > 0 && MAX_INT64 / _value >= bigData._value)
				|| (_value < 0 && MAX_INT64 / _value <= bigData._value))
			{
				return BigData(_value * bigData._value);
			}
		}
		else
		{
			if ((_value > 0 && MIN_INT64 / bigData._value >= _value)
				|| (_value < 0 && MIN_INT64 / bigData._value <= _value))
			{
				return BigData(_value * bigData._value);
			}
		}
	}
	return BigData(Mul(_strData, bigData._strData).c_str());
}

BigData BigData::operator/(const BigData & bigData)
{
	//1、被除数为0
	if ('0' == bigData._strData[1])
	{
		assert("false");
	}

	//2、两个数没溢出
	if (IsINT64OverFlow() && bigData.IsINT64OverFlow())
	{
		return _value / bigData._value;
	}
	//3、除数为1或-1
	if (bigData._value == 1 || bigData._value == -1)
	{
		return _value;
	}
	//4、除数和被除数相等
	//if (strcmp(_strData.data() + 1, bigdata._strData.data() + 1) == 0)
	//data()返回内容的字符数组形式
	if (strcmp(_strData.c_str() + 1, bigData._strData.c_str() + 1) == 0)
	{
		if (_strData[0] != bigData._strData[0])
		{
			return BigData(INT64(-1));
		}
		return BigData(INT64(1));
	}
	if (_strData.size() < bigData._strData.size() ||
		_strData.size() == bigData._strData.size() &&
		strcmp(_strData.c_str() + 1, bigData._strData.c_str() + 1) < 0)
	{
		return BigData(INT64(0));
	}
	return BigData(Div(_strData, bigData._strData).c_str());
}

string BigData::Add(string left, string right)
{
	int iLsize = left.size();
	int iRsize = right.size();

	//只需要左边为长度长的即可
	if (iLsize < iRsize)
	{
		swap(iLsize, iRsize);
		swap(left, right);
	}

	string sRet;
	sRet.resize(iLsize + 1);//相加不会超过较大数的size+1
	sRet[0] = left[0];
	int Step = 0;

	//通过模除得到每位的数字及进位Step
	for (int i = 1; i < iLsize; i++)
	{
		char cRet = left[iLsize - i] - '0' +Step;
		if (i < iRsize)
		{
			cRet += (right[iRsize - i] - '0');//cRet转为数字，-‘0’
		}

		sRet[iLsize - i + 1] = cRet % 10 + '0';//sRet比iLsize多一位,存放字符，故+‘0’
		Step = cRet / 10;
	}

	sRet[1] = Step + '0';
	return sRet;
}

string BigData::Sub(string left, string right)
{
	int iLsize = left.size();
	int iRsize = right.size();
	char cSymbol = left[0];

	right[0] = left[0];

	if (iLsize < iRsize
		|| ((iLsize == iRsize) &&  (left <right)))
	{
		swap(iLsize, iRsize);
		swap(left, right);

		if (cSymbol == '+')
		{
			cSymbol = '-';
		}
		else
		{
			cSymbol = '+';
		}
	}

	string sRet;
	sRet.resize(left.size());
	sRet[0] = cSymbol;

	for (int i = 1; i < iLsize; i++)
	{
		char cRet = left[iLsize - i] - '0';

		if (i < iRsize)
		{
			cRet -= right[iRsize - i] - '0';
		}

		if (cRet < 0)
		{
			left[iLsize - i - 1] -= 1;
			cRet += 10;
		}
		
		sRet[iLsize - i] = cRet + '0';
	}

	return sRet;
}

string BigData::Mul(string left, string right)
{
	//确定符号位
	char cSymbol = '+';
	if (left[0] != right[0])
	{
		cSymbol = '-';
	}

	//使外循环次数少一点
	int iLsize = left.size();
	int iRsize = right.size();

	if (iLsize > iRsize)
	{
		swap(iLsize, iRsize);
		swap(left, right);
	}

	//1、先取左边一个
	//2、和右边每一位相乘
	
	string sRet;
	//sRet.resize(iLsize + iRsize - 1);
	sRet.assign(iLsize + iRsize - 1, '0');
	sRet[0] = cSymbol;
	int iDataLen = sRet.size();
	int ioffset = 0;

	for (int iLIdx = 1; iLIdx < iLsize; iLIdx++)
	{
		char c = left[iLsize - iLIdx] - '0';
		char cStep = 0;

		//如果左边为0，就跳过
		if (c == 0)
		{
			ioffset++;
			continue;
		}

		for (int iRIdx = 1; iRIdx < iRsize; iRIdx++)
		{
			char cRet = c * (right[iRsize - iRIdx] - '0') + cStep;
			cRet += sRet[iDataLen - iRIdx - ioffset] - '0';
			sRet[iDataLen - iRIdx - ioffset] = cRet % 10 + '0';
			cStep = cRet / 10;
		}

		sRet[iDataLen - iRsize - ioffset] += cStep;
		ioffset++;
	}

	return sRet;
}

string BigData::Div(string left, string right)
{
	//此处用append()对字符串依次赋值
	std::string sRet;
	sRet.append(1, '+');

	if (left[0] != right[0])
	{
		sRet[0] = '-';
	}

	char* pLeft = (char*)left.c_str() + 1;
	char* pRight = (char*)right.c_str() + 1;
	int DataLen = right.size() - 1;//标记相除的除数位数
	int Lsize = left.size() - 1;
	int Rsize = right.size() - 1;

	//eg:222222/33首先取到22和33比较大小，如果大就直接相除，否则DataLen++；
	for (int iIdx = 0; iIdx < Lsize;)
	{
		if (!(IsLeftStrBig(pLeft, DataLen, pRight, Rsize)))//如果取到的数小于除数时，结果商0，向后再取一位
		{
			sRet.append(1, '0');
			DataLen++;
		}
		else
		{
			sRet.append(1, SubLoop(pLeft, DataLen, pRight, Rsize));//循环相减得到该位的商
			//判断pLeft中进行循环相减后依次去掉0，
			while (*pLeft == '0' && DataLen > 0)
			{
				pLeft++;
				DataLen--;
				iIdx++;
			}
			DataLen++;
		}

		if (DataLen > Rsize + 1)//pLeft比pRight大一位结果为0，则pLeft中含有0
		{
			pLeft++;
			DataLen--;
			iIdx++;
		}

		if (DataLen + iIdx > Lsize)//判断是否除法结束
			break;
	}

	return sRet;
}

bool BigData::IsINT64OverFlow()const
{
	//(7FFFFFFFFFFFFFFF)H = (9223372036854775807)D
	string tmp = "+9223372036854775807";
	if (_strData[0] == '-')
	{
		tmp = "-9223372036854775807";
	}

	if (_strData.size() < tmp.size())
	{
		return true;
	}
	else if ((_strData.size() == tmp.size()) && _strData <= tmp)
	{
		return true;
	}
	return false;
}

void BigData::INT64ToString()
{
	char cSymbol = '+';
	INT64 tmp = _value;

	if (_value < 0)
	{
		cSymbol = '-';
		tmp = 0 - _value;
	}

	_strData.append(1, cSymbol);
	while (tmp)
	{
		_strData.append(1, tmp % 10 + '0');
		tmp /= 10;
	}

	char *left = (char *)_strData.c_str() + 1;
	char *right = left + _strData.size() - 2;

	while (left < right)
	{
		char tmp = *left;
		*left = *right;
		*right = tmp;

		left++;
		right--;
	}
}

////判断是否left大于right
bool BigData::IsLeftStrBig(const char *pLeft, int LSize, const char *pRight, int RSize)
{
	assert(pLeft && pRight);

	char* pleft = (char*)pLeft;
	char* pright = (char*)pRight;
	if (LSize > RSize && *pleft > '0')//eg:112和33
	{
		return true;
	}
	else if (LSize == RSize)//eg:57和33
	{
		while (pright)
		{
			if (*pleft > *pright)
			{
				return true;
			}
			else if (*pleft == *pright)
			{
				pleft++;
				pright++;
			}
			else
				return false;
		}
	}
	return false;
}

char BigData::SubLoop(char *pLeft, int LSize, const char *pRight, int RSize)
{
	assert(pLeft != NULL && pRight != NULL);

	char cRet = 0;
	while (IsLeftStrBig(pLeft, LSize, pRight, RSize))//直到被减数小于减数停止运算
	{
		for (int iIdx = 0; iIdx < RSize; iIdx++)//进行减运算
		{
			char ret = pLeft[LSize - iIdx - 1] - '0';
			ret -= pRight[RSize - iIdx - 1] - '0';
			if (ret < 0)
			{
				pLeft[LSize - iIdx - 2] -= 1;
				ret += 10;
			}
			pLeft[LSize - iIdx - 1] = ret + '0';
		}
		while (*pLeft == '0'&&LSize>0)
		{
			pLeft++;
			LSize--;
		}
		cRet++;
	}
	return cRet + '0';
}

ostream & operator<<(ostream & output, const BigData & bigData)
{
	if (bigData.IsINT64OverFlow())
	{
		output << bigData._value;
	}
	else
	{
		char *pData = (char *)bigData._strData.c_str();
		if (pData[0] == '+')
		{
			pData++;
		}

		output << pData;
	}
	return output;
}
