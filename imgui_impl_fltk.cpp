#include "imgui.h"
#ifndef IMGUI_DISABLE
#include "imgui_impl_fltk.h"

// Clang warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#endif

// FLTK
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <stdint.h>

using Uint64 = uint64_t;
using Uint32 = uint32_t;
using Uint8 = uint8_t;

// FLTK Data
struct ImGui_ImplFltk_Data
{
    Fl_Gl_Window*     Window;
    Fl_Timestamp      Time;
    int               MouseButtonsDown;
    Fl_Cursor         MouseCursors[ImGuiMouseCursor_COUNT];
    Fl_Cursor         LastMouseCursor;
    int               PendingMouseLeaveFrame;
    char*             ClipboardTextData;
    bool              MouseCanUseGlobalState;

    ImGui_ImplFltk_Data()   { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple windows) instead of multiple Dear ImGui contexts.
// FIXME: multi-context support is not well tested and probably dysfunctional in this backend.
// FIXME: some shared resources (mouse cursor shape, gamepad) are mishandled when using multi-context.
static ImGui_ImplFltk_Data* ImGui_ImplFltk_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplFltk_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

// Functions
static const char* ImGui_ImplFltk_GetClipboardText(void*)
{
    ImGui_ImplFltk_Data* bd = ImGui_ImplFltk_GetBackendData();
    bd->ClipboardTextData = (char *)Fl::event_text();
    return bd->ClipboardTextData;
}

static void ImGui_ImplFltk_SetClipboardText(void*, const char* text)
{
    Fl::copy(text, 1);
}

static ImGuiKey ImGui_ImplFltk_KeycodeToImGuiKey(int keycode)
{
    switch (keycode)
    {
        case FL_Tab: return ImGuiKey_Tab;
        case FL_Left: return ImGuiKey_LeftArrow;
        case FL_Right: return ImGuiKey_RightArrow;
        case FL_Up: return ImGuiKey_UpArrow;
        case FL_Down: return ImGuiKey_DownArrow;
        case FL_Page_Up: return ImGuiKey_PageUp;
        case FL_Page_Down: return ImGuiKey_PageDown;
        case FL_Home: return ImGuiKey_Home;
        case FL_End: return ImGuiKey_End;
        case FL_Insert: return ImGuiKey_Insert;
        case FL_Delete: return ImGuiKey_Delete;
        case FL_BackSpace: return ImGuiKey_Backspace;
        case ' ': return ImGuiKey_Space;
        case FL_Enter: return ImGuiKey_Enter;
        case FL_Escape: return ImGuiKey_Escape;
        case '\'': return ImGuiKey_Apostrophe;
        case ',': return ImGuiKey_Comma;
        case '-': return ImGuiKey_Minus;
        case '.': return ImGuiKey_Period;
        case '/': return ImGuiKey_Slash;
        case ';': return ImGuiKey_Semicolon;
        case '=': return ImGuiKey_Equal;
        case '[': return ImGuiKey_LeftBracket;
        case '\\': return ImGuiKey_Backslash;
        case ']': return ImGuiKey_RightBracket;
        case '`': return ImGuiKey_GraveAccent;
        case FL_Caps_Lock: return ImGuiKey_CapsLock;
        case FL_Scroll_Lock: return ImGuiKey_ScrollLock;
        case FL_Num_Lock: return ImGuiKey_NumLock;
        case FL_Print: return ImGuiKey_PrintScreen;
        case FL_Pause: return ImGuiKey_Pause;
        case FL_KP + 0: return ImGuiKey_Keypad0;
        case FL_KP + 1: return ImGuiKey_Keypad1;
        case FL_KP + 2: return ImGuiKey_Keypad2;
        case FL_KP + 3: return ImGuiKey_Keypad3;
        case FL_KP + 4: return ImGuiKey_Keypad4;
        case FL_KP + 5: return ImGuiKey_Keypad5;
        case FL_KP + 6: return ImGuiKey_Keypad6;
        case FL_KP + 7: return ImGuiKey_Keypad7;
        case FL_KP + 8: return ImGuiKey_Keypad8;
        case FL_KP + 9: return ImGuiKey_Keypad9;
        case FL_Control_L: return ImGuiKey_LeftCtrl;
        case FL_Shift_L: return ImGuiKey_LeftShift;
        case FL_Alt_L: return ImGuiKey_LeftAlt;
        case FL_Meta_L: return ImGuiKey_LeftSuper;
        case FL_Control_R: return ImGuiKey_RightCtrl;
        case FL_Shift_R: return ImGuiKey_RightShift;
        case FL_Alt_R: return ImGuiKey_RightAlt;
        case FL_Meta_R: return ImGuiKey_RightSuper;
        case FL_Menu: return ImGuiKey_Menu;
        case '0': return ImGuiKey_0;
        case '1': return ImGuiKey_1;
        case '2': return ImGuiKey_2;
        case '3': return ImGuiKey_3;
        case '4': return ImGuiKey_4;
        case '5': return ImGuiKey_5;
        case '6': return ImGuiKey_6;
        case '7': return ImGuiKey_7;
        case '8': return ImGuiKey_8;
        case '9': return ImGuiKey_9;
        case 'a': return ImGuiKey_A;
        case 'b': return ImGuiKey_B;
        case 'c': return ImGuiKey_C;
        case 'd': return ImGuiKey_D;
        case 'e': return ImGuiKey_E;
        case 'f': return ImGuiKey_F;
        case 'g': return ImGuiKey_G;
        case 'h': return ImGuiKey_H;
        case 'i': return ImGuiKey_I;
        case 'j': return ImGuiKey_J;
        case 'k': return ImGuiKey_K;
        case 'l': return ImGuiKey_L;
        case 'm': return ImGuiKey_M;
        case 'n': return ImGuiKey_N;
        case 'o': return ImGuiKey_O;
        case 'p': return ImGuiKey_P;
        case 'q': return ImGuiKey_Q;
        case 'r': return ImGuiKey_R;
        case 's': return ImGuiKey_S;
        case 't': return ImGuiKey_T;
        case 'u': return ImGuiKey_U;
        case 'v': return ImGuiKey_V;
        case 'w': return ImGuiKey_W;
        case 'x': return ImGuiKey_X;
        case 'y': return ImGuiKey_Y;
        case 'z': return ImGuiKey_Z;
        case FL_F + 1: return ImGuiKey_F1;
        case FL_F + 2: return ImGuiKey_F2;
        case FL_F + 3: return ImGuiKey_F3;
        case FL_F + 4: return ImGuiKey_F4;
        case FL_F + 5: return ImGuiKey_F5;
        case FL_F + 6: return ImGuiKey_F6;
        case FL_F + 7: return ImGuiKey_F7;
        case FL_F + 8: return ImGuiKey_F8;
        case FL_F + 9: return ImGuiKey_F9;
        case FL_F + 10: return ImGuiKey_F10;
        case FL_F + 11: return ImGuiKey_F11;
        case FL_F + 12: return ImGuiKey_F12;
        case FL_F + 13: return ImGuiKey_F13;
        case FL_F + 14: return ImGuiKey_F14;
        case FL_F + 15: return ImGuiKey_F15;
        case FL_F + 16: return ImGuiKey_F16;
        case FL_F + 17: return ImGuiKey_F17;
        case FL_F + 18: return ImGuiKey_F18;
        case FL_F + 19: return ImGuiKey_F19;
        case FL_F + 20: return ImGuiKey_F20;
        case FL_F + 21: return ImGuiKey_F21;
        case FL_F + 22: return ImGuiKey_F22;
        case FL_F + 23: return ImGuiKey_F23;
        case FL_F + 24: return ImGuiKey_F24;
        case FL_Back: return ImGuiKey_AppBack;
        case FL_Forward: return ImGuiKey_AppForward;
    }
    return ImGuiKey_None;
}

static void ImGui_ImplFltk_UpdateKeyModifiers(int key_mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl, (key_mods & FL_CTRL) == FL_CTRL);
    io.AddKeyEvent(ImGuiMod_Shift, (key_mods & FL_SHIFT) == FL_SHIFT);
    io.AddKeyEvent(ImGuiMod_Alt, (key_mods & FL_ALT) == FL_ALT);
    io.AddKeyEvent(ImGuiMod_Super, (key_mods & FL_COMMAND) == FL_COMMAND);
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple FLTK events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
bool ImGui_ImplFltk_ProcessEvent(int event)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplFltk_Data* bd = ImGui_ImplFltk_GetBackendData();

