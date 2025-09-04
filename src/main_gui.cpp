#include "TaskManager.h"
#include "PomodoroTimer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "TimeM - ImGui Edition", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    TaskManager taskManager;
    PomodoroTimer pomodoro;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("TimeM");
        if (ImGui::BeginTabBar("Tabs")) {
            if (ImGui::BeginTabItem("Tasks")) {
                static char buf[128] = "";
                ImGui::InputText("New Task", buf, 128);
                if (ImGui::Button("Add Task")) {
                    taskManager.addTask(buf);
                    buf[0] = '\0';
                }
                for (auto& t : taskManager.getTasks()) {
                    ImGui::BulletText("%s", t.getName().c_str());
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Pomodoro")) {
                if (ImGui::Button("Start")) pomodoro.start();
                ImGui::SameLine();
                if (ImGui::Button("Stop")) pomodoro.stop();
                ImGui::Text("Time left: %d sec", pomodoro.getRemainingTime());
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Statistics")) {
                ImGui::Text("Total tasks: %zu", taskManager.getTasks().size());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
