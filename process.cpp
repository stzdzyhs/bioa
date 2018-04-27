//---------------------------------------------------------------------------


#pragma hdrstop

#include "process.h"

//---------------------------------------------------------------------------
#define WASH_TYPE 1
#pragma package(smart_init)

TestArray g_test_array;
ReagentArray g_reagent_array;
TestRowArray g_test_row_array;
ActionSequence g_action_sequence;
CupInfoArray g_cup_array;

void testinitial()
{   {
	TestConfig tc={0, 0,0, 30,1 ,0,40,2,0,50,3,0,60, 1,2,3,4,0, 1, 349,560};
	g_test_array.push(tc);
	}
	{
	TestConfig tc= {1, 1,0, 30,2 ,0,40,3,0,50,-1,0,60, 1,2,3,4,1, 2, 349,560};
	g_test_array.push(tc);
	}
	{
	TestConfig tc= {2, 3,0, 30,1 ,0,40,-1,0,50,-1,0,60,1,2,3,4, 2, 2, 349,560};
	g_test_array.push(tc);
	}

	{  Reagent_Info ri={0,0,1,3000}; g_reagent_array.push(ri);}
	{  Reagent_Info ri={0,0,3,3000}; g_reagent_array.push(ri);}
	{  Reagent_Info ri={1,0,2,3000}; g_reagent_array.push(ri);}
	{  Reagent_Info ri={2,1,1,3000}; g_reagent_array.push(ri);}
	{  Reagent_Info ri={3,1,2,3000}; g_reagent_array.push(ri);}

	{ TestRow tr={0, 0,1, 0,0,1,0,0, 1, 0, 1,20,0}; g_test_row_array.push(tr);}
	{ TestRow tr={1, 0,1, 0,0,1,0,0, 1, 1, 2,20,0}; g_test_row_array.push(tr);}
	{ TestRow tr={2, 0,1, 0,0,1,0,0, 1, 1, 2,20,0}; g_test_row_array.push(tr);}
	{ TestRow tr={3, 0,1, 0,0,1,0,0, 1, 0, 3,20,0}; g_test_row_array.push(tr);}
	{ TestRow tr={4, 0,1, 0,0,1,0,0, 1, 0, 1,20,1}; g_test_row_array.push(tr);}
	{ TestRow tr={5, 0,1, 0,0,1,0,0, 1, 1, 2,20,1}; g_test_row_array.push(tr);}
	{ TestRow tr={6, 0,1, 0,0,1,0,0, 1, 1, 2,20,1}; g_test_row_array.push(tr);}
	{ TestRow tr={7, 0,1, 0,0,1,0,0, 1, 0, 4,20,1}; g_test_row_array.push(tr);}
	{ TestRow tr={8, 0,1, 0,0,1,0,0, 1, 0, 4,20,2}; g_test_row_array.push(tr);}
	{ TestRow tr={9, 0,1, 0,0,1,0,0, 1, 1, 1,20,2}; g_test_row_array.push(tr);}
	{ TestRow tr={10, 0,1, 0,0,1,0,0, 1, 1, 1,20,2}; g_test_row_array.push(tr);}
	{ TestRow tr={11, 0,1, 0,0,1,0,0, 1, 0, 3,20,2}; g_test_row_array.push(tr);}

}
/*****************************************************************************
按照项目优先级--测试类型——测试优先级来排序



***************************************************************************/
bool comp(const TestRow &a,const TestRow &b)
{
	if (g_test_array.test_config_array[a.test_id].priority != g_test_array.test_config_array[b.test_id].priority)
		return  g_test_array.test_config_array[a.test_id].priority< g_test_array.test_config_array[b.test_id].priority;
	if (a.test_type!=b.test_type)
		return  a.test_type<b.test_type;
	if (a.priority!=b.priority)
		return  a.priority<b.priority  ;

	return false;
}


