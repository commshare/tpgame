// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <fstream>
#include <iostream>
#include <stdio.h>

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

using namespace std;


#define PutFileLog(str)\
	{\
		std::cout << str << std::endl;\
	}
#define PutLog(file,str)\
	{\
		std::cout << str << std::endl;\
	}

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
