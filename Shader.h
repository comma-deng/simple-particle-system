#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include<iostream>
#include<fstream>

using namespace std;

class Shader
{
public:
	GLuint program;
	Shader(const char *vShaderPath,const char *fShaderPath);
	void Use();

private:
	const char * vShaderSourse;
	const char * fShaderSourse;
	
	char* readShaderSource(const char *path,GLint &len);
	void compileCheck(GLuint shader,char name[]);
	void linkCheck(GLuint program);

};

Shader::Shader(const char *vShaderPath,const char *fShaderPath)
{
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint vlen;
	char *vertexShaderSource = readShaderSource(vShaderPath,vlen);
	
	glShaderSource(vertexShader,1,&vertexShaderSource,&vlen);
	glCompileShader(vertexShader);
	compileCheck(vertexShader,"vertex");

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint flen;
	char *fragmentShaderSource = readShaderSource(fShaderPath,flen);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,&flen);
	glCompileShader(fragmentShader);
    compileCheck(fragmentShader,"fragment");

	program = glCreateProgram();

	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);

	glLinkProgram(program);

	linkCheck(program);
};

void Shader:: Use()
{
	glUseProgram(program);
}

//���ϵĴ��������⣬��������ֵ������ԭ������ftell()����ֵ��fread()����ֵ��fread����ʵ�ʵõ����ַ�������������ı��ļ���\r\n��ȡ������\n����ͻᵼ�¶�ȡ���ַ���С��ʵ���ı��ļ��еĳ��ȡ�����rbȡ��rʱ�����ö�����ģʽʱ��ftell()��fread()���ص�ֵ��ͬ
char* Shader::readShaderSource(const char *path,GLint &len){
	FILE *fp = fopen(path,"rb");

	if(fp==nullptr)
	  return nullptr;

	fseek(fp,0,SEEK_END);
	int size =  ftell(fp);
	len = size;
	fseek(fp,0,SEEK_SET);
	char *buf = new char[size+1];

	fread(buf,1,size,fp);	
	buf[size + 1] = '\0';
	fclose(fp);

	return buf;
}

void Shader:: compileCheck(GLuint shader,char name[]){
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::"<<name<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	delete []infoLog;
  }
  
}

void Shader:: linkCheck(GLuint program){
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(program, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::LINK::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
	delete []infoLog;
  }

}
#endif