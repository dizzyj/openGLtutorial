#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<stb/stb_image.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Camera.h"
#include"Texture.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 800;
const unsigned int height = 800;
constexpr double pi = 3.14159265358979323846;
//vertices coordinates
GLfloat vertices[] = {
		//Vertices				//Colors		//texture coordinates
		-0.5f, 0.0, 0.5f,		0.0f, 1.0f,0.0f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.0f, 0.0f,1.0f,	5.0f, 1.0f,
		 0.5f, 0.0f, -0.5f,		0.0f, 1.0f,0.0f,	0.0f, 0.0f,
		 0.5f, 0.0f, 0.5f,		1.0f, 0.0f,0.0f,	5.0f, 0.0f,
		 0.0f, 0.8f, 0.0f,		1.0f, 0.0f,0.0f,	2.5f, 5.0f,
};
//indices tell OpenGL the order of vertices to process to get the desired image
GLuint indices[]{
	0,1,2,	
	0,2,3,
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4
};
GLfloat thefloor[] = {
	//vertices			//colors
	1.0f,0.0f,1.0f,		1.0f, 1.0f, 0.0f,  
	-1.0f,0.0f,1.0f,	1.0f, 1.0f, 0.0f,
	-1.0f,0.0f,-1.0f,	1.0f, 1.0f, 0.0f,
	1.0f,0.0f,-1.0f,	1.0f, 1.0f, 0.0f,
};
GLuint floorindices[]{
	0,1,2,
	2,3,1
};

//prototypes
void processInput(GLFWwindow* window);
//
int main() {
	glfwInit(); //initializes glfw

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //tells glfw that we are using OPENGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //tells glfw that we are using the core profile because we are not babies I guess?

	//create window
	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make window current context
	glfwMakeContextCurrent(window);

	//initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//tell opengl the size of the window
	glViewport(0, 0, width, height);

	//make the thing to draw
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//create shader program
	Shader shaderProgram("default.vert", "default.frag");
	//create and bind VAO 
	VAO VAO1;
	VAO1.Bind();
	//Create VBO and pass in vertices
	VBO VBO1(vertices, sizeof(vertices));
	//Create EBO and pass in indices
	EBO EBO1(indices, sizeof(indices));
	//link VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//unbind VAO,VBO,EBO
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//floor
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(thefloor, sizeof(thefloor));
	EBO EBO2(floorindices, sizeof(floorindices));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();


	//texture
	Texture dragon("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	dragon.texUnit(shaderProgram, "tex0", 0);
	
	//enables depth
	glEnable(GL_DEPTH_TEST);

	//camer object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	//tells the window to stay open until something tells it to close
	while (!glfwWindowShouldClose(window)) { 
		//input 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.speed *= deltaTime;
		//processInput(window); 
		camera.Inputs(window);
		//rendering commands
		// backgournd colour
		glClearColor(0.2f,0.3f,0.3f,1.0f); 
		//clears buffer bit and depth bit
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		dragon.Bind();
		//Bind the VAO so open GL knows to use it
		VAO1.Bind();
		// draw the primitives
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		//check call events and swap buffers
		glfwSwapBuffers(window); //swaps the buffers and essentially draws to the screen
		glfwPollEvents(); // polls events of the window
	}
	
	//Delete all objects we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	dragon.Delete();
	glfwDestroyWindow(window);
	glfwTerminate(); //kills glfw
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}