    switch (event)
    {
        case FL_DRAG:
        case FL_MOVE:
        {
            ImVec2 mouse_pos((float)Fl::event_x(), (float)Fl::event_y());
            io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
            io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);
            return true;
        }
        case FL_MOUSEWHEEL:
        {

            float wheel_x = -(float)Fl::event_dx();
            float wheel_y = -(float)Fl::event_dy();
            io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
            io.AddMouseWheelEvent(wheel_x, wheel_y);
            return true;
        }
        case FL_PUSH:
        case FL_RELEASE:
        {
            auto fltk_btn = Fl::event_button();
            int mouse_button = -1;
            if (fltk_btn == FL_LEFT_MOUSE) { mouse_button = 0; }
            if (fltk_btn == FL_RIGHT_MOUSE) { mouse_button = 1; }
            if (fltk_btn == FL_MIDDLE_MOUSE) { mouse_button = 2; }
            if (mouse_button == -1)
                break;
            io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
            io.AddMouseButtonEvent(mouse_button, (event == FL_PUSH));
            bd->MouseButtonsDown = (event == FL_PUSH) ? (bd->MouseButtonsDown | (1 << mouse_button)) : (bd->MouseButtonsDown & ~(1 << mouse_button));
            return true;
        }
        case FL_PASTE: {
            io.AddInputCharactersUTF8(Fl::event_text());
            return true;
        }
        case FL_KEYUP: 
        {
            auto key = Fl::event_key();
            ImGuiKey imkey = ImGui_ImplFltk_KeycodeToImGuiKey(key);
            auto state = Fl::event_state();
            ImGui_ImplFltk_UpdateKeyModifiers(state);
            io.AddKeyEvent(imkey, (event == FL_KEYUP));
            bd->Window->handle(FL_UNFOCUS);
            if (key == 'v' && state == FL_CTRL) {
                bd->Window->handle(FL_PASTE);
            }
            return true;
        }
        case FL_KEYDOWN:
        {
            auto c = Fl::event_text();
            if (c && event == FL_KEYDOWN)
                io.AddInputCharactersUTF8(c);
            ImGuiKey imkey = ImGui_ImplFltk_KeycodeToImGuiKey(Fl::event_key());
            auto state = Fl::event_state();
            ImGui_ImplFltk_UpdateKeyModifiers(state);
            io.AddKeyEvent(imkey, (event == FL_KEYDOWN));
            return true;
        }
        case FL_ENTER:
        case FL_LEAVE:
        case FL_FOCUS:
        case FL_UNFOCUS:
        {
            if (event == FL_ENTER)
            {
                bd->PendingMouseLeaveFrame = 0;
            }
            if (event == FL_LEAVE)
                bd->PendingMouseLeaveFrame = ImGui::GetFrameCount() + 1;
            if (event == FL_FOCUS)
                io.AddFocusEvent(true);
            else if (event == FL_UNFOCUS)
                io.AddFocusEvent(false);
            return true;
        }
    }
    return false;
}

