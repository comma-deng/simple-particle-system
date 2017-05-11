#define GLEW_STATIC

#include <iostream>
#include <vector>


#include <glew.h>
#include <glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "particle.h"
#include "particleSystem.h"

using namespace std;
using namespace glm;


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

	glEnable(GL_DEPTH_TEST);  //¿ªÆôÉî¶È¼ì²â


	mat4 model;
	mat4 view;
	mat4 projection;

	model = mat4();
	view = lookAt(vec3(0,0,100), vec3(0,0,0), vec3(0,1,0));
	projection = perspective(45.0f, (float)width/(float)hight, 1.0f, 1000.0f);
	
	//ParticleSystem(fireWorkType type,int numParticlePerWave,vec3 emitPosistion,int wave,float minLifeTime = 3.0f,float maxLifeTime = 6.0f,float interval = 0.05f,float speed = 10.0f,vec3 acceleration = vec3(0.0,-1.0,0.0))
	//ParticleSystem pSystem(cylinder,500,vec3(0,0,0),6.0,0.05,15.0,vec3(0.0,-1.0,0.0));
	ParticleSystem pSystem(ball,5000,vec3(0,0,0),1,3.0,3.0,1.0);
	//ParticleSystem pSystem(cylinder,5000,vec3(0,0,0),3.0,1.0,20.0,vec3(0.0,-2.0,0.0));
	pSystem.generateParticles();

	pSystem.setModel(model);
	pSystem.setView(view);
	pSystem.setProjection(projection);

	pSystem.beforeRendering();
	
	pSystem.setStartTime(glfwGetTime());

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		pSystem.rendering();
		/*
		glBindVertexArray(pSystem.VAO);
		glDrawArrays(GL_POINTS,0,pSystem.numParticlePerWave);//6*6*pSystem.numParticlePerWave * pSystem.lifeTime / pSystem.interval);
		glBindVertexArray(0);
		*/
		glfwSwapBuffers(window);
	}


}