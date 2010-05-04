/*============================================
*
* file:   DataBlockSetWriteRead.h
*
* brief:  �й̶���С���ڴ棬��̬�������ݿ�Ļ�������
*
* Authtor:Expter
*
* Data:   20090616
*
* Modity: //20090818
===========================================*/

#pragma once

typedef unsigned char BYTE;

class CDataBlockPool
{
public:
	CDataBlockPool(long MaxSize);

	long m_lTestFlag;
public:
	virtual ~CDataBlockPool(void);

    /// user define method
public:
	BYTE* Base()      { return m_pBase;}

	long  GetCurSize() { return m_nCurSize; }
	void  SetCurSize(long lSize);

	long  GetMaxSize() { return m_nMaxSize; }


	/// user define val
private:
	BYTE * m_pBase;      /// ������ʼ��ַ
	long   m_nCurSize;   /// ��ǰ���ݿ��С
	long   m_nMaxSize;   /// �����ݴ�С


	/// user define private method
private:
	/// ������Щ����
	CDataBlockPool &operator = (const CDataBlockPool &);
	CDataBlockPool(const CDataBlockPool&);

};
