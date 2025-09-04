#include "ImGuiLayer.h"

#include <glad/glad.h>

#include <imgui.h>

// Try various include layouts used by different packagers
#if __has_include(<imgui/backends/imgui_impl_glfw.h>)
  #include <imgui/backends/imgui_impl_glfw.h>
  #include <imgui/backends/imgui_impl_opengl3.h>
#elif __has_include(<backends/imgui_impl_glfw.h>)
  #include <backends/imgui_impl_glfw.h>
  #include <backends/imgui_impl_opengl3.h>
#else
  #include <imgui_impl_glfw.h>
  #include <imgui_impl_opengl3.h>
#endif

ImGuiLayer::ImGuiLayer(GLFWwindow* window, const char* glsl_version)
    : m_window(window), m_glsl_version(glsl_version) {
    IMGUI_CHECKVERSION();
    m_ctx = ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(m_glsl_version);
}

ImGuiLayer::~ImGuiLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    if (m_ctx) {
        ImGui::DestroyContext(m_ctx);
        m_ctx = nullptr;
    }
}

void ImGuiLayer::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
