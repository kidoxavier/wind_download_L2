
#ifndef _TDBFUNCTION_H_
#define _TDBFUNCTION_H_

#include "TDBAPI.h"
#include <string>  

//定义证券种类
#define ID_BASECLASS_INDEX 	0x00
#define ID_BASECLASS_SHARES 0x10
#define ID_BASECLASS_FUND 	0x20
#define ID_BASECLASS_BOND 	0x30
#define ID_BASECLASS_REPO 	0x40
#define ID_BASECLASS_QZ 	0x60
#define ID_BASECLASS_FUTURES 0x70
#define ID_BASECLASS_FOREX 	0x80
#define ID_BASECLASS_OPTION 0x90
#define ID_BASECLASS_BANKRATE 0xd0
#define ID_BASECLASS_NMETAL 0xe0
#define ID_BTEX_HASWARRANT 	0x80000000
#define ID_BTEX_DELETE 		0x40000000
#define ID_BTEX_DELISTING 	0x08000000
#define ID_BTEX_RISKWARNING 0x04000000
#define ID_BASECLASS_OTHER 	0xf0

#define ELEMENT_COUNT(arr) (sizeof(arr)/sizeof(*arr))

#define MAX_STOCK_NUM 2500

#define NUM_ORDER_QUEUE 50
#define NUM_ASKBID_LEVEL 10

void InitSetting(OPEN_SETTINGS* settings,  TDB_PROXY_SETTING* proxy_settings);
int GetKData(THANDLE hTdb, char* szCode, char* szMarket, int nBeginDate, int nEndDate, int nCycle, int nUserDef, int nCQFlag, int nAutoComplete);
int GetTickData(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate);//带买卖盘的tick
int GetTransaction(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate); //逐笔成交
int GetOrder(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate);//逐笔委托
int GetOrderQueue(THANDLE hTdb, const char* szCode, const char* szMarket, int nDate);//委托队列
void UseEZFFormula(THANDLE hTdb);
void GetCodeTable(THANDLE hTdb, char* szMarket);
int GetCodeTableFromConfigFile(const char* szMarket, TDBDefine_Code (&pCodeTable)[MAX_STOCK_NUM], int & pCount);

std::string array2str(const int* arr, int len);
std::string uarr2str(unsigned int arr[], int n);
std::string array2strn(const int* arr, int len, int totalnum);
std::string tmp_int2str(int n);
std::string arr2str(int arr[], int n);
std::string int2str(int n);
std::string OutputLocalTime(void);

#endif //_TDBFUNCTION_H_