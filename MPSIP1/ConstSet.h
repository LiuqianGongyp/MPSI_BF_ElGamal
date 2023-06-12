#pragma once

#include <vector>
#include <NTL/ZZ.h>
#include <iomanip>
using namespace std;
using namespace NTL;

const int Filename_MAX = 20;
const int MAX_BUF_LEN = 255;
#define BUFF_SIZE (1024 * 1024)
#define FILE_NAME_LENGTH 1024
#define Me 0
const int Digit = 4;

struct Mul
{
	ZZ elpk;//保存各个参与方的公钥
	int port;
};