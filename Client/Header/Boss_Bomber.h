#pragma once
#include "GameObject.h"
#include "Voxel.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CBoss_Bomber :
	public CGameObject
{
public:
	explicit CBoss_Bomber(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss_Bomber(const CBoss_Bomber& rhs);
	virtual ~CBoss_Bomber();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;
	virtual void		RenderGUI(void)override;
public:
	void				Strike(_vec3 Start_Pos, _vec3 _Strike_Pos, _int SoundType);
	
private:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void);
public:
	static CBoss_Bomber*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

protected:
	CTransform*			m_pTransformBody = nullptr;
	CVoxel*				m_pBody = nullptr;

	CRcTex*				m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_float				m_fSpeed = 300.f;
	_vec3				m_vStart_Pos = { -400.f, 200.f, -400.f };
	_vec3				m_vStrike_Pos;
	_vec3				m_vDir;
	_vec3				m_vMileage;
	_float				m_FAngle_Accum;
	_bool				m_bBoomShoot = false;;

	_float				m_fStrike_Count = 0.f;
	_float				m_fVolume = 1.f;

	_int				m_iSoundType = 0.f;
	_float				m_fSoundCheckTime = 0.f;
};

