
#include "pch.h"
#include "trans.h"

DWORD WINAPI getELPK(LPVOID arg)
{
	Mul* Temp = (Mul*)arg;
	char* buff;
	buff = new char[MAX_BUF_LEN];
	stringstream ss;
	ZZ tempelpk;
	int i = recboradcast(buff,Temp->port);
	string s(buff);
	ss << s;
	ss >> Temp->elpk;
	//arg=(LPVOID*)&Temp;
	return 0;
}