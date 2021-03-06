#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// main
int main(void)
{
	/* Initialize the library */
	if (!glfwInit()) { return -1; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment to compile on osx
#endif

	// GLFW window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << "\n";

	{
		// Setup vertex data (and buffer(s)) and configure vertex attributes
		/*
		float vertices[] = {
			// positions			// textures
			300.0f, 300.0f,	0.0f,	1.0f, 1.0f, // top right
			300.0f, 100.0f, 0.0f,	1.0f, 0.0f, // bottom right
			100.0f, 100.0f, 0.0f,	0.0f, 0.0f, // bottom left
			100.0f, 300.0f, 0.0f,	0.0f, 1.0f  // top left 
		};
		*/
		/*
		float vertices[] = {
			 // positions          // texture coords
			 0.5f,  0.5f, 0.5f,   1.0f, 1.0f, // front top right
			 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, // front bottom right
			-0.5f, -0.5f, 0.5f,   0.0f, 0.0f, // front bottom left
			-0.5f,  0.5f, 0.5f,   0.0f, 1.0f, // front top left 

			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // back top right
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // back bottom right
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // back bottom left
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f  // back top left 
		};
		*/
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		// More cubes positions
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		// Setup GL Blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Set up array buffer and vertex buffer
		VertexArray va;
		VertexBuffer vb(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		layout.Push<float>(3); // positions
		layout.Push<float>(2); // texture coords
		va.AddBuffer(vb, layout);

		//IndexBuffer ib(indices, 12);

		// Matrix stuff
		//glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		glm::mat4 projection(1.0f);
		projection = glm::perspective(45.0f, (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		// Build and compile our shader program
		Renderer renderer;
		Shader shader("resources/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform1i("u_Texture", 0);

		// Texture stuff
		Texture texture("resources/textures/fortnite.jpg");
		texture.Bind();

		// imgui
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		// variables used in main loop
		glm::vec3 translation(0.0f, 0.0f, 0.0f);

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//renderer.Clear();

			// Matrix stuff
			//model = glm::rotate(model, glm::radians(0.2f), glm::vec3(0.5f, 1.0f, 0.0f));

			// imgui
			ImGui_ImplGlfwGL3_NewFrame();

			// Process input
			processInput(window);

			// Draw calls
			//renderer.Draw(va, ib, shader);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = glfwGetTime() * 20.0f;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

				glm::mat4 mvp = projection * view * model;

				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// imgui window
			{
				ImGui::SliderFloat3("translation", &translation.x, 0.0f, 100.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			// imgui render
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers and poll for IO events (keys, mouse, ect) */
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	// Terminate imgui
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
	// Terminate GLFW (clears all GLFW allocated resources)
	glfwTerminate();

	return 0;
}

// GLFW: whenever the window size is changed (by OS or user) this callback executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on a retina displays.
	GLCall(glViewport(0, 0, width, height));
}

// Process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
