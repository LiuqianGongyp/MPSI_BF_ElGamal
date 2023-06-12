#pragma once
#include <bitset>
#include <NTL/ZZ.h>
using namespace NTL;
using std::bitset;
//const int MAX_BITMAP_SIZE = 1000;
class CBloomFilter
{
public:
	ZZ itemNum; // ����Ԫ�ظ�������n
	int hasFunsNum;	// ��ϣ�����ĸ���k
	long arraysize;
	long arrysize_8;
	unsigned char* bitarray;
	CBloomFilter(long itemsNum, double falsepositive);
	~CBloomFilter();

	void InitialBloomFilter(char* fileName);

	void InsertBloomFilter(char* str, int len);

	void GetHashValue(char* str, int len, int* hasFunsRet);

	bool IsContain(char* str, int len);


};