//---------------------------------------------------------------------------


#pragma hdrstop

#include "clientsockt.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
CCliSocket g_tcp_client;
int CCliSocket::makeupWSA()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if(WSAStartup(sockVersion, &data)!=0)
	{
		return 0;
	}
	return 1;
}

int CCliSocket::cleanWSA()
{
    WSACleanup();
	return 0;
}

CCliSocket::CCliSocket()
{

}



int CCliSocket::init()
{

	clisockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(clisockfd == INVALID_SOCKET)
	{
		printf("invalid socket!");
		return 0;
	}


	return 1;
}
CCliSocket::~CCliSocket()
{
    if(!Close())
    {
		printf( "close err\n");
    }
}
bool CCliSocket::setNonBlock()
{
	int iRet = SOCKET_ERROR;
	unsigned long ulEnable = 1;
	iRet = ioctlsocket(clisockfd, FIONBIO, &ulEnable);
	return SOCKET_ERROR != iRet;
}
bool CCliSocket::Close()
{
	if(clisockfd < 0)
        return false;
    else
	{
		closesocket(clisockfd);
        clisockfd = -1;
    }

    return true;
}

bool CCliSocket::Connect(const char* ip, int port)
{
    if(ip==NULL || ip=="" || port < 0)
        return false;
    struct sockaddr_in sddr;
    sddr.sin_family = AF_INET;
    sddr.sin_addr.s_addr = inet_addr(ip);
    sddr.sin_port = htons(port);
	if(connect(clisockfd, (struct sockaddr*)&sddr, sizeof(sddr)) < 0)
    {
		printf("connnect to server err\n");
        return false;
    }
    return true;
}

int  CCliSocket::Send(char* sendbuf, int len)
{
	/*int ret = 0;
	do
	{
		ret = send(clisockfd, sendbuf, len, 0);

	} while (ret < 0 && (errno == EAGAIN || errno == EINTR));
	return ret == len;
	*/
	return  send(clisockfd, sendbuf, len, 0);
}

int  CCliSocket::Recv(char* recvbuf, int len)
{

		return  recv(clisockfd, recvbuf, len, 0);
}

