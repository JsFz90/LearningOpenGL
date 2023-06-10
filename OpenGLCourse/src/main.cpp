#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "Shader.h"

// Function prototype Declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// App Entry point
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set minor version to 3   osea sera 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC Os
#endif 

	// GLFW window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning OpenGL", NULL, NULL); //The function returns a GLFWwindow object that we will later need for other GLFW operations
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // tell GLFW to make the context of our window the main context on the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	//  GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// There is a maximum number of vertex attributes we're allowed to declare limited by the hardware.
	int nvAttrs = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nvAttrs);
	std::cout << "Maximun number of vertex attributes supported: " << nvAttrs << std::endl;

	// build and compile our shader program
	// ------------------------------------
	Shader firstShader("src/assets/shaders/vshader.glsl", "src/assets/shaders/fshader.glsl");


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	/*float vertices[] = {
		 0.5f,  0.5f, 0.0f,   // top right        - vertex 1
		 0.5f, -0.5f, 0.0f,   // bottom right     - vertex 2
		-0.5f, -0.5f, 0.0f,   // bottom left      - vertex 3
		-0.5f,  0.5f, 0.0f    // top left         - vertex 4
	};*/

	float vertices[] = {
		// positions 0       // colors 1
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	/*unsigned int indices[] = {
		0, 1, 3,         //  triangle 1
		1, 2, 3           //  triangle 2
	};*/

	unsigned int indices[] = {
		0, 1, 2,
	};

	unsigned int VAO, VBO, EBO;
	// Gen Vertex Array Object, Vertex Buffer Object and Element Buffer Object(Index Buffer)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO); // 1. bind Vertex Array Object

	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // 0. copy our vertices array in a buffer for OpenGL to use
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. then set our vertex attributes pointers
	// Layout tell OpenGL how it should interpret the vertex data
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // layout 0 del shader, 3 values ( cada vertex ), son float, normslized false, stride, start
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbinds
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
   // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

	// Render Loop
	// -------------------------------------
	while (!glfwWindowShouldClose(window))  // The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW has been instructed to close
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// the glClearColor function is a state-setting function and glClear is a state-using function in that it uses the current state to retrieve the clearing color from.

		// draw our first triangle
		firstShader.Bind();

		//update shader uniform
		double timeValue = glfwGetTime();   // returns time in seconds
		float greenValue = static_cast<float>((sin(timeValue) / 2) + 0.5);   //sin siempre da un valor entre 0 y 1 
		firstShader.SetFloat4("ourColor", { 0.0f, greenValue, 0.0f, 1.0f });

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // GL_TRIANGLES, second argument specifies the starting index of the vertex array, last argument specifies how many vertices we want to draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indices
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen
		glfwPollEvents();       //  function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods).
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// Function Definition

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);  // GLsizei es int
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}