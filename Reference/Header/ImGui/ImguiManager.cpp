#include "stdafx.h"
#include "ImguiManager.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(ImguiManager)

ImguiManager::ImguiManager()
{
}

ImguiManager::~ImguiManager()
{
	Showdown();
}

void ImguiManager::Initalize(LPDIRECT3DDEVICE9 pDevice)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	// Setup Platform/Renderer backends

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(pDevice);

	m_pDevice = pDevice;
}

void ImguiManager::NewFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{		
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2(210, 20));
		ImGui::Begin("Frame", nullptr , ImGuiWindowFlags_NoBackground |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::SameLine();
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

void ImguiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::EndFrame()
{
	ImGui::EndFrame();

	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

void ImguiManager::Showdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}