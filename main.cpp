#include "includes.h"

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

bool label_1;
bool label_2;
bool label_3;
bool label_4;
bool label_5;
bool label_6;
bool label_7;
bool label_11;
bool label_12 = 90;
bool label_13[] = { 255.0f, 0.0f, 0.0f };
bool label_14[] = { 0.0, 255.0f, 0.0f };
bool label_15 = 1.0f;

static int tabs = 1;
bool init = false;

bool menu_show = false;

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}
	if (GetAsyncKeyState(VK_INSERT) & 1) {

		menu_show = !menu_show;
	}
	if (GetAsyncKeyState(VK_END)) {
     
		kiero::shutdown();
		return 0;
	}

	if (menu_show)
	{

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Batya aim");
		ImGui::Text("Ez egy teszt");

		if (ImGui::Button("Aim", ImVec2(152, 25)))
		{
			tabs = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("ESP", ImVec2(152, 25)))
		{
			tabs = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Misc", ImVec2(152, 25)))
		{
			tabs = 3;
		}

		if (tabs == 1) // aim
		{
			ImGui::Checkbox("Aimbot", &label_1);
		}

		if (tabs == 2) // esp
		{
			ImGui::Checkbox("Esp", &label_2);
			ImGui::Checkbox("no flash", &label_3);
			ImGui::Checkbox("Glow chams", &label_4);
			ImGui::Checkbox("Rardar hack", &label_5);
		}

		if (tabs == 3) // misc
		{
			
		}

		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
