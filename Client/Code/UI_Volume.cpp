#include "stdafx.h"
#include "..\Header\UI_Volume.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"

_float CUI_Volume:: s_fAllSound;
_float CUI_Volume::s_fShotSound;
_float CUI_Volume::s_fBGMSound;

CUI_Volume::CUI_Volume(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_Volume::CUI_Volume(const CUI_Volume& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_Volume::~CUI_Volume()
{
}

HRESULT CUI_Volume::Ready_Object()
{
	// memset
	//std::fill(&m_pButoonTexture[VOLUME_DOWN][ALL_SOUND], &m_pButoonTexture[VOLUME_END][END_SOUND], nullptr);
	//std::fill(&m_pButtonTransform[VOLUME_DOWN][ALL_SOUND], &m_pButtonTransform[VOLUME_DOWN][ALL_SOUND], nullptr);
	//std::fill(&m_pTransform[VOLUME_BACKGROUND], &m_pTransform[VOLUME_RC_END], nullptr);

	memset(&m_pButoonTexture, reinterpret_cast<_int>(nullptr), sizeof(m_pButoonTexture));
	memset(&m_pButtonTransform, reinterpret_cast<_int>(nullptr), sizeof(m_pButtonTransform));

	/*for (_int i = 0; i < END_SOUND; ++i)
	{
		for (_int j = 0; j < VOLUME_END; ++j)
		{
			m_pButoonTexture[i][j] = nullptr;
			m_pButtonTransform[i][j] = nullptr;
		}
	}*/
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
//	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	for (_int i = 0; i < 7; ++i)
	{
		D3DXMatrixOrthoLH(&m_matProj[i], WINCX, WINCY, 0.f, 1.f);
	}
	
	// 전체 창 
	m_fScaleX = 200.f;
	m_fScaleY = 200.f;
	m_fScaleZ = 0.1f;

	m_fPosX = 400.f;
	m_fPosY = 270.f;
	m_fPosZ = 0.3f;
		
	// 단추
	m_RatioSizeY = m_RatioSizeX = 16.f;
	m_RatioSizeZ = 1.f;

	m_RatioX[VOLUME_DOWN] = 50.f;
	m_RatioX[VOLUME_UP] = 150.f;
	m_RatioY[ALL_SOUND] = PERCENTY * 15.f;
	m_RatioY[BGM_SOUND] = 0.f;
	m_RatioY[SHOT_SOUND] = PERCENTY * 15.f;
	m_RatioZ = 0.1f;
	
	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	for (_int i = 0; i < 3; ++i)
	{
		for (_int j = 0; j < 2; ++j)
		{
			m_pButtonTransform[i][j]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
		}
	}
	/*m_pButtonTransform[ALL_SOUND][VOLUME_DOWN]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
	m_pButtonTransform[ALL_SOUND][VOLUME_UP]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
	m_pButtonTransform[BGM_SOUND][VOLUME_DOWN]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
	m_pButtonTransform[BGM_SOUND][VOLUME_UP]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
	m_pButtonTransform[SHOT_SOUND][VOLUME_DOWN]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
	m_pButtonTransform[SHOT_SOUND][VOLUME_UP]->Set_Scale(m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ);
*/

	lstrcpy(szVolumeOption, L"Volume Option");
	lstrcpy(szALLVolumeSound, L"ALL Sound Volume");
	lstrcpy(szStageVolumeSound, L"BGM Sound Volume");
	lstrcpy(szShotVolumeSound, L"Shot Sound Volume");
	lstrcpy(szGameContinue, L"Game Continue");
	lstrcpy(szGameEscape, L"Game Escape");
	

	CUI_Volume::s_fAllSound = 0.4f;
	CUI_Volume::s_fShotSound = 0.4f;
	CUI_Volume::s_fBGMSound = 0.4f;

	szAllSoundCount = to_wstring(_int(CUI_Volume::s_fAllSound * 100.f));
	szBGMSoundCount = to_wstring(_int(CUI_Volume::s_fShotSound * 100.f));
	szShotSoundCount = to_wstring(_int(CUI_Volume::s_fBGMSound * 100.f));
	return S_OK;
}



_int CUI_Volume::Update_Object(const _float & fTimeDelta)
{

	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		if (Get_DIKeyState_Custom(DIK_F3) == KEY_STATE::TAP)
		{
			m_bShow = !m_bShow;
			ShowCursor(m_bShow);
			static_cast<CTankCamera*>(Engine::Get_Camera(L"TankCamera"))->Set_MouseFix(m_bShow);
		}

		// f3을 눌러서 on이 되면
		if (m_bShow)
		{
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);
			Update_Pos();
			Move_Pos();
			Sound_Volume_Control();
		}

		__super::Update_Object(fTimeDelta);
	}
	return 0;
}

