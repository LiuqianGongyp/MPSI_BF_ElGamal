
#include "pch.h"
#include "tel.h"
using namespace std;

using namespace NTL;
void TElSetup(int l, ZZ* P, ZZ* Q, ZZ* G)
{
	switch (l) {
	case 12:
		*P = 2207;
		*Q = 1103;
		*G = 1425;
		break;
	case 16:
		*P = 57803;
		*Q = 28901;
		*G = 43052;
		break;
	case 25:
		*P = 17842883;
		*Q = 8921441;
		*G = 9372126;
		break;
	case 32:
		*P = 3048814907;
		*Q = 1524407453;
		*G = 2128317804;
		break;
	}
	//ZZ p, q, r;
	//while (1)
	//{
	//	GenPrime(p, l);
	//	//cout << p << endl;
	//	q = (p - 1) / 2;
	//	if (ProbPrime(q))
	//		break;
	//}
	//while (1)
	//{
	//	RandomBnd(r, p - 4);
	//	if (PowerMod(r, 2, p) != 1 && PowerMod(r, q, p) != 1)
	//	{
	//		break;
	//	}
	//}
	//*G = PowerMod(r, 2 * (q - 1), p);
	//*P = p; *Q = q;
}
ZZ TELKGen(ZZ p, ZZ q, ZZ g, ZZ pk, Mul* ELpk, int NumParty)
{
	ZZ PK(pk);
	for (int i = 0; i < NumParty - 1; i++) {
		PK = MulMod(PK, ELpk[i].elpk, p);
	}
	return PK;
}
void TELEnc(ZZ p,ZZ q,ZZ g,ZZ PK,ZZ m,ZZ* c1,ZZ* c2)
{
	ZZ r;
	RandomBnd(r, q);
	*c1 = PowerMod(g, r, p);
	*c2 = MulMod(PowerMod(g, m, p), PowerMod(PK, r, p), p);
}
void TELDec(ZZ p,ZZ SK,ZZ c1,ZZ c2,ZZ* M)
{
	*M = MulMod(c2, InvMod(PowerMod(c1, SK, p), p), p);
}