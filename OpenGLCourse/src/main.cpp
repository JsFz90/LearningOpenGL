#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Function prototype Declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders Code
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n" 
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* FragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n "
	"}\0";


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

	// build and compile our shader program
	// ------------------------------------
	// Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);  // ID
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);    // 2do parametro cantidad de Strings
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << std::endl;
	}

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << infoLog << std::endl;
	}

	// Link Shaders
	unsigned int program = glCreateProgram(); // Retorna un ID
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	// check for linking errors
	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
	}

	// Delete shader objects once we've linked them into the program object; we no longer need them anymore:
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,   // bottom left      - vertex 1
		 0.5f, -0.5f, 0.0f,   // bottom right     - vertex 2
		 0.0f,  0.5f, 0.0f    // top              - vertex 3
	};

	unsigned int VAO, VBO;
	// Gen Vertex Array Object, Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO); // 1. bind Vertex Array Object

	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // 0. copy our vertices array in a buffer for OpenGL to use
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers
	// Layout tell OpenGL how it should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // layout 0 del shader, 3 values ( cada vertex ), son float, normslized false, stride, start
	glEnableVertexAttribArray(0);
	

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
		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // GL_TRIANGLES, second argument specifies the starting index of the vertex array, last argument specifies how many vertices we want to draw

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // will swap the color buffer (a large 2D buffer that contains color values for each pixel in GLFW's window) that is used to render to during this render iteration and show it as output to the screen
		glfwPollEvents();       //  function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can register via callback methods).
	}


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