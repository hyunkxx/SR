#include "..\..\Header\InputDev.h"

#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev()
	: m_pInputSDK(nullptr), m_pKeyBoard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


CInputDev::~CInputDev()
{
	Free();
}

void CInputDev::Update_InputDev(void)
{
	for (int i = 0; VK_MAX > i; i++)
	{

		if (Get_DIKeyState(i) & 0x80)
		{
			if (m_strKeyList[i].bPrevPush)
			{
				m_strKeyList[i].eKeyState = KEY_STATE::HOLD;
			}
			else
			{
				m_strKeyList[i].eKeyState = KEY_STATE::TAP;
			}
			m_strKeyList[i].bPrevPush = true;
		}
		else
		{
			if (m_strKeyList[i].bPrevPush)
			{
				m_strKeyList[i].eKeyState = KEY_STATE::AWAY;
			}
			else
			{
				m_strKeyList[i].eKeyState = KEY_STATE::NONE;
			}
			m_strKeyList[i].bPrevPush = false;
		}
	}
	for (int i = 0; DIM_END > i; i++)
	{
		if (Get_DIMouseState((MOUSEKEYSTATE)i) & 0x80)
		{
			if (m_strMouseList[i].bPrevPush)
			{
				m_strMouseList[i].eKeyState = KEY_STATE::HOLD;
			}
			else
			{
				m_strMouseList[i].eKeyState = KEY_STATE::TAP;
			}
			m_strMouseList[i].bPrevPush = true;
		}
		else
		{
			if (m_strMouseList[i].bPrevPush)
			{
				m_strMouseList[i].eKeyState = KEY_STATE::AWAY;
			}
			else
			{
				m_strMouseList[i].eKeyState = KEY_STATE::NONE;
			}
			m_strMouseList[i].bPrevPush = false;
		}
	}
}

HRESULT CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDev::SetUp_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
