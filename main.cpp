#include <iostream>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "OBJ_Loader.h"

#include "shaders.h"
#include "camera.h"

#include "cube.h"
#include "ground.h"
#include "skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void logFPS();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void toggleCursor(GLFWwindow* window, Camera& camera, float deltaTime);

void cubeImGuiDisplay(Cube& cube);
void cameraImGuiDisplay(Camera& camera);

unsigned int loadTextureJPG(const char* path);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 35.0f, 3.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool wireframeMode = false;
bool cursorEnabled = false;

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Art Display", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	unsigned int grassTexture = loadTextureJPG("textures/texture-grass-field.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture);


	Cube::initRenderData();
	Cube cube(glm::vec3(0.0f, 15.0f, 0.0f), 30.0f, 20.0f, 30.0f);

	Ground ground(100.0f);
	ground.initRenderData();

	SkyBox skybox;
	skybox.initiateBuffer();
	skybox.loadImages();

	Shader crazyLights("shaders/crazyCube.vert", "shaders/crazyCube.frag");
	crazyLights.useShader();

	Shader groundShader("shaders/ground.vert", "shaders/ground.frag");
	groundShader.useShader();

	Shader skyBoxShader("shaders/skyBox.vert", "shaders/skyBox.frag");
	skyBoxShader.useShader();


	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	std::cout << "CWD: " << cwd << std::endl;
	objl::Loader loader;
	bool is_loaded = loader.LoadFile("/home/will/Dev/artDisplay/assets/cat/12221_Cat_v1_l3.obj"); 
	if (!is_loaded)
		std::cerr << "Failed to load OBJ file!" << std::endl;

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 600.0f);
	
	crazyLights.setMat4("projection", projection);
	groundShader.setMat4("projection", projection);
	skyBoxShader.setMat4("projection", projection);
	glfwSetKeyCallback(window, keyCallback);

	// RENDER LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		cubeImGuiDisplay(cube);
		cameraImGuiDisplay(camera);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 600.0f);
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.137f, 0.969f, 0.941f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		float timeValue = glfwGetTime();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 cubeModel =  cube.getModelMatrix();

		crazyLights.useShader();
		crazyLights.setFloat("iTime", timeValue);
		crazyLights.setMat4("view", view);
		crazyLights.setMat4("projection", projection);
		crazyLights.setMat4("model",cubeModel);
		cube.draw(crazyLights);

		groundShader.useShader();
		groundShader.setMat4("view", view);
		groundShader.setMat4("projection", projection);
		groundShader.setInt("grassTexture", 0);
		ground.draw(groundShader);

		glDepthFunc(GL_LEQUAL);	

		skyBoxShader.useShader();
		skyBoxShader.setInt("skybox", 0);
		glm::mat4 skyBoxView = glm::mat4(1.0f);
		skyBoxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		skyBoxShader.setMat4("view", skyBoxView);
		skyBoxShader.setMat4("projection", projection);
		skybox.draw();
		
		glDepthFunc(GL_LESS);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//logFPS();
		glfwSwapBuffers(window);
		glfwPollEvents();
		//glfwSetKeyCallback(window, keyCallback);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	ground.cleanupRenderData();	
	Cube::cleanupRenderData();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Toggle wireframe mode when 'M' is pressed
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		wireframeMode = !wireframeMode;

		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		toggleCursor(window, camera, deltaTime);
}
// for enabling and disabling the cursor
void toggleCursor(GLFWwindow* window, Camera& camera, float deltaTime)
{
	static bool cursorEnabled = false;

	cursorEnabled = !cursorEnabled;

	if (cursorEnabled)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera.DisableMouseControl();
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.EnableMouseControl();
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

}

void logFPS() 
{
	static float previousTime = glfwGetTime();
	static int frameCount = 0;

	float currentTime = glfwGetTime();
	float frameSeconds = currentTime - previousTime;

	frameCount++;

	if (frameSeconds > 1.0f)
	{
		float framesPerSecond = frameCount / (currentTime - previousTime);
		std::cout << "FPS: " << framesPerSecond << std::endl;
		previousTime = currentTime;
		frameCount = 0;
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void cameraImGuiDisplay(Camera& camera) {
	ImGui::Begin("Camera Controls");

	ImGui::SliderFloat("Zoom", &camera.Zoom, 0.5f, 160.0f);
	ImGui::SliderFloat("Mouse Sensitivity", &camera.MouseSensitivity, 0.01f, 0.2f);
	ImGui::SliderFloat("Movement Speed", &camera.MovementSpeed, 0.1f, 50.0f);

	ImGui::End();
}

void cubeImGuiDisplay(Cube& cube) {
	ImGui::Begin("Cube Controls");
	ImGui::SliderFloat3("Position", glm::value_ptr(cube.getPosition()), -50.0f, 50.0f);
	ImGui::SliderFloat("Angle", &cube.angle, 0.0f, 360.0f);
	ImGui::SliderFloat("Width", &cube.width, 0.1f, 50.0f);
	ImGui::SliderFloat("Height", &cube.height, 0.1f, 50.0f);
	ImGui::SliderFloat("Depth", &cube.depth, 0.1f, 50.0f);

	ImGui::End();
}

unsigned int loadTextureJPG(const char* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture: " << path << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}


