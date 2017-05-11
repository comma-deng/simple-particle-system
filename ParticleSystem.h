#pragma once

#include <glew.h>

#include <glm/gtc/random.hpp>

#include "particle.h"
#include "Shader.h"

void quad(vector<vec3>& vertices,vector<vec3>& points,int a,int b,int c,int d)
{
	points.push_back(vertices[a]);
	points.push_back(vertices[b]);
	points.push_back(vertices[c]);

	points.push_back(vertices[a]);
	points.push_back(vertices[c]);
	points.push_back(vertices[d]);

}

enum fireWorkType
{
	ball,
	cone,
	cylinder
};

class ParticleSystem
{
public:
	ParticleSystem(fireWorkType type,int numParticlePerWave,vec3 emitPosistion,int wave,float minLifeTime = 3.0f,float maxLifeTime = 6.0f,float interval = 0.05f,float speed = 10.0f,vec3 acceleration = vec3(0.0,-1.0,0.0))
	{
		this->shader = new Shader("vShader.glsl","fShader.glsl");
		this->numParticlePerWave = numParticlePerWave;
		this->emitPosistion = emitPosistion;
		this->minLifeTime = minLifeTime;
		this->maxLifeTime = maxLifeTime;
		this->wave = wave;
		this->interval = interval;
		this->speed = speed;
		this->acceleration = acceleration;
		this->type = type;

		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
	}

	void generateParticles()
	{	
		vector<vec3> points;
		
		for(int i=0;i<wave;i++)
		{
			for(int j=0;j<numParticlePerWave;j++)
			{
				points.push_back(emitPosistion);
			}
		}
		
		vector<vec3> velocities;

		switch (type)
		{
		case ball:
			{
				for(int i=0;i<numParticlePerWave;i++)
				{
					vec3 velocity = ballRand(speed);
					for(int j=0;j<wave;j++)
					{
						velocities.push_back(velocity);
					}
				}
			}
			break;
		case cylinder:
			{
				for(int i=0;i<numParticlePerWave;i++)
				{
					for(int j=0;j<wave;j++)
					{
						float rad = 10.0f/ 180.0f * 3.14f;
						float rate = linearRand(0.5f,1.0f);
						vec2 circle = circularRand(speed*sin(rad) * rate);
						vec3 velocity = vec3(circle.x,speed * cos(rad),circle.y ) / linearRand(0.0f,speed/1.4f); // + ballRand(speed/30.0f); 
						//vec3 velocity = vec3(circle.x,speed/1.4f,circle.y )  + ballRand(speed/10.0f); 
						velocities.push_back(velocity);
					}
				}
			}
			break;
		case cone:
			{
				for(int i=0;i<numParticlePerWave;i++)
				{
					float rad = 30.0f/ 180.0f * 3.14f;
					if(i%2)
						rad/=2;
					float rate = linearRand(0.5f,1.0f);
					vec2 circle = circularRand(speed*sin(rad) * rate);
					vec3 velocity = vec3(circle.x,speed * cos(rad),circle.y ) / linearRand(0.0f,speed/1.4f);
					for(int j=0;j<wave;j++)
					{
						 // + ballRand(speed/30.0f); 
						//vec3 velocity = vec3(circle.x,speed/1.4f,circle.y )  + ballRand(speed/10.0f); 
						velocities.push_back(velocity );
					}
				}
			}
			break;
		default:
			break;
		}
		
		vector<float> init_times;
		for(int i=0;i<numParticlePerWave;i++)
		{
			for(int j=0;j<wave;j++)
			{
				init_times.push_back(j*interval);
			}
		}

		vector<float> life_times;
		float min_lifeTime = 3.0
			 ,max_lifeTime = 3.0;
		for(int i=0;i<numParticlePerWave;i++)
		{
			for(int j=0;j<wave;j++)
			{
				life_times.push_back(linearRand(min_lifeTime,max_lifeTime));
			}
		}
		
		glBindVertexArray(VAO);

		int totalSize = points.size() *  (sizeof(vec3)*2 + sizeof(float)*2 );
		int offset = 0;
		glGenBuffers(1,&VBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,totalSize ,NULL,GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER,offset,points.size()*sizeof(vec3),points.data());
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vec3),(GLvoid *)offset);//参数从左至右：第几个分量，组件数量，类型，每两个分量之间的距离，第一个组件的偏移量
		offset += points.size() * sizeof(vec3);
		
		glBufferSubData(GL_ARRAY_BUFFER,offset,points.size()*sizeof(vec3),velocities.data());
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vec3),(GLvoid *)offset);
		offset += points.size() * sizeof(vec3);

		glBufferSubData(GL_ARRAY_BUFFER,offset,points.size() * sizeof(float),init_times.data());
		glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,sizeof(float),(GLvoid *)offset);
		offset += points.size() * sizeof(float);

		glBufferSubData(GL_ARRAY_BUFFER,offset,points.size() * sizeof(float),life_times.data());
		glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,sizeof(float),(GLvoid *)offset);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}

	void beforeRendering()
	{
		shader->Use(); //传参数之前记得用这个

		GLuint modelLoc = glGetUniformLocation(shader->program,"model");  //声明uniform变量，与shader进行通信。只要保证此处变量名与glsl中一致即可。
		GLuint viewLoc = glGetUniformLocation(shader->program, "view");
		GLuint projLoc = glGetUniformLocation(shader->program, "projection"); 

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 

		GLuint initColorLoc = glGetUniformLocation(shader->program,"initColor");
		GLuint finalColorLoc = glGetUniformLocation(shader->program,"finalColor");
	
		glUniform4f(initColorLoc,0.8,0.0,0.0,1.0);
		glUniform4f(finalColorLoc,0.8,0.5,0.0,0.0);

		//GLuint lifeTimeLoc = glGetUniformLocation(shader->program,"lifeTime");
		//glUniform1f(lifeTimeLoc,lifeTime);

		GLuint accelerationLoc =  glGetUniformLocation(shader->program,"acceleration");
		glUniform3fv(accelerationLoc,1,value_ptr(acceleration));
	}

	void rendering()
	{
		float timediff = glfwGetTime() - startTime;
		GLuint timeLoc = glGetUniformLocation(shader->program,"time"); 
		glUniform1f(timeLoc,timediff);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS,0,numParticlePerWave * wave);//6*6*pSystem.numParticlePerWave * pSystem.lifeTime / pSystem.interval);
		glBindVertexArray(0);
	}

	void setModel(mat4 model)
	{
		this->model = model;
	}

	void setView(mat4 view)
	{
		this->view = view;
	}

	void setProjection(mat4 projection)
	{
		this->projection = projection;
	}

	void setStartTime(float startTime)
	{
		this->startTime = startTime;
	}


public:
	int numParticlePerWave;
	GLuint VAO;
	GLuint VBO;
	vec3 emitPosistion;

	float minLifeTime;
	float maxLifeTime;
	int wave;
	float interval;
	float speed ;  ///速率
	vec3 acceleration;

	Shader *shader;

	mat4 model;
	mat4 view;
	mat4 projection;

	float startTime;

	fireWorkType type;
};