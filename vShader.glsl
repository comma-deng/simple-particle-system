#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 speed;
layout (location = 2) in float lifeTime;


//out vec4 color;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

uniform vec4 initColor;
uniform vec4 finalColor;

uniform float time; 
//uniform float lifeTime;

uniform vec3 acceleration;

//out vec4 color;

out VS_OUT {
    vec4 color;
	vec3 dir;
} vs_out;


void main()
{
	float myTime = time;
	float surviveTime = myTime - int(myTime / lifeTime) * lifeTime;
	/*
	if(surviveTime<0)
	{
		surviveTime = 0;
	}
	*/
   //gl_Position = projection * view *  model * vec4(position + (speed + acceleration * surviveTime / 2) * surviveTime, 1.0);
	//gl_Position = projection  *  model * vec4(position , 1.0);
	gl_Position = vec4(position + (speed + acceleration * surviveTime / 2) * surviveTime , 1.0);
	vs_out.color = mix(initColor,finalColor,surviveTime/lifeTime);
	vs_out.dir = normalize(speed);
	//gl_PointSize = 9.0; / gl_Position.z;
}