#pragma once
#include "hello_imgui/hello_imgui_error.h"
#include "hello_imgui/screen_bounds.h"
#include "hello_imgui/hello_imgui_screenshot.h"

#include <functional>
#include <memory>


//
// Callbacks for the Rendering Backend (OpenGL, ...)
//
namespace HelloImGui
{
    using VoidFunction = std::function<void()>;

    // Anatomy of an ImGui app lifecycle (cf ImGui examples)
    // =====================================================
    //
    //     - "==>": Events related to the rendering backend (OpenGL, Metal, Vulkan, DirectX, ...)
    //     - "-->": Events related to the windowing backend (SDL, Glfw, ...)
    //    int main()
    //    {
    //        // Init Windowing Backend (SDL, Glfw, ...): call glfwInit() or SDL_Init()
    //        // See: AbstractRunner::Impl_InitBackend()
    //
    //        // Create Window, for Sdl or Glfw:
    //        // --> AbstractRunner::Impl_CreateWindow())
    //        //   This will call
    //        //       - SdlWindowHelper::CreateWindow() or GlfwWindowHelper::CreateWindow()
    //        //              which may set some renderer specific hints
    //        //             ==> search for RenderingCallbacks_Impl_Hint_WindowingBackend in the code
    //        //     - Then some possible  rendering callback customization
    //                ==> search for RenderingCallbacks_Prepare_WithWindow_PreImGuiInit in the code
    //
    //        // Init ImGui: ImGui::CreateContext(), etc. This is done inside
    //        // --> AbstractRunner::Setup()
    ////      //   Configure rendering backend Post ImGui Init
    //        //    ==> search for RenderingCallbacks_Prepare_PosImGuiInit in the code
    //
    //        // This loop control is inside AbstractRunner::Run()
    //        //     its body is inside --> AbstractRunner::CreateFramesAndRender()
    //        while( ! appFinished)
    //        {
    //
    //                // Poll windowing backend events
    //                //   --> inside AbstractRunner::Impl_PollEvents()
    //
    //                // Create a new frame in the rendering backend
    //                //  ==> call RenderingCallbacks.Impl_NewFrame_3D
    //
    //                // Create a new frame in ImGui and in the Windowing Backend
    //                // (for example ImGui_ImplGlfw_NewFrame() + ImGui::NewFrame()))
    //                // --> AbstractRunner::Impl_NewFrame_Backend()
    //
    //                // Show widgets with ImGui (user code)
    //
    //                // Render widgets with ImGui
    //                ImGui::Render();
    //
    //                // Render widgets with the rendering backend
    //                // ==> call RenderingCallbacks.Impl_RenderDrawData_To_3D()
    //
    //                // Render additional platform windows (e.g. Dear ImGui SDL2+OpenGL3 backend)
    //                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    //                {
    //                    ImGui::UpdatePlatformWindows();
    //                    ImGui::RenderPlatformWindowsDefault();
    //                }
    //
    //                // Swap buffers in the rendering backend
    //                // ==> call RenderingCallbacks.Impl_SwapBuffers()
    //        }
    //
    //        // Shutdown ImGui
    //        ImGui::DestroyContext();
    //
    //        // Shutdown Windowing Backend
    //        // --> AbstractRunner::Impl_Cleanup()
    //
    //        // Shutdown rendering backend
    //        // ==> call RenderingCallbacks.Impl_Shutdown_3D()
    //    }

    //
    // There is no Init backend here, since it needs to account for the Windowing Backend and the Rendering Backend.
    // As a consequence, it is implemented in the Rendering Backend, with #ifdefs
    //
    // "Callbacks" that are dependent on the combination Rendering backend (OpenGL, Metal, Vulkan) + Windowing Backend (Glfw, SDL):
    //     Search for RenderingCallbacks_Impl_Hint_WindowingBackend in the code
    //     Search for RenderingCallbacks_Prepare_WithWindow_PreImGuiInit in the code
    //     Search for RenderingCallbacks_Prepare_PosImGuiInit in the code
    //     Search for RenderingCallbacks_Impl_SwapBuffers in the code
    //
    struct RenderingCallbacks
    {
        VoidFunction                  Impl_NewFrame_3D          = [] { HIMG_ERROR("Empty function"); };
        std::function<void(ImVec4)>   Impl_Frame_3D_ClearColor  = [] (ImVec4) { HIMG_ERROR("Empty function"); };
        VoidFunction                  Impl_RenderDrawData_To_3D = [] { HIMG_ERROR("Empty function"); };
        VoidFunction                  Impl_Shutdown_3D          = [] { HIMG_ERROR("Empty function"); };
        std::function<ImageBuffer()>  Impl_ScreenshotRgb_3D     = [] { return ImageBuffer{}; };
        std::function<ScreenSize()>   Impl_GetFrameBufferSize;   //= [] { return ScreenSize{0, 0}; };
    };

    using RenderingCallbacksPtr = std::shared_ptr<RenderingCallbacks>;
} // namespace HelloImGui