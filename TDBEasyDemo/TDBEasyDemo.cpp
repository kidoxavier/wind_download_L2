// TDBEasyDemo.cpp : �������̨Ӧ�ó������ڵ㡣
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

// ������ȡ�����Ŀ¼
string dir_output;


// log�ļ�
ofstream filelog; 

int main()
{
	//��ȡ�����ļ����������ع�Ʊ�б��Լ����������б�
	cerr << "Info\t" << OutputLocalTime() << "Start downloading L2 raw data from Wind!\n" << endl; 

	// ��log�ļ�
	MakeLocalLogFile(filelog);
	
	//��ʼ�������˻���Ϣ,�����˺����룬windԶ�̷�������
	OPEN_SETTINGS settings = {0};
    LoadUserConfig(settings);

	//����TDB_Open����¼windԶ�̷�����, ���һ�β��ɹ�����ÿ��60s����һ��
    THANDLE hTdb;
	LogInWithUserConfig(hTdb, settings);

	int nRet = TDB_SUCCESS;

	//��ȡ�����г��Ĺ�Ʊ���룬��API��ʵʱ����
	//GetCodeTable(hTdb, "SZ-2-0");
	//GetCodeTable(hTdb, "SH-2-0");
	//��ȡ�����г��Ĺ�Ʊ���룬���ļ��ж�ȡ	
	/*TDBDefine_Code pCodetableSZ[MAX_STOCK_NUM];
	TDBDefine_Code pCodetableSH[MAX_STOCK_NUM];	
	int stkNumSZ=0,stkNumSH=0;
	int retSH = -1, retSZ = -1;*/
	//while(retSH < 0){
	//	retSH = GetCodeTableFromConfigFile("SH-2-0", pCodetableSH, stkNumSH);
	//	//Sleep(1000); //�ȴ�1s
	//}
	//while(retSZ < 0){
	//	retSZ = GetCodeTableFromConfigFile("SZ-2-0", pCodetableSZ, stkNumSZ);
	//	//Sleep(1000); //�ȴ�1s
	//}

	// ��ȡ��Ʊ���룬�����Լ������ļ���ָ����Ʊ���������أ�
	vector<string> stkList;
	vector<string>::size_type stkStart,stkEnd;	
	vector<string> dateList;
	string dateStart, dateEnd;
	//string dir_output;
	int ret;
    ret = LoadStkDateDirConfig(stkList, stkStart, stkEnd, dateList, dateStart, dateEnd, dir_output);	  	

	// ����ѭ��������Ʊ�����ڣ������һ������й�Ʊ
	int todayDate, startDate, endDate;
	startDate = atoi(dateStart.c_str());
	endDate = atoi(dateEnd.c_str());
	for(vector<string>::size_type dayi = 0; dayi!=dateList.size(); ++dayi)
	{
		// ֻ���������ļ����õ�����
		todayDate = atoi(dateList[dayi].c_str());
		if (todayDate < startDate || todayDate > endDate){			
			continue;
		}
	
		
		// �����������ڵ��ļ��У�����ļ����Ƿ����
		ProcessTodayDir(dateList[dayi]);

	
		// ��ʼ�������й�Ʊ
		for(vector<string>::size_type stocki = stkStart; stocki < stkEnd; ++stocki)
		{
			string thisStk = stkList[stocki];			
			// �������
			cerr << "Info\t" << OutputLocalTime() << "Downloading " << dateList[dayi] << " " << thisStk << endl; 
			filelog << "Info\t" << OutputLocalTime() << "Downloading " << dateList[dayi] << " " << thisStk << endl; 

			// �ж��г�
			string thisStkMarket;
			if (thisStk.substr(7,2) == "SZ")		
				thisStkMarket = "SZ-2-0";		
			else
				thisStkMarket = "SH-2-0";			
	
			// ��ʼ�������ݣ�sh�����������ί��
			// K��
			//GetK(hTdb, "000001.sz", CYC_SECOND,  1, REFILL_NONE, 1, 20120101, 20131231);
			//GetK(hTdb, "000001.sz", CYC_MINUTE,  1, REFILL_NONE, 1, 20120101, 20131231);
			//GetK(hTdb, "000001.sz", CYC_DAY,	1, REFILL_NONE, 1, 20120101, 20131231);
				
			// ��ʼ���ظù�Ʊ���������
			int nRetTickData 	= 0;
			int nRetTransaction = 0;
			int nRetOrderQueue 	= 0;
			int nRetOrder 		= 0;
			bool flagSuccess 	= false;	// ��־�Ƿ�ɹ�ȡ������
			int  flagRetry 		= 1;
			while (!flagSuccess){
				// �������ݣ��������һЩ����error��־������������
				nRetTickData = GetTickData(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate); 
				flagRetry = ResponseToTDBReturn(nRetTickData, hTdb, settings);
				if (flagRetry)		// ���flagRetry��0������while��������
					{flagSuccess = false;continue;}
									
				nRetTransaction = GetTransaction(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate); 		
				flagRetry = ResponseToTDBReturn(nRetTransaction, hTdb, settings);
				if (flagRetry)		// ���flagRetry��0������while��������
					{flagSuccess = false;continue;}
				
				nRetOrderQueue = GetOrderQueue(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
				flagRetry = ResponseToTDBReturn(nRetOrderQueue, hTdb, settings);
				if (flagRetry)		// ���flagRetry��0������while��������
					{flagSuccess = false;continue;}
				
				// shanghaiû��order
				if ("SZ-2-0" == thisStkMarket){		
					nRetOrder = GetOrder(hTdb, thisStk.c_str(), thisStkMarket.c_str(), todayDate);
					flagRetry = ResponseToTDBReturn(nRetOrder, hTdb, settings);
					if (flagRetry)		// ���flagRetry��0������while��������
						{flagSuccess = false;continue;}
				}

				// ȫ�������������Ժ󣬽�flag��1������whileѭ��
				flagSuccess = true;
			}
			cerr << "Info\t" << OutputLocalTime() << "Complete " << dateList[dayi] << " " << thisStk << endl; 
			filelog << "Info\t" << OutputLocalTime() << "Complete " << dateList[dayi] << " " << thisStk << endl; 
		}
	}
	
	cerr << "------------������Ȼ�˳�-----------------\n" << OutputLocalTime() << endl; 
	filelog << "------------������Ȼ�˳�-----------------\n" << OutputLocalTime() << endl; 
	cerr << "�����������������" << endl; 
	getchar();
 	if (hTdb)
		nRet = TDB_Close(hTdb);
	filelog.close();

	{
		//GetKData(hTdb, "600163.sh", "sh-2-0", 20150910, 20150915, CYC_MINUTE, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_MINUTE, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_DAY, 0, 0, 0);
		//GetKData(hTdb, "000001.sz", "SZ-2-0", 20150424, 20150424, CYC_SECOND, 0, 0, 1);
		//GetTickData(hTdb, "000001.sz", "SZ-2-0", 20160721);//�������̵�tick
		//GetTickData(hTdb, "601336.sh", "SH-2-0", 20160721);//�������̵�tick
		//GetTransaction(hTdb, "000001.sz", "SZ-2-0", 20160721);
		//GetTransaction(hTdb, "600508.sh", "SH-2-0", 20160721);
		//GetOrder(hTdb, "000001.sz", "SZ-2-0", 20160721);//���ί��
		//GetOrder(hTdb, "600508.sh", "SH-2-0", 20160721);//SHû�����ί�У�����warning
		//GetOrderQueue(hTdb, "000001.sz", "SZ-2-0", 20160513);//ί�ж���
		//GetOrderQueue(hTdb, "600311.sh", "SH-2-0", 20160513);//ί�ж���

		//GetTickData(hTdb, "601336.sh", "SH-2-0", 20160721);//�������̵�tick
		//GetTransaction(hTdb, "601336.sh", "SH-2-0", 20160721);
		//GetOrderQueue(hTdb, "601336.sh", "SH-2-0", 20160721);//ί�ж���
		//UseEZFFormula(hTdb);
	}
	
	//for (int stki = 0; stki<stkNumSZ; ++stki){
	//	printf("%d\t%s\n",stki,pCodetableSZ[stki].chWindCode);
		//GetTickData(hTdb, pCodetableSZ[stki].chWindCode, pCodetableSZ[stki].chMarket, 20160721);//�������̵�tick		
		//GetTransaction(hTdb, pCodetableSZ[stki].chWindCode, pCodetableSZ[stki].chMarket, 20160721);	
		//GetOrder(hTdb, pCodetableSZ[stki].chWindCode, pCodetableSZ[stki].chMarket, 20160721);//���ί��
		//GetOrderQueue(hTdb, pCodetableSZ[stki].chWindCode, pCodetableSZ[stki].chMarket, 20160721);//ί�ж���			
	//}

	//for (int stki = 0; stki<stkNumSH; ++stki){
	//	printf("%d\t%s\n",stki,pCodetableSH[stki].chWindCode);
	//	GetTickData(hTdb, pCodetableSH[stki].chWindCode, pCodetableSH[stki].chMarket, 20160721);//�������̵�tick		
	//	GetTransaction(hTdb, pCodetableSH[stki].chWindCode, pCodetableSH[stki].chMarket, 20160721);	
	//	//GetOrder(hTdb, pCodetableSH[stki].chWindCode, pCodetableSH[stki].chMarket, 20160721);//���ί��
	//	GetOrderQueue(hTdb, pCodetableSH[stki].chWindCode, pCodetableSH[stki].chMarket, 20160721);//ί�ж���			
	//}
	
}

int LoadStkDateDirConfig(vector<string>& stkList, vector<string>::size_type& stkStart, vector<string>::size_type& stkEnd, vector<string>& dateList, string& dateStart, string& dateEnd, string& dir_output)
{
	ifstream downloadfile;  
	std::string strLine;
	std::string pathStkList, pathDateList;		
	
	// ��download config�ļ�����ȡ�ļ�path�͹�Ʊ���������ط�Χ
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

	// ��ȡ��Ʊ�б�
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

	// ��ȡ�����б�
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

	    // settings.nTimeOutVal = 30;   //��ȡ����ʱ��ָ�����糬ʱ��������Ϊ0����Ϊ2���ӣ���������nTimeOutVal���δ�յ���Ӧ���ݰ������ڲ���ر�����
	    std::getline(accountfile, strLine);
	    settings.nTimeOutVal = atoi(strLine.c_str());

		// settings.nRetryCount = 4;   //��ȡ����ʱ�������ߣ�ָ������������Ϊ0����������������nRetryCount��֮���Ե��ߣ��򷵻��������
	    std::getline(accountfile, strLine);
	    settings.nRetryCount = atoi(strLine.c_str());

	    // settings.nRetryGap = 0;     //����֮��������ʱ������������Ϊ0����Ϊ1�룩	
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
	//htdb��ֵ��loginRes����ֶΣ���¼ʧ�ܷ���Ϊ�գ�����������ֵ�ģ�loginRes���info�ֶο��Եó�����ԭ��	
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
			LogInWithUserConfig(hTdb, settings);		// ���µ�¼
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
	// ȷ�������Ŀ¼�Ƿ����
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

	// ȷ�ϵ�����Ŀ¼�Ƿ����
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