void CUI_Volume::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	Add_RenderGroup(RENDER_UI, this);
}

void CUI_Volume::Render_Object(void)
{
	_matrix OldViewMatrix, OldProjMatrix;


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);


	if (m_bShow)
	{
		Render_Font(L"Font_Retro", szVolumeOption, &_vec2(m_fPosX - ( PERCENTX * 6.f) , m_fPosY - (PERCENTY * 30.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", szALLVolumeSound, &_vec2(m_fPosX - (PERCENTX * 22.f), m_fPosY - (PERCENTY * 17.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", szStageVolumeSound, &_vec2(m_fPosX - (PERCENTX * 22.f), m_fPosY - (PERCENTY * 2.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", szShotVolumeSound, &_vec2(m_fPosX - (PERCENTX * 22.f), m_fPosY + (PERCENTY * 13.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", szGameContinue, &_vec2(m_fPosX - (PERCENTX * 22.f), m_fPosY + (PERCENTY * 26.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", szGameEscape, &_vec2(m_fPosX + (PERCENTX * 9.f ), m_fPosY + (PERCENTY * 26.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		Render_Font(L"Font_Retro", (szAllSoundCount).c_str(), &_vec2(m_fPosX + (PERCENTX * 12.f), m_fPosY - (PERCENTY * 17.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", (szBGMSoundCount).c_str(), &_vec2(m_fPosX + (PERCENTX * 12.f), m_fPosY - (PERCENTY * 2.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		Render_Font(L"Font_Retro", (szShotSoundCount).c_str(), &_vec2(m_fPosX + (PERCENTX * 12.f), m_fPosY + (PERCENTY * 13.f)), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));



		_matrix			ViewMatrix[7];

		for (_int i = 0; i < 7; ++i)
		{
			D3DXMatrixIdentity(&ViewMatrix[i]);
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix[0]);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj[0]);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();

		for (_int i = 0; i < END_SOUND; ++i)
		{
			for (_int j = 0; j < VOLUME_END; ++j)
			{

				m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pButtonTransform[i][j]->Get_WorldMatrix());

				m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix[1 + (i * 2) + (j)]);
				m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj[1 + (i * 2) + (j)]);

				m_pButoonTexture[i][j]->Set_Texture(0);
				m_pRcTex->Render_Buffer();
			}
		}
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);
}

CUI_Volume * CUI_Volume::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Volume*		pInstance = new CUI_Volume(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("World_Bubble Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Volume::Move_Pos(void)
{
	// Move Pos Box
	if (PtInRect(&rc[8], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB))
		{
			_float fX, fY;

			fX = m_fPosX - ptMouse.x;
			fY = m_fPosY - ptMouse.y;

			m_fPosX = (ptMouse.x - fX);
			m_fPosY = (ptMouse.y + fY);
		}
	}


}

void CUI_Volume::Sound_Volume_Control(void)
{


	// all sound
	if (PtInRect(&rc[0], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB) )
		{
			s_fAllSound = s_fAllSound - 0.01f;

			if (s_fAllSound <= 0.f)
			{s_fAllSound = 0.f;	}

			s_fBGMSound = s_fShotSound = s_fAllSound;
			szShotSoundCount = szBGMSoundCount = szAllSoundCount = to_wstring(_int(s_fAllSound * 100.f));
		}
	}
	if (PtInRect(&rc[1], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB))
		{
			s_fAllSound = s_fAllSound + 0.01f;

			if (s_fAllSound >= 1.f)
			{	s_fAllSound = 1.f;}

			s_fBGMSound = s_fShotSound = s_fAllSound;
			szShotSoundCount = szBGMSoundCount = szAllSoundCount = to_wstring(_int(s_fAllSound * 100.f));
		}
	}

	// bgm sound
	if (PtInRect(&rc[2], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB) )
		{
			s_fBGMSound = s_fBGMSound - 0.01f;

			if (s_fBGMSound <= 0.f)
			{
				s_fBGMSound = 0.f;
			}
			szBGMSoundCount = to_wstring(_int(s_fBGMSound * 100.f));

		}
	}
	if (PtInRect(&rc[3], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB) )
		{
			s_fBGMSound = s_fBGMSound + 0.01f;

			if (s_fBGMSound >= 1.f)
			{
				s_fBGMSound = 1.f;
			}
			szBGMSoundCount = to_wstring(_int(s_fBGMSound * 100.f));
		}
	}

	// shot sound
	if (PtInRect(&rc[4], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB) )
		{
			s_fShotSound = s_fShotSound - 0.01f;

			if (s_fShotSound <= 0.f)
			{
				s_fShotSound = 0.f;
			}
			szShotSoundCount = to_wstring(_int(s_fShotSound * 100.f));
		}
	}
	if (PtInRect(&rc[5], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB) )
		{
			s_fShotSound = s_fShotSound + 0.01f;

			if (s_fShotSound >= 1.f)
			{
				s_fShotSound = 1.f;
			}
			szShotSoundCount = to_wstring(_int(s_fShotSound * 100.f));
		}
	}
	// Continue Box
	if (PtInRect(&rc[6], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB))
		{
			m_bShow = false;
			ShowCursor(m_bShow);
			static_cast<CTankCamera*>(Engine::Get_Camera(L"TankCamera"))->Set_MouseFix(m_bShow);
		}
	}

	// Escape Box
	if (PtInRect(&rc[7], ptMouse))
	{
		if (Get_DIMouseState(DIM_LB))
		{
			DestroyWindow(g_hWnd);
		}
	}

}


