#include "ZenithApp.h"

#include <iostream>
#include <filesystem>


static const std::string ASSETS_PATH = "../assets/";

void ZenithApp::drawCurrentModel(CurrentModel& cur, Shader& shaderProgram, Camera& camera)
{
    if (std::holds_alternative<Model>(cur)) {
        std::get<Model>(cur).Draw(shaderProgram, camera);
    } else if (std::holds_alternative<FBXModel>(cur)) {
        std::get<FBXModel>(cur).Draw(shaderProgram, camera);
    }
}

int ZenithApp::run()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    // OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a movable windowed mode window (not exclusive fullscreen)
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int scrWidth = 1280;
    int scrHeight = 720;
    GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, "Zenith Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Center the window on the primary monitor work area (keeps taskbar accessible)
#if GLFW_VERSION_MAJOR >= 3
    int wx = 0, wy = 0, ww = 0, wh = 0;
    int mx = 0, my = 0, mw = 0, mh = 0;
#ifdef GLFW_MOUSE_PASSTHROUGH // use as proxy for newer GLFW with workarea
    glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);
#else
    // Fallback: use video mode if workarea is unavailable
    mx = 0; my = 0; mw = mode ? mode->width : 1920; mh = mode ? mode->height : 1080;
#endif
    glfwGetWindowSize(window, &ww, &wh);
    wx = mx + (mw - ww) / 2;
    wy = my + (mh - wh) / 2;
    glfwSetWindowPos(window, wx, wy);
