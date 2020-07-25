
#include "Headers.h"

#define IMGUI_DISABLE_WIN32_FUNCTIONS
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include "Scene.h"
#include "GLRenderer.h"
#include "Log.h"
#include <sstream>
#include <list>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "assimp-vc142-mtd.lib")

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		        type, severity, message);
	}
}

std::shared_ptr<Scene> scene;
std::shared_ptr<GLRenderer> renderer;
uint32_t activeInstanceId = UINT32_MAX;

void resize(GLFWwindow* window, int width, int height)
{
	scene->MainCamera().SetAspect(static_cast<float>(width) / static_cast<float>(height));
	renderer->SetViewport(width, height);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_W)
	{
		scene->MainCamera().Forward();
	}
	else if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_A)
	{
		scene->MainCamera().Left();
	}
	else if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_S)
	{
		scene->MainCamera().Backward();
	}
	else if ((action == GLFW_PRESS || action == GLFW_REPEAT) && key == GLFW_KEY_D)
	{
		scene->MainCamera().Right();
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse)
	{
		return;
	}
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xW, yW;
		glfwGetCursorPos(window, &xW, &yW);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		yW = static_cast<double>(height) - yW;
		uint32_t instanceId;
		if (activeInstanceId != UINT32_MAX)
		{
			auto& previousPickedInstance = scene->Instance(activeInstanceId);
			previousPickedInstance.SetRenderBoundingBox(false);
		}
		if (renderer->PickInstance(xW / (static_cast<double>(width) * 0.5) - 1.0, yW / (static_cast<double>(height) * 0.5) - 1.0, instanceId))
		{
			auto& pickedInstance = scene->Instance(instanceId);
			pickedInstance.SetRenderBoundingBox(true);
			activeInstanceId = instanceId;
		}
		else
		{
			activeInstanceId = UINT32_MAX;
		}
	}
}

void RenderMeshIcon(const Mesh& mesh)
{
	ImGui::BeginGroup();
	if (mesh.IconTextureId() != 0)
	{
		ImGui::Image(reinterpret_cast<void*>(scene->Texture(mesh.IconTextureId()).GetTextureId()), {64, 64});
	}
	// ImGui::SameLine();
	ImGui::Text(mesh.Name().c_str());
	ImGui::EndGroup();
}

int main(int argc, char** argv)
{
	glfwInit();
	auto initialWidth = 640;
	auto initialHeight = 480;
	auto window = glfwCreateWindow(initialWidth, initialHeight, "BZEngine - Level Editor", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui::StyleColorsDark();

	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetKeyCallback(window, keyboard);
	glfwSetMouseButtonCallback(window, mouse);
	
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	scene.reset(new Scene());

	renderer.reset(new GLRenderer(scene));

	const auto cubeMesh = scene->AddMesh("assets/meshes/cube/cube.obj", "assets/meshes/cube/", "assets/meshes/cube/", "Cube");
	// const auto treeMesh = scene->AddMesh("assets/meshes/tree/Tree.obj", "assets/meshes/tree/", "assets/meshes/tree/", "Tree");
	
	constexpr int MAX_X = 1;
	constexpr int MAX_Z = 1;

	for (auto x = 0; x < MAX_X; ++x)
	{
		for (auto z = 0; z < MAX_Z; ++z)
		{
			const auto cubeTransform = scene->AddTransform({ {1.0f, 1.0f, 1.0f}, {}, {}, {-(MAX_X - 1) + (static_cast<float>(x) * 1.25f), 0.0f, -(MAX_Z - 1) + (static_cast<float>(z) * 1.25f)} });
			const auto index = x * MAX_X + z;
			const auto formatString = "%03d";
			auto indexStringSize = std::snprintf(nullptr, 0, formatString, index) + 1;
			std::unique_ptr<char[]> indexStringBuffer(new char[indexStringSize]);
			std::snprintf(indexStringBuffer.get(), indexStringSize, formatString, index);
			const auto instanceName = "Cube." + std::string(indexStringBuffer.get(), indexStringBuffer.get() + (indexStringSize - 1));
			const auto cubeInstance = scene->AddInstance({ cubeMesh, cubeTransform, instanceName });
		}
	}

	// const auto treeTransform = scene->AddTransform({ glm::vec3{1.0f}, {}, {}, {-4.0f, 0.0f, -4.0f }, "Tree" });
	// const auto treeInstance = scene->AddInstance({ treeMesh, treeTransform, "Tree.001" });

	const auto mainCamera = scene->AddCamera({
		{2.0f, 1.5f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, glm::radians(70.0f), {}, 0.1f,
		200.0f
		}, true);

	// resize(window, initialWidth, initialHeight);

	glfwMaximizeWindow(window);
	
	while (!glfwWindowShouldClose(window))
	{
		renderer->RenderFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1996 => 564, 0 => 664
		
		if (activeInstanceId != UINT32_MAX) {
			ImGui::SetNextWindowPos({ 0.0f, 0.0f });
			ImGui::Begin("Selected Instance", {}, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
			auto activeInstance = scene->Instance(activeInstanceId);
			auto activeInstanceMesh = scene->Mesh(activeInstance.MeshId());
			auto activeInstanceTransform = scene->Transform(activeInstance.TransformId());
			ImGui::Text("Instance: %s", activeInstance.Name().c_str());
			ImGui::Text("Mesh: %s", activeInstanceMesh.Name().c_str());
			ImGui::Separator();
			ImGui::Text("Transform");
			ImGui::Text("Translation: %s", glm::to_string(activeInstanceTransform.Translation()).c_str());
			ImGui::Text("Rotation: %s", glm::to_string(activeInstanceTransform.Rotation()).c_str());
			ImGui::Text("Rotation Origin: %s", glm::to_string(activeInstanceTransform.RotationOrigin()).c_str());
			ImGui::Text("Scale: %s", glm::to_string(activeInstanceTransform.Scale()).c_str());
			ImGui::End();
		}

		const auto viewport = renderer->Viewport();
		const auto width = static_cast<float>(viewport.x);
		const auto height = static_cast<float>(viewport.y);
		ImGui::SetNextWindowPos({ width - 0.2f * width, 0.0f});
		ImGui::SetNextWindowSize({ 0.2f * width, 0.4f * height });

		ImGui::Begin("Assets", {}, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
		const auto& allMeshes = scene->Meshes();
		std::vector<const char*> allMeshNames;
		allMeshNames.resize(allMeshes.size());
		for (auto i = 0; i < allMeshes.size(); ++i)
		{
			allMeshNames[i] = allMeshes[i].Name().c_str();
			RenderMeshIcon(allMeshes[i]);
		}

		// static int currentItem = 0;
		// ImGui::Combo("Meshes", &currentItem, &allMeshNames[0], allMeshNames.size());

		ImGui::End();
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}