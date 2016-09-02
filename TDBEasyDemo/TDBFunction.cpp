
#include "TDBFunction.h"
//#include <conio.h>
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

using namespace std;

// 数据提取后输出目录
extern string dir_output;
// log文件
extern ofstream filelog;

std::string arr2str(int arr[], int n)
{
    std::string str;
    str.reserve(32*n);
    for (int i=0; i<n; i++)
    {
        char szBuf[32] = {0};
        _snprintf(szBuf, sizeof(szBuf)-1, "%d", arr[i]);
        str = str + szBuf + " ";
    }
    return str;
}

std::string uarr2str(unsigned int arr[], int n)
{
    std::string str;
    str.reserve(32*n);
    for (int i=0; i<n; i++)
    {
        char szBuf[32] = {0};
        _snprintf(szBuf, sizeof(szBuf)-1, "%d", arr[i]);
        str = str + szBuf + " ";
    }
    return str;
}

std::string int2str(int n)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), "%d", n);
	return std::string(szBuf);
}

std::string tmp_int2str(int n)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), "%d", n);
	return std::string(szBuf);
}


std::string array2str(const int* arr, int len)
{
	std::string str;
	for (int i=0; i<len; i++)
	{
		if (i == len-1)
		{
			str += tmp_int2str(arr[i]) + " ";
		}
		else 
		{
			str += tmp_int2str(arr[i]) + ",";
		}
	}
	return str;
}

std::string array2strn(const int* arr, int len, int totalnum)
{
	// len是arr中的数据个数，total是总共要输出的格数，如果len<totalnum，剩余的格用空逗号补齐
	// 使用场景:要输出50个委托队列的时候，一般len只有不到10，其他都要用逗号补齐个数
	std::string str;
/*	bool flagDataShort;
	if (len >= totalnum)
		flagDataShort = false;
	else
		flagDataShort = true;*/
	
	for (int i=0; i<totalnum; i++)
	{
		if (i < len)
		{
			str.append(tmp_int2str(arr[i]));
		}

		if (i < totalnum-1)
		{
			str.append(",");
		}
	}
	return str;
}

/*
std::string array2str(const int* arr, int len)
{
	std::string str;
	for (int i=0; i<len; i++)
	{
		if (i == len-1)
		{
			str += int2str(arr[i]) + " ";
		}
		else 
		{
			str += int2str(arr[i]) + ",";
		}
	}
	return str;
}
*/



////// 配置初始化登录及服务器参数
////void InitSetting(OPEN_SETTINGS* settings,  TDB_PROXY_SETTING* proxy_settings)
////{
////	//设置服务器信息
////	char* svr_ip = new char[20];
////	int svr_port = 0;
////	char* username = new char[20];
////	char* password = new char[20];
////
////	svr_ip = "114.80.154.34";
////	svr_port = 6271;
////	username = "TD60442001";
////	password = "30976596";
////	
////	strcpy(settings->szIP, svr_ip);
////	sprintf(settings->szPort, "%d", svr_port);
////	strcpy(settings->szUser, username);
////	strcpy(settings->szPassword,  password);
////	settings->nRetryCount = 100;
////	settings->nRetryGap = 100;
////	settings->nTimeOutVal = 100;
////
////	proxy_settings->nProxyType = TDB_PROXY_HTTP11;
////	strcpy(proxy_settings->szProxyHostIp,   "10.100.3.42");
////	sprintf(proxy_settings->szProxyPort, "%d",12345);
////	strcpy(proxy_settings->szProxyUser,   "1");
////	strcpy(proxy_settings->szProxyPwd,   "1");
////
/////*
////	strcpy(settings.szIP,   svr_ip);
////	sprintf(settings.szPort, "%d", svr_port);
////	strcpy(settings.szUser, username);
////	strcpy(settings.szPassword,  password);
////	settings.nRetryCount = 100;
////	settings.nRetryGap = 100;
////	settings.nTimeOutVal = 100;
////
////	proxy_settings.nProxyType = TDB_PROXY_HTTP11;
////	strcpy(proxy_settings.szProxyHostIp,   "10.100.3.42");
////	sprintf(proxy_settings.szProxyPort, "%d",12345);
////	strcpy(proxy_settings.szProxyUser,   "1");
////	strcpy(proxy_settings.szProxyPwd,   "1");
////*/
////
////}

