#pragma once

#include <vector>
#include <NTL/ZZ.h>
#include <iomanip>
using namespace std;
using namespace NTL;

//#define Numparties 2
//const long V_MAX = 10000;
const int Filename_MAX = 20;
//const long X_1_SIZE = 10000;

const int MAX_BUF_LEN = 255;
#define BUFF_SIZE (1024 * 1024)
#define FILE_NAME_LENGTH 1024
#define Me 1
const int Digit = 4;

struct Mul
{
	ZZ elpk;//保存各个参与方的公钥
	int port;
};