#endif

    // Load GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to load GL via GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Initialize viewport from actual framebuffer size (handles HiDPI and resizing)
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    if (fbWidth <= 0 || fbHeight <= 0) { fbWidth = scrWidth; fbHeight = scrHeight; }
    glViewport(0, 0, fbWidth, fbHeight);

    // Shader
    Shader shaderProgram((ASSETS_PATH + std::string("default.vert")).c_str(),
                         (ASSETS_PATH + std::string("default.frag")).c_str());

    // Light uniforms
    glm::vec4 lightColor = glm::vec4(1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // GL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Camera
    Camera camera(scrWidth, scrHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    // Presets list
    std::vector<Preset> presets = {
        {"Violin (FBX)", ASSETS_PATH + std::string("Models/Violin/Violin.fbx"), Preset::Type::FBX},
        {"TestModel (FBX)", ASSETS_PATH + std::string("Models/TestModelFBX/TestModel.fbx"), Preset::Type::FBX},
        {"Adam Head (GLTF)", ASSETS_PATH + std::string("Models/adamHead/adamHead.gltf"), Preset::Type::GLTF},
        {"BoomBox (GLTF)", ASSETS_PATH + std::string("Models/boomBox/BoomBox.gltf"), Preset::Type::GLTF},
        {"Trees (GLTF)", ASSETS_PATH + std::string("Models/trees/scene.gltf"), Preset::Type::GLTF},
    };

    int selectedPreset = -1;
    CurrentModel currentModel = std::monostate{};

    auto loadPreset = [&](int idx){
        if (idx < 0 || idx >= (int)presets.size()) return;
        const Preset& p = presets[idx];

        // Validate file exists before attempting to load
        if (!std::filesystem::exists(p.path)) {
            std::cerr << "[Load] File does not exist: " << p.path << "\n";
            return;
        }

        bool loaded = false;
        if (p.type == Preset::Type::GLTF) {
            try {
                // Construct in-place; if constructor throws, variant remains unchanged
                currentModel.emplace<Model>(p.path.c_str());
                loaded = true;
            } catch (const std::exception& e) {
                std::cerr << "[Load] Failed to load GLTF model: " << p.path << " | " << e.what() << "\n";
            } catch (...) {
                std::cerr << "[Load] Failed to load GLTF model: " << p.path << " | unknown error\n";
            }
        } else {
            try {
                // Build a temporary FBXModel to verify it actually has meshes
                FBXModel tmp(p.path.c_str());
                if (tmp.meshCount() == 0) {
                    std::cerr << "[Load] FBX has 0 meshes (skipping): " << p.path << "\n";
                } else {
                    currentModel = std::move(tmp);
                    loaded = true;
                }
            } catch (const std::exception& e) {
                std::cerr << "[Load] Failed to load FBX model: " << p.path << " | " << e.what() << "\n";
            } catch (...) {
                std::cerr << "[Load] Failed to load FBX model: " << p.path << " | unknown error\n";
            }
        }

        if (loaded) {
            selectedPreset = idx;
        } else {
            // Keep previous state unchanged on failure
            std::cerr << "[Load] Keeping previous model (load failed).\n";
        }
    };

    // ImGui setup
    ImGuiLayer imgui(window, "#version 330");

    // FPS counter
    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    // Track fullscreen-like borderless state and last windowed placement
    bool borderless = false;
    int savedX = 0, savedY = 0, savedW = scrWidth, savedH = scrHeight;
    bool f11Prev = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string title = "Zenith Engine - " + FPS + "FPS / " + ms + "ms";
            if (selectedPreset >= 0 && selectedPreset < (int)presets.size()) {
                title += " | ";
                title += presets[selectedPreset].name;
            } else {
                title += " | No Model";
            }
            glfwSetWindowTitle(window, title.c_str());
            prevTime = crntTime;
            counter = 0;
        }

        // Handle dynamic resize: update viewport and camera aspect
        int curFbW = 0, curFbH = 0;
        glfwGetFramebufferSize(window, &curFbW, &curFbH);
        if (curFbW > 0 && curFbH > 0) {
            if (curFbW != fbWidth || curFbH != fbHeight) {
                fbWidth = curFbW; fbHeight = curFbH;
                glViewport(0, 0, fbWidth, fbHeight);
                camera.width = fbWidth;
                camera.height = fbHeight;
            }
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start ImGui frame first to know if UI wants mouse
        imgui.beginFrame();

        // Toggle borderless (windowed fullscreen) with F11
        bool f11Now = glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS;
        if (f11Now && !f11Prev) {
            if (!borderless) {
                // Save current windowed placement
                glfwGetWindowPos(window, &savedX, &savedY);
                glfwGetWindowSize(window, &savedW, &savedH);
                // Use monitor work area to keep taskbar accessible
                int mx = 0, my = 0, mw = 0, mh = 0;
#ifdef GLFW_MOUSE_PASSTHROUGH
                glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);
#else
                mx = 0; my = 0; mw = mode ? mode->width : 1920; mh = mode ? mode->height : 1080;
#endif
                glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
                glfwSetWindowPos(window, mx, my);
                glfwSetWindowSize(window, mw, mh);
                // Update viewport immediately after size change
                glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
                if (fbWidth > 0 && fbHeight > 0) {
                    glViewport(0, 0, fbWidth, fbHeight);
                    camera.width = fbWidth;
                    camera.height = fbHeight;
                }
                borderless = true;
            } else {
                // Restore decorations and placement
                glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
                glfwSetWindowPos(window, savedX, savedY);
                glfwSetWindowSize(window, savedW, savedH);
                // Update viewport immediately after size change
                glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
                if (fbWidth > 0 && fbHeight > 0) {
                    glViewport(0, 0, fbWidth, fbHeight);
                    camera.width = fbWidth;
                    camera.height = fbHeight;
                }
                borderless = false;
            }
        }
        f11Prev = f11Now;

        // Decide whether to capture mouse for camera (hide/lock cursor)
        ImGuiIO& io = ImGui::GetIO();
        bool uiWantsMouse = io.WantCaptureMouse;
        bool moveLook = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && !uiWantsMouse;
        if (moveLook) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.Inputs(window);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            camera.firstClick = true; // ensure no jump when resuming
        }
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        drawCurrentModel(currentModel, shaderProgram, camera);

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(260, 300), ImGuiCond_Once);
        if (ImGui::Begin("Models")) {
            for (int i = 0; i < (int)presets.size(); ++i) {
                bool isSelected = (i == selectedPreset);
                if (ImGui::Selectable(presets[i].name.c_str(), isSelected)) {
                    std::cout << "Loading preset: " << presets[i].name << std::endl;
                    loadPreset(i);
                }
            }
            if (ImGui::Button("Unload Model")) {
                // Idempotent unload: reset to empty state
                currentModel = std::monostate{};
                selectedPreset = -1;
                std::cerr << "[Unload] Model cleared.\n";
            }
        }
        ImGui::End();

        imgui.endFrame();

        glfwSwapBuffers(window);
    }

    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