//////请求代码表
////void GetCodeTable(THANDLE hTdb, char* szMarket)
////{
////	TDBDefine_Code* pCodetable = NULL;
////	int pCount;
////	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);
////	if (ret == TDB_NO_DATA)
////	{
////		cerr<< "无代码表！\n" << endl;
////		return;
////	}
////
////	//创建记录文档
////	//const char STOCK_MASK = 0xf0;
////	char FileName[64];
////	if (~strcmp(szMarket, "SZ-2-0")){
////		strcpy(FileName,"E:\\CodeTable_SZ.csv");
////	}	
////	else if (~strcmp(szMarket, "SH-2-0")){
////		strcpy(FileName,"E:\\CodeTable_SH.csv");
////	}
////		
////	//sprintf(FileName,".\%s_codeTable.csv",szMarket);
////	FILE *fp=NULL;
////	fp = fopen(FileName,"w");
////	if(NULL == fp)
////    {
////		cerr<< "Error: Can not open file," << FileName << "\n" << endl;
////    }
////	else
////	{
////		fprintf(fp,"chWindCode, chCode, chMarket, chCNName, nType\n");	
////		for(int i=0; i<pCount; ++i)
////		{
////			//只输出A股股票，不输出B股、债券等
////			//A股16，中小板17，创业板18
////			//if (pCodetable[i].nType == 16 || pCodetable[i].nType == 17 || pCodetable[i].nType == 18){
////				fprintf(fp,"%s,", pCodetable[i].chWindCode);
////				fprintf(fp,"%s,", pCodetable[i].chCode);			
////				fprintf(fp,"%s,", pCodetable[i].chMarket);
////				fprintf(fp,"%s,", pCodetable[i].chCNName);
////				//fprintf(fp,"%s,", pCodetable[i].chENName);
////				fprintf(fp,"%d\n", pCodetable[i].nType);
////			//}
////		}		
////		//释放
////		fclose(fp);
////		fp = NULL;
////	}
////	
////	TDB_Free(pCodetable);
////
////}

////int GetCodeTableFromConfigFile(const char* szMarket, TDBDefine_Code (&pCodeTable)[MAX_STOCK_NUM], int & pCount)
////{
////	//读取记录文档
////	char FileName[64];
////	if (~strcmp(szMarket, "SZ-2-0")){
////		strcpy(FileName,"E:\\CodeTable_SZ.csv");
////	}	
////	else if (~strcmp(szMarket, "SH-2-0")){
////		strcpy(FileName,"E:\\CodeTable_SH.csv");
////	}
////		
////	FILE *fp=NULL;
////	fp = fopen(FileName,"r");
////	if(NULL == fp)
////    {
////		cerr<< "Error: Can not open file," << FileName << "\n" << endl;
////		return -1;
////    }
////	else
////	{		
////		char line[1024];
////		fgets (line, sizeof(line), fp);		//读取第一行title		
////		while(fgets (line, sizeof(line), fp))			/*判断是否文件尾，不是则循环*/
////		{
////
////			/*char *buf = strtok(line, ",");
////			strcpy_s(pCodeTable[pCount].chWindCode, trim(buf));
////			buf= strtok(NULL, ",");
////			strcpy_s(pCodeTable[pCount].chCode, trim(buf));
////			buf= strtok(NULL, ",");
////			strcpy_s(pCodeTable[pCount].chMarket, trim(buf));
////			buf= strtok(NULL, ",");
////			strcpy_s(pCodeTable[pCount].chCNName, trim(buf));
////			//buf= strtok(NULL, ",");
////			//strcpy_s(pCodeTable[pCount].chENName, trim(buf));	
////			buf= strtok(NULL, ",");
////			pCodeTable[pCount].nType = atoi(trim(buf));	*/
////			
////
////		 	//sscanf(line,"%s,%s,%s,%s,%s,%d",pCodeTable[pCount].chWindCode, pCodeTable[pCount].chCode, pCodeTable[pCount].chMarket,\
////				//pCodeTable[pCount].chCNName, &pCodeTable[pCount].nType);
////		 	sscanf(line,"%[^,],%[^,],%[^,],%[^,],%d",pCodeTable[pCount].chWindCode, pCodeTable[pCount].chCode, pCodeTable[pCount].chMarket,\
////				pCodeTable[pCount].chCNName, &pCodeTable[pCount].nType);
////			
////			++pCount;
////			//printf("%d\n",pCount);
////		}
////				
////		//释放
////		fclose(fp);
////		fp = NULL;
////		return 0;
////	}	
////}


