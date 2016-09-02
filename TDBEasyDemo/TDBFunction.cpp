
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

// ������ȡ�����Ŀ¼
extern string dir_output;
// log�ļ�
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
	// len��arr�е����ݸ�����total���ܹ�Ҫ����ĸ��������len<totalnum��ʣ��ĸ��ÿն��Ų���
	// ʹ�ó���:Ҫ���50��ί�ж��е�ʱ��һ��lenֻ�в���10��������Ҫ�ö��Ų������
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



////// ���ó�ʼ����¼������������
////void InitSetting(OPEN_SETTINGS* settings,  TDB_PROXY_SETTING* proxy_settings)
////{
////	//���÷�������Ϣ
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

//////��������
////void GetCodeTable(THANDLE hTdb, char* szMarket)
////{
////	TDBDefine_Code* pCodetable = NULL;
////	int pCount;
////	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);
////	if (ret == TDB_NO_DATA)
////	{
////		cerr<< "�޴����\n" << endl;
////		return;
////	}
////
////	//������¼�ĵ�
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
////			//ֻ���A�ɹ�Ʊ�������B�ɡ�ծȯ��
////			//A��16����С��17����ҵ��18
////			//if (pCodetable[i].nType == 16 || pCodetable[i].nType == 17 || pCodetable[i].nType == 18){
////				fprintf(fp,"%s,", pCodetable[i].chWindCode);
////				fprintf(fp,"%s,", pCodetable[i].chCode);			
////				fprintf(fp,"%s,", pCodetable[i].chMarket);
////				fprintf(fp,"%s,", pCodetable[i].chCNName);
////				//fprintf(fp,"%s,", pCodetable[i].chENName);
////				fprintf(fp,"%d\n", pCodetable[i].nType);
////			//}
////		}		
////		//�ͷ�
////		fclose(fp);
////		fp = NULL;
////	}
////	
////	TDB_Free(pCodetable);
////
////}

////int GetCodeTableFromConfigFile(const char* szMarket, TDBDefine_Code (&pCodeTable)[MAX_STOCK_NUM], int & pCount)
////{
////	//��ȡ��¼�ĵ�
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
////		fgets (line, sizeof(line), fp);		//��ȡ��һ��title		
////		while(fgets (line, sizeof(line), fp))			/*�ж��Ƿ��ļ�β��������ѭ��*/
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
////		//�ͷ�
////		fclose(fp);
////		fp = NULL;
////		return 0;
////	}	
////}


//int GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete)
//{
//	//����K��
//	TDBDefine_ReqKLine* req = new TDBDefine_ReqKLine;
//	strncpy(req->chCode, szCode, ELEMENT_COUNT(req->chCode));
//	strncpy(req->chMarketKey, szMarket, ELEMENT_COUNT(req->chMarketKey));
//
//	req->nCQFlag = REFILL_BACKWARD/*(REFILLFLAG)nCQFlag*/;//��Ȩ��־�����û�����
//	req->nBeginDate = nBeginDate;//��ʼ����
//	req->nEndDate = nEndDate;//��������
//	req->nBeginTime = 0;//��ʼʱ��
//	req->nEndTime = 0;//����ʱ��
//
//	req->nCycType = (CYCTYPE)nCycle;
//	req->nCycDef = 0;
//
//	//���ؽṹ��ָ��
//	TDBDefine_KLine* kLine = NULL;
//	//������
//	int pCount;
//	//API����K��
//	TDB_GetKLine(hTdb,req,&kLine,&pCount);
//	delete req;
//	req = NULL;
//
//	printf("---------------------------K Data--------------------\n");
//	printf("����������%d,��ӡ 1/100 ��\n\n",pCount);
//	for(int i=0;i<pCount;)
//	{
//	printf("WindCode:%s\n Code:%s\n Date:%d\n Time:%d\n Open:%d\n High:%d\n Low:%d\n Close:%d\n Volume:%I64d\n Turover:%I64d\n MatchItem:%d\n Interest:%d\n",
//	kLine[i].chWindCode,kLine[i].chCode,kLine[i].nDate,kLine[i].nTime,kLine[i].nOpen,kLine[i].nHigh,kLine[i].nLow,kLine[i].nClose,
//	kLine[i].iVolume,kLine[i].iTurover,kLine[i].nMatchItems,kLine[i].nInterest);
//	i +=100;
//	}
//	//�ͷ�
//	TDB_Free(kLine);
//}



