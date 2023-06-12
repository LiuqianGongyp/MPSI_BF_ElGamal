#pragma once
#include <NTL/ZZ.h>
#include <string>
#include <iostream>
#include "ConstSet.h"
#include "BloomFilter.h"
using namespace NTL;
//#define Numparties 5
//const long V_MAX = 10000;
//const int Filename_MAX = 20;
//const long X_1_SIZE = 10000;
struct Party {
	ZZ sk, pk;
	char* filename;
	//string outfilename;
	ZZ* ciphertext1;
	ZZ* ciphertext2;
	ZZ* T;
};
void TElSetup(int l, ZZ* P, ZZ* Q, ZZ* G);
ZZ TELKGen(ZZ p, ZZ q, ZZ g, ZZ pk, Mul* ELpk, int NumParty);
void TELEnc(ZZ p, ZZ q, ZZ g, ZZ PK, ZZ m, ZZ* c1, ZZ* c2);
void TELDec(ZZ p, ZZ SK, ZZ c1, ZZ c2, ZZ* M);