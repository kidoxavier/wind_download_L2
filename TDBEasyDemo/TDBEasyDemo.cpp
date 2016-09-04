// TDBEasyDemo.cpp : 定义控制台应用程序的入口点。
//
# include "TDBEasyDemo.h"

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

	// 打开log文件
	MakeLocalLogFile(filelog);
	
	//初始化设置账户信息,包括账号密码，wind远程服务器等
	OPEN_SETTINGS settings = {0};
    LoadUserConfig(settings);

	//调用TDB_Open，登录wind远程服务器, 如果一次不成功，会每隔60s重试一次
    THANDLE hTdb;
	LogInWithUserConfig(hTdb, settings);

	int nRet = TDB_SUCCESS;

	// 读取股票代码，日期以及配置文件（指定股票、日期下载）
	vector<string> downloadList;
	vector<string>::size_type listStart,listEnd;	
	int ret;
    //ret = LoadStkDateDirConfig(stkList, stkStart, stkEnd, dateList, dateStart, dateEnd, dir_output);	  	
	ret = LoadDownloadList(downloadList, listStart, listEnd, dir_output);

	// 进行循环遍历download list
	for(vector<string>::size_type listi = 0; listi != downloadList.size(); ++listi)
	{
        if (listi < listStart || listi > listEnd)         
            continue;
 	
		// 
		string thisFile = downloadList[listi];
		// 20140729_SZ002003_orderqueue.csv
		string thisDate = thisFile.substr(0,8);
		int todayDate = atoi(thisDate.c_str());
		string thisStk = thisFile.substr(11,6) + "." + thisFile.substr(9,2);
		string::size_type pos_dot = thisFile.find(".");
		string thisFileType = thisFile.substr(18,pos_dot-18);
		// 创建当天日期的文件夹，检查文件夹是否存在
		ProcessTodayDir(thisDate);

		// 开始遍历所有文件
			
		// 输出进度
		cerr << "Info\t" << OutputLocalTime() << "Downloading " << thisDate << " " << thisStk << " " << thisFileType << endl; 
		filelog << "Info\t" << OutputLocalTime() << "Downloading " << thisDate << " " << thisStk << " " << thisFileType << endl; 

		// 判断市场
		string thisStkMarket;
		if (thisStk.substr(7,2) == "SZ")		
			thisStkMarket = "SZ-2-0";		
		else
			thisStkMarket = "SH-2-0";			
		
		// 开始下载该股票当天的数据
		int nRetTickData 	= 0;
		int nRetTransaction = 0;
		int nRetOrderQueue 	= 0;
		int nRetOrder 		= 0;
		bool flagSuccess 	= false;	// 标志是否成功取到数据
		int  flagRetry 		= 1;
		while (!flagSuccess){

			// 下载数据，如果遇到一些特殊error标志，则重新下载
			if (thisFileType == "tickdata"){
				nRetTickData = GetTickData(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate); 
				flagRetry = ResponseToTDBReturn(nRetTickData, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
			}
			else if (thisFileType == "order"){
				// shanghai没有order
				if ("SZ-2-0" == thisStkMarket){ 	
					nRetOrder = GetOrder(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
					flagRetry = ResponseToTDBReturn(nRetOrder, hTdb, settings);
					if (flagRetry)		// 如果flagRetry非0，返回while重新下载
						{flagSuccess = false;continue;}
				}
			}
			else if (thisFileType == "orderqueue"){					
				nRetOrderQueue = GetOrderQueue(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
				flagRetry = ResponseToTDBReturn(nRetOrderQueue, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
			}
			else if (thisFileType == "transaction"){					
				nRetTransaction = GetTransaction(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);		
				flagRetry = ResponseToTDBReturn(nRetTransaction, hTdb, settings);
				if (flagRetry)		// 如果flagRetry非0，返回while重新下载
					{flagSuccess = false;continue;}
			}
			else{			
				cerr << "Warning\t" << OutputLocalTime() << "Error File Type " << thisDate << " " << thisStk << " " << thisFileType << endl; 
				filelog << "Warning\t" << OutputLocalTime() << "Error File Type " << thisDate << " " << thisStk << " " << thisFileType << endl; 
			}


			// 全部数据下载完以后，将flag置1，跳出while循环
			flagSuccess = true;
			
			cerr << "Info\t" << OutputLocalTime() << "Complete " << thisDate << " " << thisStk << " " << thisFileType << endl; 
			filelog << "Info\t" << OutputLocalTime() << "Complete " << thisDate << " " << thisStk << " " << thisFileType << endl; 

		}
	}
	
	cerr << "------------程序自然退出-----------------\n" << OutputLocalTime() << endl; 
	filelog << "------------程序自然退出-----------------\n" << OutputLocalTime() << endl; 
	cerr << "输入任意键结束程序" << endl; 
	getchar();
 	if (hTdb)
		nRet = TDB_Close(hTdb);
	filelog.close();

	
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
		getchar();
		exit (1); 
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
		getchar();		
		exit (1); 
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
		getchar();		
		exit (1); 
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
		exit (1); 
	}

	return 1;
		
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
		// getchar();
        // exit(0);
    }	
	cerr << "Info\t" << OutputLocalTime() << "TDB_Open Succeed!" << endl; 	
	filelog << "Info\t" << OutputLocalTime() << "TDB_Open Succeed!" << endl; 	
	return 1;		
}

int MakeLocalLogFile(ofstream& filelog)
{

	time_t tmptime; 
	tmptime = time(NULL);
	char tmpdate[32];
	string tmpstr; 
	strftime(tmpdate,sizeof(tmpdate),"%Y%m%d_%H%M%S",localtime(&tmptime));
	tmpstr = "WindL2Download_";
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
	return 1;

}


int ResponseToTDBReturn(int& nRet, THANDLE& hTdb, OPEN_SETTINGS& settings)
{
	switch(nRet)
	{ 
		case TDB_NETWORK_ERROR:		 
		case TDB_NETWORK_TIMEOUT:		 
		case TDB_LOGIN_FAILED:
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Network Error! Retry Login..." << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Network Error! Retry Login..." << endl;	
			LogInWithUserConfig(hTdb, settings);		// 重新登录
			return -1;
			break;
			
		case TDB_OUT_OF_MEMORY:
			cerr << "Warning\t" << OutputLocalTime() << "Warning: Out of memory! Waiting 30s... " << endl; 
			filelog << "Warning\t" << OutputLocalTime() << "Warning: Out of memory! Waiting 30s... " << endl; 
			Sleep(1000*30);
			return -2;
			break;

		case TDB_SUCCESS:	
		case TDB_NO_DATA:			
			return 0;
			break;   			

		default:
			return 0;
			break;
	}

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
			getchar();
			exit(0);
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
			getchar();
			exit(0);
		}
	}

	return 1;
}


