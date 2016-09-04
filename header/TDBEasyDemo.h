
#ifndef _TDBEASYDEMO_H_
#define _TDBEASYDEMO_H_

#include "TDBFunction.h"
#include "TDBAPI.h"

#include <string>  
#include <vector>
#include <iostream>  
#include <fstream>  

using namespace std;

int LoadStkDateDirConfig(vector<string>& stkList, vector<string>::size_type& stkStart, vector<string>::size_type& stkEnd, vector<string>& dateList, string& dateStart, string& dateEnd, string& dir_output); 
int LoadUserConfig(OPEN_SETTINGS& settings);
int LogInWithUserConfig(THANDLE& hTdb, OPEN_SETTINGS& settings);
int MakeLocalLogFile(ofstream& filelog);
int ResponseToTDBReturn(int& nRet, THANDLE& hTdb, OPEN_SETTINGS& settings);
int ProcessTodayDir(string today_str);
int LoadDownloadList(vector<string>& list, vector<string>::size_type& listStart, vector<string>::size_type& listEnd, string& dir_output);

#endif //_TDBEASYDEMO_H_