void ActionSequence::clearAll()
{
	this->action_list.clear();
}
void ActionSequence::insertTestRow(int i)
{
	vector<ActionRow> arv;
   //	if(tr.pre_dilute)
   int lasttime=0;
   ActionRow ar;
   int j=g_cup_array.getFreecup();
   if(j==-1) // no free cup
   {
	   ar.a.type=4;
	   ar.start_time=0;
	   ar.end_time= T4;
	   lasttime=T4;
	   arv.push_back(ar);
	   this->from_time=getendtime();
	   //insert last row  to clear all cups
	   g_cup_array.clearall() ;
   }

   TestRow tr=g_test_row_array.test_array[i];
	j=g_cup_array.getFreecup();
   //加样品 Action
   ar.a.type=0;
   ar.a.params.get_sample.sample_pos= tr.position;

   ar.a.params.get_sample.cup_pos=j;
   g_cup_array.cv[j].is_in_use=1;
   ar.a.params.get_sample.sample_volume=tr.sample_volume;
   ar.a.params.get_sample.sample_type=tr.sample_type;
   ar.a.params.get_sample.wash_type=WASH_TYPE;
   ar.start_time=lasttime;
   ar.end_time=lasttime+T0;
   ar.step=0;
   ar.ptestrow=i;
   arv.push_back(ar);
   lasttime=T0;
   if(tr.pre_dilute)
   {   //预稀释
		ar.a.type=1;
		ar.a.params.get_reagent.reagent_id=tr.dilute_reagent;
		ar.a.params.get_reagent.cup_pos =j;
		ar.a.params.get_reagent.sample_volume=tr.sample_volume*tr.dilute_info;
		ar.a.params.get_reagent.sample_type=tr.dilute_type;
		ar.a.params.get_reagent.wash_type=WASH_TYPE;
		ar.start_time=lasttime;
		ar.end_time=lasttime+T1;
		ar.step=1;
		ar.ptestrow=i;
		lasttime=ar.end_time;
		arv.push_back(ar);
   }
   TestConfig tc=g_test_array.test_config_array[tr.test_id];
   if(tc.reagent1!=-1)
   {   //加a
		ar.a.type=1;

		ar.a.params.get_reagent.reagent_id=g_test_array.test_config_array[tr.test_id].reagent1;
		ar.a.params.get_reagent.cup_pos =j;
		ar.a.params.get_reagent.sample_volume=g_test_array.test_config_array[tr.test_id].v1;
		ar.a.params.get_reagent.sample_type=g_test_array.test_config_array[tr.test_id].sample_type1;
		ar.a.params.get_reagent.wash_type=WASH_TYPE;
		ar.start_time=lasttime;
		ar.end_time=lasttime+T1;
		ar.step=2;
		ar.ptestrow=i;
		lasttime=ar.end_time;
		arv.push_back(ar);

   }
   if(tc.reagent2!=-1)
   {   //加b
			ar.a.type=1;

		ar.a.params.get_reagent.reagent_id=g_test_array.test_config_array[tr.test_id].reagent2;
		ar.a.params.get_reagent.cup_pos =j;
		ar.a.params.get_reagent.sample_volume=g_test_array.test_config_array[tr.test_id].v2;
		ar.a.params.get_reagent.sample_type=g_test_array.test_config_array[tr.test_id].sample_type2;
		ar.a.params.get_reagent.wash_type=WASH_TYPE;
		ar.start_time=lasttime+g_test_array.test_config_array[tr.test_id].time1;
		ar.end_time=ar.start_time+T1;
		ar.step=3;
		ar.ptestrow=i;
		lasttime=ar.end_time;
		arv.push_back(ar);
   }
   if(tc.reagent3!=-1)
   {   //c
			ar.a.type=1;

		ar.a.params.get_reagent.reagent_id=g_test_array.test_config_array[tr.test_id].reagent3;
		ar.a.params.get_reagent.cup_pos =j;
		ar.a.params.get_reagent.sample_volume=g_test_array.test_config_array[tr.test_id].v3;
		ar.a.params.get_reagent.sample_type=g_test_array.test_config_array[tr.test_id].sample_type3;
		ar.a.params.get_reagent.wash_type=WASH_TYPE;
		ar.start_time=lasttime+g_test_array.test_config_array[tr.test_id].time2;
		ar.end_time=ar.start_time+T1;
		ar.step=4;
		ar.ptestrow=i;
		lasttime=ar.end_time;
		arv.push_back(ar);
   }
   if(tc.reagent4!=-1)
   {   //加d
		ar.a.type=1;

		ar.a.params.get_reagent.reagent_id=g_test_array.test_config_array[tr.test_id].reagent3;
		ar.a.params.get_reagent.cup_pos =j;
		ar.a.params.get_reagent.sample_volume=g_test_array.test_config_array[tr.test_id].v3;
		ar.a.params.get_reagent.sample_type=g_test_array.test_config_array[tr.test_id].sample_type3;
		ar.a.params.get_reagent.wash_type=WASH_TYPE;
		ar.start_time=lasttime+g_test_array.test_config_array[tr.test_id].time3;
		ar.end_time=ar.start_time+T1;
		ar.step=5;
		ar.ptestrow=i;
		lasttime=ar.end_time;
		arv.push_back(ar);

   }
   //读取结果
   ar.a.type=2;
   ar.a.params.get_result.cup_pos=j;
   ar.a.params.get_result.wavenum= g_test_array.test_config_array[tr.test_id].wavenum;
   ar.a.params.get_result.wavelength0= g_test_array.test_config_array[tr.test_id].wl0;
   ar.a.params.get_result.wavelength1= g_test_array.test_config_array[tr.test_id].wl1;
   ar.start_time=lasttime+g_test_array.test_config_array[tr.test_id].time4;
   ar.end_time=ar.start_time+T2;
   ar.step=6;
   ar.ptestrow=i;
   lasttime=ar.end_time;
   arv.push_back(ar);

	insertOneRow(arv);
}
void ActionSequence::makelist() // 通过调用insertTestRow 生成动作序列
{
	from_time=0;
	endtime=0;
	for(unsigned int i=0;i<g_test_row_array.test_array.size();i++)
	{
		this->insertTestRow(i);
    }
}

