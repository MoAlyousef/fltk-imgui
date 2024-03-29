// Dear ImGui: standalone example application for FLTK + OpenGL 3, using
// programmable pipeline (FLTK is a cross-platform general purpose GUI library)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/
// folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_fltk.h"
#include "imgui_impl_opengl3.h"
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/gl.h>
#include <chrono>
#include <stdio.h>
#include <thread>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class GlWin : public Fl_Gl_Window {
  public:
    GlWin(int x, int y, int w, int h, const char *label = nullptr)
        : Fl_Gl_Window(x, y, w, h, label) {
    }
    GlWin(int w, int h, const char *label = nullptr)
        : Fl_Gl_Window(w, h, label) {
    }
    int handle(int ev) override {
        int ret = Fl_Gl_Window::handle(ev);
        return ret | ImGui_ImplFltk_ProcessEvent(ev);
    }
};

// Main code
int main(int, char **) {
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Create window with graphics context
    Fl_Double_Window *win =
        new Fl_Double_Window(1280, 720, "Dear ImGui FLTK+OpenGL3 example");
    GlWin *glwin = new GlWin(5, 5, 1270, 710);
    glwin->mode(FL_OPENGL3);
    if (glwin == nullptr)
        return 1;
    glwin->end();
    win->end();
    win->show();
    glwin->make_current();
    glwin->swap_interval(1); // enable vsync

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplFltk_InitForOpenGL(glwin);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (Fl::wait()) {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplFltk_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in
        // ImGui::ShowDemoWindow()! You can browse its code to learn more about
        // Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End
        // pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello,
                                           // world!" and append into it.

            ImGui::Text(
                "This is some useful text."); // Display some text (you can use
                                              // a format strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window); // Edit bools storing our window
                                                // open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat(
                "float", &f, 0.0f,
                1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3(
                "clear color",
                (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets
                               // return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin(
                "Another Window",
                &show_another_window); // Pass a pointer to our bool variable
                                       // (the window will have a closing button
                                       // that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w = glwin->pixel_w(), display_h = glwin->pixel_h();
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glwin->swap_buffers();
        // here we force a redraw at our own rate,
        // otherwise our app will only update when it detects an event
        win->redraw();
        // this is our rate: 16ms/frame -> ~60fps
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplFltk_Shutdown();
    ImGui::DestroyContext();

    // deleting the window will also delete the glwin
    delete win;

    return 0;
}