int LoadDownloadList(vector<string>& list, vector<string>::size_type& listStart, vector<string>::size_type& listEnd, string& dir_output)
{
	ifstream downloadfile;  
	std::string strLine;
	std::string pathStkList;		
	
	// 打开download config文件，读取文件path和股票、日期下载范围
	//std::string pathConfigFile("E:\\wind_L2_config\\downloadConfig.txt");
	std::string pathConfigFile("downloadListConfig.txt");	
	downloadfile.open(pathConfigFile.c_str(), ios::in);	
	if (downloadfile.is_open()){
	    std::getline(downloadfile, pathStkList);
	    std::getline(downloadfile, strLine);
		listStart = atoi(strLine.c_str());
	    std::getline(downloadfile, strLine);
		listEnd = atoi(strLine.c_str());

	    std::getline(downloadfile, dir_output);

		downloadfile.close();
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << pathConfigFile << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << pathConfigFile << endl; 
		getchar();
		exit (1); 
	}	

	// 读取下载列表
	downloadfile.open(pathStkList.c_str(), ios::in);
	if (downloadfile.is_open()){
		while (getline(downloadfile,strLine)){
			list.push_back(strLine);
		}
		downloadfile.close();
	}
	else{
		// print error open
		cerr << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 
		filelog << "Error\t" << OutputLocalTime() << "Error opening file: " << pathStkList << endl; 
		getchar();		
		exit (1); 
	}	

	return 0;

}

