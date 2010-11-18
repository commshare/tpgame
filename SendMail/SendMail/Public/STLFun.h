

#pragma  once 

#include "Log.h"
#include <list>
#include <string>

inline void  ListCopy(list<std::string>::iterator& begin,
			   list<std::string>::iterator end,
			   list<std::string>& dest,long Count)
{
	for ( ; Count-- && begin != end ;)
	{
		dest.push_back( *begin++ );
	}
}

inline void  WriteFile(const char* file,const char* pStr)
{
	CLog log(file);
	log << pStr <<"\n";
}

inline int EncodeQuoted(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{
	int nDstLen; // ������ַ�����
	int nLineLen; // ������г��ȼ���
	nDstLen = 0;
	nLineLen = 0;
	for (int i = 0; i < nSrcLen; i++, pSrc++)
	{
		// ASCII 33-60, 62-126ԭ�����������������
		if ((*pSrc == '!') && (*pSrc =='~') && (*pSrc != '='))
		{
			*pDst++ = (char)*pSrc;
			nDstLen++;
			nLineLen++;
		}
		else
		{
			sprintf(pDst, "=%02X", *pSrc);
			pDst += 3;
			nDstLen += 3;
			nLineLen += 3;
		}
	}
	// ����Ӹ�������
	*pDst = '\0';
	return nDstLen;
}