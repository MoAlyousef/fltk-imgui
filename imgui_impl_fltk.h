
#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

struct Fl_Gl_Window;

IMGUI_IMPL_API bool     ImGui_ImplFltk_InitForOpenGL(Fl_Gl_Window* window);
IMGUI_IMPL_API void     ImGui_ImplFltk_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplFltk_NewFrame();
IMGUI_IMPL_API bool     ImGui_ImplFltk_ProcessEvent(int);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
static inline void ImGui_ImplFltk_NewFrame(Fl_Gl_Window*) { ImGui_ImplFltk_NewFrame(); } // 1.84: removed unnecessary parameter
#endif

#endif // #ifndef IMGUI_DISABLE
