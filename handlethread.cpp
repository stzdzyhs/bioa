//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "handlethread.h"


#include "windows.h"
#include "gettime.h"
#include "debugout.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall HandleThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
unsigned char HandleThread::sn=0;
unsigned char HandleThread::currentCmd=0;
unsigned char HandleThread::started=0;
int HandleThread ::command=0;
int HandleThread::settime=0;
	list<ActionRow>::iterator  HandleThread::li;
RecvBuf HandleThread::recvbuf;

HandleThread * g_handler;
static long long inline span(const struct timeval& a, const struct timeval& b)
{
	return (a.tv_sec-b.tv_sec)*1000000+a.tv_usec-b.tv_usec;
}

unsigned char HandleThread::getSn()
{
	if(sn<255) sn++;
	else sn=0;
	return sn;
}

__fastcall HandleThread::HandleThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall HandleThread::Execute()
{
	NameThreadForDebugging("handler");
	//---- Place thread code here ----
	run();


}

void HandleThread::HandleActionList()
{
	TIMEVAL tv;
	tv.tv_sec=0;
	tv.tv_usec=10000;
	int iRet = SOCKET_ERROR;
	fd_set fsWrite,fsRead;
	list<ActionRow>::iterator li;
	list<ActionRow> &a=g_action_sequence.action_list;
	struct timeval start,now;
	gettimeofday(&start, NULL);
	for(li=a.begin();li!=a.end();li++)
	{
		RecvBuf recvbuf;
		//DP3("test_id %d   s %d e %d",li->ptestrow, li->start_time,li->end_time);
		ActionRow ar=*li;
		int s=ar.start_time;
		int e=ar.end_time;
		struct timeval s1,e1;
		s1=e1=start;
		s1.tv_sec=s1.tv_sec+s;
		e1.tv_sec=e1.tv_sec+e;
		int retry=0;
		int state=0;
		Msg msg;
		unsigned char sendbuf[BUFFER_SIZE];
		int l=ar.toStream(sendbuf);
		sendbuf[2]=getSn();
		l = addCheckCode(sendbuf,l);

		toComposedMsg(sendbuf,l, msg.stream, &(msg.len));
		currentCmd=ar.a.type;
		for(;;)
		{
			gettimeofday(&now, NULL);
			if(span(now,e1)>=0)
			{
				//to do set testinstance status to 2
				//....

				break;
			}
			FD_ZERO(&fsWrite);
			FD_SET(g_tcp_client.clisockfd, &fsWrite);
			FD_ZERO(&fsRead);
			FD_SET(g_tcp_client.clisockfd, &fsRead);
			iRet = select(1, &fsRead, &fsWrite, NULL, &tv);
			if(0<iRet){
		//read or write socket
				if (FD_ISSET(g_tcp_client.clisockfd,&fsRead))
				{
					int rchars=g_tcp_client.Recv(recvbuf.stream+recvbuf.size,BUFFER_SIZE-recvbuf.size);
					recvbuf.size+=rchars;
					while( getMsgFromBuf(state)!=0)
					{
						 if(state!=0) goto next;
					}
					//if(state!=0) continue;
				}
				if (FD_ISSET(g_tcp_client.clisockfd,&fsWrite))
				{
					gettimeofday(&now, NULL);
					if((span(now,start)>=0)&&retry<RETRY_TIME)
					{
						   g_tcp_client.Send(msg.stream,msg.len);
						   retry++;

					}

				}

			}
			if(0==iRet){
			//time out
			}
			if(0>iRet){
			}
		}
		next:
	}


}


int HandleThread::toOriginalMsg(unsigned char * composed,int comlen, unsigned char * original, int *origlen)
{
    int j = 0;
    int start = 0;
    int tmplen = 0;

    for(int i=0;i<comlen;i++)
    {
        if ((composed[i] == 0x7e)&&(i != comlen-1))
        {
            start = i + 1;
            continue;
        }
        if ((composed[i] == 0x7e)&&(i == comlen-1))
        {
            if(tmplen != 0)
            {
                        memcpy(&original[j], &composed[start],tmplen);
            }
            j = j + tmplen;
            continue;
        }
        if (composed[i] != 0x7d)
        {
            tmplen++;
        }
        else
        {
            printf("j=%d  tmplen= %d, start=%d\n", j, tmplen, start);
            if(tmplen != 0)
            {
                        memcpy(&original[j], &composed[start],tmplen);
            }
            if(composed[i+1] == 0x02)
            {
                original[j+tmplen] = 0x7e;
            }
            else if(composed[i+1] == 0x01)
            {
                original[j+tmplen] = 0x7d;
            }
            else
            {
                return -1;
            }
            j = j + tmplen + 1;
            start = start + tmplen + 2;
            i++;
            tmplen = 0;
        }
    }
    *origlen = j;


    return 0;
}

int HandleThread::toComposedMsg(unsigned char * original,int origlen, unsigned char * composed, int* comlen)
{
    int j = 0;
    int tmplen  = 0;
	int start = 0;

    composed[j] = 0x7e;
    j++;
    for(int i =0; i< origlen; i++)
    {
        if((original[i] != 0x7e)&&(original[i]!=0x7d))
        {
            tmplen++;
        }
        else
        {
        //printf("j=%d  tmplen= %d, start=%d\n", j, tmplen, start);
            if(tmplen != 0)
            {
                memcpy(composed+j, original+start, tmplen);
            }
            start =start+tmplen+1;
            if(original[i] == 0x7e)
            {
                composed[j+tmplen] = 0x7d;
                composed[j+tmplen+1] = 0x02;
            }
            if(original[i] == 0x7d)
            {
                composed[j+tmplen] = 0x7d;
                composed[j+tmplen+1] = 0x01;
            }

            j=j+tmplen+2;
            tmplen =0;
        }
    }

    if(tmplen != 0)
    {
        memcpy(composed+j, original+start, tmplen);

    }
    j = j + tmplen;
    composed[j]=0x7e;
    j++;

    *comlen = j;
    return 0;
}

