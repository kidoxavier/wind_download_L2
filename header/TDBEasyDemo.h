
#ifndef _TDBEASYDEMO_H_
#define _TDBEASYDEMO_H_

#include <conio.h>
//#include "iostream"
#include <string>
#include <algorithm>
#include <assert.h>
#include <time.h>
#include <iostream>  
#include <fstream>  
#include <stdlib.h>  
#include <io.h>
#include <direct.h>
#include "Windows.h"

#include "TDBFunction.h"
#include "TDBAPI.h"

int LoadStkDateDirConfig(vector<string>& stkList, vector<string>::size_type& stkStart, vector<string>::size_type& stkEnd, vector<string>& dateList, string& dateStart, string& dateEnd, string& dir_output); 
int LoadUserConfig(OPEN_SETTINGS& settings);
int LogInWithUserConfig(OPEN_SETTINGS& settings);
int MakeLocalLogFile(ofstream& filelog);
int ResponseToTDBReturn(int& nRet, THANDLE& hTdb, OPEN_SETTINGS& settings);

#endif //_TDBEASYDEMO_H_