//int GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete)
//{
//	//请求K线
//	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
//	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
//	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));
//
//	req->nCQFlag = REFILL_BACKWARD/*(REFILLFLAG)nCQFlag*/;//除权标志，由用户定义
//	req->nBeginDate = nBeginDate;//开始日期
//	req->nEndDate = nEndDate;//结束日期
//	req->nBeginTime = 0;//开始时间
//	req->nEndTime = 0;//结束时间
//
//	req->nCycType = (CYCTYPE)nCycle;
//	req->nCycDef = 0;
//
//	//返回结构体指针
//	TDBDefine_KLine* kLine = NULL;
//	//返回数
//	int pCount;
//	//API请求K线
//	TDB_GetKLine(hTdb,req,&kLine,&pCount);
//	delete req;
//	req = NULL;
//
//	printf("---------------------------K Data--------------------\n");
//	printf("数据条数：%d,打印 1/100 条\n\n",pCount);
//	for(int i=0;i<pCount;)
//	{
//	printf("WindCode:%s\n Code:%s\n Date:%d\n Time:%d\n Open:%d\n High:%d\n Low:%d\n Close:%d\n Volume:%I64d\n Turover:%I64d\n MatchItem:%d\n Interest:%d\n",
//	kLine[i].chWindCode,kLine[i].chCode,kLine[i].nDate,kLine[i].nTime,kLine[i].nOpen,kLine[i].nHigh,kLine[i].nLow,kLine[i].nClose,
//	kLine[i].iVolume,kLine[i].iTurover,kLine[i].nMatchItems,kLine[i].nInterest);
//	i +=100;
//	}
//	//释放
//	TDB_Free(kLine);
//}



