//---------------------------------------------------------------------------
#ifndef LogFileH
#define LogFileH
//---------------------------------------------------------------------------

//可按日期生成多个日志， 还可分年月日频率生成文件名

//这个代码我用工业现场24X7值守的程序纪录各种信息， 简单易用;
//一般用一个全局日志对象， 有临界排斥可以多线程安全使用。
//有两个类
//class LogFile;//用户定义日志文件名
//class LogFileEx;//有日志文件名自动生成功能 , 可分年月日频率生成文件名, 可指定日志存放的目录
//LogFile gLog("My.Log");
//gLog.Log("test", 4);//记录日志
//gLog.Log("系统启动");
//LogFileEx gLog(".", LogFileEx :: YEAR);//一年生成一个日志文件
//LogFileEx gLog(".//Log", LogFileEx :: MONTH);//一月生成一个日志文件
//LogFileEx gLog(".//Log", LogFileEx :: DAY);//一天生成一个日志文件
//注意日志所属目录创建失败会自动退出， 请注意目录的合法性， 文件生成频率看情况掌握
//24小时运行的程序可以每天生成一个日志文件， 以免内容过多


#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#include <vcl.h>

class LogFile
{
public:
	LogFile(String szFileName = "Log.log"); //设定日志文件名
	virtual ~LogFile();

	const String GetFileName() {
		return _szFileName;
	}

	void SetFileName(const String szName); //修改文件名， 同时关闭上一个日志文件

	bool IsOpen() {
		return _hFile != INVALID_HANDLE_VALUE;
	}

	void Close();

	void Log(const UTF8String szText){
		Log(szText.c_str(), szText.Length() );
	}

protected:
	CRITICAL_SECTION _csLock;
	String _szFileName;
	HANDLE _hFile;

	bool OpenFile(); //打开文件， 指针到文件尾
	DWORD Write(LPCVOID lpBuffer, DWORD dwLength);

	void Log(LPCVOID lpBuffer, DWORD dwLength); //追加日志内容

	virtual void WriteLog( LPCVOID lpBuffer, DWORD dwLength); //写日志, 可以扩展修改
	void Lock()  {
		::EnterCriticalSection(&_csLock);
	}

	void Unlock() {
		::LeaveCriticalSection(&_csLock);
	}

private://屏蔽函数
	LogFile(const LogFile&);
	LogFile&operator = (const LogFile&);
};

/*
class LogFileEx : public LogFile
{
public:
	enum LOG_TYPE{YEAR = 0, MONTH = 1, DAY = 2};
	LogFileEx(const char *szPath = ".", LOG_TYPE iType = MONTH);
	~LogFileEx();
	const char * GetPath(){ return _szPath; }
	void Log(LPCVOID lpBuffer, DWORD dwLength);
	void Log(const char *szText){ Log(szText, strlen(szText)); }

protected:
	char _szPath[MAX_PATH + 1];
	char _szLastDate[20];
	int _iType;
	void SetPath(const char *szPath);

private://屏蔽函数
	LogFileEx(const LogFileEx&);
	LogFileEx&operator = (const LogFileEx&);
};
*/


#endif


