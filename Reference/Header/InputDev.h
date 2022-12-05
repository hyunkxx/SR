#pragma once

#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev :	public CBase
{
	DECLARE_SINGLETON(CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev();

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	KEY_STATE	Get_DIKeyState_Custom(_int _key) { return m_strKeyList[_key].eKeyState; }

	
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long*)&m_MouseState) + eMoveState);
	}

public:
	HRESULT			Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void			Update_InputDev(void);
	void			SetUp_InputDev(void);

private:
	_byte			m_byKeyState[256];		// 모든 키보드의 입력 값을 저장하기 위한 배열
	tagKey			m_strKeyList[256];
	DIMOUSESTATE	m_MouseState;			// 마우스 메세지가 발생했을 때 마우스 정보를 보관하는 구조체

	LPDIRECTINPUT8				m_pInputSDK;
	
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

public:
	virtual void		Free(void);
};

END