static bool ImGui_ImplFltk_Init(Fl_Gl_Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    bool mouse_can_use_global_state = true;

    // Setup backend capabilities flags
    ImGui_ImplFltk_Data* bd = IM_NEW(ImGui_ImplFltk_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_fltk";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->Window = window;
    bd->Time = Fl::now();
    bd->MouseCanUseGlobalState = mouse_can_use_global_state;

    io.SetClipboardTextFn = ImGui_ImplFltk_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplFltk_GetClipboardText;
    io.ClipboardUserData = nullptr;

    // Load mouse cursors
    bd->MouseCursors[ImGuiMouseCursor_Arrow] = FL_CURSOR_ARROW;
    bd->MouseCursors[ImGuiMouseCursor_TextInput] = FL_CURSOR_INSERT;
    bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = FL_CURSOR_CROSS;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = FL_CURSOR_NS;
    bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = FL_CURSOR_WE;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = FL_CURSOR_NESW;
    bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = FL_CURSOR_NWSE;
    bd->MouseCursors[ImGuiMouseCursor_Hand] = FL_CURSOR_HAND;
    bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = FL_CURSOR_WAIT;

    // Set platform dependent data in viewport
    // Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandleRaw = nullptr;

#if defined(FL_LIBRARY) || defined(FL_INTERNALS)
        main_viewport->PlatformHandleRaw = fl_xid(window);
#endif

    return true;
}

bool ImGui_ImplFltk_InitForOpenGL(Fl_Gl_Window* window)
{
    return ImGui_ImplFltk_Init(window);
}

void ImGui_ImplFltk_Shutdown()
{
    ImGui_ImplFltk_Data* bd = ImGui_ImplFltk_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();

    bd->LastMouseCursor = FL_CURSOR_ARROW;

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasGamepad);
    IM_DELETE(bd);
}

static void ImGui_ImplFltk_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;
    ImGui_ImplFltk_Data* bd = ImGui_ImplFltk_GetBackendData();

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        bd->Window->cursor(FL_CURSOR_NONE);
    }
    else
    {
        // Show OS mouse cursor
        Fl_Cursor expected_cursor = bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] : bd->MouseCursors[ImGuiMouseCursor_Arrow];
        if (bd->LastMouseCursor != expected_cursor)
        {
            bd->Window->cursor(expected_cursor);
            bd->LastMouseCursor = expected_cursor;
        }
    }
}

void ImGui_ImplFltk_NewFrame()
{
    ImGui_ImplFltk_Data* bd = ImGui_ImplFltk_GetBackendData();
    IM_ASSERT(bd != nullptr && "Did you call ImGui_ImplFltk_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w = bd->Window->w(), h = bd->Window->h();
    int display_w = bd->Window->pixel_w(), display_h = bd->Window->pixel_h();
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / (float)w, (float)display_h / (float)h);

    auto current_time = Fl::now();
    auto delta = (float)Fl::seconds_between(current_time, bd->Time);
    io.DeltaTime = bd->Time.sec > 0.0 ? delta :(float)(1.0f / 60.0f);
    bd->Time = current_time;

    if (bd->PendingMouseLeaveFrame && bd->PendingMouseLeaveFrame >= ImGui::GetFrameCount() && bd->MouseButtonsDown == 0)
    {
        bd->PendingMouseLeaveFrame = 0;
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }

    ImGui_ImplFltk_UpdateMouseCursor();
}

//-----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // #ifndef IMGUI_DISABLE
