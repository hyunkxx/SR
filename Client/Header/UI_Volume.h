#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Volume : public Engine::CGameObject
{
private:
	explicit CUI_Volume(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Volume(const CUI_Volume& rhs);
	virtual ~CUI_Volume();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Volume*		Create(LPDIRECT3DDEVICE9 pGraphicDev);


	void				Update_Pos(void);
	void				Update_Move_Pos(void);
	void				 Sound_Volume_Control(void);
	_bool			Get_Volume_Show(void) { return m_bShow; }


private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;

	enum VOILUME_TYPE { VOLUME_DOWN, VOLUME_UP, VOLUME_END };
	enum SOUND_VOLUME_TYPE { ALL_SOUND, BGM_SOUND, SHOT_SOUND, END_SOUND };
	enum GAME_INOUT { GAME_CONTINUE, GAME_ESCAPE, GAME_INOUT_END };

	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTexture*					m_pButoonTexture[END_SOUND][VOLUME_END];
	CTransform*				m_pTransform = nullptr;
	CTransform*				m_pButtonTransform[END_SOUND][VOLUME_END];


	_matrix  m_matProj[7];
private:

	// 전체 창 크기
	RECT					rc[9];
	POINT					ptMouse;

	_float					m_fScaleX, m_fScaleY, m_fScaleZ;
	_float					m_fPosX, m_fPosY, m_fPosZ;

	// 단추 키 _ 고정 크기 , _ 고정 위치(UP과 down은 x값만 다르고, 각 단추는 y값만 다름)
	_float					m_RatioSizeX, m_RatioSizeY, m_RatioSizeZ; // 단추 크기
	_float					m_RatioX[VOLUME_END]; // Up or Down
	_float					m_RatioY[END_SOUND];	 // All , BGM , SHOT
	_float					m_RatioZ;

	//UI 창 보이기
	_bool					m_bShow = false;

	// ui창 옮기기
	_bool					m_bMovePos = false;
	_float					m_fMovePosY;



	//
	_tchar szVolumeOption[14];
	_tchar szALLVolumeSound[17];
	_tchar szStageVolumeSound[17];
	_tchar szShotVolumeSound[18];
	_tchar szGameContinue[14];
	_tchar szGameEscape[12];

	wstring szAllSoundCount;
	wstring szBGMSoundCount;
	wstring szShotSoundCount;
public:
	// 음량 조절
	static _float s_fAllSound;
	static _float s_fShotSound;
	static _float s_fBGMSound;
};

