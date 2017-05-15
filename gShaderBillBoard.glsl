#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec4 color;
	vec3 dir;
	float rate;
} gs_in[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

out vec4 fColor;
out vec2 TexCoord;
out float rate;

void main() 
{
	fColor = gs_in[0].color;
	vec3 pos = vec3(gl_in[0].gl_Position);

	vec3 worldUp = vec3(0.0,1.0,0.0);
	vec3 front = normalize(camPos - pos);
	vec3 right = cross(front,worldUp);
	vec3 up = worldUp ;//cross(right,front);

	mat4 mvp = projection * view *  model;

	gl_Position = mvp * vec4(pos,1.0);
	TexCoord = vec2(1.0, 1.0);
	rate = gs_in[0].rate;
	EmitVertex();

	gl_Position = mvp * vec4(pos + right,1.0);
	TexCoord = vec2(0.0, 1.0);
	rate = gs_in[0].rate;
	EmitVertex();

	gl_Position = mvp * vec4(pos + up,1.0);
	TexCoord = vec2(1.0, 0.0);
	rate = gs_in[0].rate;
	EmitVertex();

	gl_Position = mvp * vec4(pos + up + right,1.0);
	TexCoord = vec2(0.0, 0.0);
	rate = gs_in[0].rate;
	EmitVertex();

    EndPrimitive();
}