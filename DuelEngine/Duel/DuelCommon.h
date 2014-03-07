/*
	[7/15/2012 OMEGA] created

	Used for defs and constants.
	
*/


#ifndef _DUELCOMMON_H_
#define _DUELCOMMON_H_

/////////////////////////////////
// config 
#include "DuelConfig.h"
/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// [4/2/2013 OMEGA]
// ������ı�̹淶 v1.0:
// 1. ���������ж������, ���ⲿ�Ѷ���Ķ���, ������ͳһʹ����D��ͷ, �������ڶ���ĸ������ͻ���enum�Ϳ��Բ���Ҫ��.
// 2. ���Ա����ͳһʹ��Сд���ʿ�ͷ��Camel����.
// 3. ���Ա����ͳһʹ��m��ͷ, ����bool�ͱ���, ͳһ��mb��ͷ.
// 4. enum�ͱ�����ȫ��ʹ������ĸ��д��Camel����, ������enum���ֵ�ÿ�����ʵ�����ĸ��ϼ��»�����Ϊֵ�Ŀ�ͷ.

//////////////////////////////////
#include <stdlib.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
//#include <cReal>
#include <iomanip>
#include <io.h>
//////////////////////////////////
#ifdef DUEL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

//////////////////////////////////
// STL include
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>	// hashmap
#define HashMap std::tr1::unordered_map
#include <unordered_set>	// hashset
#define	HashSet	std::tr1::unordered_set
#include <set>
#include <list>
#include <deque>
#include <bitset>
#include <limits>
//////////////////////////////////
// boost config
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#include <wrl.h>
#include <wrl/client.h>
#include <Windows.Foundation.h>
#include <windows.storage.h>
#include <windows.applicationmodel.h>
#else
//#include <boost/config/user.hpp>
#include <boost/config/user.hpp>
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8



#include "DuelType.h"
#include "DuelResultCode.h"
#include "DuelSTLWrapper.h"
#include "DuelSignal.h"
#include "DuelPreDeclare.h"
#include "DuelRTTI.h"
#include "DuelObject.h"
#include "DuelMathLib.h"
#include "DuelThreadLib.h"
#include "DuelXML.h"
#include "DuelString.h"
#include "DuelSingleton.h"


#endif