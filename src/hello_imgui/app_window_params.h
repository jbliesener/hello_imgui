#pragma once
#include "imgui.h"
#include "hello_imgui/screen_bounds.h"
#include <string>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

namespace HelloImGui
{
enum class FullScreenMode
{
    NoFullScreen,
    FullScreen,                    // Full screen with specified resolution
    FullScreenDesktopResolution,   // Full screen with current desktop mode & resolution
    FullMonitorWorkArea            // Fake full screen, maximized window on the selected monitor
};


enum class WindowSizeState
{
    Standard,
    Minimized,
    Maximized
};


enum class WindowPositionMode
{
    OsDefault,
    MonitorCenter,
    FromCoords,
};


enum class WindowSizeMeasureMode
{
    // ScreenCoords: measure window size in screen coords.
    //     Note: screen coordinates *might* differ from real pixel on high dpi screens; but this depends on the OS.
    //         - For example, on apple a retina screenpixel size 3456x2052 might be seen as 1728x1026 in screen
    //           coordinates
    //         - Under windows, and if the application is DPI aware, ScreenCoordinates correspond to real pixels, 
    //           even on high density screens
    ScreenCoords,

    // RelativeTo96Ppi enables to give screen size that are independant from the screen density.
    // For example, a window size expressed as 800x600 will correspond to a size
    //    800x600 (in screen coords) if the monitor dpi is 96
    //    1600x120 (in screen coords) if the monitor dpi is 192
    RelativeTo96Ppi
};


/**
@@md#WindowGeometry

__WindowGeometry__ is a struct that defines the window geometry.

Members:
* `size`: _int[2], default="{800, 600}"_. Size of the application window
  used if fullScreenMode==NoFullScreen and sizeAuto==false
* `sizeAuto`: _bool, default=false_
  If true, adapt the app window size to the presented widgets.
  After the first frame was displayed, HelloImGui will measure its size,
  and the backend application window will be resized. As a consequence, the application window size may
  vary between the first and the second frame.

* `fullScreenMode`: _FullScreenMode, default=NoFullScreen_.
   You can choose between several full screen modes:
   ```cpp
        NoFullScreen,
        FullScreen,                    // Full screen with specified resolution
        FullScreenDesktopResolution,   // Full screen with current desktop mode & resolution
        FullMonitorWorkArea            // Fake full screen, maximized window on the selected monitor
    ```
* `positionMode`: _WindowPositionMode, default = OsDefault_.
   You can choose between several window position modes:
   ```cpp
        OsDefault,
        MonitorCenter,
        FromCoords,
    ```
* `monitorIdx`: _int, default = 0_.
  used if positionMode==MonitorCenter or if fullScreenMode!=NoFullScreen
* `windowSizeState`: _WindowSizeState, default=Standard_
   You can choose between several window size states:
   ```cpp
        Standard,
        Minimized,
        Maximized
    ```
* `windowSizeMeasureMode`: _WindowSizeMeasureMode_, default=RelativeTo96Ppi
  how the window size is specified:
  * RelativeTo96Ppi enables to give screen size that are independant from the screen density.
     For example, a window size expressed as 800x600 will correspond to a size
        - 800x600 (in screen coords) if the monitor dpi is 96
        - 1600x120 (in screen coords) if the monitor dpi is 192
      (this works with Glfw. With SDL, it only works under windows)
  * ScreenCoords: measure window size in screen coords
    (Note: screen coordinates might differ from real pixels on high dpi screen)

* `resizeAppWindowAtNextFrame`: _bool_, default=false;
  If you set this to flag to true at any point during the execution, the application window
  will then try to resize based on its content on the next displayed frame,
  and this flag will subsequently be set to false.
  Example:
  ```cpp
  // Will resize the app window at next displayed frame
  HelloImGui::GetRunnerParams()->appWindowParams.windowGeometry.resizeAppWindowAtNextFrame = true;
  ```

  :::Note: this flag is intended to be used during execution, not at startup (use sizeAuto at startup):::
@@md
**/
struct WindowGeometry
{
    // used if fullScreenMode==NoFullScreen and sizeAuto==false. Value=(800, 600)
    ScreenSize size = DefaultWindowSize;

