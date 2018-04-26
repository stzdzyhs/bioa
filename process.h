//---------------------------------------------------------------------------

#ifndef processH
#define processH
#include <vcl.h>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
#define TEST_NUMBER 512
#define REAGENT_NUMBER 512
#define CUP_NUMBER  44
#define T0 10   //取样到反应杯最长时间，必须要在10s 内完成，包括通讯在内
#define T1  10   // 取试剂到反应杯 包括搅拌 清洗等
#define  T2 10    //读取结果
#define  T4 100 //清洗所有杯子
//---------------------------------------------------------------------------

//测试项目步骤
struct TestConfig{
	int test_id;

	int reagent1;
	int sample_type1;
	int time1;
	int reagent2;
	int sample_type2;
	int time2;
	int reagent3;  //if -1 no reagent
	int sample_type3;
	int time3;
	int reagent4;
	int sample_type4;
	int time4;
	int v1,v2,v3,v4;
	int priority;    //0 is highest

	int wavenum;//读取波长数目
	int wl0;//主波长
	int wl1;//副波长


};



struct TestRow{
	int sample_no;
	unsigned short position;
	int sample_type;     //过量采样 1，正常采样 0
	char pre_dilute; // 预稀释
	int  dilute_info;
	int dilute_type;//是否过量采样稀释
	int dilute_reagent;
	int sample_id;
	int sample_cup_type;
	int priority; //smaller is highest  常规 1 加急 0
	int test_type;  // 空白1 定标 2，质控 3  常规 04
	//TDateTime reg_time;
	int sample_volume;
	int  test_id;
    int test_no;  //确定测试id

};
struct Reagent_pos{

	unsigned short circle; //内圈0，外圈0
	unsigned short pos;
};

struct Reagent_Info{
	int reagent_id;
	Reagent_pos r_pos;
	double  left_volume;

};
typedef vector<Reagent_Info>  ReagentInfoArray;

class TestArray{
public:
	TestConfig test_config_array[TEST_NUMBER];
	void push(TestConfig tc){
		test_config_array[tc.test_id]=tc;
	}
};

class ReagentArray{
public:
	ReagentInfoArray  reagent_info_array[REAGENT_NUMBER];
	void push(Reagent_Info ri){
		reagent_info_array[ri.reagent_id].push_back(ri);
	}
};

struct CupInfo{
	int cup_pos;
	char is_in_use;
	char status;
};

class CupInfoArray{
public:
	CupInfo cv[CUP_NUMBER];
	void clear(int i){
		cv[i].is_in_use=0;
		cv[i].status=0;
	}
	void clearall(){
		for(int i=0; i<CUP_NUMBER;i++)
			clear(i);
	}
	int getFreecup()
	{
		for(int i=0; i<CUP_NUMBER;i++)
			if(!cv[i].is_in_use) return i;
		// full return -1
		return -1;
    }
};


//登记的所有项目质控，定标 常规，都先放在数据库里面，然后点击开始生成
bool comp(const TestRow &a,const TestRow &b);
class TestRowArray{
public:
	vector<TestRow> test_array;

	void rearrange(){ // 按照规则排序 ，先按项目优先级，然后按照测试优先级 和test_type
		sort(test_array.begin(),test_array.end(),comp);
	}
	void push(TestRow tr){
		test_array.push_back(tr);
	}


};

struct Action{
	unsigned short type;// 0  加样品到反应被 1 加试剂到反应杯  2 读取结果
	//0
	union{
		//0 加样品到反应杯
		struct {
			unsigned short sample_pos;
			unsigned short cup_pos;
			int sample_volume;
			char sample_type;
			char wash_type;
		}get_sample;
		// 1 加试剂到反应杯
		struct {
			int reagent_id;
			Reagent_pos r_pos;
			unsigned short cup_pos;
			int sample_volume;
			char sample_type;
			char wash_type;
		}get_reagent;
		//2 读取结果
		struct{
			unsigned short cup_pos;
			int wavenum;
			int  wavelength0;
			int  wavelength1;
		}get_result;
		 //3 清洗反应杯
		struct{
			unsigned short cup_pos;
		}wash_cup;
		//4  清洗所有反应杯
		//5 复位
		//6 灌注
		//7反应杯空白
		//8 通信检查
		//9反应杯注水
		//10加样针清洗
		//11程序检查
		//12 试剂余量检测
		struct {
			int  startpos;
			int  endpos;
		}reagent_check;

	}params;



};

struct ActionRow{
	Action a;
	int step;
	int ptestrow;
	int start_time;
	int end_time;
	int span_to_next; //seconds
	int span_to_prev;
	operator <(const ActionRow &a){ return start_time<a.start_time;}
	int  toStream(unsigned char * dest);
};

//sorted TestRowArray -->  action sequence when start
class ActionSequence{
public:
	list<ActionRow> action_list;
	int starttime;
	int endtime;
	int from_time;
	void clearAll();
	void insertTestRow(int i);
	void insertOneRow( vector<ActionRow> & av);
	void makelist(); // 通过调用insertTestRow 生成动作序列
	int getendtime();
	int get_next_available(int i);
	int get_available_time(vector<ActionRow> & av);//获取插入的时间
};


void testinitial();
extern TestArray g_test_array;
extern ReagentArray g_reagent_array;
extern TestRowArray g_test_row_array;
extern ActionSequence g_action_sequence;

#endif