//tick
int GetTickData(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate)
{
	//������Ϣ
	TDBDefine_ReqTick req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
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
		//������¼�ĵ�
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_tickdata.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w");
		
		char bufw[1024*256];	//256k�����buf instead of ϵͳĬ�ϵ�4k buf,�ٴ������˲�֪��Ϊʲô
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"��ô���,����,ʱ��,���¼�,�ɽ���,�ɽ���,�ɽ�����,IOPV/��Ϣ,\
		//	�ɽ���־,������־,�ۼƳɽ���,�ۼƳɽ���,��߼�,��ͼ�,���̼�,������,\
		//	��1��,��2��,��3��,��4��,��5��,��6��,��7��,��8��,��9��,��10��,��1��,��2��,��3��,��4��,��5��,��6��,��7��,��8��,��9��,��10��,\
		//	��1��,��2��,��3��,��4��,��5��,��6��,��7��,��8��,��9��,��10��,,��1��,��2��,��3��,��4��,��5��,��6��,��7��,��8��,��9��,��10��,\
		//	��Ȩƽ��������,��Ȩƽ�������,��������,��������\n");	
	
		//fprintf(fp,"wind_code,date,time,price,volume,turover,match_items,interest,\
		//	trade_flag,bs_flag,accvolume,accturover,high,low,open,pre_close,\
		//	ask1,ask2,ask3,ask4,ask5,ask6,ask7,ask8,ask9,ask10,bid1,bid2,bid3,bid4,bid5,bid6,bid7,bid8,bid9,bid10,\
		//	asize1,asize2,asize3,asize4,asize5,asize6,asize7,asize8,asize9,asize10,bsize1,bsize2,bsize3,bsize4,bsize5,bsize6,bsize7,bsize8,bsize9,bsize10,\
		//	ask_av_price,bid_av_price,total_ask_volume,total_bid_volume\n");	



		
		//printf("---------------------------------------Tick Data------------------------------------------\n");
		//printf("���յ� %d ��Tick���ݣ� ��ӡ 1/100 ����\n", pCount);

		for(int i=0; i<pCount; ++i)
		{
			TDBDefine_Tick& pTickCopy = pTick[i];
			//printf("��ô��� chWindCode:%s \n", pTickCopy.chWindCode);
			//printf("���� nDate:%d \n", pTickCopy.nDate);
			//printf("ʱ�� nTime:%d \n", pTickCopy.nTime);
	
			//printf("�ɽ��� nPrice:%d \n", pTickCopy.nPrice);
			//printf("�ɽ��� iVolume:%I64d \n", pTickCopy.iVolume);
			//printf("�ɽ���(Ԫ) iTurover:%I64d \n", pTickCopy.iTurover);
			//printf("�ɽ����� nMatchItems:%d \n", pTickCopy.nMatchItems);
			//printf(" nInterest:%d \n", pTickCopy.nInterest);
	
			//printf("�ɽ���־: chTradeFlag:%c \n", pTickCopy.chTradeFlag);
			//printf("BS��־: chBSFlag:%c \n", pTickCopy.chBSFlag);
			//printf("���ճɽ���: iAccVolume:%I64d \n", pTickCopy.iAccVolume);
			//printf("���ճɽ���: iAccTurover:%I64d \n", pTickCopy.iAccTurover);
	
			//printf("��� nHigh:%d \n", pTickCopy.nHigh);
			//printf("��� nLow:%d \n", pTickCopy.nLow);
			//printf("���� nOpen:%d \n", pTickCopy.nOpen);
			//printf("ǰ���� nPreClose:%d \n", pTickCopy.nPreClose);
			
			//д�ļ�
			fprintf(fp,"%s,%d,%d,",pTickCopy.chWindCode, pTickCopy.nDate, pTickCopy.nTime); 
			fprintf(fp,"%d,%I64d,%I64d,%d,%d,",pTickCopy.nPrice, pTickCopy.iVolume, pTickCopy.iTurover, pTickCopy.nMatchItems, pTickCopy.nInterest);	
			fprintf(fp,"%c,%c,%I64d,%I64d,",pTickCopy.chTradeFlag, pTickCopy.chBSFlag, pTickCopy.iAccVolume, pTickCopy.iAccTurover);	
			fprintf(fp,"%d,%d,%d,%d,",pTickCopy.nHigh, pTickCopy.nLow, pTickCopy.nOpen, pTickCopy.nPreClose);	
	
			//�������ֶ�
			std::string strOut= array2strn(pTickCopy.nAskPrice,ELEMENT_COUNT(pTickCopy.nAskPrice),NUM_ASKBID_LEVEL);
			//printf("������ nAskPrice:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
					
			strOut= array2strn(pTickCopy.nBidPrice,ELEMENT_COUNT(pTickCopy.nBidPrice),NUM_ASKBID_LEVEL);
			//printf("����� nBidPrice:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
			
			strOut= array2strn((const int*)pTickCopy.nAskVolume,ELEMENT_COUNT(pTickCopy.nAskVolume),NUM_ASKBID_LEVEL);
			//printf("������ nAskVolume:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
	
			strOut= array2strn((const int*)pTickCopy.nBidVolume,ELEMENT_COUNT(pTickCopy.nBidVolume),NUM_ASKBID_LEVEL);
			//printf("������ nBidVolume:%s \n", strOut.c_str());
			fprintf(fp,"%s,",strOut.c_str());	
			
			//printf("��Ȩƽ�������� nAskAvPrice:%d \n", pTickCopy.nAskAvPrice);
			//printf("��Ȩƽ������� nBidAvPrice:%d \n", pTickCopy.nBidAvPrice);
			//printf("�������� iTotalAskVolume:%I64d \n", pTickCopy.iTotalAskVolume);
			//printf("�������� iTotalBidVolume:%I64d \n", pTickCopy.iTotalBidVolume);
	
			fprintf(fp,"%d,%d,%I64d,%I64d\n",pTickCopy.nAskAvPrice, pTickCopy.nBidAvPrice, pTickCopy.iTotalAskVolume, pTickCopy.iTotalBidVolume); 
						
		}
		//�ͷ�
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

