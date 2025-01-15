/**
 * @file gui.cpp
 * @brief GUI implementation
 * @author Rafael Garcia
 * @date 2024-12-16
 * @version 1.0.0
 */

#include "gui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * @brief Window
 */
static GLFWwindow* window = nullptr;

/**
 * @brief Track window size dynamically
 */
static int windowWidth = 1280;
static int windowHeight = 720;

/**
 * @brief Window resize callback function
 * @param window GLFW window
 * @param width New window width
 * @param height New window height
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

/**
 * @brief Initialize the GUI
 * @return true if the GUI was initialized successfully, false otherwise
 */
bool init_gui() {

    // Mission failed, we'll get 'em next time
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make the window ( will add baddass logo later )
    window = glfwCreateWindow(windowWidth, windowHeight, "Note+", nullptr, nullptr);

    // No window, denied
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Register resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    std::cout << "GUI initialized" << std::endl;
    return true;
}

// Global sidebar toggle
static bool showSidebar = true;
static bool sidebarExpanded = true;
static float sidebarWidth = 250.0f;
static float slimSidebarWidth = 60.0f;

/**
 * @brief Render the GUI
 * @return true if rendered successfully, false otherwise
 */
bool render_gui() {
    if (glfwWindowShouldClose(window)) return false;

    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Ribbon Bar Container (Width dynamically adjusts)
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)windowWidth, 100));
    ImGui::Begin("Ribbon", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // Ribbon Tabs
    if (ImGui::BeginTabBar("RibbonTabs", ImGuiTabBarFlags_Reorderable)) {
        if (ImGui::BeginTabItem("Home")) {
            ImGui::Spacing();

            if (ImGui::Button("Toggle Sidebar")) {
                showSidebar = !showSidebar;
            }

            ImGui::SameLine();
            ImGui::Button("Paste", ImVec2(60, 40));
            ImGui::SameLine();
            ImGui::Button("Cut", ImVec2(60, 40));
            ImGui::SameLine();
            ImGui::Button("Copy", ImVec2(60, 40));

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Insert")) {
            ImGui::Text("Insert Options");
            ImGui::Button("Picture", ImVec2(100, 50));
            ImGui::SameLine();
            ImGui::Button("Table", ImVec2(100, 50));
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();

    // Main Content Area
    ImGui::SetNextWindowPos(ImVec2(0, 100));
    ImGui::SetNextWindowSize(ImVec2((float)windowWidth, windowHeight - 100));
    ImGui::Begin("MainArea", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::Columns(2, "MainColumns", false);

    // Sidebar (Conditional)
    if (showSidebar) {
        ImGui::SetColumnWidth(0, sidebarExpanded ? sidebarWidth : slimSidebarWidth);
        ImGui::BeginChild("Sidebar", ImVec2(0, 0), true);

        // Sidebar Header
        ImGui::Text("Sidebar");
        ImGui::Separator();

        // Toggle Slim/Expanded Sidebar
        if (ImGui::Button(sidebarExpanded ? "<<" : ">>", ImVec2(-FLT_MIN, 0))) {
            sidebarExpanded = !sidebarExpanded;
        }
        ImGui::Separator();

        // Render Sidebar Options (Icons Only or Icons with Text)
        if (sidebarExpanded) {
            // Expanded Sidebar (Text and Icon)
            if (ImGui::Button("📂 Open File", ImVec2(-FLT_MIN, 40))) {
                // Open File Logic
            }
            if (ImGui::Button("⚙️ Settings", ImVec2(-FLT_MIN, 40))) {
                // Settings Logic
            }
            if (ImGui::Button("❌ Exit", ImVec2(-FLT_MIN, 40))) {
                glfwSetWindowShouldClose(window, true);
            }
        } else {
            // Slim Sidebar (Icons Only)
            if (ImGui::Button("📂", ImVec2(-FLT_MIN, 40))) {
                // Open File Logic
            }
            if (ImGui::Button("⚙️", ImVec2(-FLT_MIN, 40))) {
                // Settings Logic
            }
            if (ImGui::Button("❌", ImVec2(-FLT_MIN, 40))) {
                glfwSetWindowShouldClose(window, true);
            }
        }

        ImGui::EndChild();
    }
    ImGui::NextColumn();

    // Main Editor Area
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);
    ImGui::Text("Welcome to Noteplus - Document Editor");
    ImGui::Text("Edit your content here.");
    ImGui::EndChild();

    ImGui::Columns(1);

    ImGui::End();

    // Render ImGui
    ImGui::Render();
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    return true;
}

/**
 * @brief Cleanup the GUI
 */
void cleanup_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "GUI cleaned up" << std::endl;
}
