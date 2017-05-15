#version 330 core

in vec4 fColor;
in vec2 TexCoord;
in float rate;
out vec4 finalColor;

uniform sampler2D gColorMap;

void main()
{
	//vec4 color = texture2D(gColorMap, TexCoord);
	//finalColor = vec4(vec3(color),color.a * (1-rate));
	//finalColor = vec4(vec3(fColor),fColor.a * (1-rate));
	finalColor = fColor;
} 