//tick
int GetTickData(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate)
{
	//请求信息
	TDBDefine_ReqTick req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Tick *pTick = NULL;
	int pCount;
	int nRet = TDB_GetTick(hTdb,&req,&pTick, &pCount);

	const string strCode = szCode;
	if (nRet == TDB_SUCCESS){
		
		FILE *fp=NULL;
		//创建记录文档
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_tickdata.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w");
		
		char bufw[1024*256];	//256k的输出buf instead of 系统默认的4k buf,再大就溢出了不知道为什么
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"万得代码,日期,时间,最新价,成交量,成交额,成交笔数,IOPV/利息,\
		//	成交标志,买卖标志,累计成交量,累计成交额,最高价,最低价,开盘价,昨收盘,\
		//	卖1价,卖2价,卖3价,卖4价,卖5价,卖6价,卖7价,卖8价,卖9价,卖10价,买1价,买2价,买3价,买4价,买5价,买6价,买7价,买8价,买9价,买10价,\
		//	卖1量,卖2量,卖3量,卖4量,卖5量,卖6量,卖7量,卖8量,卖9量,卖10量,,买1量,买2量,买3量,买4量,买5量,买6量,买7量,买8量,买9量,买10量,\
		//	加权平均叫卖价,加权平均叫买价,叫卖总量,叫买总量\n");	
	
		//fprintf(fp,"wind_code,date,time,price,volume,turover,match_items,interest,\
		//	trade_flag,bs_flag,accvolume,accturover,high,low,open,pre_close,\
		//	ask1,ask2,ask3,ask4,ask5,ask6,ask7,ask8,ask9,ask10,bid1,bid2,bid3,bid4,bid5,bid6,bid7,bid8,bid9,bid10,\
		//	asize1,asize2,asize3,asize4,asize5,asize6,asize7,asize8,asize9,asize10,bsize1,bsize2,bsize3,bsize4,bsize5,bsize6,bsize7,bsize8,bsize9,bsize10,\
		//	ask_av_price,bid_av_price,total_ask_volume,total_bid_volume\n");	



		
		//printf("---------------------------------------Tick Data------------------------------------------\n");
		//printf("共收到 %d 条Tick数据， 打印 1/100 条：\n", pCount);

		for(int i=0; i<pCount; ++i)
		{
			TDBDefine_Tick& pTickCopy = pTick[i];
			//printf("万得代码 chWindCode:%s \n", pTickCopy.chWindCode);
			//printf("日期 nDate:%d \n", pTickCopy.nDate);
			//printf("时间 nTime:%d \n", pTickCopy.nTime);
	
			//printf("成交价 nPrice:%d \n", pTickCopy.nPrice);
			//printf("成交量 iVolume:%I64d \n", pTickCopy.iVolume);
			//printf("成交额(元) iTurover:%I64d \n", pTickCopy.iTurover);
			//printf("成交笔数 nMatchItems:%d \n", pTickCopy.nMatchItems);
			//printf(" nInterest:%d \n", pTickCopy.nInterest);
	
			//printf("成交标志: chTradeFlag:%c \n", pTickCopy.chTradeFlag);
			//printf("BS标志: chBSFlag:%c \n", pTickCopy.chBSFlag);
			//printf("当日成交量: iAccVolume:%I64d \n", pTickCopy.iAccVolume);
			//printf("当日成交额: iAccTurover:%I64d \n", pTickCopy.iAccTurover);
	
			//printf("最高 nHigh:%d \n", pTickCopy.nHigh);
			//printf("最低 nLow:%d \n", pTickCopy.nLow);
			//printf("开盘 nOpen:%d \n", pTickCopy.nOpen);
			//printf("前收盘 nPreClose:%d \n", pTickCopy.nPreClose);
			
			//写文件
			fprintf(fp,"%s,%d,%d,",pTickCopy.chWindCode, pTickCopy.nDate, pTickCopy.nTime); 
			fprintf(fp,"%d,%I64d,%I64d,%d,%d,",pTickCopy.nPrice, pTickCopy.iVolume, pTickCopy.iTurover, pTickCopy.nMatchItems, pTickCopy.nInterest);	
			fprintf(fp,"%c,%c,%I64d,%I64d,",pTickCopy.chTradeFlag, pTickCopy.chBSFlag, pTickCopy.iAccVolume, pTickCopy.iAccTurover);	
			fprintf(fp,"%d,%d,%d,%d,",pTickCopy.nHigh, pTickCopy.nLow, pTickCopy.nOpen, pTickCopy.nPreClose);	
	
			//买卖盘字段
			std::string strOut= array2strn(pTickCopy.nAskPrice,ELEMENT_COUNT(pTickCopy.nAskPrice),NUM_ASKBID_LEVEL);
			//printf("叫卖价 nAskPrice:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
					
			strOut= array2strn(pTickCopy.nBidPrice,ELEMENT_COUNT(pTickCopy.nBidPrice),NUM_ASKBID_LEVEL);
			//printf("叫买价 nBidPrice:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
			
			strOut= array2strn((const int*)pTickCopy.nAskVolume,ELEMENT_COUNT(pTickCopy.nAskVolume),NUM_ASKBID_LEVEL);
			//printf("叫卖量 nAskVolume:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
	
			strOut= array2strn((const int*)pTickCopy.nBidVolume,ELEMENT_COUNT(pTickCopy.nBidVolume),NUM_ASKBID_LEVEL);
			//printf("叫买量 nBidVolume:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
			
			//printf("加权平均叫卖价 nAskAvPrice:%d \n", pTickCopy.nAskAvPrice);
			//printf("加权平均叫买价 nBidAvPrice:%d \n", pTickCopy.nBidAvPrice);
			//printf("叫卖总量 iTotalAskVolume:%I64d \n", pTickCopy.iTotalAskVolume);
			//printf("叫买总量 iTotalBidVolume:%I64d \n", pTickCopy.iTotalBidVolume);
	
			fprintf(fp,"%d,%d,%I64d,%I64d\n",pTickCopy.nAskAvPrice, pTickCopy.nBidAvPrice, pTickCopy.iTotalAskVolume, pTickCopy.iTotalBidVolume); 
						
		}
		//释放
		TDB_Free(pTick);

		fclose(fp);
		return 0;
	}
    else{
		cerr << "Warning\t" << OutputLocalTime() << "Not successfully getting Tick data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		filelog << "Warning\t" << OutputLocalTime() << "Not successfully getting Tick data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		return nRet;
    }

	
}

//逐笔成交
int GetTransaction(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//请求
	TDBDefine_ReqTransaction req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = NULL;
	int pCount;
    int nRet = TDB_GetTransaction(hTdb, &req, &pTransaction, &pCount);

	const string strCode = szCode;
	if (nRet == TDB_SUCCESS){

		FILE *fp = NULL;
		//创建记录文档
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_transaction.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k的输出buf instead of 系统默认的4k buf,再大就溢出了不知道为什么
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"万得代码,日期,时间,成交代码,成交类别,买卖标志,成交价,成交量,叫卖序号,叫买序号\n");	
		//fprintf(fp,"wind_code,date,time,function_code,order_kind,bs_flag,trade_price,trade_volume,ask_order,bid_order\n");	
		for (int i=0; i<pCount; ++i)
		{
			const TDBDefine_Transaction& trans = pTransaction[i];
			// 可能遇到中间有\0的情况，不能用这种方法输出
			fprintf(fp,"%s,%d,%d,%c,%c,%c,%d,%d,%d,%d\n",trans.chWindCode, trans.nDate, trans.nTime, trans.chFunctionCode, trans.chOrderKind, trans.chBSFlag, trans.nTradePrice, trans.nTradeVolume, trans.nAskOrder, trans.nBidOrder);

		}

		//释放
		fclose(fp); 
		TDB_Free(pTransaction);
		return 0;
	}
	else{
		cerr << "Warning\t" << OutputLocalTime() << "Not successfully getting Transaction data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		filelog << "Warning\t" << OutputLocalTime() << "Not successfully getting Transaction data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		return nRet;
	}

}

