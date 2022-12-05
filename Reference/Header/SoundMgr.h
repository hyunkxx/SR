#pragma once
#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL SoundMgr : public CBase
{
	DECLARE_SINGLETON(SoundMgr)

private:
	explicit SoundMgr();
	virtual ~SoundMgr();

public:
	void Ready_Sound(void);

	void PlaySound_SR(_tchar* pSoundKey, SoundType eID, _float fVolume);
	void PlayBGM(_tchar* pSoundKey, _float fVolume);
	void StopSound(SoundType eID);
	void StopAll();
	void SetChannelVolume(SoundType eID, _float fVolume);

	virtual void Free(void)override;

private:
	void LoadSoundFile();

	// ���� ���ҽ� ������ ���� ��ü 
	map<_tchar*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[SOUND_END];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};

END