//��ʳɽ�
int GetTransaction(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//����
	TDBDefine_ReqTransaction req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
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
		//������¼�ĵ�
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_transaction.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k�����buf instead of ϵͳĬ�ϵ�4k buf,�ٴ������˲�֪��Ϊʲô
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"��ô���,����,ʱ��,�ɽ�����,�ɽ����,������־,�ɽ���,�ɽ���,�������,�������\n");	
		//fprintf(fp,"wind_code,date,time,function_code,order_kind,bs_flag,trade_price,trade_volume,ask_order,bid_order\n");	
		for (int i=0; i<pCount; ++i)
		{
			const TDBDefine_Transaction& trans = pTransaction[i];
			// ���������м���\0����������������ַ������
			fprintf(fp,"%s,%d,%d,%c,%c,%c,%d,%d,%d,%d\n",trans.chWindCode, trans.nDate, trans.nTime, trans.chFunctionCode, trans.chOrderKind, trans.chBSFlag, trans.nTradePrice, trans.nTradeVolume, trans.nAskOrder, trans.nBidOrder);

		}

		//�ͷ�
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

//���ί��
int GetOrder(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//����
	TDBDefine_ReqOrder req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
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
		//������¼�ĵ�
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_order.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k�����buf instead of ϵͳĬ�ϵ�4k buf,�ٴ������˲�֪��Ϊʲô
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"��ô���,����,ʱ��,ί�б��,������ί�к�,ί�����,ί�д���,ί�м�,ί����\n"); 
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
		//�ͷ�
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