//逐笔委托
int GetOrder(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//请求
	TDBDefine_ReqOrder req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Order* pOrder = NULL;
	int pCount = 0;
	int nRet = TDB_GetOrder(hTdb, &req, &pOrder, &pCount);
	
	const string strCode = szCode;
	if (nRet == TDB_SUCCESS){

		FILE *fp = NULL;
		//创建记录文档
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_order.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k的输出buf instead of 系统默认的4k buf,再大就溢出了不知道为什么
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"万得代码,日期,时间,委托编号,交易所委托号,委托类别,委托代码,委托价,委托量\n"); 
		//fprintf(fp,"wind_code,date,time,index,order,order_kind,function_code,order_price,order_volume\n");	
		for (int i=0; i<pCount; ++i)
		{
			const TDBDefine_Order& order = pOrder[i];
			fprintf(fp,"%s,", order.chWindCode);		
			fprintf(fp,"%d,", order.nDate);
			fprintf(fp,"%d,", order.nTime);
			fprintf(fp,"%d,", order.nIndex);
			fprintf(fp,"%d,", order.nOrder);
			fprintf(fp,"%c,", order.chOrderKind);
			fprintf(fp,"%c,", order.chFunctionCode);
			fprintf(fp,"%d,", order.nOrderPrice);
			fprintf(fp,"%d\n", order.nOrderVolume);
		}
		//释放
		fclose(fp); 
		TDB_Free(pOrder);
		return 0;
	}
	else{
		cerr << "Warning\t" << OutputLocalTime() << "Not successfully getting Order data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		filelog << "Warning\t" << OutputLocalTime() << "Not successfully getting Order data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		return nRet;
	}

}

//委托队列
int GetOrderQueue(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//请求
	TDBDefine_ReqOrderQueue req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //代码写成你想获取的股票代码
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_OrderQueue* pOrderQueue = NULL;
	int pCount = 0;
	int nRet = TDB_GetOrderQueue(hTdb, &req, &pOrderQueue, &pCount);

	const string strCode = szCode;
	if (nRet == TDB_SUCCESS){

		FILE *fp = NULL;
		//创建记录文档
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_orderqueue.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k的输出buf instead of 系统默认的4k buf,再大就溢出了不知道为什么
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"wind_code,date,time,side,price,order_items,ab_items,ab1,ab2,ab3,ab4,ab5,ab6,ab7,ab8,ab9,ab10,ab11,ab12,ab13,ab14,ab15,ab16,ab17,ab18,ab19,ab20,ab21,ab22,ab23,ab24,ab25,ab26,ab27,ab28,ab29,ab30,ab31,ab32,ab33,ab34,ab35,ab36,ab37,ab38,ab39,ab40,ab41,ab42,ab43,ab44,ab45,ab46,ab47,ab48,ab49,ab50\n"); 
		//fprintf(fp,"万得代码,日期,时间,买卖方向,委托价格,订单数量,明细个数,订单明细\n");	
		for (int i=0; i<pCount; ++i)
		{
			const TDBDefine_OrderQueue& que = pOrderQueue[i];
			fprintf(fp,"%s,", que.chWindCode);
			fprintf(fp,"%d,", que.nDate);
			fprintf(fp,"%d,", que.nTime);
			fprintf(fp,"%c,", que.nSide);
			fprintf(fp,"%d,", que.nPrice);
			fprintf(fp,"%d,", que.nOrderItems);
			fprintf(fp,"%d,", que.nABItems);
			fprintf(fp,"%s\n", array2strn(que.nABVolume, que.nABItems, NUM_ORDER_QUEUE).c_str());
		}

		//释放
		fclose(fp); 
		TDB_Free(pOrderQueue);
		return 0;
	}
	else{
		cerr << "Warning\t" << OutputLocalTime() << "Not successfully getting Orderqueue data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		filelog << "Warning\t" << OutputLocalTime() << "Not successfully getting Orderqueue data " << nDate << " " << strCode << ". Errorcode:" << nRet << endl; 
		return nRet;
	}

}

