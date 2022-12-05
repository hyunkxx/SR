#include "..\..\Header\KeyMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
}
CKeyMgr::~CKeyMgr()
{
}

HRESULT CKeyMgr::Ready_KeyMgr(void)
{
	ZeroMemory(&vecKeyList, sizeof(vecKeyList));

	return S_OK;
}

void CKeyMgr::Update_KeyMgr(void)
{
	for (int i = 0; VK_MAX > i; i++)
	{

		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (vecKeyList[i].bPrevPush)
			{
				vecKeyList[i].eKeyState = KEY_STATE::HOLD;
			}
			else
			{
				vecKeyList[i].eKeyState = KEY_STATE::TAP;
			}
			vecKeyList[i].bPrevPush = true;
		}
		else
		{
			if (vecKeyList[i].bPrevPush)
			{
				vecKeyList[i].eKeyState = KEY_STATE::AWAY;
			}
			else
			{
				vecKeyList[i].eKeyState = KEY_STATE::NONE;
			}
			vecKeyList[i].bPrevPush = false;
		}
	}
}

void CKeyMgr::Free(void)
{

}
