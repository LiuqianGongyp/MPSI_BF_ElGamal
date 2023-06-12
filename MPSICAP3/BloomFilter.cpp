
#include "pch.h"
#include "MurmurHash3.h"
#include "BloomFilter.h"
#include <assert.h>
#include <math.h>
#include <string>
#include <iostream>
#include <NTL/ZZ.h>
#include <ctime>
using namespace std;

using namespace NTL;
#define SEED 0x97c29b3a
#define MAX 0xFFFFFFFFFFFFFFFF
CBloomFilter::CBloomFilter(long itemsNum, double falsepositive)
{
	this->itemNum = itemsNum; // 过滤元素个数，即n
	this->arraysize = itemsNum * ceil(- log(falsepositive) / (log(2) * log(2)));//m
	this->hasFunsNum = ceil(-log(falsepositive) / (log(2)));	// 哈希函数的个数k
	/*if (ceil(log(2) * this->arraysize / itemsNum) > 30)
		this->hasFunsNum = 30;
	else
		this->hasFunsNum = ceil(log(2) * this->arraysize / itemsNum);*/
	if (this->arraysize % 8 == 0)
	{
		this->arrysize_8 = itemsNum * ceil(-log(falsepositive) / (log(2) * log(2))) / 8;
	}
	else
	{
		this->arrysize_8 = this->arraysize / 8 + 1;
	}
	this->bitarray = new unsigned char[this->arrysize_8];//位数组
	memset(this->bitarray, 255, this->arrysize_8);
}


CBloomFilter::~CBloomFilter(void)
{
}

// 读取文件中的字符串，按空格区分，然后将他们都存到位数组中
void CBloomFilter::InitialBloomFilter(char* fileName)
{
	memset(this->bitarray, 255, this->arrysize_8);
	FILE* fin;
	fopen_s(&fin,fileName, "r");
	assert(fin);/*
	clock_t a, b,e,d;
	double c=0;*/
	char ch;
	char str[1000] = { '\0' };
	int  iCnt = 0;
	int add = 0;
	int Final;
	//e = clock();
	do
	{
		Final = fscanf_s(fin, "%c", &ch);
		if ((ch != ' ' ) && Final != EOF)
		{
			str[iCnt] = ch;
			iCnt++;
		}
		else
		{
			str[iCnt] = '\0';
			/*string S;
			S = str;
			cout << S ;*/
			//a = clock();
			this->InsertBloomFilter(str, strlen(str));/*
			b = clock();
			c = b - a + c;*/
			iCnt = 0;
			add++;
		}
	} while (Final!= EOF && add < this->itemNum);
	if (NULL != fin)
	{
		fclose(fin);
		fin = NULL;
	}/*
	d = clock();
	cout << (double)(d - e) / CLOCKS_PER_SEC << endl;
	cout << c / CLOCKS_PER_SEC << endl;*/
}

// 将某个元素插入到位数组中
void CBloomFilter::InsertBloomFilter(char* str, int len)
{
	uint64_t H[2], combinedHash;
	MurmurHash3_x64_128(str, len, SEED, H);
	combinedHash = H[1];
	for (int i = 0; i < this->hasFunsNum; i++)
	{
		//assert(combinedHash >= 0);
		int bitPos = (combinedHash&MAX) % this->arraysize;
		//cout << bitPos << endl;
		this->bitarray[bitPos / 8] &= ~(char)(128 >> (bitPos % 8));
		combinedHash += H[0];
	}
}

//获取k个哈希值
void CBloomFilter::GetHashValue(char* str, int len, int* hasFunsRet)
{
	uint64_t H[2], combinedHash;
	MurmurHash3_x64_128(str, len, SEED, H);
	combinedHash = H[1];
	for (int i = 0; i < this->hasFunsNum; i++)
	{
		//assert(combinedHash >= 0);
		int bitPos = (combinedHash & MAX) % this->arraysize;
		hasFunsRet[i] = bitPos;
		//cout << bitPos << endl;
		combinedHash += H[0];
	}
}

// 判断一个元素是否在位数组中
bool CBloomFilter::IsContain(char* str, int len)
{
	uint64_t H[2], combinedHash;
	MurmurHash3_x64_128(str, strlen(str), SEED, H);
	combinedHash = H[1];
	for (int i = 0; i < this->hasFunsNum; i++)
	{
		int bitPos = (combinedHash & MAX) % this->arraysize;
		if ((this->bitarray[bitPos/8]& (128 >> (bitPos % 8))) == (128 >> (bitPos % 8)))
		{
			return false;
		}
		combinedHash += H[0];
	}
	return true;
}