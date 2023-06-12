#pragma once

#include "ConstSet.h"
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <sstream>
#include <string>
#include "afxdialogex.h"
#pragma comment(lib, "ws2_32.lib")


int boradcast(char* buff,int port);
int recboradcast(char* buff,int port);
void SendBitString(char* bitarray, ZZ BitSize);
string zToString(const ZZ* z, ZZ Size);
string zToString(const ZZ z);
void Stringtoz(ZZ* z, string a, ZZ Size, int k);
void ReceiveBits(ZZ* cipher, CString IPAdress, long long* X1Size);

