#pragma once
#include <NTL/ZZ.h>
#include <string>
#include "ConstSet.h"
#include <iostream>
#include "BloomFilter.h"
using namespace NTL;
struct Party {
	ZZ sk, pk;
	char* filename;
	//string outfilename;
	ZZ* ciphertext1;
	ZZ* ciphertext2;
};
void TElSetup(int l, ZZ* P, ZZ* Q, ZZ* G);
ZZ TELKGen(ZZ p, ZZ q, ZZ g, ZZ pk, Mul* ELpk, int NumParty);
void TELEnc(ZZ p, ZZ q, ZZ g, ZZ PK, ZZ m, ZZ* c1, ZZ* c2);
void TELDec(ZZ p, ZZ SK, ZZ c1, ZZ c2, ZZ* M);