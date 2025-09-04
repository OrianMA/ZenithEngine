#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

// Forward-declare ImGui types to avoid heavy includes in header
struct ImGuiContext;

// Minimal wrapper that initializes Dear ImGui with GLFW + OpenGL3 backends
class ImGuiLayer {
public:
    ImGuiLayer(GLFWwindow* window, const char* glsl_version = "#version 330");
    ~ImGuiLayer();

    void beginFrame();
    void endFrame();

private:
    GLFWwindow* m_window {nullptr};
    ImGuiContext* m_ctx {nullptr};
    const char* m_glsl_version {"#version 330"};
};