//ί�ж���
int GetOrderQueue(THANDLE hTdb, const char* szCode, const char* szMarketKey, int nDate)
{
	//����
	TDBDefine_ReqOrderQueue req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode)); //����д�������ȡ�Ĺ�Ʊ����
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
		//������¼�ĵ�
		char FileName[64];
		sprintf(FileName,"%s%d\\%d_%s%s_orderqueue.csv",dir_output.c_str(),nDate,nDate,strCode.substr(7,2).c_str(),strCode.substr(0,6).c_str());
		fp=fopen(FileName,"w"); 

		char bufw[1024*256];	//256k�����buf instead of ϵͳĬ�ϵ�4k buf,�ٴ������˲�֪��Ϊʲô
		setvbuf(fp, bufw, _IOFBF, sizeof(bufw));

		//fprintf(fp,"wind_code,date,time,side,price,order_items,ab_items,ab1,ab2,ab3,ab4,ab5,ab6,ab7,ab8,ab9,ab10,ab11,ab12,ab13,ab14,ab15,ab16,ab17,ab18,ab19,ab20,ab21,ab22,ab23,ab24,ab25,ab26,ab27,ab28,ab29,ab30,ab31,ab32,ab33,ab34,ab35,ab36,ab37,ab38,ab39,ab40,ab41,ab42,ab43,ab44,ab45,ab46,ab47,ab48,ab49,ab50\n"); 
		//fprintf(fp,"��ô���,����,ʱ��,��������,ί�м۸�,��������,��ϸ����,������ϸ\n");	
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

		//�ͷ�
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
//ָ�깫ʽ
void UseEZFFormula(THANDLE hTdb)
{
	//��ʽ�ı�д����ο�<<TRANSEND-TS-M0001 �ױ๫ʽ������V1(2).0-20110822.pdf>>;
	std::string strName = "KDJ";
	std::string strContent = "INPUT:N(9), M1(3,1,100,2), M2(3);"
		"RSV:=(CLOSE-LLV(LOW,N))/(HHV(HIGH,N)-LLV(LOW,N))*100;"
		"K:SMA(RSV,M1,1);"
		"D:SMA(K,M2,1);"
		"J:3*K-2*D;";

	//��ӹ�ʽ�������������룬�����������д��󷵻�
	TDBDefine_AddFormulaRes* addRes = new TDBDefine_AddFormulaRes;
	int nErr = TDB_AddFormula(hTdb, strName.c_str(), strContent.c_str(),addRes);
	printf("Add Formula Result:%s",addRes->chInfo);

	//��ѯ�������ϵĹ�ʽ���ܿ������Ǹղ��ϴ���"KDJ"
	TDBDefine_FormulaItem* pEZFItem = NULL;
	int nItems = 0;
	//����Ϊ�ձ�ʾ��ѯ�����������еĹ�ʽ
	nErr = TDB_GetFormula(hTdb, NULL, &pEZFItem, &nItems);

	for (int i=0; i<nItems; i++)
	{
		std::string strNameInner(pEZFItem[i].chFormulaName, 0, sizeof(pEZFItem[i].chFormulaName));
		std::string strParam(pEZFItem[i].chParam, 0, sizeof(pEZFItem[i].chParam));
		printf("��ʽ���ƣ�%s, ����:%s \n", strNameInner.c_str(), strParam.c_str());
	}

	struct EZFCycDefine
	{
		char chName[8];
		int  nCyc;
		int  nCyc1;
	}
	EZFCyc[5]={
		{"����", 2, 0},
		{"30��", 0, 30},
		{"5����", 0, 5},
		{"1����", 0, 1},
		{"15��", 11, 15}};

		//��ȡ��ʽ�ļ�����
		TDBDefine_ReqCalcFormula reqCalc = {0};
		strncpy(reqCalc.chFormulaName, "KDJ", sizeof(reqCalc.chFormulaName));
		strncpy(reqCalc.chParam, "N=9,M1=3,M2=3", sizeof(reqCalc.chParam));
		strncpy(reqCalc.chCode, "000001.SZ", sizeof(reqCalc.chCode));
		strncpy(reqCalc.chMarketKey, "SZ-2-0", sizeof(reqCalc.chMarketKey));
		reqCalc.nCycType = (CYCTYPE)(EZFCyc[0].nCyc); //0��ʾ����
		reqCalc.nCycDef = EZFCyc[0].nCyc1; 
		reqCalc.nCQFlag = (REFILLFLAG)0;		  //��Ȩ��־
		reqCalc.nCalcMaxItems = 4000; //��������������
		reqCalc.nResultMaxItems = 100;	//���͵Ľ�������������

		TDBDefine_CalcFormulaRes* pResult = new TDBDefine_CalcFormulaRes;
		nErr = TDB_CalcFormula(hTdb, &reqCalc, pResult);
		//�жϴ������

		printf("��������: %d ��:\n", pResult->nRecordCount);
		char szLineBuf[1024] = {0};
		//����ֶ���
		for (int j=0; j<pResult->nFieldCount;j++)
		{
			std::cout << pResult->chFieldName[j] << "  ";
		}
		std::cout << endl << endl;
		//�������
		for (int i=0; i<pResult->nRecordCount; i++)
		{
			for (int j=0; j<pResult->nFieldCount;j++)
			{
				std::cout << (pResult->dataFileds)[j][i] << "  ";
			}
			std::cout << endl;
		}

		//ɾ��֮ǰ�ϴ��Ĺ�ʽָ��
		TDBDefine_DelFormulaRes pDel = {0};
		nErr = TDB_DeleteFormula(hTdb, "KDJ", &pDel);
		printf("ɾ��ָ����Ϣ:%s", pDel.chInfo);
		//�ͷ��ڴ�
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