#pragma once

class ImguiManager
{
	DECLARE_SINGLETON(ImguiManager)
private:
	ImguiManager();
	~ImguiManager();
public:
	void Initalize(LPDIRECT3DDEVICE9 pDevice);
	void NewFrame();
	void EndFrame();
	void Render();
	void Showdown();
private:
	LPDIRECT3DDEVICE9 m_pDevice;
};

