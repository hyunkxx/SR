#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CUI_Start : public Engine::CGameObject
{
public:
	enum HOTKEY_NUM {
		NUM_HELP, NUM_UI, NUM_P, NUM_O, NUM_F3, NUM_F7, NUM_CE, NUM_T, NUM_BLANK1
		, NUM_BASE, NUM_MOVE, NUM_LB, NUM_M, NUM_G, NUM_V, NUM_1, NUM_2, NUM_BLANK2
		, NUM_SKILL, NUM_3, NUM_4, NUM_5, NUM_BLANK3
		, NUM_M55, NUM_Q, NUM_BLANK4
		, NUM_AH_64A, NUM_MOVE2, NUM_LB2, NUM_RB, NUM_LC, NUM_END
	};

private:
	explicit CUI_Start(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Start(const CUI_Start& rhs);
	virtual ~CUI_Start();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta)override;
	virtual void LateUpdate_Object(void)  override;
	virtual void Render_Object(void)  override;
	virtual const   _vec3		Get_Info(void) { return _vec3(); }
	virtual			void		Move_Info(_vec3 _Info) {}
	static CUI_Start*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Add_Component(void);

	virtual void Free(void) override;


	CRcTex*						m_pRcTex = nullptr;
	CTexture*					m_pTexture = nullptr;
	CTransform*				m_pTransform = nullptr;

	CTexture*					m_pSliderTexture = nullptr;
	CTransform*				m_pSliderTransform = nullptr;

	_matrix  m_matProj;

	_float					m_fScaleX;
	_float					m_fScaleY;
	_float					m_fScaleZ;

	_float					m_fPosX;
	_float					m_fPosY;
	_float					m_fPosZ;


	_bool					m_bHelp = false;

	_float					m_fSliderScaleX;
	_float					m_fSliderScaleY;
	_float					m_fSliderScaleZ;

	_float					m_fSliderPosX;
	_float					m_fSliderPosY;
	_float					m_fSliderPosZ;

	_float					m_fFontPosY;

	wstring					m_szDescription[NUM_END];
	wstring					m_szDescription_Field[NUM_END];

public:
	_bool					Get_HelpWin(void) { return m_bHelp; }

	void						Key_input(void);
	void						Update_Pos(void);

	void						Ready_Font(void);

	void						UI_Render_Font(void);
};

