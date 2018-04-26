//---------------------------------------------------------------------------

#ifndef clientsocktH
#define clientsocktH
#include<windows.h>
#include<STDIO.H>
#include <Sockets.hpp>
//#include<iostream>
//#include<cstring>
//using namespace std;
#pragma comment(lib, "ws2_32.lib")
//---------------------------------------------------------------------------
#define BUFFER_SIZE 1024
struct  RecvStream{
	unsigned char  stream[BUFFER_SIZE];
	int size;
};

class RecvBuf
{
public:
    unsigned char stream[BUFFER_SIZE];
    int size;
    RecvBuf()
    {
        size = 0;
    }
    int getDataFromBuf(unsigned char *buf, int *len)
    {
        int j,begin = -1,end=-1;
        if(size == 0)
        {
            *len = 0;
            return 0;
        }

        for (j=0;j<size;j++)
        {
            if (stream[j] == 0x7e)
            {
                if(begin == -1)
                {
                    begin=j;
                }
                else
                {
                    end=j;
                    break;
                }
            }
        }
        if(end == begin +1)
        {
            memmove(stream, stream+end, size - end);
            size = size -end;
            return -1;
        }
        if((begin != -1)&&(end == -1)) //not found end of the packet
        {
            if (size==BUFFER_SIZE)
            {
                size=0;
                return 0;
            }
            memmove(stream, stream+begin, size - begin);
            size = size -begin;
            return 0;
        }
        if((begin == -1)&&(end == -1)) // no begin or end added 07 07
        {
            *len = 0;
            size = 0;
            return 0;
        }

        int check=verifyMsg(begin,end);
        printf("check is %d\n",check);
        if (0==check){
            memcpy(buf, stream+begin, end-begin +1);
            memmove(stream, stream+end+1,size - end-1);
            size = size -end -1;
            *len = end -begin +1;
            return *len;

        }
        else{
            memmove(stream, stream+end,size - end);
            size = size -end ;
            *len = 0;
            return -1;

        }
 /*       if((size -end -1) >0)
		{
			memcpy(buf, stream+begin, end-begin +1);
			memmove(stream, stream+end+1,size - end-1);
			size = size -end -1;
			*len = end -begin +1;
			return *len;
		}
		else if ((size - end -1) == 0)
		{
			memcpy(buf, stream+begin, size-begin);

			*len = size -begin;
			size=0;
			return *len;
		}
		else
		{
			*len = 0;
			return 0;
		}*/
	}
	int verifyMsg(int s, int e)
	{
		//int j = 0;
		int start = 0;
		//int tmplen = 0;
        int comlen =e-s+1;
        unsigned char c=0x00;
        if(comlen<15)
        {
            return -2;
        }


        for(int i=0;i<comlen-2;i++)
        {
            if ((stream[i+s] == 0x7e))
            {
                start = i +s+ 1;
                continue;
            }

            if (stream[i+s] != 0x7d)
            {
                c = c^stream[i+s] ;
            }
            else
            {
                if((i+s)!=(e-2))
                {
                    if(stream[i+s+1] == 0x02)
                    {
                        c = c^0x7e;
                        //i++;
                    }
                    else if(stream[i+s+1] == 0x01)
                    {
                        c = c^0x7d;
                        //i++;
                    }
                    else
                    {
                        return -1;
                    }

                    i++;

                }
                else
                {
                    if(stream[i+s+1] == 0x02)
                    {
                        return c== 0x7e?0:-1;
                        //i++;
                    }
                    else if(stream[i+s+1] == 0x01)
                    {
                        return c== 0x7d?0:-1;
                        //i++;
                    }
                    else
                    {
                        return -1;
                    }
                }

            }
        }
        if (c==stream[e-1])
            return 0;
        else

        return -3;
    }

};

struct Msg
{
	unsigned char stream[BUFFER_SIZE];
	int len;
};

class CCliSocket
{
public:
    CCliSocket();
	virtual ~CCliSocket();
	int init();
	bool Connect(const char* ip, int port);
	bool setNonBlock();
	int Send(char* sendbuf, int len);
	int Recv(char* recvbuf, int len);

	static int makeupWSA();
	static int cleanWSA();
private:
    bool Close();
public:
	SOCKET  clisockfd;
};

extern CCliSocket g_tcp_client;

#define ACK_TIMEOUT 100
#define RETRY_TIME 3

#endif
