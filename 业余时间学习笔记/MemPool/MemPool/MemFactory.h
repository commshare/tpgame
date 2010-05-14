
#pragma  once 

#include "Pool\BlockPool.h"
#include "Pool\HeapPool.h"

class CMemFactory
{
	
public:
	CMemFactory();

	~CMemFactory(){};

	char*     Alloc(unsigned long size);

	void      Free(void* pAddr, unsigned long size);
	
private:
	/// ���ַ��䷽ʽ
	BlockPool  m_Block;
	HeapPool   m_Heap;
};