    // If true, adapt the app window size to the presented widgets. This is done at startup
    bool sizeAuto = false;

    FullScreenMode fullScreenMode = FullScreenMode::NoFullScreen;

    WindowPositionMode positionMode = WindowPositionMode::OsDefault;

    // used if windowPositionMode==FromCoords, default=(40, 40)
    ScreenPosition position = DefaultScreenPosition;

    // used if positionMode==MonitorCenter or if fullScreenMode!=NoFullScreen
    int monitorIdx = 0;

    WindowSizeState windowSizeState = WindowSizeState::Standard;

    WindowSizeMeasureMode windowSizeMeasureMode = WindowSizeMeasureMode::RelativeTo96Ppi;

    // If true, the application window will try to resize based on its content on the next displayed frame
    bool resizeAppWindowAtNextFrame = false;
};


// If there is a notch on the iPhone, you should not display inside these insets
struct EdgeInsets
{
    double top = 0.;     // Typically around 47
    double left = 0.;    // Typically 0
    double bottom = 0.;  // Typically around 34
    double right = 0.;   // Typically 0
};


/**
@@md#AppWindowParams

__AppWindowParams__ is a struct that defines the application window display params.
See [doc_src/hello_imgui_diagram.png](https://raw.githubusercontent.com/pthom/hello_imgui/master/src/hello_imgui/doc_src/hello_imgui_diagram.png)
for details.

Members:
* `windowTitle`: _string, default=""_. Title of the application window
* `windowGeometry`: _WindowGeometry_
  Enables to precisely set the window geometry (position, monitor, size, full screen, fake full screen, etc.)
   _Note: on a mobile device, the application will always be full screen._
* `restorePreviousGeometry`: _bool, default=false_.
  If true, then save & restore windowGeometry from last run (the geometry will be written in imgui_app_window.ini)
* `resizable`: _bool, default = false_. Should the window be resizable. This is taken into account at
  creation.
* `hidden`: _bool, default = false_. Should the window be hidden. This is taken into account dynamically (you
  can show/hide the window with this). Full screen windows cannot be hidden.

* `borderless`: _bool, default = false_. Should the window have borders. This is taken into account at creation.
* `borderlessMovable`: _bool, default = true_. If the window is borderless, should it be movable.
   If so, a drag zone is displayed at the top of the window when the mouse is over it.
* `borderlessResizable`: _bool, default = true_. If the window is borderless, should it be resizable.
   If so, a drag zone is displayed at the bottom-right of the window when the mouse is over it.
* `borderlessClosable`: _bool, default = false_. If the window is borderless, should it have a close button.
   If so, a close button is displayed at the top-right of the window when the mouse is over it.
* `borderlessHighlightColor`: _ImVec4, default = ImVec4(0.2f, 0.4f, 1.f, 0.f)_.
   Color of the highlight displayed on resize/move zones. If borderlessHighlightColor.w==0,
   then the highlightColor will be automatically set to ImGui::GetColorU32(ImGuiCol_TitleBgActive, 0.6f)

* `edgeInsets`: _EdgeInsets_. iOS only, out values filled by HelloImGui:
  if there is a notch on the iPhone, you should not display inside these insets.
  HelloImGui handles this automatically, if handleEdgeInsets is true and
  if runnerParams.imGuiWindowParams.defaultImGuiWindowType is not NoDefaultWindow.
  (warning, these values are updated only after a few frames, they are typically 0 for the first 4 frames)
* `handleEdgeInsets`: _bool, default = true_. iOS only, if true, HelloImGui will handle the edgeInsets.

@@md
**/
struct AppWindowParams
{
    std::string windowTitle;

    WindowGeometry windowGeometry;

    // if true, then save & restore from last run
    bool restorePreviousGeometry = false;

    bool resizable = true;
    bool hidden = false;

    bool   borderless = false;
    bool   borderlessMovable = true;
    bool   borderlessResizable = true;
    bool   borderlessClosable = true;
    ImVec4 borderlessHighlightColor = ImVec4(0.2f, 0.4f, 1.f, 0.3f);


    EdgeInsets edgeInsets;
    bool       handleEdgeInsets = true;
};

}  // namespace HelloImGui