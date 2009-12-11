/*
 *	 Aiϵͳ�Ĺ������ֶ���
 */

#pragma  once

#include "stdafx.h"

/*
 *	 ����2ά����������
 */
const  int	RunWay_X [9] = { 0,-1,-1,-1, 0, 1, 1, 1, 0 };
const  int	RunWay_Y [9] = { 1, 1, 0,-1,-1,-1, 0, 1, 0 };

/*
 *	 ����һ������Ŀ�꣬�������ֵ
 */
const  int  Max_DirSize =  8;

/*
 *	��Ai ��������
 */
enum OPERATE_RESULT
{
	OR_OK								=  0,	// �ɹ�
	OR_ERROR							= -1,	// δ֪����
	OR_DIE								= -2,	// ��������
	OR_TARGET_DIE						= -3,	// Ŀ��������
	OR_LACK_MANA						= -4,	// ��������
	OR_NO_TARGET						= -5,	// û��Ŀ��
	OR_INVALID_TARGET					= -6,	// ��ЧĿ��
	OR_OUT_RANGE						= -7,	// ������Χ
	OR_IN_RANGE							= -8,	// ����̫��
	OR_BUSY								= -9,	// ��������������
};

//  ���Ŀ��nToX,nToY�ķ���
inline int  GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)
{
	if(nFromX < nToX)
	{
		if(nFromY < nToY)
			return 7;
		else if(nFromY > nToY)
			return 5;
		else
			return 6;
	}
	else if(nFromX > nToX)
	{
		if(nFromY < nToY)
			return 2;
		else if(nFromY > nToY)
			return 4;
		else
			return 3;
	}
	else // if(nFromX == nToX)
	{
		if(nFromY < nToY)
			return 0;
		else if(nFromY > nToY)
			return 4;
	}
	return Max_DirSize;
}
