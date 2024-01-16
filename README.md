# fltk-imgui demo

Use FLTK's Fl_Gl_Window to create an imgui window.

## Building
This assumes that FLTK 1.4.0 is installed. It grabs the imgui sources via CMake's fetchContent.
```bash
cmake -Bbin
cmake --build bin --parallel
./bin/app
```