HRESULT CUI_Volume::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VolumeTransform", pComponent });

	// ????? 이유를 모르겠지만, 인식을 제대로 못한다. for문에서 강제로 탈출을 하든, 값이 제대로 입력이 안되는 듯.
	// 계속 돌려보면, 버튼 위치가 제각각 (그러는 와중에 1 2 번은 고정이다. 참 기이할세.)
	/*wstring szVolumeUPDOWN,  szTransform;

	for (_int i = 0; i < END_SOUND; ++i)
	{
		for (_int j = 0; j < VOLUME_END; ++j)
		{
			if (j == 0)
			{
				szVolumeUPDOWN = L"Proto_Volume_Down_Tex" + to_wstring(i);
				szTransform = L"Proto_S_V_D_Transform" + to_wstring(i);
			}
			else if (j == 1)
			{
				szVolumeUPDOWN = L"Proto_Volume_Up_Tex" + to_wstring(i);
				szTransform = L"Proto_S_V_U_Transform" + to_wstring(i);
			}

			pComponent = m_pButoonTexture[i][j] = static_cast<CTexture*>(Clone_Prototype((szVolumeUPDOWN).c_str()));
			NULL_CHECK_RETURN(m_pButoonTexture[i][j], E_FAIL);
			m_mapComponent[ID_DYNAMIC].insert({((szVolumeUPDOWN).c_str()), pComponent });

			pComponent = m_pButtonTransform[i][j] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
			NULL_CHECK_RETURN(m_pButtonTransform[i][j], E_FAIL);
			m_mapComponent[ID_DYNAMIC].insert({((szTransform).c_str()), pComponent });
		}
	}*/

	pComponent = m_pButoonTexture[ALL_SOUND][VOLUME_DOWN] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Down_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[ALL_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Down_Tex0", pComponent });

	pComponent = m_pButoonTexture[ALL_SOUND][VOLUME_UP] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Up_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[ALL_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Up_Tex0", pComponent });

	pComponent = m_pButoonTexture[BGM_SOUND][VOLUME_DOWN] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Down_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[BGM_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Down_Tex1", pComponent });

	pComponent = m_pButoonTexture[BGM_SOUND][VOLUME_UP] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Up_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[BGM_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Up_Tex1", pComponent });

	pComponent = m_pButoonTexture[SHOT_SOUND][VOLUME_DOWN] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Down_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[SHOT_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Down_Tex2", pComponent });

	pComponent = m_pButoonTexture[SHOT_SOUND][VOLUME_UP] = static_cast<CTexture*>(Clone_Prototype(L"Proto_Volume_Up_Tex0"));
	NULL_CHECK_RETURN(m_pButoonTexture[SHOT_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Volume_Up_Tex2", pComponent });




	pComponent = m_pButtonTransform[ALL_SOUND][VOLUME_DOWN] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[ALL_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_D_Transform0" , pComponent });

	pComponent = m_pButtonTransform[ALL_SOUND][VOLUME_UP] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[ALL_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_U_Transform0" , pComponent });

	pComponent = m_pButtonTransform[BGM_SOUND][VOLUME_DOWN] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[BGM_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_D_Transform1" , pComponent });

	pComponent = m_pButtonTransform[BGM_SOUND][VOLUME_UP] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[BGM_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_U_Transform1" , pComponent });

	pComponent = m_pButtonTransform[SHOT_SOUND][VOLUME_DOWN] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[SHOT_SOUND][VOLUME_DOWN], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_D_Transform2" , pComponent });

	pComponent = m_pButtonTransform[SHOT_SOUND][VOLUME_UP] = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pButtonTransform[SHOT_SOUND][VOLUME_UP], E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_S_V_U_Transform2" , pComponent });


	return S_OK;
}

void CUI_Volume::Free(void)
{
	for (_int i = 0; i < 3; ++i)
	{
		for (_int j = 0; j < 2; ++j)
		{

			Safe_Release(m_pButoonTexture[i][j]);
			Safe_Release(m_pButtonTransform[i][j]); // 2개는 왜 해제를 못하지??????
		}
	}
	__super::Free();
	
}

void CUI_Volume::Update_Pos(void)
{
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	m_pButtonTransform[ALL_SOUND][VOLUME_DOWN]->Set_Pos(m_fPosX + m_RatioX[VOLUME_DOWN] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY - m_RatioY[ALL_SOUND]), m_fPosZ - m_RatioZ);
	m_pButtonTransform[ALL_SOUND][VOLUME_UP]->Set_Pos(m_fPosX + m_RatioX[VOLUME_UP] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY - m_RatioY[ALL_SOUND]), m_fPosZ - m_RatioZ);

	m_pButtonTransform[BGM_SOUND][VOLUME_DOWN]->Set_Pos(m_fPosX + m_RatioX[VOLUME_DOWN] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY - m_RatioY[BGM_SOUND]), m_fPosZ - m_RatioZ);
	m_pButtonTransform[BGM_SOUND][VOLUME_UP]->Set_Pos(m_fPosX + m_RatioX[VOLUME_UP] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY - m_RatioY[BGM_SOUND]), m_fPosZ - m_RatioZ);

	/*m_pButtonTransform[SHOT_SOUND][VOLUME_DOWN]->Set_Pos(m_fPosX + m_RatioX[VOLUME_DOWN] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY) +(m_RatioY[SHOT_SOUND]), m_fPosZ - m_RatioZ);
	m_pButtonTransform[SHOT_SOUND][VOLUME_UP]->Set_Pos(m_fPosX + m_RatioX[VOLUME_UP] - (WINCX * 0.5f), (WINCY * 0.5f) - (m_fPosY )+ (m_RatioY[SHOT_SOUND]), m_fPosZ - m_RatioZ);*/


	m_pButtonTransform[SHOT_SOUND][VOLUME_DOWN]->Set_Pos(m_fPosX + m_RatioX[VOLUME_DOWN] - (WINCX * 0.5f), (WINCY * 0.5f)  - m_fPosY - m_RatioY[SHOT_SOUND], m_fPosZ - m_RatioZ);
	m_pButtonTransform[SHOT_SOUND][VOLUME_UP]->Set_Pos(m_fPosX + m_RatioX[VOLUME_UP] - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY - m_RatioY[SHOT_SOUND], m_fPosZ - m_RatioZ);


	// all down
	SetRect(&rc[0] , _int((m_fPosX + m_RatioX[VOLUME_DOWN]) - m_RatioSizeX)	, _int((m_fPosY - m_RatioY[ALL_SOUND]) - m_RatioSizeY) , _int((m_fPosX + m_RatioX[VOLUME_DOWN]) + m_RatioSizeX) , _int((m_fPosY - m_RatioY[ALL_SOUND]) + m_RatioSizeY));

	// all up
	SetRect(&rc[1] , _int((m_fPosX + m_RatioX[VOLUME_UP]) - m_RatioSizeX) , _int((m_fPosY - m_RatioY[ALL_SOUND]) - m_RatioSizeY) 	, _int((m_fPosX + m_RatioX[VOLUME_UP]) + m_RatioSizeX) , _int((m_fPosY - m_RatioY[ALL_SOUND]) + m_RatioSizeY));

	// bgm down
	SetRect(&rc[2] , _int((m_fPosX + m_RatioX[VOLUME_DOWN]) - m_RatioSizeX)	, _int((m_fPosY - m_RatioY[BGM_SOUND]) - m_RatioSizeY)	, _int((m_fPosX + m_RatioX[VOLUME_DOWN]) + m_RatioSizeX)	, _int((m_fPosY - m_RatioY[BGM_SOUND]) + m_RatioSizeY));

	// bgm up
	SetRect(&rc[3] , _int((m_fPosX + m_RatioX[VOLUME_UP]) - m_RatioSizeX)	, _int((m_fPosY - m_RatioY[BGM_SOUND]) - m_RatioSizeY)	, _int((m_fPosX + m_RatioX[VOLUME_UP]) + m_RatioSizeX) , _int((m_fPosY - m_RatioY[BGM_SOUND]) + m_RatioSizeY));

	// shot down
	SetRect(&rc[4] , _int((m_fPosX + m_RatioX[VOLUME_DOWN]) - m_RatioSizeX)	, _int((m_fPosY + m_RatioY[SHOT_SOUND]) - m_RatioSizeY) , _int((m_fPosX + m_RatioX[VOLUME_DOWN]) + m_RatioSizeX) , _int((m_fPosY + m_RatioY[SHOT_SOUND]) + m_RatioSizeY));

	//  shot up
	SetRect(&rc[5] , _int((m_fPosX + m_RatioX[VOLUME_UP]) - m_RatioSizeX) , _int((m_fPosY + m_RatioY[SHOT_SOUND]) - m_RatioSizeY) , _int((m_fPosX + m_RatioX[VOLUME_UP]) + m_RatioSizeX) , _int((m_fPosY + m_RatioY[SHOT_SOUND]) + m_RatioSizeY));

	// Continue Box
	SetRect(&rc[6] , _int(m_fPosX - PERCENTX * 22.f) , _int(m_fPosY + PERCENTY * 26.f)	, _int(m_fPosX  - PERCENTX * 6.5f)	, _int(m_fPosY + PERCENTY * 31.f));

	// Escape Box
	SetRect(&rc[7] , _int(m_fPosX + PERCENTX * 9.f) , _int(m_fPosY + PERCENTY * 26.f) , _int(m_fPosX + PERCENTX * 22.f) , _int(m_fPosY + PERCENTY * 31.f));

	// move Box
	SetRect(&rc[8] , _int(m_fPosX - PERCENTX * 6.f)	, _int(m_fPosY - PERCENTY * 30.f)	, _int(m_fPosX + PERCENTX * 9.f)	, _int(m_fPosY - PERCENTY * 25.f));

}
