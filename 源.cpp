#define GLEW_STATIC

#include <iostream>
#include <vector>


#include <glew.h>
#include <glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "particle.h"
#include "particleSystem.h"s
#include "Camera.h"

using namespace std;
using namespace glm;


int keys[1024];
Camera camera(glm::vec3(0.0f, 0.0, 100.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if(key==GLFW_KEY_W || key==GLFW_KEY_S || key==GLFW_KEY_A || key==GLFW_KEY_D)
    {
        if(action == GLFW_PRESS)
		{
            keys[key] = true;
		}
        else if(action == GLFW_RELEASE)
            keys[key] = false;	
		
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}	

void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


int main()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1000,800,"cooma's window",nullptr,nullptr);
	if(window == nullptr) 
	{
		std::cout<<"fail to create glfw window!"<<std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK)
	{
		std::cout<<"failed to initialize glew!";
		return -1;

	}
	int width,hight;
	glfwGetFramebufferSize(window,&width,&hight);
	glViewport(0,0,width,hight);

	glEnable(GL_DEPTH_TEST);  //开启深度检测

	glfwSetKeyCallback(window,key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mat4 model;
	mat4 view;
	mat4 projection;

	model = mat4();
	view = camera.GetViewMatrix();
	projection = perspective(45.0f, (float)width/(float)hight, 1.0f, 1000.0f);
	
	//ParticleSystem pSystem(cone,point,200,vec3(0,0,0),6.0,6.0,3.0,vec3(0.0,-0.0,0.0));
	
	//ParticleSystem pSystem(ball,point,10000,vec3(0,0,0),3.0,3.0,20.0,vec3(0.0,-2.0,0.0));
	ParticleSystem pSystem(cylinder,point,10000,vec3(0,0,0),3.0,6.0,20.0,vec3(0.0,-2.0,0.0));
	
	pSystem.generateParticles();

	pSystem.setModel(model);
	pSystem.setView(view);
	pSystem.setProjection(projection);

	pSystem.initial();
	
	pSystem.setStartTime(glfwGetTime());

	//点的大小会随距离变化
	glEnable(GL_PROGRAM_POINT_SIZE);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Do_Movement();

		view = camera.GetViewMatrix();
		pSystem.setView(view);
		
		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		pSystem.rendering();
		
		glfwSwapBuffers(window);
	}


}