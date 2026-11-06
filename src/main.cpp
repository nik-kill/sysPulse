#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <cstdio>

namespace {
void glfw_error_callback(int error, const char* description) {
	std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}
}

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(1280, 720, "SysPulse", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("SysPulse");
		ImGui::TextUnformatted("Hello, SysPulse!");
		ImGui::TextUnformatted("Windows desktop monitor - v0.1.0");
		ImGui::End();

		ImGui::Render();
		int display_width = 0;
		int display_height = 0;
		glfwGetFramebufferSize(window, &display_width, &display_height);
		glViewport(0, 0, display_width, display_height);
		glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
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
