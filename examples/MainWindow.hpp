#pragma once

#include <d3d9.h>
#include <tchar.h>
#include <pwindow/Window.hpp>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>


namespace vgfx
{

class MainWindow : public pwindow::Window
{
public:

    MainWindow(const char* title, int x, int y, int width, int height) : pwindow::Window(title, x, y, width, height)
    {
        CreateDeviceD3D(Handle());

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

        ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(Handle());
        ImGui_ImplDX9_Init(s_d3ddevice);
    }

    virtual void OnCloseWindow()
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
    }

    virtual void OnDraw()
    {
        ImGui_ImplWin32_NewFrame();
        ImGui_ImplDX9_NewFrame();


        ImGui::NewFrame();

        //ImGui::Begin("My shapes");

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Get the current ImGui cursor position
        ImVec2 p = ImGui::GetCursorScreenPos();

        // Draw a red circle
        draw_list->AddCircleFilled(ImVec2(p.x + 50, p.y + 50), 30.0f, IM_COL32(255, 0, 0, 255), 16);

        // Draw a 3 pixel thick yellow line
        draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + 100.0f, p.y + 100.0f), IM_COL32(255, 255, 0, 255), 3.0f);

        // Advance the ImGui cursor to claim space in the window (otherwise the window will appears small and needs to be resized)
        ImGui::Dummy(ImVec2(200, 200));

        //ImGui::End();
        ImGui::EndFrame();

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//        s_d3ddevice->SetRenderState(D3DRS_ZENABLE, FALSE);
//        s_d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//        s_d3ddevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
//        s_d3ddevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (s_d3ddevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            s_d3ddevice->EndScene();
        }
        HRESULT result = s_d3ddevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
//        if (result == D3DERR_DEVICELOST && s_d3ddevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
//            ResetDevice();
    }

    virtual void OnSize(int width, int height)
    {
        if (s_d3ddevice != nullptr)
        {
            s_d3dpp.BackBufferWidth = width;
            s_d3dpp.BackBufferHeight = height;
            ResetDevice();
        }
    }

private:

    bool CreateDeviceD3D(HWND hWnd)
    {
        if ((s_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
            return false;

        // Create the D3DDevice
        ZeroMemory(&s_d3dpp, sizeof(s_d3dpp));
        s_d3dpp.Windowed = TRUE;
        s_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        s_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
        s_d3dpp.EnableAutoDepthStencil = TRUE;
        s_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
        //s_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
        if (s_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &s_d3dpp, &s_d3ddevice) < 0)
            return false;

        return true;
    }

    void CleanupDeviceD3D()
    {
        if (s_d3ddevice) { s_d3ddevice->Release(); s_d3ddevice = nullptr; }
        if (s_d3d) { s_d3d->Release(); s_d3d = nullptr; }
    }

    void ResetDevice()
    {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        HRESULT hr = s_d3ddevice->Reset(&s_d3dpp);
        if (hr == D3DERR_INVALIDCALL)
            IM_ASSERT(0);
        ImGui_ImplDX9_CreateDeviceObjects();
    }

    // Data
    static inline LPDIRECT3D9              s_d3d { nullptr };
    static inline LPDIRECT3DDEVICE9        s_d3ddevice { nullptr };
    static inline D3DPRESENT_PARAMETERS    s_d3dpp;
};//class Window


}//namespace vgfx

