// TDBEasyDemo.cpp : 定义控制台应用程序的入口点。
//
#include "TDBEasyDemo.h"

#include <stdio.h>
#include <string.h> 
#include <algorithm>
#include <assert.h>
#include <time.h>
#include <iostream>  
#include <fstream>  
#include <stdlib.h>  
#include <io.h>
#include <direct.h>
#include "Windows.h"
#include <string>  
#include <vector>

using namespace std;


#ifdef NDEBUG
#define AssertEx(expr) expr;
#else
#define AssertEx(expr) {int n = (int)(expr); assert(n);} 
#endif

/*
static int ErrNum = 0;
static char ErrCode[100][10] = {0};
*/

// 数据提取后输出目录
string dir_output;


// log文件
ofstream filelog; 

int main()
{
	//读取配置文件，配置下载股票列表以及下载日期列表
	cerr << "Info\t" << OutputLocalTime() << "Start downloading L2 raw data from Wind!\n" << endl; 
	int ret;

	// 打开log文件	
	ret = MakeLocalLogFile(filelog);
	if (ret)
	{
		getchar();
		return -1;
	}	
	
	//初始化设置账户信息,包括账号密码，wind远程服务器等
	OPEN_SETTINGS settings = {0};
    ret = LoadUserConfig(settings);
	if (ret)
	{
		getchar();
		filelog.close();		
		return -1;
	}		

	//调用TDB_Open，登录wind远程服务器, 如果一次不成功，会每隔60s重试一次
    THANDLE hTdb;
	ret = LogInWithUserConfig(hTdb, settings);
	if (ret)
	{
		getchar();
	 	if (hTdb)
			TDB_Close(hTdb);
		filelog.close();		
		return -1;
	}

	int nRet = TDB_SUCCESS;

	//获取两个市场的股票代码，从API上实时返回
	//GetCodeTable(hTdb, "SZ-2-0");
	//GetCodeTable(hTdb, "SH-2-0");
	//获取两个市场的股票代码，从文件中读取	
	/*TDBDefine_Code pCodetableSZ[MAX_STOCK_NUM];
	TDBDefine_Code pCodetableSH[MAX_STOCK_NUM];	
	int stkNumSZ=0,stkNumSH=0;
	int retSH = -1, retSZ = -1;*/
	//while(retSH < 0){
	//	retSH = GetCodeTableFromConfigFile("SH-2-0", pCodetableSH, stkNumSH);
	//	//Sleep(1000); //等待1s
	//}
	//while(retSZ < 0){
	//	retSZ = GetCodeTableFromConfigFile("SZ-2-0", pCodetableSZ, stkNumSZ);
	//	//Sleep(1000); //等待1s
	//}

	// 读取股票代码，日期以及配置文件（指定股票、日期下载）
	vector<string> stkList;
	vector<string>::size_type stkStart,stkEnd;	
	vector<string> dateList;
	string dateStart, dateEnd;
	//string dir_output;
    ret = LoadStkDateDirConfig(stkList, stkStart, stkEnd, dateList, dateStart, dateEnd, dir_output);	  	
	if (ret)
	{
		getchar();
	 	if (hTdb)
			nRet = TDB_Close(hTdb);
		filelog.close();			
		return -1;
	}
	
	// 进行循环遍历股票和日期，先完成一天的所有股票
	int todayDate, startDate, endDate;
	startDate = atoi(dateStart.c_str());
	endDate = atoi(dateEnd.c_str());
	for(vector<string>::size_type dayi = 0; dayi!=dateList.size(); ++dayi)
	{
		// 只处理配置文件设置的日期
		todayDate = atoi(dateList[dayi].c_str());
		if (todayDate < startDate || todayDate > endDate){			
			continue;
		}
			
		// 创建当天日期的文件夹，检查文件夹是否存在
		ret = ProcessTodayDir(dateList[dayi]);
		if (ret)
		{
			getchar();
		 	if (hTdb)
				nRet = TDB_Close(hTdb);
			filelog.close();				
			return -1;
		}

	
		// 开始遍历所有股票
		for(vector<string>::size_type stocki = stkStart; stocki < stkEnd; ++stocki)
		{
			string thisStk = stkList[stocki];			
			// 输出进度
			cerr << "Info\t" << OutputLocalTime() << "Downloading " << dateList[dayi] << " " << thisStk << endl; 
			filelog << "Info\t" << OutputLocalTime() << "Downloading " << dateList[dayi] << " " << thisStk << endl; 

			// 判断市场
			string thisStkMarket;
			if (thisStk.substr(7,2) == "SZ")		
				thisStkMarket = "SZ-2-0";		
			else
				thisStkMarket = "SH-2-0";			
	
			// 开始下载数据，sh不用下载逐笔委托				
			// 开始下载该股票当天的数据
			int nRetTickData 	= 0;
			int nRetTransaction = 0;
			int nRetOrderQueue 	= 0;
			int nRetOrder 		= 0;
			bool flagSuccess 	= false;	// 标志是否成功取到数据
			int  flagRetry 		= 1;
			while (!flagSuccess){
				// 下载数据，如果遇到一些特殊error标志，则重新下载
				nRetTickData = GetTickData(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate); 
				flagRetry = ResponseToTDBReturn(nRetTickData, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
									
				nRetTransaction = GetTransaction(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate); 		
				flagRetry = ResponseToTDBReturn(nRetTransaction, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
				
				nRetOrderQueue = GetOrderQueue(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
				flagRetry = ResponseToTDBReturn(nRetOrderQueue, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
				
				// shanghai没有order
				if ("SZ-2-0" == thisStkMarket){		
					nRetOrder = GetOrder(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
					flagRetry = ResponseToTDBReturn(nRetOrder, hTdb, settings);
					if (flagRetry)		// 如果flagRetry非0，返回while重新下载
						{flagSuccess = false;continue;}
				}

				// 全部数据下载完以后，将flag置1，跳出while循环
				flagSuccess = true;
			}
			cerr << "Info\t" << OutputLocalTime() << "Complete " << dateList[dayi] << " " << thisStk << endl; 
			filelog << "Info\t" << OutputLocalTime() << "Complete " << dateList[dayi] << " " << thisStk << endl; 
		}
	}
	
	cerr << "------------程序自然退出-----------------\n" << OutputLocalTime() << endl; 
	filelog << "------------程序自然退出-----------------\n" << OutputLocalTime() << endl; 
	cerr << "输入任意键结束程序" << endl; 
	getchar();
 	if (hTdb)
		// 测试后发现，不论是否TDB_Close关闭成功，都返回0；
		nRet = TDB_Close(hTdb);
	filelog.close();
	return 0;
	{
		//GetKData(hTdb, "600163.sh", "sh-2-0", 20150910, 20150915, CYC_MINUTE, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_MINUTE, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_DAY, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_SECOND, 0, 0, 1);
		//GetTickData(hTdb, "000001.sz", "SZ-2-0", 20160721);//带买卖盘的tick
		//GetTickData(hTdb, "601336.sh", "SH-2-0", 20160721);//带买卖盘的tick
		//GetTransaction(hTdb, "000001.sz", "SZ-2-0", 20160721);
		//GetTransaction(hTdb, "600508.sh", "SH-2-0", 20160721);
		//GetOrder(hTdb, "000001.sz", "SZ-2-0", 20160721);//逐笔委托
		//GetOrder(hTdb, "600508.sh", "SH-2-0", 20160721);//SH没有逐笔委托，返回warning
		//GetOrderQueue(hTdb, "000001.sz", "SZ-2-0", 20160513);//委托队列
		//GetOrderQueue(hTdb, "600311.sh", "SH-2-0", 20160513);//委托队列

		//GetTickData(hTdb, "601336.sh", "SH-2-0", 20160721);//带买卖盘的tick
		//GetTransaction(hTdb, "601336.sh", "SH-2-0", 20160721);
		//GetOrderQueue(hTdb, "601336.sh", "SH-2-0", 20160721);//委托队列
		//UseEZFFormula(hTdb);
	}
	
}

int LoadStkDateDirConfig(vector<string>& stkList, vector<string>::size_type& stkStart, vector<string>::size_type& stkEnd, vector<string>& dateList, string& dateStart, string& dateEnd, string& dir_output)
{
	ifstream downloadfile;  
	std::string strLine;
	std::string pathStkList, pathDateList;		
	
	// 打开download config文件，读取文件path和股票、日期下载范围
	//std::string pathConfigFile("E:\\wind_L2_config\\downloadConfig.txt");
	std::string pathConfigFile("downloadConfig.txt");	
	downloadfile.open(pathConfigFile.c_str(), ios::in);	
	if (downloadfile.is_open()){
	    std::getline(downloadfile, pathStkList);
	    std::getline(downloadfile, strLine);
		stkStart = atoi(strLine.c_str());
	    std::getline(downloadfile, strLine);
		stkEnd = atoi(strLine.c_str());
	    //downloadfile >> stkStart;
	    //downloadfile >> stkEnd;

	    std::getline(downloadfile, pathDateList);
	    std::getline(downloadfile, dateStart);
	    std::getline(downloadfile, dateEnd);
	    std::getline(downloadfile, dir_output);

		downloadfile.close();
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << pathConfigFile << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << pathConfigFile << endl; 
		return -1;
	}	

	// 读取股票列表
	downloadfile.open(pathStkList.c_str(), ios::in);
	if (downloadfile.is_open()){
		while (getline(downloadfile,strLine)){
			stkList.push_back(strLine);
		}
		downloadfile.close();
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 
		//getchar();		
		return -1; 
	}	

	// 读取日期列表
	downloadfile.open(pathDateList.c_str(), ios::in);
	if (downloadfile.is_open()){
		while (getline(downloadfile,strLine)){
			dateList.push_back(strLine);
		}
		downloadfile.close();
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 	
		return -1;
	}

	return 0;

}

int LoadUserConfig(OPEN_SETTINGS& settings)
{
	
	ifstream accountfile;  
	//accountfile.open("E:\\wind_L2_config\\acount.txt", ios::in);
	accountfile.open("account.txt", ios::in);

	if (accountfile.is_open()){
		std::string strLine;
	    //printf("Input IP:");
	    std::getline(accountfile, strLine);
	    strncpy(settings.szIP, strLine.c_str(), sizeof(settings.szIP)-1);
	    settings.szIP[ELEMENT_COUNT(settings.szIP)-1] = 0;

	    //printf("Input Port:");
	    std::getline(accountfile, strLine);
	    strncpy(settings.szPort, strLine.c_str(), sizeof(settings.szPort)-1);
	    settings.szPort[ELEMENT_COUNT(settings.szPort)-1] = 0;

	    //printf("Input User:");
	    std::getline(accountfile, strLine);
	    strncpy(settings.szUser, strLine.c_str(), sizeof(settings.szUser)-1);
	    settings.szUser[ELEMENT_COUNT(settings.szUser)-1] = 0;

	    //printf("Input Password:");
	    std::getline(accountfile, strLine);
	    strncpy(settings.szPassword, strLine.c_str(), sizeof(settings.szPassword)-1);
	    settings.szPassword[ELEMENT_COUNT(settings.szPassword)-1] = 0;

	    // settings.nTimeOutVal = 30;   //获取数据时，指定网络超时（秒数，为0则设为2分钟），若超过nTimeOutVal秒后未收到回应数据包，则内部会关闭连接
	    std::getline(accountfile, strLine);
	    settings.nTimeOutVal = atoi(strLine.c_str());

		// settings.nRetryCount = 4;   //获取数据时，若掉线，指定重连次数（为0则不重连），若重连nRetryCount次之后仍掉线，则返回网络错误
	    std::getline(accountfile, strLine);
	    settings.nRetryCount = atoi(strLine.c_str());

	    // settings.nRetryGap = 0;     //掉线之后重连的时间间隔（秒数，为0则设为1秒）	
	    std::getline(accountfile, strLine);
	    settings.nRetryGap = atoi(strLine.c_str()); 

		accountfile.close();

		cerr << "Info\t" << OutputLocalTime() << "Connecting IP: " << settings.szIP << endl; 
		cerr << "Info\t" << OutputLocalTime() << "Connecting Port: " << settings.szPort << endl; 
		filelog << "Info\t" << OutputLocalTime() << "Connecting IP: " << settings.szIP << endl; 
		filelog << "Info\t" << OutputLocalTime() << "Connecting Port: " << settings.szPort << endl; 
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << "account.txt" << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << "account.txt" << endl; 
		getchar();
		return -1; 
	}

	return 0;
		
}


int LogInWithUserConfig(THANDLE& hTdb, OPEN_SETTINGS& settings)
{
    TDBDefine_ResLogin loginAnswer={0};	
	hTdb = TDB_Open(&settings, &loginAnswer);  
	//htdb的值和loginRes里的字段，登录失败返回为空，其他就是有值的，loginRes里的info字段可以得出错误原因	
	while (!hTdb)
    {
        cerr << "Warning\t" << OutputLocalTime() << "TDB_Open failed:" << loginAnswer.szInfo << " , retry after 30s...!\n" << endl; 
        filelog << "Warning\t" << OutputLocalTime() << "TDB_Open failed:" << loginAnswer.szInfo << " , retry after 30s...!\n" << endl; 
		Sleep(1000*30);
		hTdb = TDB_Open(&settings, &loginAnswer);  
    }	
	cerr << "Info\t" << OutputLocalTime() << "TDB_Open Succeed!" << endl; 	
	filelog << "Info\t" << OutputLocalTime() << "TDB_Open Succeed!" << endl; 	
	return 0;		
}

int MakeLocalLogFile(ofstream& filelog)
{

	// 确认log输出目录是否存在
	string logdir = "locallog\\";
	if(access(logdir.c_str(),0)==0){
		//Exists, do nothing
	}
	else{
		//Create the folder
		if(mkdir(logdir.c_str())==0){
			cerr<< "Info\t" << OutputLocalTime() << "Log folder created." << logdir <<endl;
		}
		else{
			cerr<< "Error\t" << OutputLocalTime() << "Error when creating date folder."<< logdir <<endl; 
			return -1;
		}
	}


	time_t tmptime; 
	tmptime = time(NULL);
	char tmpdate[32];
	string tmpstr; 
	strftime(tmpdate,sizeof(tmpdate),"%Y%m%d_%H%M%S",localtime(&tmptime));
	tmpstr = logdir + "WindL2Download_";
	tmpstr.append(tmpdate);
	tmpstr.append(".log");
	if(access(tmpstr.c_str(),0)==0){
		//The file exists, open it.
		cerr<<"--->>> The date log file exists.Open it."<<endl;
		//to write, append, and set the pointer at the end of the file.
		filelog.open(tmpstr.c_str(),ios::out|ios::app|ios::ate);

	}
	else{
		//Create the file.	
		cerr<<"-->>> The date log file does not exist. Create it."<<endl;
		filelog.open(tmpstr.c_str(),ios::out);
	}

	filelog << "Log file created at: " << OutputLocalTime() << endl;
	return 0;

}




int ProcessTodayDir(string today_str)
{
	// 确认主输出目录是否存在
	if(access(dir_output.c_str(),0)==0){
		//Exists, do nothing
	}
	else{
		//Create the folder
		if(mkdir(dir_output.c_str())==0){
			cerr<< "Info\t" << OutputLocalTime() << "Date folder created." << dir_output <<endl;
			filelog<< "Info\t" << OutputLocalTime() << "Date folder created." << dir_output <<endl;
		}
		else{
			cerr<< "Error\t" << OutputLocalTime() << "Error when creating date folder."<< dir_output <<endl; 
			filelog<< "Error\t" << OutputLocalTime() << "Error when creating date folder."<< dir_output <<endl;	
			//getchar();
			return -1;
		}
	}

	// 确认当天子目录是否存在
	string dir_today = dir_output;
	dir_today += today_str;
	dir_today += "\\";		
	if(access(dir_today.c_str(),0)==0){
		//Exists, do nothing
	}
	else{
		//Create the folder
		if(mkdir(dir_today.c_str())==0){
			cerr<< "Info\t" << OutputLocalTime() << "Date folder created." << dir_today <<endl;
			filelog<< "Info\t" << OutputLocalTime() << "Date folder created." << dir_today <<endl;
		}
		else{
			cerr<< "Error\t" << OutputLocalTime() << "Error when creating date folder."<< dir_today <<endl; 
			filelog<< "Error\t" << OutputLocalTime() << "Error when creating date folder."<< dir_today <<endl;	
			//getchar();
			return -1;
		}
	}

	return 0;
}

/* --------------------------------------------------------------------------------- 
函数功能:对TDB返回值进行处理.
		TDB_NETWORK_ERROR\TDB_NETWORK_TIMEOUT\TDB_LOGIN_FAILED，30s后重新登录，再重新下载该股票当天所有数据；
		TDB_OUT_OF_MEMORY，30s后重新下载该股票当天所有数据；
		TDB_NO_DATA，输出警告，不重新尝试下载；
--------------------------------------------------------------------------------- */ 
int ResponseToTDBReturn(int& nRet, THANDLE& hTdb, OPEN_SETTINGS& settings)
{
	switch(nRet)
	{ 
		case TDB_NETWORK_ERROR:		 
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Network error! Retry Login..." << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Network error! Retry Login..." << endl;
			Sleep(1000*30);			
			LogInWithUserConfig(hTdb, settings);		// 重新登录			
			return nRet;
			break;
		case TDB_NETWORK_TIMEOUT:	
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Network timeout! Retry Login..." << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Network timeout! Retry Login..." << endl;
			Sleep(1000*30);		
			LogInWithUserConfig(hTdb, settings);		// 重新登录
			return nRet;			
			break;
		case TDB_LOGIN_FAILED:
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Login failed! Retry Login..." << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Login failed! Retry Login..." << endl;	
			Sleep(1000*30);					
			LogInWithUserConfig(hTdb, settings);		// 重新登录
			return nRet;
			break;			
		case TDB_OUT_OF_MEMORY:
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Out of memory! Waiting 30s... " << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Out of memory! Waiting 30s... " << endl; 
			Sleep(1000*30);
			return nRet;
			break;
		case TDB_NO_DATA:	
			cerr << "Warning\t" << OutputLocalTime() << "Warning: No data return!" << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: No data return!" << endl;			
			return 0;
			break; 
		case TDB_SUCCESS:	
			return 0;
			break;   			 			
		default:
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Other error code!" << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Other error code!" << endl;				
			return 0;
			break;
	}

}

