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
	KEY_STATE	Get_DIKeyState_Custom(_int _key)
	{
		if (256 < _key || 0 > _key)
			return KEY_STATE::AWAY;

		return m_strKeyList[_key].eKeyState;
	}
	KEY_STATE	Get_DIMouseState_Custom(MOUSEKEYSTATE _key)
	{
		if (DIM_END <= _key || 0 > _key)
			return KEY_STATE::AWAY;

		return m_strMouseList[_key].eKeyState;
	}
	
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
	_byte			m_byKeyState[256];		// ��� Ű������ �Է� ���� �����ϱ� ���� �迭
	tagKey			m_strKeyList[256];
	tagKey			m_strMouseList[DIM_END];
	DIMOUSESTATE	m_MouseState;			// ���콺 �޼����� �߻����� �� ���콺 ������ �����ϴ� ����ü

	LPDIRECTINPUT8				m_pInputSDK;
	
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

public:
	virtual void		Free(void);
};

END