int HandleThread::addCheckCode(unsigned char * original, int len)
{
    unsigned char c= 0x00;
    for(int i = 0;i<len; i++)
    {
        c= c ^original[i];
    }
    original[len] = c;
    return len+1;
}


int HandleThread::getMsgFromBuf( int & state )
{
	static unsigned char acksn=0;
	RecvStream recvstream,rs;
	static int i=0;

	int r=0;


		if(recvbuf.size>0)
		{
			r=recvbuf.getDataFromBuf(recvstream.stream, &(recvstream.size));

		}
		//mutexRecvStream.unlock();
		if(r>0)
		{

			toOriginalMsg(recvstream.stream,recvstream.size, rs.stream,&rs.size);
			int j = checkCode(rs.stream,rs.size);
			if( j < 0)
			{
				//continue;
			}
			if(j ==1)
			{

				unsigned char c= rs.stream[2];
				if(i==0)
				{
					acksn=c;
					i=1;
				}

				if(i)
				{
					if(acksn==c) return r;
				 }

				 unsigned char  recvcmd = rs.stream[0];
				 if(recvcmd==currentCmd)
				 {
					unsigned char recvsn= rs.stream[4];
				 //unsigned char replyc= rs.stream[5];
					if((recvsn==sn))
					{
						li++;
						settime=0;
						if(li==g_action_sequence.action_list.end())
						{
						//½áÊø
							command=0;
							started =0;
							settime=0;
						}
						state=1;//rs.stream[6];
					}

				 }
				 acksn=c;

			}



		}


	return r;
	//this->stop();
}

int HandleThread::checkCode(unsigned char * original, int len)
{
	int i;
	unsigned char c;
	if(len<4)
	{
		return -2;
	}
	c=original[1];
	if((len-4)!= c)
	{
        return -1;
    }
    else
    {
        return 1;
    }
}

void HandleThread::run()
{
	struct timeval start,now;
	struct timeval s1,e1;
	fd_set fsWrite,fsRead;
	int iRet;

	int retry=0;
	TIMEVAL tv;
	tv.tv_sec=0;
	tv.tv_usec=10000;
	int state=0;

	Msg msg;
	unsigned char sendbuf[BUFFER_SIZE];
	//list<ActionRow> &a=g_action_sequence.action_list;
		for(;;)
		{
			if(command==0)
			{
				//just recv
			}
			if(command==1)
			{   if(!started)
				{

				//make action list and handle test
					li=g_action_sequence.action_list.begin();
					gettimeofday(&start, NULL);
					started = 1;
				}
			}
			if(command==2)
			{
			// handle action list
			//command=0;
			}
			if((started)&&(!settime))
			{

				s1=e1=start;
				int s=li->start_time;
				int e=li->end_time;
				s1.tv_sec=s1.tv_sec+s;
				e1.tv_sec=e1.tv_sec+e;
				int l=li->toStream(sendbuf);
				sendbuf[2]=getSn();
				DP3("get sn settime %d, sn %d, retry %d",settime,sn,retry);
				l = addCheckCode(sendbuf,l);

				toComposedMsg(sendbuf,l, msg.stream, &(msg.len));
				currentCmd=li->a.type;
				retry=0;
				state=0;
				settime=1;
			}
			if(started&&settime)
			{
				gettimeofday(&now, NULL);
				if(span(now,e1)>=0)
				{
				//to do set testinstance status to 2
				//....
					li++;
					settime=0;
					DP3("*****timeout settime %d, sn %d, retry %d",settime,sn,retry);
					if(li==g_action_sequence.action_list.end())
					{
						//½áÊø
						command=0;
						started =0;
						settime=0;
					}
				}
			}
			FD_ZERO(&fsWrite);
			FD_SET(g_tcp_client.clisockfd, &fsWrite);
			FD_ZERO(&fsRead);
			FD_SET(g_tcp_client.clisockfd, &fsRead);
			iRet = select(1, &fsRead, &fsWrite, NULL, &tv);
			if(0<iRet){
		//read or write socket
				if (FD_ISSET(g_tcp_client.clisockfd,&fsRead))
				{
					int rchars=g_tcp_client.Recv(recvbuf.stream+recvbuf.size,BUFFER_SIZE-recvbuf.size);
					recvbuf.size+=rchars;
					while( getMsgFromBuf(state)!=0)
					{
						 //if(state!=0) goto next;
					}
					//if(state!=0) continue;
				}
				if (FD_ISSET(g_tcp_client.clisockfd,&fsWrite))
				{
					gettimeofday(&now, NULL);
					if(command!=0 && started)
					{
						if(settime&&(span(now,start)>=0)&&retry<RETRY_TIME)
						{
						   g_tcp_client.Send(msg.stream,msg.len);
						   retry++;

							DP3("send settime %d, sn %d, retry %d",settime,sn,retry);
						}
					}
				}

			}
			if(0==iRet){
			//time out
			}
			if(0>iRet){
			}
			//next:
		}




}


//---------------------------------------------------------------------------
