cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(imgui STATIC
    imconfig.h
    imgui_internal.h
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    backends/imgui_impl_opengl3_loader.h
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_win32.h

    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_win32.cpp

)