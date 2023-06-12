
#include "pch.h"
#include "communication.h"

int boradcast(char* buff,int port)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// ����socket api
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	// ����socket
	SOCKET connect_socket;
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connect_socket)
	{
		err = WSAGetLastError();
		printf("socket error! error code is %d/n", err);
		return -1;
	}

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_BROADCAST;

	bool bOpt = true;
	//���ø��׽���Ϊ�㲥����
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	int nAddrLen = sizeof(SOCKADDR);

	// ��������
	int nSendSize = sendto(connect_socket, buff, strlen(buff), 0, (SOCKADDR*)&sin, nAddrLen);
	if (SOCKET_ERROR == nSendSize)
	{
		err = WSAGetLastError();
		printf("sendto error!, error code is %d/n", err);
		return -1;
	}

	return 0;
}

int recboradcast(char* buff,int port)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// ����socket api
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	// ����socket
	SOCKET connect_socket;
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connect_socket)
	{
		err = WSAGetLastError();
		printf("socket error! error code is %d/n", err);
		return -1;
	}

	// �������׽���
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = 0;

	// �����������ϵĹ㲥��ַ��������
	SOCKADDR_IN sin_from;
	sin_from.sin_family = AF_INET;
	sin_from.sin_port = htons(port);
	sin_from.sin_addr.s_addr = INADDR_BROADCAST;

	//���ø��׽���Ϊ�㲥���ͣ�
	bool bOpt = true;
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	// ���׽���
	err = bind(connect_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)
	{
		err = WSAGetLastError();
		printf("bind error! error code is %d/n", err);
		return -1;
	}

	int nAddrLen = sizeof(SOCKADDR);
	// ��������
	int nSendSize = recvfrom(connect_socket, buff, MAX_BUF_LEN, 0, (SOCKADDR*)&sin_from, &nAddrLen);
	if (SOCKET_ERROR == nSendSize)
	{
		err = WSAGetLastError();
		printf("recvfrom  error! error code is %d/n", err);
		return -1;
	}
	//buff[nSendSize] = '/0';
	closesocket(connect_socket);
	return 0;
}
string zToString(const ZZ z) {
	stringstream buffer;
	buffer << z;
	return buffer.str();
}
string zToString(const ZZ* z, ZZ Size) {
	stringstream buffer;
	for (int i = 0; i < Size; i++) {
		buffer << setw(Digit - floor(log(z[i]) / log(10))) << setfill('0') << 0 << z[i];
	}
	//cout << buffer.str() << endl;
	return buffer.str();
}
void SendBitString(char* bitarray, ZZ BitSize)
{
	int s;                     /* socket for accepting connections    */
	int ns;                    /* socket connected to client          */
	//_onexit(exitFunc);
	unsigned short port;       /* port server binds to                */
	//char buff[BUFF_SIZE];              /* buffer for sending & receiving data */
	struct sockaddr_in client; /* client address information          */
	struct sockaddr_in server; /* server address information          */
	int namelen;               /* length of client name               */
	port = 5010;
	//long long int BitSize = strlen(bitarray);//�����ͱ��ع�ģ


	WSADATA wsadata;
	WSAStartup(0x202, &wsadata);

	//����socket����
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error\n");
		exit(2);
	}

	//socket�ͷ����ַ��
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		printf("bind error\n");
		exit(3);
	}

	//��������ֻ����һ���ͻ�������
	if (listen(s, 1) != 0)
	{
		printf("listen error\n");
		exit(4);
	}

	//�ȴ�����
	namelen = sizeof(client);
	while (true) {
		//ѭ�� һֱ�ȴ��ͻ��˵�����
		if ((ns = accept(s, (struct sockaddr*)&client, &namelen)) == -1)
		{
			printf("accept error\n");
			exit(5);
		}
		long long int sendSize = 0;
		//�Ƚ����ش���С�����ݷ��͸��ͻ���
		char* buff = strdup(zToString(BitSize).c_str());
		//_ltoa(BitSize, buff, 10);
		if (send(ns, buff, strlen(buff), 0) < 0) {
			printf("send BitSize to client error\n");
			exit(7);
		}
		BitSize = BitSize * (Digit + 1);

		int index = 0;
		while (sendSize < (BitSize - BUFF_SIZE)) {
			send(ns, bitarray + index * BUFF_SIZE, BUFF_SIZE, 0);
			sendSize += BUFF_SIZE;
			index++;
			cout << index << endl;
		}
		//send(ns, bitarray + index * BUFF_SIZE, BitSize % BUFF_SIZE, 0);
		if (send(ns, bitarray + index * BUFF_SIZE, BitSize % BUFF_SIZE, 0) < 0) {
			printf("send Bits to client error\n");
			exit(8);
		}
		else {
			break;
		}
	}
	printf("P1 ended successfully\n");
	closesocket(s);
	closesocket(ns);
	//exit(0);
}
void Stringtoz(ZZ* z, string a, ZZ Size, int k) {
	stringstream sstream;
	for (int i = 0; i < Size; i++) {
		sstream.clear();
		sstream << a.substr(i * k, k);
		sstream >> z[i];
	}
}
void ReceiveBits(ZZ* cipher, CString IPAdress)
{
	int s;                     /* client socket*/
	//_onexit(exitFunc);
	WSADATA wsadata;
	WSAStartup(0x202, &wsadata);

	unsigned short port;       //����˿�
	char buf[BUFF_SIZE];       //����
	struct hostent* hostnm;    //�����ַ��Ϣ
	struct sockaddr_in server; //����sockaddr��Ϣ

	hostnm = gethostbyname(IPAdress);
	if (hostnm == (struct hostent*)0)
	{
		fprintf(stderr, "Gethostbyname failed\n");
		exit(2);
	}


	port = 5010;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = *((unsigned long*)hostnm->h_addr);

	//����socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket error\n");
		exit(3);
	}

	//׼�����ӷ����
	printf("ready to connet to the other party ...\n");
	while (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		;
	}
	//�Ƚ��ձ��ش���С
	int iRecv = 0;
	memset(buf, 0, BUFF_SIZE);
	iRecv = recv(s, buf, BUFF_SIZE, 0);
	if (iRecv < 0) {
		printf("recv BitSize error\n");
		exit(5);
	}
	long long int totalBitSize = atoll(buf);
	printf("totalBitSize:%lld\n", totalBitSize);
	//cipher = new ZZ[totalBitSize];
	ZZ BitSize = (ZZ)totalBitSize;
	totalBitSize = totalBitSize * (Digit + 1);
	//���ձ��ش�
	long long int BitRecv = 0;
	stringstream buffer;
	buffer.clear();
	while (BitRecv < totalBitSize) {
		memset(buf, 0, BUFF_SIZE);
		iRecv = recv(s, buf, BUFF_SIZE, 0);
		for (int i = 0; i < iRecv; i++) {

			buffer << buf[i];
		}
		if (iRecv < 0)
		{
			printf("Recv error\n");
			exit(6);
		}
		if (iRecv == 0) {
			break;
		}
		BitRecv += iRecv;
	}
	Stringtoz(cipher, buffer.str(), BitSize, Digit+1);
	printf("P1 Ended Successfully\n");
	closesocket(s);
	//exit(0);
}