/*
//指标公式
void UseEZFFormula(THANDLE hTdb)
{
	//公式的编写，请参考<<TRANSEND-TS-M0001 易编公式函数表V1(2).0-20110822.pdf>>;
	std::string strName = "KDJ";
	std::string strContent = "INPUT:N(9), M1(3,1,100,2), M2(3);"
		"RSV:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;"
		"K:SMA(RSV,M1,1);"
		"D:SMA(K,M2,1);"
		"J:3*K-2*D;";

	//添加公式到服务器并编译，若不过，会有错误返回
	TDBDefine_AddFormulaRes* addRes = new TDBDefine_AddFormulaRes;
	int nErr = TDB_AddFormula(hTdb, strName.c_str(), strContent.c_str(),addRes);
	printf("Add Formula Result:%s",addRes->chInfo);

	//查询服务器上的公式，能看到我们刚才上传的"KDJ"
	TDBDefine_FormulaItem* pEZFItem = NULL;
	int nItems = 0;
	//名字为空表示查询服务器上所有的公式
	nErr = TDB_GetFormula(hTdb, NULL, &pEZFItem, &nItems);

	for (int i=0; i<nItems; i++)
	{
		std::string strNameInner(pEZFItem[i].chFormulaName, 0, sizeof(pEZFItem[i].chFormulaName));
		std::string strParam(pEZFItem[i].chParam, 0, sizeof(pEZFItem[i].chParam));
		printf("公式名称：%s, 参数:%s \n", strNameInner.c_str(), strParam.c_str());
	}

	struct EZFCycDefine
	{
		char chName[8];
		int  nCyc;
		int  nCyc1;
	}
	EZFCyc[5]={
		{"日线", 2, 0},
		{"30分", 0, 30},
		{"5分钟", 0, 5},
		{"1分钟", 0, 1},
		{"15秒", 11, 15}};

		//获取公式的计算结果
		TDBDefine_ReqCalcFormula reqCalc = {0};
		strncpy(reqCalc.chFormulaName, "KDJ", sizeof(reqCalc.chFormulaName));
		strncpy(reqCalc.chParam, "N=9,M1=3,M2=3", sizeof(reqCalc.chParam));
		strncpy(reqCalc.chCode, "000001.SZ", sizeof(reqCalc.chCode));
		strncpy(reqCalc.chMarketKey, "SZ-2-0", sizeof(reqCalc.chMarketKey));
		reqCalc.nCycType = (CYCTYPE)(EZFCyc[0].nCyc); //0表示日线
		reqCalc.nCycDef = EZFCyc[0].nCyc1; 
		reqCalc.nCQFlag = (REFILLFLAG)0;		  //除权标志
		reqCalc.nCalcMaxItems = 4000; //计算的最大数据量
		reqCalc.nResultMaxItems = 100;	//传送的结果的最大数据量

		TDBDefine_CalcFormulaRes* pResult = new TDBDefine_CalcFormulaRes;
		nErr = TDB_CalcFormula(hTdb, &reqCalc, pResult);
		//判断错误代码

		printf("计算结果有: %d 条:\n", pResult->nRecordCount);
		char szLineBuf[1024] = {0};
		//输出字段名
		for (int j=0; j<pResult->nFieldCount;j++)
		{
			std::cout << pResult->chFieldName[j] << "  ";
		}
		std::cout << endl << endl;
		//输出数据
		for (int i=0; i<pResult->nRecordCount; i++)
		{
			for (int j=0; j<pResult->nFieldCount;j++)
			{
				std::cout << (pResult->dataFileds)[j][i] << "  ";
			}
			std::cout << endl;
		}

		//删除之前上传的公式指标
		TDBDefine_DelFormulaRes pDel = {0};
		nErr = TDB_DeleteFormula(hTdb, "KDJ", &pDel);
		printf("删除指标信息:%s", pDel.chInfo);
		//释放内存
		delete pEZFItem;
		TDB_ReleaseCalcFormula(pResult);
}
*/

string OutputLocalTime(void)
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	string localtime;
	char tmptime[64];
	sprintf(tmptime, "%02d:%02d:%02d.%03d\t",sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
	localtime = tmptime;
	return localtime;
}