int ActionSequence::getendtime()
{
		list<ActionRow>::iterator iter;
		if (this->action_list.size()==0) {
			return 0;

		}
		iter=action_list.end();
		--iter;
		return iter->end_time;
}

void ActionSequence::insertOneRow( vector<ActionRow> & av)
{
	vector<ActionRow>::iterator ai;
	int time=get_available_time(av);
	for(ai=av.begin();ai!=av.end();ai++)
	{
		ai->start_time=ai->start_time+time;
		ai->end_time=ai->end_time+time;
		this->action_list.push_back(*ai);
	}
	action_list.sort();
	endtime= getendtime();
}
int ActionSequence::get_available_time(vector<ActionRow> & av)
{
	int start,i;
	if(from_time>=endtime) return from_time;
	for(i=from_time;i<endtime;i=get_next_available(i))
	{
	  vector<ActionRow>::iterator ai;
	  list<ActionRow>::iterator lai;
	  bool b=true;
	  for(ai=av.begin();ai!=av.end();ai++)
	  {
			int s=ai->start_time+i;
			int e=ai->end_time+i;
			for(lai=action_list.begin();lai!=action_list.end();lai++)
			{
				 int s1=lai->start_time;
				 int e1=lai->end_time;
				 if((s>=s1 && s<e1)||(e>s1&&e<=e1)||(s<=s1&& e>e1))
				 {
					 b=false;
					 goto con;
				 }
			}

	  }
	  if(b){start=i; break;}
	  con:
	}
	if(i==endtime) start=endtime;
    return start;



}

int ActionSequence::get_next_available(int i)
{
	  list<ActionRow>::iterator lai;
	  int j;

	  for(j=i+1;j<endtime;j++)
	  {
			bool b=true;
			for(lai=action_list.begin();lai!=action_list.end();lai++)
			{

				int s1=lai->start_time;
				int e1=lai->end_time;
				if ((j>=s1)&&(j<e1)) {
				   b=false;
				   j=e1-1;
				   break;
				}
			}
			if(b) return j;
	  }
	  return endtime;
}
#define WORD_COPY(d,i,v) {d[i]=(0xff00&v)>>8; d[i+1]=(0x00ff&v);}
#define DWORD_COPY(d,i,v) {d[i]=(0xff000000&v)>>24; d[i+1]=(0x00ff0000&v)>>16;d[i+2]=(0x0000ff00&v)>>8; d[i+3]=v&0xff;}
int ActionRow::toStream(unsigned char * dest)
{
	int i=3,r=0;
	int v;
	dest[0]=a.type;
	switch(a.type){
	case 0:
		dest[1]=6;
		dest[i+0]=a.params.get_sample.sample_pos;
		dest[i+1]=a.params.get_sample.cup_pos;
		v = a.params.get_sample.sample_volume;
		WORD_COPY(dest,i+2,v);
		dest[i+4]=a.params.get_sample.sample_type;
		dest[i+5]=a.params.get_sample.wash_type;
		r= 9;
		break;
	case 1:
		dest[1]=7;
		dest[i+0] = a.params.get_reagent.r_pos.circle;
		dest[i+1] = a.params.get_reagent.r_pos.pos;
		dest[i+2] = a.params.get_reagent.cup_pos;
		v=a.params.get_reagent.sample_volume;
		WORD_COPY(dest,i+3,v);
		dest[i+5]=a.params.get_reagent.sample_type;
		dest[i+6]=a.params.get_reagent.wash_type;
		r = 10;
		break;
	case 2:
		dest[1]=6;
		dest[i+0]=a.params.get_result.cup_pos;
		dest[i+1]=a.params.get_result.wavenum;
		//a.params.get_result.wavelength0;
		WORD_COPY(dest,i+2,a.params.get_result.wavelength0);
		WORD_COPY(dest,i+4,a.params.get_result.wavelength1);
		r = 9;
		break;
	default:
		dest[1]=0;
		r= 3;
		break;


	}
	return r;
}
